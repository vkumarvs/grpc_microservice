FROM ubuntu:18.04
COPY  ./service.tar /
RUN tar -xf service.tar
WORKDIR /service/
RUN cp knapLibs.conf /etc/ld.so.conf.d/
RUN ldconfig
CMD ./grpcdataserver
