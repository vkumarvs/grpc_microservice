```
# gRPC file/data exchange demo

Solution Structure
    ├── client/service.tar           # Tar file containing client docker file
    ├── server/service.tar           # Tar file containing server docker file
    ├── docker-compose.yml           # file to containerize grpc demo service
    ├── dataservice.proto            # Protocol buffer 3 definitions
    ├── grpcdataclient.cc            # grpc client code
    ├── grpcdataserver.cc            # grpc server code
    ├── Makefile                     # client/server makefile
    ├── README.md                    # Project Information.


This demo is just an attempt to use gRPC client server program that exchanges
number,string and a  file data in synchronous mode.

To exchange file data it uses client/server streams and client uses two threads
to read and write the streams.

Ideal way to take care of large file data exchange is to use ashynchronous mode.
Using ashynchronous mode would be improvement instead of using current streaming
mode.

```
**Solution:**

**How to run containerized solution (Ubuntu 18.04 container):**
```
Clone the repo on your machine:
    git clone git@github.com:vkumarvs/grpc_microservice.git

Invoke the docker-compose file :
   docker-compose up

Stop the service:
   docker-compose stop
```
**Testing:**
```
    Client program sends messages every 6 seconds from client container
    For this demo it replaces the need for a test script
```

**Code:**
```
    This demo contains the client/server code but it requires one to download
    and install gRPC code and protocol buffer 3 as mentioned in this link
    https://grpc.io/docs/tutorials/basic/c.html
```






