#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include <grpcpp/grpcpp.h>
#include "dataservice.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpcdemo::StringReq;
using grpcdemo::StringRsp;
using grpcdemo::Demoservice;
using grpcdemo::DataNumberReq;
using grpcdemo::DataNumberReply;
using grpcdemo::FileContent;
using grpc::ClientReaderWriter;
using namespace std;

class DemoGrpcClient {
 public:
  DemoGrpcClient(std::shared_ptr<Channel> channel)
      : stub_(Demoservice::NewStub(channel)) {}

  std::string StringData(const std::string& user) {
    StringReq request;
    request.set_name(user);
    StringRsp reply;
    ClientContext context;

    Status status = stub_->StringData(&context, request, &reply);
    if (status.ok()) {
      return reply.message();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "RPC failed";
    }
  }

  int NumberData(int data) {
    DataNumberReq dataReq;
    dataReq.set_data(data);
    DataNumberReply dataRes;
    ClientContext context;

    Status status = stub_->NumberData(&context, dataReq, &dataRes);
    if (status.ok()) {
	    std::cout <<"response at client:" <<dataRes.data()<<std::endl;
      return dataRes.data();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }

  void FileData() {
    ClientContext context;

    std::shared_ptr<ClientReaderWriter<FileContent, FileContent> > stream(
        stub_->FileData(&context));

    //Spawn a different thread to write data to the demo service
    std::thread writer([stream]() {
      std::vector<string> lines{
        ("This is my first line from some file"),
        ("This is my second line from some file"),
        ("Third line"),
        ("Fourth line")};
     FileContent line;
      for (auto x:lines) {

        line.set_data(x);
        std::cout << "Client send:\t" << x << std::endl;
        stream->Write(line);
      }
      stream->WritesDone();
    });

    // Demo service send the same data client write to the service
    FileContent line;
    while (stream->Read(&line)) {
      std::cout << "Server response: \t" << line.data() <<std::endl;
    }
    writer.join();
    Status status = stream->Finish();
    if (!status.ok()) {
      std::cout << "FileContent failed." << std::endl;
    }
  }

 private:
  std::unique_ptr<Demoservice::Stub> stub_;
};

int main(int argc, char** argv) {

    //Create the channel to demo server
#if 0
    cout <<"Please enter host ip" <<endl;
    string ip;
    cin>>ip;
    ip = ip + ":50051";
    DemoGrpcClient democlient(grpc::CreateChannel(
                ip, grpc::InsecureChannelCredentials()));

    cout <<"\nEnter options \n" "s - Enter single string without space\n" "n - Test number data \n" "f - Simulate file data read and write in sync mode.\n"
        "q - quit \n" <<endl;

    while(1) {
        char x;
        cin>>x;
        if(x == 'q')
            break;
        switch(x) {
            case 's': {
                          std::string user;
                          cout<<"Enter string data \n";
                          cin>>user;
                          //Send string data
                          string reply = democlient.StringData(user);
                          std::cout << "Demoservice received: " << reply << std::endl;
                      }
                      break;
            case 'n': {
                          int value = 0;
                          cout<<"Enter integer value: same value would be sent back to the client from server \n";
                          cin>>value;
                          std::cout << "Demoservice received number: " << democlient.NumberData(value)<< std::endl;
                      }
                      break;
            case 'f':
                      {
                          //Simulate sending a file data
                          cout<<"This option sends multiple lines(like reading from a file) to the server and server writes back same lines to the client \n";
                          democlient.FileData();
                      }
                      break;
            default:
                      cout <<"Only single string with s option\n";
                      exit(0);
        }
    }
#else
    while(1) {
        DemoGrpcClient democlient(grpc::CreateChannel(
                    "grpc_grpcsrvr_1:50051", grpc::InsecureChannelCredentials()));
        string reply = democlient.StringData("testString");
        std::cout << reply << std::endl;
        std::cout << democlient.NumberData(1000)<< std::endl;
        democlient.FileData();
        this_thread::sleep_for(chrono::milliseconds(6000));
    }
#endif

    return 0;
}
