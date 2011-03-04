#ifndef _gbssl_core_new_hxx
#define _gbssl_core_new_hxx

#include <QString>
#include <QObject>
#include <vector>
#include <QTimer>
#include <libconfig.h++>

#include "eyes_view.h"

using namespace std;
using namespace libconfig;

struct sdate
{
    unsigned short day,
                   month,
                   day_num;
    unsigned int   hour,
                   year;
};

class percental
{
public:
    short                   mod,
                            mod_prev;
    double                  load,
                            stable;
    vector <double>         probes,
                            sector_small;
    unsigned int          buff_size,
                            steps,
                            lin_num,
                            current_probe,
                            current_probe_small,
                            loseless,
                            calculate();
    char                    frequency;
    void                    get_load (double),
                            init (QString adress);
    bool                    buffered;
};

class unital
{
public:
    short                       mod,
                                mod_prev;
    vector<unsigned short>      probes,
                                sector_small;
    unsigned int              buff_size,
                                value,
                                stable,
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
    short               mod;
    unsigned int      value,
                        start,
                        steps,
                        lin_num,
                        wide,
                        end,
                        calculate();
    char                frequency;
    void                init (QString adress);
};

class bul
{
public:
    int                     total_mod;
    unsigned int            step,
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
    unsigned short          outline,
                            prev_outline,
                            eye,
                            tired,
                            hot,
                            shy,
                            layer2,
                            layer3,
                            layer4,
                            value;
    void                    update();
};

class Core : public QObject
{
    Q_OBJECT
public:
            Core            ();
    void    bulwers_update   (),
            bulwers_init    (),
            gui_refresh     (),
            gui_init        (),
            load_config     ( Config * set ),
            run             (),
            graphics_prepare();
    QString face_prev         ;
    bool    wake_up           ,
            wake_up_prepare ();
public slots:
    void    on_timer_tick   ();
private:
    QTimer        * timer;
};

#endif //gbssl_core_new_hxx
