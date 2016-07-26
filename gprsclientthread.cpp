#include "gprsclientthread.h"
#include <QHostAddress>

GPRSClientThread::GPRSClientThread(QTcpSocket *GPRSClientSocket)
{
   newGPRSClientSocket=GPRSClientSocket;
   stopFlag=true;
   CtrlConstructionFlag=false;
}

GPRSClientThread::~GPRSClientThread()
{
   qDebug()<<"----~GPRSClientThread----";
}

void GPRSClientThread::run()
{ qDebug()<<"----GPRSClientThread Run....----";
  qDebug()<<"----GPRSClientThread Socket is"<<this->newGPRSClientSocket->peerAddress()<<" "<<this->newGPRSClientSocket->peerPort();
  int byteSize;
  while(stopFlag)
    {try{
       if(CtrlConstructionFlag==true)
       { QByteArray msg=CtrlConstructionData.toLatin1();
         byteSize=newGPRSClientSocket->write(msg);
         CtrlConstructionFlag=false;
         qDebug()<<"----GPRSClientThread Write"<<msg;
         qDebug()<<"----The size of msg is "<<byteSize;
         qDebug()<<"----GPRSClientThread Socket is"<<this->newGPRSClientSocket->peerAddress()<<" "<<this->newGPRSClientSocket->peerPort();

       }
         }catch(QString exception)
        {  qDebug()<<"GPRSClientThread Exception "<<exception;
           break;
        }
    }
     stopFlag=true;
     qDebug()<<"----GPRSClientThread stopped----";
}

void GPRSClientThread::stop()
{
    stopFlag=false;
    //newGPRSClientSocket->abort();
}


void GPRSClientThread::setSendCtrlConstruction(bool ctrlFlag, QString ctrlData)
{
    CtrlConstructionFlag=ctrlFlag;
    CtrlConstructionData=ctrlData;
}

