#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>

class myTcpServer:public QTcpServer
{
public:
    myTcpServer();
    ~myTcpServer();
};

#endif // MYTCPSERVER_H
