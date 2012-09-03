#ifndef CAMERA_HXX
#define CAMERA_HXX

#ifdef _eyes_debug_hxx
#error debug.hxx must be included AFTER any opencv headers!
#endif

// OpenCV includes.
#include <opencv2/opencv.hpp>
#include "eyes.hxx"
#include <QElapsedTimer>

using namespace std;

#define REP(x,n) for(int x=0; x<n; x++)
#define FOR(x,b,e) for(int x=b; x<=e; x++)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair

typedef pair<int,int> PII;
typedef vector<PII> VPII;

struct plama
{
    VPII punkty;
};

struct pixel
{
    unsigned short  B,
                    G,
                    R;
};

struct environment
{
    unsigned short* input;
    unsigned short  max_tolerance,
                    min_tolerance,
                    colindex;
    unsigned int    greencounter,
                    bluecounter,
                    redcounter,
                    yellowcounter,
                    pinkcounter,
                    darkcounter,
                    greycounter,
                    lightcounter,
                    tabsize,
                    delay;
    double          B_correct,
                    G_correct,
                    R_correct,
                    Rperc,
                    Yperc,
                    Gperc,
                    Bperc,
                    Pperc,
                    Lperc,
                    Dperc,
                    Hperc,
                    global_avg;
    pixel**         envmap;
    QElapsedTimer   timer;
    bool            checked;
};

struct funsys
{
    double          fun,
                    newfun,
                    forgetcalm,
                    minfun;
    QElapsedTimer   funchunktimer,
                    funtimer,
                    funforgettimer,
                    totforgettimer;
    unsigned int    funcounter,
                    funchunk,
                    totforget;
};

class camcapture
{
public:
    bool            cam_init(),
                    motion_detect(vector <plama> plamy),
                    main();
    void            init_motionpics(),
                    init_debug(),
                    sleepdetect(),
                    optimize(int last_delay),
                    envread(pixel** input),
                    funcalc();
    IplImage*       get_image();
    IplImage*       get_motionpics(double tolerance, IplImage *input);
    bool**          img2bool(IplImage *input);
    pixel**         img2env(IplImage *input);
    vector <plama>  splash_detect(bool **input, int min_splash_size);

    CvSize          motionpicsSize;
    CvCapture       *cam;
    IplImage        *src,
                    *difference,
                    *resized,
                    *movingAverage,
                    *dst,
                    *temp;
    bool            first,
                    sleep,
                    debug,
                    deactiveworking,
                    tmp_halted,
                    halted,
                    enabled;
    bool**          boolimage;
    vector <vector <pair<PII, PII> > >  midpoints;
    vector<int>     bucket,
                    prevdists;
    vector<plama>   plamy;
    int             timer,
                    forget_timer,
                    prevmax,
                    min_splash_size,
                    max_buff,
                    max_dist,
                    min_dist,
                    deactive_perc,
                    active_perc,
                    motioncounter,
                    deactive_delay,
                    deactive_timer,
                    delay,
                    sleepdelay,
                    delayunit,
                    max_cpu_usage,
                    sleep_cpu_usage,
                    deactive_global_cpu_load,
                    tmp_deactive_timer,
                    retry_times,
                    fps_adaptation_time,
                    mindelay,
                    minsleepdelay,
                    catch_speed;
    double          reference_sleep_average,
                    reference_active_average,
                    reference_fps,
                    reference_sleepfps,
                    min_active_fps,
                    min_sleep_fps,
                    fps,
                    averagecalc(double ifps);
    pair<int, int>  motionpos,
                    operationsarea;
    QElapsedTimer   fps_adaptation_timer,
                    deactivetimer;
    environment     env;
    funsys          fun;
};

#endif // CAMERA_HXX
