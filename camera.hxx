#ifndef CAMERA_HXX
#define CAMERA_HXX

#ifdef _eyes_debug_hxx
#error debug.hxx must be included AFTER any opencv headers!
#endif

// OpenCV includes.
#include <opencv2/opencv.hpp>
//#include "highgui.h"

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

class camcapture
{
public:
    bool            cam_init();
    void            init_motionpics(),
                    sleepdetect();
    IplImage*       get_image();
    IplImage*       get_motionpics(double tolerance, IplImage *src);
    bool**          img2bool(IplImage *input);
    vector <plama>  splash_detect(bool **input, int min_splash_size);
    pair<int, int>  motion_detect(vector <plama> plamy);

    CvSize          motionpicsSize;
    CvCapture       *cam;
    IplImage        *src,
                    *difference,
                    *resized,
                    *movingAverage,
                    *dst,
                    *temp;
    bool            first,
                    sleep;
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
                    deactive_timer;
    double          switchmode();

};

#endif // CAMERA_HXX
