#include "connection.hxx"


connectionThread::connectionThread(int ID, QObject *parent, connectionGate *gate)
{
    server = gate;
    this->socketDescriptor = ID;
}

void connectionThread::run()
{
    qDebug() << "Starting connection";
    socket = new QTcpSocket();
    this->confirmed = false;
    if (!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

    exec();
}



void connectionThread::readyRead()
{
    QString input = socket->readAll().data();

    qDebug() << "Read " << input << "from socket " << socketDescriptor;
    if (!confirmed)
    {
        if (input.contains("EYES"))
        {
            confirmed = true;
            qDebug() << "Authorisation success";
            socket->write("CONFIRMED");
            server->registerClient(socketDescriptor);
        }
        else
        {
            qDebug() << "Auth failed, destroying thread...";
            socket->close();
            socket->deleteLater();
            exit(0);
        }
    }
    else
    {
        //socket->write("GOTIT");
        if (input.contains("ASK"))
        {
            qDebug() << socketDescriptor << "actual client";
            qDebug() << "in base:" << server->commandsset.size();
            qDebug() << "in base:" << server->result.size();
            qDebug() << "in base:" << server->responses.size();
            for (int i = 0; i<server->commandsset.size();i++)
            {
                qDebug() << "in base:" << server->commandsset[i].ST;
                if (server->commandsset[i].ST == socketDescriptor)
                {
                    if (!server->commandsset[i].ND.isEmpty())
                    {
                        socket->write( &(server->commandsset[i].ND.toStdString())[0] );
                    }
                    else
                        socket->write("NOPE");
                    break;
                }
            }
        }
        if (input.contains("REQUEST"))
        {
            if (!server->perform(input, socketDescriptor))
            {
                socket->write("ERROR");
            }
            else
            {
                for (int i = 0; i < server->result.size(); i++)
                {
                    if (server->result[i].ST == socketDescriptor)
                    {
                        socket->write( &(server->result[i].ND.toStdString())[0] );
                        break;
                    }
                }
            }
        }
        if (input.contains("RESPONSE"))
        {
            for (int i = 0; i < server->responses.size(); i++)
            {
                if (server->responses[i].ST == socketDescriptor)
                {
                    qDebug() << "got response";
                    server->responses[i].ND = input;
                    break;
                }
            }
        }
    }
}

void connectionThread::disconnected()
{
    qDebug() << "Destroying socket " << socketDescriptor;

    socket->deleteLater();
    server->removeClient(socketDescriptor);
    disconnect(socket, 0, 0, 0);
    //exit(0);
}


void connectionGate::restartServer()
{
    disconnect(ownsocket, 0, 0, 0);
    disconnect(clienttimer, 0, 0, 0);
    result.clear();
    responses.clear();
    commandsset.clear();
    startServer();
}


void connectionGate::registerClient(int descriptor)
{
    result.push_back(make_pair(descriptor, *new QString));
    commandsset.push_back(make_pair(descriptor, *new QString));
    responses.push_back(make_pair(descriptor, *new QString));
}


void connectionGate::removeClient(int descriptor)
{
    for (int i = 0; i < result.size(); i++)
    {
        if (result[i].ST == descriptor)
        {
            result.erase(result.begin()+i);
            responses.erase(responses.begin()+i);
            commandsset.erase(commandsset.begin()+i);
        }
    }
}


connectionGate::connectionGate(QObject *parent)
{
    //port = 'e' * 1000 + 'y' * 100 + 'e' * 10 + 's';
    port = 8000 + 'e'+'y'+'e'+'s';
}


void connectionGate::startServer()
{
    if (!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Error during starting server, looking for existing...";
        ownsocket = new QTcpSocket(this);
        ownsocket->connectToHost(QHostAddress::Any, port);
        if (!ownsocket->waitForConnected(3000))
        {
            qDebug() << "Fatal - cannot start new server, neither connect to existing";
            return;
        }
        else
        {
            ownsocket->write("EYES");
            ownsocket->flush();
            ownsocket->waitForBytesWritten(3000);
            if (!ownsocket->waitForReadyRead(3000))
            {
                qDebug() << "No response... disconnecting, Fatal - cannost start new server, neither connect to existing";
                ownsocket->close();
                return;
            }
            else
            {
                QString confirmation;
                confirmation = ownsocket->readAll().data();
                if (!confirmation.contains("CONFIRMED"))
                {
                    qDebug() << "Error - could not get confirmation. disconnecting. Fatal - cannost start new server, neither connect to existing";
                }
                else
                {
                    qDebug() << "Connected succesfully!";
                    result.push_back(make_pair(0, *new QString()));
                    commandsset.push_back(make_pair(0, *new QString()));
                    responses.push_back(make_pair(0, *new QString()));
                    clienttimer = new QTimer();
                    clienttimer->setInterval(5000);
                    connect(clienttimer, SIGNAL(timeout()), this, SLOT(askserver()));
                    connect(ownsocket, SIGNAL(disconnected()), this, SLOT(restartServer()));
                    clienttimer->start();
                }
            }
        }
    }
    else
    {
        qDebug() << "Server started";
    }
}

void connectionGate::incomingConnection(int socketDescriptor)
{
    qDebug() << "Connecting..." << socketDescriptor;
    connectionThread * thread = new connectionThread(socketDescriptor, 0, this);
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));
    thread->start();
}

void connectionGate::askserver()
{
    if (commandsset[0].ND.isEmpty())
    {
        ownsocket->write("ASK");
        ownsocket->flush();
        ownsocket->waitForBytesWritten(3000);
        if (!ownsocket->waitForReadyRead(3000))
        {
            qDebug() << "Connection timeout???";
        }
        else
        {
            QString input;
            input = ownsocket->readAll().data();
            if (input.contains("NOPE"))
            {
                qDebug() << "nothing to do... waiting.";
            }
            else
            {
                qDebug() << "received: " << input << " from server...";
                if (!perform(input, 0))
                {
                    ownsocket->write("ERROR");
                    ownsocket->flush();
                    ownsocket->waitForBytesWritten(3000);
                }
                else
                {
                    ownsocket->write(&("RESPONSE|" + result[0].ND.toStdString())[0]);
                    ownsocket->flush();
                    ownsocket->waitForBytesWritten(3000);
                }
            }
        }
    }
    else
    {
        ownsocket->write(&("REQUEST|" + commandsset[0].ND.toStdString())[0]);
        commandsset[0].ND.clear();
        ownsocket->flush();
        ownsocket->waitForBytesWritten(3000);
        if (!ownsocket->waitForReadyRead(3000))
        {
            qDebug() << "Disconnected???";
        }
        else
        {
            QString input;
            input = ownsocket->readAll().data();
            if (input.contains("ERROR"))
            {
                qDebug() << "Error during operations";
            }
            else
            {
                responses[0].ND = input;
            }
        }
    }
}

bool connectionGate::perform(QString request, int label)
{
    QStringList commands = request.split("|");
    cerr << "Received commands set from " << label << " client:\n";
    for (int i = 1; i < commands.size(); i++)
    {
        cerr << commands.at(i).toStdString() << "\n";
        if (commands.at(i).contains("CONFIG"))
        {
            QStringList configlist = commands.at(i).split("<");
            for (int j = 1; j < configlist.size(); j++)
            {
                QStringList labelAndVal = configlist.at(j).split("=");
                if (labelAndVal.size() == 2)
                {
                    if (!setConfVal(labelAndVal.at(0), labelAndVal.at(1)))
                        return 0;
                }
                else
                    return 0;
            }
        }
        if (commands.at(i).contains("KILL"))
        {
            exit(0);
        }
        if (commands.at(i).contains("REBOOT"))
        {
            QProcess::startDetached("./eyes");
            exit(0);
        }
        if (commands.at(i).contains("SAY"))
        {
            cerr << "\nMESSAGE: " << commands.at(i).toStdString() << "\n";
        }
    }
    return 1;
}

bool connectionGate::setConfVal(QString adress, QString val)
{
    if (!index->setVariable(adress, val))
        return 0;
    return 1;
}

bool varIndex::registerVariable(long long *pointer, _type vartype, QString name, bool locked)
{
    if (Map.count(name) > 0)
        return 0;
    varMapContent * newVarMap = new varMapContent;
    newVarMap->pointer.t_int = pointer;
    newVarMap->type = vartype;
    newVarMap->locked = locked;
    Map[name]=*newVarMap;
    return 1;
}

bool varIndex::registerVariable(long double *pointer, _type vartype, QString name, bool locked)
{
    if (Map.count(name) > 0)
        return 0;
    varMapContent * newVarMap = new varMapContent;
    newVarMap->pointer.t_ldouble = pointer;
    newVarMap->type = vartype;
    newVarMap->locked = locked;
    Map[name]=*newVarMap;
    return 1;
}

bool varIndex::registerVariable(double *pointer, _type vartype, QString name, bool locked)
{
    if (Map.count(name) > 0)
        return 0;
    varMapContent * newVarMap = new varMapContent;
    newVarMap->pointer.t_double = pointer;
    newVarMap->type = vartype;
    newVarMap->locked = locked;
    Map[name]=*newVarMap;
    return 1;
}

bool varIndex::registerVariable(bool *pointer, _type vartype, QString name, bool locked)
{
    if (Map.count(name) > 0)
        return 0;
    varMapContent * newVarMap = new varMapContent;
    newVarMap->pointer.t_bool = pointer;
    newVarMap->type = vartype;
    newVarMap->locked = locked;
    Map[name]=*newVarMap;
    return 1;
}

bool varIndex::registerVariable(char **pointer, _type vartype, QString name, bool locked)
{
    if (Map.count(name) > 0)
        return 0;
    varMapContent * newVarMap = new varMapContent;
    newVarMap->pointer.t_charpointer = pointer;
    newVarMap->type = vartype;
    newVarMap->locked = locked;
    Map[name]=*newVarMap;
    return 1;
}

bool varIndex::registerVariable(string *pointer, _type vartype, QString name, bool locked)
{
    if (Map.count(name) > 0)
        return 0;
    varMapContent * newVarMap = new varMapContent;
    newVarMap->pointer.t_string = pointer;
    newVarMap->type = vartype;
    newVarMap->locked = locked;
    Map[name]=*newVarMap;
    return 1;
}

bool varIndex::setVariable(QString name, QString value)
{
    if (Map.count(name) > 0)
    {
        if (!Map[name].locked)
        {
            bool ok = false;
            if (Map[name].type == varIndex::Tint)
            {
                value.toInt(&ok);
                if (ok)
                {
                    (*((int*)Map[name].type)) = value.toInt();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tuint)
            {
                value.toUInt(&ok);
                if (ok)
                {
                    (*((unsigned int*)Map[name].pointer.t_int)) = value.toUInt();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tshort)
            {
                value.toShort(&ok);
                if (ok)
                {
                    (*((short*)Map[name].pointer.t_int)) = value.toShort();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tushort)
            {
                value.toUShort(&ok);
                if (ok)
                {
                    (*((unsigned short*)Map[name].pointer.t_int)) = value.toUShort();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tlonglong)
            {
                value.toLongLong(&ok);
                if (ok)
                {
                    (*((long long*)Map[name].pointer.t_int)) = value.toLongLong();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tulonglong)
            {
                value.toULongLong(&ok);
                if (ok)
                {
                    (*((unsigned long long*)Map[name].pointer.t_int)) = value.toULongLong();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tbool)
            {
                if (value.contains("OFF"))
                {
                    (*((bool*)Map[name].pointer.t_bool)) = false;
                    return 1;
                }
                else if (value.contains("ON"))
                {
                    (*((bool*)Map[name].pointer.t_bool)) = true;
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tdouble)
            {
                value.toDouble(&ok);
                if (ok)
                {
                    (*((double*)Map[name].pointer.t_double)) = value.toDouble();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tlongdouble)
            {
                value.toDouble(&ok);
                if (ok)
                {
                    (*((long double*)Map[name].pointer.t_double)) = (long double)value.toDouble();
                    return 1;
                }
                else
                    return 0;
            }
            if (Map[name].type == varIndex::Tstring)
            {
                (*((string*)Map[name].pointer.t_string)) = value.toStdString();
                return 1;
            }
            if (Map[name].type == varIndex::Tcharpointer)
            {
                (*((char**)Map[name].pointer.t_string)) = &value.toStdString()[0];
            }
        }
        cerr << "Element locked\n";
        return 0;
    }
    cerr << "Element not found..." << Map.size() << "\n";
    return 0;
}
