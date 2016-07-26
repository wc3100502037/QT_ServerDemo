#include "myclient.h"

 myclient::myclient(QObject parent,int clientID)
  :public QTcpSocket(parent)
{
  this->clientID=clientID;




}
