#include "clientthread.h"
#include <QThread>
#include <QDebug>
#include <QHostAddress>
ClientThread::ClientThread(QTcpSocket* socket)
{
  this->newsocket=socket;
  this->stopFlag=true;
   //qDebug()<<"new ClientThread"<<"IPadress is"<<this->newsocket->peerAddress();
}

ClientThread::~ClientThread()
{

}

void ClientThread::run()
{qDebug()<<"----ClientThread Run...----";
 qDebug()<<"----ClientThread Socket is"<<this->newsocket->peerAddress()<<" "<<this->newsocket->peerPort();
 int i=0;
 while(stopFlag)
 {qDebug()<<"ClientThread run "+QString::number(i++)+",stopFlag="<<stopFlag;
   if(this->newsocket->waitForReadyRead())
   {
       QString msg=newsocket->readAll();
       if(msg!="")
       emit dataSendToMain(msg,this->newsocket);
       qDebug()<<"----ClientThread ReadyRead Msg is"<<msg;
       qDebug()<<"----ClientThread Socket is"<<this->newsocket->peerAddress()<<" "<<this->newsocket->peerPort();

   }

 }
 stopFlag=true;
 qDebug()<<"----ClientThread stopped----";
}

void ClientThread::stop()
{  qDebug()<<"----ClientThread::stop----";
   stopFlag=false;
// this->newsocket->abort();
}
