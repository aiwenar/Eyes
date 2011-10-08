#ifndef _gbssl_core_new_hxx
#define _gbssl_core_new_hxx

#include <QString>
#include <QObject>
#include <vector>
#include <QTimer>
#include <libconfig.h++>

#include "eyes.hxx"

using namespace std;
using namespace libconfig;

struct sdate
{
    unsigned short              day,
                                month,
                                day_num;
    unsigned int                hour,
                                year;
};

class percental
{
public:
    short                       mod,
                                mod_prev;
    vector <double>             probes,
                                sector_small;
    double                      load,
                                stable;
    unsigned int                buff_size,
                                steps,
                                lin_num,
                                current_probe,
                                current_probe_small,
                                loseless,
                                calculate();
    char                        frequency;
    void                        get_load (double),
                                init (QString adress);
    bool                        buffered;
};

class unital
{
public:
    short                       mod,
                                mod_prev;
    vector<unsigned short>      probes,
                                sector_small;
    unsigned int                value,
                                stable,
                                buff_size,
                                steps,
                                unit,
                                lin_num,
                                current_probe,
                                current_probe_small,
                                loseless,
                                calculate();
    char                        frequency;
    void                        get_load (unsigned short),
                                init (QString adress);
    bool                        buffered;
};

class timal
{
public:
    short                       mod;
    unsigned int                value,
                                start,
                                steps,
                                lin_num,
                                wide,
                                end,
                                calculate();
    char                        frequency;
    void                        init (QString adress);
};

class bul
{
public:
    int                         total_mod;
    unsigned int                step,
                                wall_01,
                                wall_02,
                                wall_03,
                                wall_04,
                                wall_05,
                                wall_06,
                                wall_07,
                                wall_08,
                                wall_09,
                                wall_10,
                                wall_11,
                                wall_12,
                                wall_13,
                                wall_14,
                                wall_15;
    unsigned short              outline,
                                prev_outline,
                                eye,
                                tired,
                                hot,
                                shy,
                                layer2,
                                layer3,
                                layer4,
                                value;
    void                        update();
};

class hard_dbg
{
private:
    unsigned short              line    ,
                                spacer  ;
    void                        chck_s();
public:
    unsigned short              max_s   ;
    void                        print() ;
    bool                        enabled ;
};

class auto_calc
{
public:
    double                      c_cpu,
                                c_mem,
                                c_temp,
                                impact;
    unsigned long               save_next,
                                save_interval;
    bool                        enabled;
};

struct sended_anims
{
    QString                     close,
                                open,
                                eyes,
                                face_prev;
    unsigned short              start,
                                end;
};

#endif //gbssl_core_new_hxx
