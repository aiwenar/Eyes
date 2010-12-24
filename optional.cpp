#include <iostream>
#include <string.h>
#include <time.h>
#include </usr/include/qt4/Qt/qstring.h>
#include </usr/include/qt4/Qt/qvector.h>
#include </usr/include/qt4/Qt/qregexp.h>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <cmath>


struct sdate
{
    int day;
    int mounth;
    int day_num;
    int hour;
    int year;
};

sdate get_time ();
int temperatura ();
int bat_plugged ();
int bateria ();


using namespace std;

int main ()
{
    int i=0;
    while (i<7){
    cout <<get_time ().day << endl << get_time ().day_num <<
            endl << get_time().hour << endl << get_time().mounth << endl;
    //cout << endl << "temp " << temperatura () << endl << bat_plugged () << " " << bateria << endl;
i++;
sleep (1);
}
}

sdate get_time ()
{
    //static int initialized = 0;
    //if (initialized == 0){
    time_t timer;

    timer=time(NULL);

    //initialized = 1;

    sdate get_date;
    QString tmp1 = asctime ( localtime ( &timer ) );
    QStringList l_date = tmp1.split ( ' ' );
    if ( l_date[1] == "Mon" )get_date.day = 1;
    if ( strcmp ( "Tue", l_date[1].toStdString () ) == 0 )get_date.day = 1;
    if ( strcmp ( "Wed", l_date[1].toStdString () ) == 0 )get_date.day = 1;
    if ( strcmp ( "Thu", l_date[1].toStdString () ) == 0 )get_date.day = 1;
    if ( strcmp ( "Fri", l_date[1].toStdString () ) == 0 )get_date.day = 1;
    if ( strcmp ( "Sat", l_date[1].toStdString () ) == 0 )get_date.day = 1;
    if ( strcmp ( "San", l_date[1].toStdString () ) == 0 )get_date.day = 1;
    if ( strcmp ( "Jan", l_date[1].toStdString () ) == 0 )get_date.mounth = 1;
    if ( strcmp ( "Feb", l_date[1].toStdString () ) == 0 )get_date.mounth = 2;
    if ( strcmp ( "Mar", l_date[1].toStdString () ) == 0 )get_date.mounth = 3;
    if ( strcmp ( "Apr", l_date[1].toStdString () ) == 0 )get_date.mounth = 4;
    if ( strcmp ( "May", l_date[1].toStdString () ) == 0 )get_date.mounth = 5;
    if ( strcmp ( "Jun", l_date[1].toStdString () ) == 0 )get_date.mounth = 6;
    if ( strcmp ( "Jul", l_date[1].toStdString () ) == 0 )get_date.mounth = 7;
    if ( strcmp ( "Aug", l_date[1].toStdString () ) == 0 )get_date.mounth = 8;
    if ( strcmp ( "Sep", l_date[1].toStdString () ) == 0 )get_date.mounth = 9;
    if ( strcmp ( "Oct", l_date[1].toStdString () ) == 0 )get_date.mounth = 10;
    if ( strcmp ( "Nov", l_date[1].toStdString () ) == 0 )get_date.mounth = 11;
    if ( strcmp ( "Dec", l_date[1].toStdString () ) == 0 )get_date.mounth = 12;

    get_date.day_num = (int)l_date[3];
    QVector<QString> tmp2 = l_date[4].split ( ':' );
    get_date.hour = (3600*((int)tmp2[0]) + (60*((int)tmp2[1])) + ((int)tmp2[2]));
    get_date.year = (int)l_date[5];

    return date;
}

//--------------------------

int temperatura ()
{
        fstream calosc ("/proc/acpi/thermal_zone/TZ00/temperature", fstream::in);

        string texcik;
        while (calosc.good())
                texcik+=calosc.get();

        int tempjuczer=texcik.find_first_of ("temperature");

        int i=0;
        int a=0;
        char temp[27];

        for (;i<27;i++) {
                temp[a] = texcik[25+a];
                a++;
                }

        int temperature;
        temperature = atoi (temp);

        return temperature;
}

//--------------------------

int bat_plugged ()
{
        fstream calosc ("/proc/acpi/battery/BAT1/state", fstream::in);

        string texcik;
        while (calosc.good()){
        texcik+=calosc.get();}

        if (texcik[25] == 'n')
        {
                return 0;
        }

        static int pluged = 0;

        if (texcik[82] == 'c' && pluged !=1) //battery has been just pluged
        {
                pluged = 1;
                return 1;
        }
         if (texcik[82] != 'c' && pluged !=0) //battery has been just unpluged
        {
                pluged = 0;
                return 2;
        }
        if (texcik[82] == 'c' && pluged == 1) //battery is still pluged
                return 3;
        if (texcik[82] != 'c' && pluged == 0) //battery is still unpluged
                return 4;

        else return 5;
    }

int bateria ()
{
        fstream calosc ("/proc/acpi/battery/BAT1/state", fstream::in);

        string texcik;
        while (calosc.good()){
        texcik+=calosc.get();}

        int baterry=texcik.find_first_of ("capacity:");


        int i=0;
        int a=0;
        char bat[155];

        for (;i<155;i++)
        {
                bat[a] = texcik[151+a];
                a++;
        }

        static int powr;
        powr = atoi (bat);

        return powr;
}
