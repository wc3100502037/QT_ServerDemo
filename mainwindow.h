#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QHostInfo>
#include <clientthread.h>
#include <gprsclientthread.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ListenpushButton_clicked();

    void on_SendpushButton_clicked();

    void on_ClearpushButton_clicked();

    void incomingConnection();

    void DisConnection();

    void ReadData();

    void on_pushButton_clicked();

    void on_sendToConnectingBtn_clicked();

    void on_recConnectingBtn_clicked();

    void HandleRecThreadData(QString,QTcpSocket*);

    void SendToGPRSClient(QString);

    void handleSocketError();

    void handleRecDataforNoThread(QString);//不使用线程，接收到数据后，调用此槽函数
private:
    Ui::MainWindow *ui;
    void Init();
    QTcpServer *myTcpServer;
    QTcpSocket *myClientSocket;
    QString MyHostName;
    QList<QTcpSocket*> socketList;
    ClientThread *myClientThread;
    GPRSClientThread *myGPRSClientSocket;
    QString rcvStringForNoThread;//不使用线程的发送数据
    QTcpSocket *mySocketForNoThread;//不使用线程的发送数据
signals:
    void GPRSConstructionSend(bool,QString);
};

#endif // MAINWINDOW_H
