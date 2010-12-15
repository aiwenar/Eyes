#include <./HARDWARE.h>

int C_LOAD ()
{
    glibtop_init();
    glibtop_cpu cpu;
    glibtop_get_cpu (&cpu);

    static unsigned short cpu_load = 0;
    static unsigned long p_idle = 0;
    static unsigned long p_total = 0;
    static unsigned long a_idle = 0;
    static unsigned long a_total = 0;
    static unsigned short d_total = 0;
    static unsigned short d_idle = 0;
    a_idle = cpu.idle;
    a_total = cpu.total;

    d_total = a_total - p_total;
    d_idle = a_idle - p_idle;
    cpu_load = (100*(d_total - d_idle)) / d_total;
    p_idle = a_idle;
    p_total = a_total;

    return cpu_load;
} 

int M_LOAD ()
{
    return mem_load;
}

int P_LIST ()
{
    return proclist.number;
}

int U_TIME ()
{
    glibtop_init();
    glibtop_uptime uptime;
    glibtop_get_uptime(&uptime);

    return uptime.uptime;
}
