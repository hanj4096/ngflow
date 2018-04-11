# ngflow
**Fast and scalable, next generation TCP/IP network stack**

[TOC]

**0x00 Introduction**
currently only tap interface is supported.

**0x01 Build**
git clone https://github.com/hanj4096/ngflow
cd ngflow
export NGFLOW_SDK=`pwd`
cd example/http_server
make

**0x02 Usage**
sudo ./httpd
wget http://172.16.0.2/

**0x03 example**
**http_server**
>a simple http server
**tcpecho**
>a simple tcp echo server
**udpecho**
>a simple udp echo server

**0x04 ngflow internals**
>to be continued


