#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QObject>

class ClientThread:public QThread
{  Q_OBJECT
public:
    ClientThread(QTcpSocket* socket);
    ~ClientThread();
    void stop();
protected:
    void run();

private:
  QTcpSocket *newsocket;
  QDataStream in;
  volatile bool stopFlag;

signals:
  void dataSendToMain(QString,QTcpSocket*);

};

#endif // CLIENTTHREAD_H
