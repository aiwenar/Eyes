#ifndef CONNECTION_HXX
#define CONNECTION_HXX

#include "eyes.hxx"
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QProcess>
#include <vector>
#include <map>

using namespace std;

class connectionGate;
class connectionThread;

struct varIndex
{
    union varPointer
    {
        long long   * t_int;
        double      * t_double;
        long double * t_ldouble;
        string      * t_string;
        char       ** t_charpointer;
        bool        * t_bool;
    };
    enum _type
    {
        Tint,
        Tuint,
        Tshort,
        Tushort,
        Tlonglong,
        Tulonglong,
        Tdouble,
        Tlongdouble,
        Tstring,
        Tcharpointer,
        Tbool
    };
    struct varMapContent
    {
        varPointer  pointer;
        _type       type;
        bool        locked;
    };

    map < QString, varMapContent > Map;
    bool registerVariable(long double * pointer, _type vartype, QString name, bool locked);
    bool registerVariable(long long * pointer, _type vartype, QString name, bool locked);
    bool registerVariable(string * pointer, _type vartype, QString name, bool locked);
    bool registerVariable(double * pointer, _type vartype, QString name, bool locked);
    bool registerVariable(char ** pointer, _type vartype, QString name, bool locked);
    bool registerVariable(bool * pointer, _type vartype, QString name, bool locked);
    bool setVariable(QString name, QString value);
    QString readVariable(QString name);
};


class connectionThread : public QThread
{
    Q_OBJECT
public:
    explicit    connectionThread(int ID, QObject *parent = 0, connectionGate *gate = 0);
    void        run();
signals:
    void        error(QTcpSocket::SocketError socketerror);
public slots:
    void        readyRead();
    void        disconnected();
private:
    bool        confirmed;
    QTcpSocket* socket;
    int         socketDescriptor;
    connectionGate *server;
};


class connectionGate : QTcpServer
{
    Q_OBJECT
public:
                connectionGate(QObject *parent = 0);
    void        startServer();
    void        registerClient(int descriptor);
    void        removeClient(int descriptor);
    vector<pair <int, QString> > commandsset;
    vector<pair <int, QString> > result;
    vector<pair <int, QString> > responses;
    bool        perform(QString request, int label);
    varIndex  * index;
protected:
    void        incomingConnection(int socketDescriptor);
private slots:
    void        askserver();
    void        restartServer();
private:
    bool        setConfVal(QString adress, QString val);
    int         port;
    QTcpSocket *ownsocket;
    QTimer     *clienttimer;
};



#endif // CONNECTION_HXX
