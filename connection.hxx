#ifndef CONNECTION_HXX
#define CONNECTION_HXX

#include "eyes.hxx"
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>





class connection_gate : public QThread
{
public:
                        connection_gate();
    void                serverStart();
private:
    QTcpServer          *tcpServer;
    QList<QTcpSocket *> *clientsList;
    QList<quint16>      *blockSizeList;

public slots:
    void                serverStop(),
                        addClient(),
                        serverQuit(),
                        removeClient(),
                        sendForward();
};




#endif // CONNECTION_HXX
