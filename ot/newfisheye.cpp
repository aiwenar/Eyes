
#include <math.h>
#include <unistd.h>
#include <getopt.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <vector>
#include <algorithm>
#define mane main
#define REP(x,n) for(int x=0; x<n; x++)
#define FOR(x,b,e) for(int x=b; x<=e; x++)
#define ST first
#define ND second
#define PB push_back
#define MP make_pair

using namespace std;
using namespace cv;

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

    pair <int, int> aspectCorrection;

    pair<double, double>
                    aspect,
                    scale,
                    shift;

    double          mirrorL2Diff;
};

void mirror::sampleImage(const IplImage *arr, double idx0, double idx1, CvScalar &res)
{
    if(idx0<0 || idx1<0 || idx0>(cvGetSize(arr).height-1) || idx1>(cvGetSize(arr).width-1))
    {
        res.val[0]=0;
        res.val[1]=0;
        res.val[2]=0;
        res.val[3]=0;
        return;
    }
    double idx0_fl=floor(idx0);
    double idx0_cl=ceil(idx0);
    double idx1_fl=floor(idx1);
    double idx1_cl=ceil(idx1);

    CvScalar s1=cvGet2D(arr,(int)idx0_fl,(int)idx1_fl);
    CvScalar s2=cvGet2D(arr,(int)idx0_fl,(int)idx1_cl);
    CvScalar s3=cvGet2D(arr,(int)idx0_cl,(int)idx1_cl);
    CvScalar s4=cvGet2D(arr,(int)idx0_cl,(int)idx1_fl);
    double x = idx0 - idx0_fl;
    double y = idx1 - idx1_fl;
    res.val[0]= s1.val[0]*(1-x)*(1-y) + s2.val[0]*(1-x)*y + s3.val[0]*x*y + s4.val[0]*x*(1-y);
    res.val[1]= s1.val[1]*(1-x)*(1-y) + s2.val[1]*(1-x)*y + s3.val[1]*x*y + s4.val[1]*x*(1-y);
    res.val[2]= s1.val[2]*(1-x)*(1-y) + s2.val[2]*(1-x)*y + s3.val[2]*x*y + s4.val[2]*x*(1-y);
    res.val[3]= s1.val[3]*(1-x)*(1-y) + s2.val[3]*(1-x)*y + s3.val[3]*x*y + s4.val[3]*x*(1-y);
}

pair <double, double> mirror::getRadial(double x, double y, double cx, double cy, double k)
{
    x = (x*scale.ST+shift.ST);
    y = (y*scale.ND+shift.ND);
    double resX = x+((x-cx)*k*((x-cx)*(x-cx)+(y-cy)*(y-cy)));
    double resY = y+((y-cy)*k*((x-cx)*(x-cx)+(y-cy)*(y-cy)));
    return make_pair(resX, resY);
}

double mirror::calc_shift(double x1, double x2, double cx, double k)
{
    double thresh = 1;
    double x3 = x1+(x2-x1)*0.5;
    double res1 = x1+((x1-cx)*k*((x1-cx)*(x1-cx)));
    double res3 = x3+((x3-cx)*k*((x3-cx)*(x3-cx)));

    //  std::cerr<<"x1: "<<x1<<" - "<<res1<<" x3: "<<x3<<" - "<<res3<<std::endl;

    if(res1>-thresh and res1 < thresh)
        return x1;
    if(res3<0)
    {
        return calc_shift(x3,x2,cx,k);
    }
    else
    {
        return calc_shift(x1,x3,cx,k);
    }
}

void mirror::fisheye(IplImage *src, double distortion, pair<int, int> center)
{
    IplImage* dst = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
    double centerX=(double)(center.ST*src->width)/100.0;
    double centerY=(double)(center.ND*src->height)/100.0;
    int width = src->width;
    int height = src->height;

    shift.ST = calc_shift(0,centerX-1,centerX,distortion);
    double newcenterX = width-centerX;
    pair <double, double> shift_2;
    shift_2.ST = calc_shift(0,newcenterX-1,newcenterX,distortion);

    shift.ND = calc_shift(0,centerY-1,centerY,distortion);
    double newcenterY = height-centerY;
    shift_2.ND = calc_shift(0,newcenterY-1,newcenterY,distortion);

    scale.ST = (width-shift.ST-shift_2.ST)/width;
    scale.ND = (height-shift.ND-shift_2.ND)/height;

    //std::cerr<<xshift<<" "<<shift.ND<<" "<<xscale<<" "<<yscale<<std::endl;
    //std::cerr<<cvGetSize(src).height<<std::endl;
    //std::cerr<<cvGetSize(src).width<<std::endl;

    for(int j=0;j<cvGetSize(dst).height;j++)
    {
        for(int i=0;i<cvGetSize(dst).width;i++)
        {
            CvScalar s;
            pair <double, double> xy;
            xy = getRadial((double)i,(double)j,centerX,centerY,distortion);
            sampleImage(src,xy.ND,xy.ST,s);
            cvSet2D(dst,j,i,s);
        }
    }
    cvReleaseImage(&src);
    src = cvCloneImage(dst);
}

int mane(int argc, char** argv)
{
  mirror mir;
  IplImage* src = cvLoadImage( argv[1], 1 );
  IplImage* newd;
  newd = cvCloneImage(src);
  mir.fisheye(newd, 0.00001, make_pair(50, 50));
  //cv::blur(cv::Mat(newd), cv::Mat(newd), cvSize(9, 5));
  cv::GaussianBlur(cv::Mat(newd), cv::Mat(newd), cvSize(9*1.8 + (1 - int(9*1.8) % 2), 5*1.8 + (1 - int(5*1.8) % 2)), 0, 0);
  cv::Mat source;
  cv::GaussianBlur(cv::Mat(newd), source, cvSize(50*1.8 + (1 - int(50*1.8) % 2), 30*1.8 + (1 - int(30*1.8) % 2)), 0, 0);
  addWeighted(cv::Mat(newd), 0.55, source, 0.45, 0, cv::Mat(newd));
  cvSaveImage(argv[2],newd,0);
}
