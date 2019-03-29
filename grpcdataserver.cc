#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "dataservice.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpcdemo::StringReq;
using grpcdemo::StringRsp;
using grpcdemo::Demoservice;
using grpcdemo::DataNumberReq;
using grpcdemo::DataNumberReply;
using grpcdemo::FileContent;
using grpc::ServerReaderWriter;
using namespace std;

class DemoServiceImpl final : public Demoservice::Service {

  Status StringData(ServerContext* context, const StringReq* request,
                       StringRsp* reply) override {
    reply->set_message("String:\t" + request->name());
    return Status::OK;
  }

  Status NumberData(ServerContext* context, const DataNumberReq* request,
                       DataNumberReply* reply) override {
    cout <<"Number:\t" <<request->data()<<endl;
    reply->set_data(request->data());
    return Status::OK;
  }

  Status FileData(ServerContext* context,
                   ServerReaderWriter<FileContent, FileContent>* stream) override {
    FileContent line;
    while (stream->Read(&line)) {
          cout <<"File Line:\t" <<line.data() <<endl;
          stream->Write(line);
    }
    return Status::OK;
  }

};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  DemoServiceImpl service;

  ServerBuilder builder;

  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
