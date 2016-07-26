#ifndef MYCLIENT
#define MYCLIENT

#include <QTcpSocket>

class myclient : public QTcpSocket
{
    Q_OBJECT;

public:
    explicit myclient(QObject *parent,int clientID);
private:
    int clientID;




}


#endif // MYCLIENT

