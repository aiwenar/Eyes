#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/proclist.h>
#include <glibtop/uptime.h>
#include "core.hxx"

double C_LOAD ()
{
  glibtop_init();
  glibtop_cpu gcpu;
  glibtop_get_cpu (&gcpu);

  static double cpu_load = 0;
  static unsigned long p_idle = 0;
  static unsigned long p_total = 0;
  static unsigned long a_idle = 0;
  static unsigned long a_total = 0;
  static unsigned long d_total = 0;
  static unsigned long d_idle = 0;
  a_idle = gcpu.idle;
  a_total = gcpu.total;

  d_total = a_total - p_total;
  d_idle = a_idle - p_idle;
  cpu_load = (100*(d_total - d_idle)) / d_total;
  p_idle = a_idle;
  p_total = a_total;

  return cpu_load;
}

double M_LOAD ()
{
  glibtop_init();
  glibtop_mem memory;
  glibtop_get_mem(&memory);

  /*unsigned int mem_total = memory.total;
  unsigned int mem_used = memory.user;
  unsigned int mem_load = mem_used/mem_total;
  */
  return 100*( (double)memory.user / (double)memory.total );
}

unsigned short P_LIST ()
{
  int which,arg;

  glibtop_init();
  glibtop_proclist proclist;
  glibtop_get_proclist(&proclist,which,arg);

  return proclist.number;
}

int U_TIME ()
{
  glibtop_init();
  glibtop_uptime uptime;
  glibtop_get_uptime(&uptime);

  return uptime.uptime;
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
    QStringList l_date ( tmp1.split ( ' ' ) );
    if ( l_date[0] == "Mon" )get_date.day = 1;
    if ( l_date[0] == "Tue" )get_date.day = 2;
    if ( l_date[0] == "Wed" )get_date.day = 3;
    if ( l_date[0] == "Thu" )get_date.day = 4;
    if ( l_date[0] == "Fri" )get_date.day = 5;
    if ( l_date[0] == "Sat" )get_date.day = 6;
    if ( l_date[0] == "Sun" )get_date.day = 7;

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

    if (l_date[2] == 0)
    {
        get_date.day_num = l_date[3].toInt ();
        QStringList tmp2 ( l_date[4].split ( ':' ) );
        get_date.hour = ( 3600 * ( tmp2[0].toInt () ) + ( 60 * ( tmp2[1].toInt() ) ) + tmp2[2].toInt () );
        get_date.year = l_date[5].toInt ();
    }
    else
    {
        get_date.day_num = l_date[2].toInt ();
        QStringList tmp2 ( l_date[3].split ( ':' ) );
        get_date.hour = ( 3600 * ( tmp2[0].toInt () ) + ( 60 * ( tmp2[1].toInt() ) ) + tmp2[2].toInt () );
        get_date.year = l_date[4].toInt ();
    }

    return get_date;
}
