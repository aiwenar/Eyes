#ifndef HARDWARE_HXX
#define HARDWARE_HXX

double          C_LOAD      ();
double          M_LOAD      ();
int             U_TIME      ();
sdate           get_time    ();
int             bateria     ();
int             bat_plugged ();
unsigned short  temperatura ();

extern          unital          temperature;
extern          percental       battery;
extern          unsigned int    battery_capacity;

using namespace std;

#endif // HARDWARE_HXX
