#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <clientthread.h>
#include <gprsclientthread.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
  ui->SendlineEdit->setReadOnly(true);
  ui->SendpushButton->setEnabled(false);
  ui->TexttextEdit->setReadOnly(true);

  myTcpServer=new QTcpServer(this);
  MyHostName=QHostInfo::localHostName();
  QHostInfo info=QHostInfo::fromName(MyHostName);
  ui->LocalIPlineEdit->setText(info.addresses().first().toString());

  connect(myTcpServer,SIGNAL(newConnection()),this,SLOT(incomingConnection()));

  qDebug()<<"localHostName:"<<MyHostName<<endl
          <<"IP Address:"<<info.addresses();
  foreach(QHostAddress address,info.addresses())
   {
      if(address.protocol()==QAbstractSocket::IPv4Protocol)
      {
        qDebug()<<"Local IPv4 Is:"<<address.toString();
      }
   }

  ui->recConnectingBtn->setEnabled(false);
  ui->sendToConnectingBtn->setEnabled(false);

}

void MainWindow::incomingConnection()
{ qDebug()<<"Main->incomingConnection()";
  QString connectedHost;
  myClientSocket=myTcpServer->nextPendingConnection();
  socketList.append(myClientSocket);
  connectedHost=connectedHost+myClientSocket->peerAddress().toString()+";"+QString::number(myClientSocket->peerPort());
  qDebug()<<"Connected Host is"<<"IP Adress is"<<myClientSocket->peerAddress()<<",Port is"<<myClientSocket->peerPort();
  ui->ConnectedHostcomboBox->addItem(connectedHost);
  ui->recConnectingComboBox->addItem(connectedHost);
  ui->subConnectingComboBox->addItem(connectedHost);

  ui->recConnectingBtn->setEnabled(true);
  ui->sendToConnectingBtn->setEnabled(true);
  //connect(myClientSocket,SIGNAL(readyRead()),this,SLOT(ReadData()));//有数据来，执行槽

}

void MainWindow::DisConnection()
{
    qDebug()<<"Main->DisConnection";

}

void MainWindow::on_ListenpushButton_clicked()
{   qDebug()<<"----ListenButton_Clicked----";
    qDebug()<<ui->ListenpushButton->text();
  if(ui->ListenpushButton->text()=="Listen")
      {qDebug()<<"ListenBtn->text==listen";

        bool ok=myTcpServer->listen(QHostAddress::Any,ui->PortlineEdit->text().toInt());
        qDebug()<<ok;
        if(ok)
         {qDebug()<<"ok is true";
          ui->ListenpushButton->setText("Cutoff");
          ui->TexttextEdit->append(tr("监听成功"));
          ui->SendpushButton->setEnabled(true);
          ui->SendlineEdit->setReadOnly(false);
         }
       }else{
        qDebug()<<"----excute else----";
        qDebug()<<ui->ListenpushButton->text();
        myTcpServer->close();

        ui->ListenpushButton->setText("Listen");
        ui->SendpushButton->setEnabled(false);
        ui->TexttextEdit->append(tr("断开"));
           }
}

void MainWindow::on_SendpushButton_clicked()
{
    qDebug()<<"----SendButton_Clicked----";
    QString data=ui->SendlineEdit->text();
    QByteArray dataToByte;
    dataToByte=data.toLatin1();
    if(data!="")
    {
        //myClientSocket->write(dataToByte);
        emit GPRSConstructionSend(true,data);
        qDebug()<<"----send data is"<<dataToByte<<"----";
    }
}

void MainWindow::on_ClearpushButton_clicked()
{
 qDebug()<<"----ClearButton_clicked----";

  ui->TexttextEdit->clear();

}

void MainWindow::ReadData()
{
     qDebug()<<"----ReadData Start----";
     QString recData=myClientSocket->readAll();
    // if(recData!="")
    // ui->TexttextEdit->append(tr("数据来自:%1，数据内容:%2").arg(myClientSocket->peerAddress().toString()).arg(recData));

}


void MainWindow::on_pushButton_clicked()
{
   myClientSocket->abort();
}
//发送到子机
void MainWindow::on_sendToConnectingBtn_clicked()
{
  qDebug()<<"----on_subConnectingBtn_clicked----";
  if(ui->sendToConnectingBtn->text()=="Link")
  {
  mySocketForNoThread=socketList.at(ui->subConnectingComboBox->currentIndex());
  qDebug()<<"----mySocketForNoThread is"<<mySocketForNoThread->peerAddress()<<","<<mySocketForNoThread->peerPort();
  qDebug()<<"----ui_currentIndex is"<<ui->subConnectingComboBox->currentIndex();
  ui->sendToConnectingBtn->setText("CutOff");
  ui->subConnectingComboBox->setEnabled(false);
  }else if(ui->sendToConnectingBtn->text()=="CutOff")
  {   ui->subConnectingComboBox->setEnabled(true);
      ui->sendToConnectingBtn->setText("Link");

  }
//此段代码为使用线程发送
/*
  if(ui->sendToConnectingBtn->text()=="Link")
  {  qDebug()<<"GPRSSocket excute Link";
     ui->sendToConnectingBtn->setText("CutOff");
     myGPRSClientSocket=new GPRSClientThread(socketList.at(ui->subConnectingComboBox->currentIndex()));

     myGPRSClientSocket->start();
     connect(this,SIGNAL(GPRSConstructionSend(bool,QString)),myGPRSClientSocket,SLOT(setSendCtrlConstruction(bool,QString)));
  }else if(ui->sendToConnectingBtn->text()=="CutOff")
  {  qDebug()<<"GPRSSocket excute CutOff";
     ui->sendToConnectingBtn->setText("Link");
     myGPRSClientSocket->stop();
  }
*/
}
//待接收子机信息
void MainWindow::on_recConnectingBtn_clicked()
{
  qDebug()<<"----on_recConnectingBtn_clicked----";
  QString currentData=ui->recConnectingComboBox->currentText();
  int currentIndex=ui->recConnectingComboBox->currentIndex();
  QStringList clientInfo=currentData.split(";",QString::SkipEmptyParts);
  //qDebug()<<"The Ip Adress is"<<clientInfo.at(0);
  //qDebug()<<"The port is "<<clientInfo.at(1);

  if(ui->recConnectingBtn->text()=="Link")
  { qDebug()<<"excute Link";
    ui->recConnectingBtn->setText("CutOff");
    myClientThread=new ClientThread(socketList.at(ui->recConnectingComboBox->currentIndex()));
    QObject::connect(myClientThread,SIGNAL(dataSendToMain(QString,QTcpSocket*)),this,SLOT(HandleRecThreadData(QString,QTcpSocket*)));
    myClientThread->start();
    ui->recConnectingComboBox->setEnabled(false);

  }else if(ui->recConnectingBtn->text()=="CutOff")
  { qDebug()<<"excute CutOff";
    ui->recConnectingBtn->setText("Link");
    ui->recConnectingComboBox->setEnabled(true);
    if(myClientThread->isRunning())
    { //QObject::disconnect(myClientThread,SIGNAL(dataSendToMain(QString,QTcpSocket*)),this,SLOT(HandleRecThreadData(QString,QTcpSocket*)));
      myClientThread->stop();
    }
    //socketList.removeAt(ui->recConnectingComboBox->currentIndex());
   // ui->recConnectingComboBox->removeItem(ui->recConnectingComboBox->currentIndex());
  }

}
//槽，发送跨线程信号给发送线程
void MainWindow::HandleRecThreadData(QString msg,QTcpSocket* tcpSocket)
{/*
  QString tcpMsg=msg;
  ui->TexttextEdit->append(tr("数据来自:%1，数据内容:%2").arg(tcpSocket->peerAddress().toString()).arg(tcpMsg));
  rcvStringForNoThread=msg;
  emit GPRSConstructionSend(true,tcpMsg);//触发信号，传递数据到GPRS线程
*/
    ui->TexttextEdit->append(tr("数据来自:%1，数据内容:%2").arg(tcpSocket->peerAddress().toString()).arg(msg));
    QByteArray TcpMsg=msg.toLatin1();
    mySocketForNoThread->write(TcpMsg);
}

void MainWindow::SendToGPRSClient(QString)
{

}

void MainWindow::handleSocketError()
{

}

void MainWindow::handleRecDataforNoThread(QString)
{

}



