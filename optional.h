#ifndef OPTIONAL
#define OPTIONAL

#include <string.h>
#include <time.h>
#include </usr/include/qt4/Qt/qstring.h>
#include </usr/include/qt4/Qt/qvector.h>
#include </usr/include/qt4/Qt/qregexp.h>
#include </usr/include/qt4/Qt/qstringlist.h>
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

#endif // OPTIONAL