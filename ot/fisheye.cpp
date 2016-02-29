    #include <math.h>
    #include <unistd.h>
    #include <getopt.h>
    #include <iostream>
    #include <opencv2/opencv.hpp>
    #include <opencv2/contrib/contrib.hpp>
    #define mane main


    void sampleImage(const IplImage* arr, double idx0, double idx1, CvScalar& res)
    {
      if(idx0<0 || idx1<0 || idx0>(cvGetSize(arr).height-1) || idx1>(cvGetSize(arr).width-1)){
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

    double xscale;
    double yscale;
    double xshift;
    double yshift;

    double getRadialX(double x,double y,double cx,double cy,double k){
      x = (x*xscale+xshift);
      y = (y*yscale+yshift);
      double res = x+((x-cx)*k*((x-cx)*(x-cx)+(y-cy)*(y-cy)));
      return res;
    }

    double getRadialY(double x,double y,double cx,double cy,double k){
      x = (x*xscale+xshift);
      y = (y*yscale+yshift);
      double res = y+((y-cy)*k*((x-cx)*(x-cx)+(y-cy)*(y-cy)));
      return res;
    }

    double thresh = 1;
    double calc_shift(double x1,double x2,double cx,double k){
      double x3 = x1+(x2-x1)*0.5;
      double res1 = x1+((x1-cx)*k*((x1-cx)*(x1-cx)));
      double res3 = x3+((x3-cx)*k*((x3-cx)*(x3-cx)));

      //  std::cerr<<"x1: "<<x1<<" - "<<res1<<" x3: "<<x3<<" - "<<res3<<std::endl;

      if(res1>-thresh and res1 < thresh)
        return x1;
      if(res3<0){
        return calc_shift(x3,x2,cx,k);
      }
      else{
        return calc_shift(x1,x3,cx,k);
      }
    }

    int mane(int argc, char** argv)
    {
      IplImage* src = cvLoadImage( argv[1], 1 );
      IplImage* dst = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
      IplImage* dst2 = cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
      double K=atof(argv[3]);
      double centerX=atoi(argv[4]);
      double centerY=atoi(argv[5]);
      int width = cvGetSize(src).width;
      int height = cvGetSize(src).height;

      xshift = calc_shift(0,centerX-1,centerX,K);
      double newcenterX = width-centerX;
      double xshift_2 = calc_shift(0,newcenterX-1,newcenterX,K);

      yshift = calc_shift(0,centerY-1,centerY,K);
      double newcenterY = height-centerY;
      double yshift_2 = calc_shift(0,newcenterY-1,newcenterY,K);
      //  scale = (centerX-xshift)/centerX;
      xscale = (width-xshift-xshift_2)/width;
      yscale = (height-yshift-yshift_2)/height;

      //std::cerr<<xshift<<" "<<yshift<<" "<<xscale<<" "<<yscale<<std::endl;
      //std::cerr<<cvGetSize(src).height<<std::endl;
      //std::cerr<<cvGetSize(src).width<<std::endl;

      for(int j=0;j<cvGetSize(dst).height;j++){
        for(int i=0;i<cvGetSize(dst).width;i++){
          CvScalar s;
          double x = getRadialX((double)i,(double)j,centerX,centerY,K);
          double y = getRadialY((double)i,(double)j,centerX,centerY,K);
          sampleImage(src,y,x,s);
          cvSet2D(dst,j,i,s);

        }
      }
    #if 0
      cvNamedWindow( "Source1", 1 );
      cvShowImage( "Source1", dst);
      cvWaitKey(0);
    #endif

      cvSaveImage(argv[2],dst,0);

    #if 0
      for(int j=0;j<cvGetSize(src).height;j++){
        for(int i=0;i<cvGetSize(src).width;i++){
          CvScalar s;
          sampleImage(src,j+0.25,i+0.25,s);
          cvSet2D(dst,j,i,s);
        }
      }

      cvNamedWindow( "Source1", 1 );
      cvShowImage( "Source1", src);
      cvWaitKey(0);

    #endif

}
