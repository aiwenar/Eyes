#include <iostream>
#include <string.h>
#include <time.h>
#include </usr/include/qt4/Qt/qstring.h>
#include </usr/include/qt4/Qt/qvector.h>
#include </usr/include/qt4/Qt/qregexp.h>
#include <qstringlist.h>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <cmath>


struct sdate
{
    int day;
    int month;
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
            endl << get_time().hour << endl << get_time().month << endl;
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
    QStringList l_date = new QStringList ();
    l_date = tmp1.split ();
    if ( l_date[1] == "Mon" )get_date.day = 1;
    if ( l_date[1] == "Tue" )get_date.day = 2;
    if ( l_date[1] == "Wed" )get_date.day = 3;
    if ( l_date[1] == "Thu" )get_date.day = 4;
    if ( l_date[1] == "Fri" )get_date.day = 5;
    if ( l_date[1] == "Sat" )get_date.day = 6;
    if ( l_date[1] == "Sun" )get_date.day = 7;

    if ( l_date[1] == "Jan" )get_date.month = 1;
    if ( l_date[1] == "Feb" )get_date.month = 2;
    if ( l_date[1] == "Mar" )get_date.month = 3;
    if ( l_date[1] == "Apr" )get_date.month = 4;
    if ( l_date[1] == "May" )get_date.month = 5;
    if ( l_date[1] == "Jun" )get_date.month = 6;
    if ( l_date[1] == "Jul" )get_date.month = 7;
    if ( l_date[1] == "Aug" )get_date.month = 8;
    if ( l_date[1] == "Sep" )get_date.month = 9;
    if ( l_date[1] == "Oct" )get_date.month = 10;
    if ( l_date[1] == "Nov" )get_date.month = 11;
    if ( l_date[1] == "Dec" )get_date.month = 12;

    get_date.day_num = (int)l_date[3];
    QStringList tmp2 = new QStringList ();
    tmp2 = l_date[4].split ( ':' );
    get_date.hour = (3600*((int)tmp2[0]) + (60*((int)tmp2[1])) + ((int)tmp2[2]));
    get_date.year = (int)l_date[5];

    return get_date;
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
