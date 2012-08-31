#include "connection.hxx"

connection_gate::connection_gate()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(addClient()));
}

void connection_gate::serverStart()
{
    if (!tcpServer->listen(QHostAddress::Any, 80))
    {
        cerr << "Server Could not be started!\n";
        return;
    }
    else
    {
       QString ipAddress;
       QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

       for (int i = 0; i < ipAddressesList.size(); ++i)
       {
          if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
          {
             ipAddress = ipAddressesList.at(i).toString();
             break;
          }
       }

       if (ipAddress.isEmpty())
       {
           cerr << "Other PCs not found\n";
          ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
       }
    }
}

void connection_gate::serverStop()
{
    tcpServer->close();
}

void connection_gate::addClient()
{
    QTcpSocket *client = tcpServer->nextPendingConnection();
    clientsList->push_back(client);
    blockSizeList->push_back(0);

    connect(client, SIGNAL(disconnected()), this, SLOT(removeClient()));
    connect(client, SIGNAL(disconnected()), client, SLOT(deleteLater()));
    connect(client, SIGNAL(readyRead()), this, SLOT(sendForward()));
}

void connection_gate::sendForward()
{
    QTcpSocket *client = (QTcpSocket*) sender();
    int index = clientsList->indexOf(client);

    QDataStream in(client);
    in.setVersion(QDataStream::Qt_4_6);

    if (blockSizeList->at(index) == 0)
    {
       if (client->bytesAvailable() < (int)sizeof(quint16))
       {
          return;
       }

       quint16 sizeOfBlock;
       in >> sizeOfBlock;
       (*blockSizeList)[index] = sizeOfBlock;
    }

    if (client->bytesAvailable() < blockSizeList->at(index))
    {
       return;
    }

    QString nick;
    in >> nick;

    QString message;
    in >> message;

    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_6);

    out << blockSizeList->at(index);
    out << nick;
    out << message;

    for (int i = 0; i < clientsList->size(); ++i)
    {
       if (i != index)
       {
          clientsList->at(i)->write(data);
       }
    }

    (*blockSizeList)[index] = (quint16)0;
}
