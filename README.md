# ngflow  
**Fast and scalable, next generation TCP/IP network stack**

## Introduction
currently only tap interface is supported.

## Build
git clone https://github.com/hanj4096/ngflow  
cd ngflow  
export NGFLOW_SDK=`pwd`  
cd example/http_server  
make  


##Usage
sudo ./httpd  
wget http://172.16.0.2/

## example
**http_server**  
>a simple http server  

**tcpecho**  
>a simple tcp echo server

**udpecho**
>a simple udp echo server

## ngflow internals
>to be continued



