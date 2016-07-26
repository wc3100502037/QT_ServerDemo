#ifndef GPRSCLIENTTHREAD_H
#define GPRSCLIENTTHREAD_H
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
class GPRSClientThread:public QThread
{  Q_OBJECT
public:
    GPRSClientThread(QTcpSocket* GPRSClientSocket);
    ~GPRSClientThread();
     void stop();
protected:
   void run();

private:
   bool stopFlag;
   QTcpSocket *newGPRSClientSocket;
   bool CtrlConstructionFlag;
   QString CtrlConstructionData;
public slots:
   void setSendCtrlConstruction(bool,QString);
signals:
   void recGPRSData(int);
};

#endif // GPRSCLIENTTHREAD_H
