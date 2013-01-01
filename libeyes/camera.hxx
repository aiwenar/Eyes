#ifndef CAMERA_HXX
#define CAMERA_HXX

#ifdef _eyes_debug_hxx
#error debug.hxx must be included AFTER any opencv headers!
#endif

// OpenCV includes.
#include <opencv2/opencv.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <cvaux.hpp>
#include "eyes.hxx"
#include "tm.hh"
#include <QElapsedTimer>
#include <QImage>

using namespace std;

#define REP(x,n) for(int x=0; x<n; x++)
#define FOR(x,b,e) for(int x=b; x<=e; x++)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair

typedef pair<int,int> PII;
typedef vector<PII> VPII;

class eyes_view;

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
                    colindex,
                    tooDarkMargin;
    unsigned int    greencounter,
                    bluecounter,
                    redcounter,
                    yellowcounter,
                    pinkcounter,
                    darkcounter,
                    greycounter,
                    lightcounter,
                    tabsize,
                    delay,
                    minbright,
                    maxbright,
                    eyesize;
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
    bool            checked,
                    tooDark,
                    eyemanagement;
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

struct mirror
{
    bool            processMirror(IplImage * src);

    void            init_mirrors(IplImage * srcExample),
                    sampleImage(const IplImage* arr, double idx0, double idx1, CvScalar& res),
                    fisheye(IplImage*src, double distortion, pair <int, int> center);

    double          calc_shift(double x1,double x2,double cx,double k);

    pair <double, double>
                    getRadial(double x,double y,double cx,double cy,double k);

    CvRect          mirrorLsrc,
                    mirrorRsrc,
                    mirrorWorkspace;

    CvSize          mirrorLsize,
                    mirrorRsize;

    IplImage      * mirrorL,
                  * mirrorR,
                  * prevMirror;

    pair <int, int> aspectCorrection,
                    gaussSizeL,
                    gaussSizeR,
                    paintcornerR,
                    paintcornerL;

    pair<double, double>
                    aspect,
                    scale,
                    shift,
                    distortionSize;

    double          mirrorL2Diff,
                    alphacorrection;

    unsigned short  quality;

    bool            distort,
                    boxblur,
                    gaussian,
                    bloom,
                    enabled;
};

class camcapture
{
public:

    //methods

    bool            cam_init(),
                    motion_detect(vector <plama> plamy),
                    main(),
                    compareRect(CvRect a, CvRect b, double size_precision, double pos_precision),
                    addFaceData(vector <IplImage*>  input, vector<PII> inputmatches, vector<vector <int> > * prevrecords);

    void            init_motionpics(),
                    init_debug(),
                    sleepdetect(),
                    optimize(int last_delay),
                    envread(pixel** input),
                    funcalc(),
                    faceprocessing(IplImage * source),
                    unrotate (vector<IplImage*> input, CvHaarClassifierCascade* cascade),
                    rescue_cascades();

    int             searchFace(IplImage * input, cv::Ptr<cv::FaceRecognizer> inputModel, double precision),
                    screensaver_management();

    double          averagecalc(double ifps);

    IplImage*       get_image();

    IplImage*       get_motionpics(double tolerance, IplImage *input);

    bool**          img2bool(IplImage *input);

    pixel**         img2env(IplImage *input);

    vector <plama>  splash_detect(bool **input, int min_splash_size);

    vector<CvRect>  detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade* cascade),
                    mergePartialFaces(vector<CvRect> input, double minMatchPerc),
                    generateAvgRect(vector<CvRect> input[], int size);

    vector<IplImage*>       cropImages(IplImage *img, vector<CvRect> region);

    vector<PII>     trackFaces(vector<CvRect> inputL, vector<CvRect> inputR, int maxDist, double ignoreDist);


    //variables


    CvSize          motionpicsSize;

    cv::Ptr<cv::FaceRecognizer> facesModel;

    CvSeq*          facerects;

    CvCapture     * cam;

    IplImage      * src,
                  * difference,
                  * compare_pic,
                  * resized,
                  * movingAverage,
                  * dst,
                  * temp,
                  * facegrey,
                  * srcforrec;

    IplImage **     eigenVector,
                    avgFace;

    IplImage ***    faceImgArr;

    bool            first,
                    sleep,
                    debug,
                    deactiveworking,
                    tmp_halted,
                    halted,
                    enabled,
                    facePresent,
                    faceDetectEnabled,
                    faceDetectInSleep,
                    recognitionInProgress,
                    faceRecognitionEnabled,
                    faceRecognitionFirstRun,
                    overdetect,
                    activate_screensaver,
                    deactivate_screensaver,
                    turnoff_screen;

    bool*           correctcascade;

    bool**          boolimage;



    vector <vector <pair<PII, PII> > >      midpoints;

    vector<int>                             bucket,
                                            prevdists,
                                            facesBankIndex,
                                            facesBankQuantities,
                                            faceSamples,
                                            avgRecognitions;

    vector<vector <int> >                   pathRecords;

    vector<pair <pair <int,int>, pair <int,int> > > rotvec;

    vector<pair <int, vector <IplImage> > > newFacesImgs;

    vector <IplImage*>                      faceimg;

    vector <CvHaarClassifierCascade*>       faceCascade;

    vector <cv::Mat>                        facesBank;

    vector <CvRect>                       * faceAreas,
                                            faceRectsPrev,
                                            avgRects;

    vector<plama>                           plamy;

    vector <vector <int> >                  freeFaceImgs;



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
                    catch_speed,
                    faceDetectDelay,
                    faceDetectSleepDelay,
                    currentcascade,
                    presenceCounter,
                    presenceBufferSize,
                    maxRecognitionBufferSize,
                    faceImageDropDelay,
                    newFaceOverdetectSkipSamples,
                    prevNewFace,
                    maxFacesPerImg;
    int **          hungarianInput;

    double          reference_sleep_average,
                    reference_active_average,
                    reference_fps,
                    reference_sleepfps,
                    min_active_fps,
                    min_sleep_fps,
                    fps,
                    minfacesize,
                    minMergeArea,
                    minPosMatch,
                    minSizeMatch,
                    faceRecognisePrecision,
                    faceRecognizerTreshold,
                    faceTrackMaxDist,
                    lookAtMotionTimeMin,
                    lookAtMotionTimeMax,
                    newFaceLookAtTimeMin,
                    newFaceLookAtTimeMax,
                    newFaceLookAtCurrent,
                    minL2Diff;

    pair<int, int>  motionpos,
                    operationsarea,
                    newFace;

    QElapsedTimer   fps_adaptation_timer,
                    deactivetimer,
                    detectionTimer,
                    newFaceLookAtTimer;

    environment     env;
    funsys          fun;
    mirror          mir;
    string          cascadesPath,
                    facesBankPath;
    IplImage      * QImage2IplImage (QImage *qimg);
    QImage        * IplImage2QImage (IplImage *iplImg);
    eyes_view     * eyes;
};

#endif // CAMERA_HXX
