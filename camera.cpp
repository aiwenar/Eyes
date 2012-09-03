#include "camera.hxx"
#include "hardware.hxx"
#include "hungarian.h"
#include "core.hxx"
#include <unistd.h>
#include <sys/stat.h>

camcapture ccap;
extern hardware HRDWR;
extern percental cpu;

using namespace cv;

static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

bool camcapture::cam_init()
{
    cam = NULL;
    cam = cvCaptureFromCAM(-1);
    if (cam == NULL)
    {
        cerr << "Could not initialize camera\n";
        return 0;
    }
    src = cvQueryFrame(cam);
    if (src == NULL)
    {
        cerr << "Could not initialize camera\n";
        return 0;
    }
    return 1;
}

IplImage* camcapture::get_image()
{
    src = cvQueryFrame(cam);
    cvResize(src, resized);
    return resized;
}

void camcapture::init_motionpics()
{
    difference = cvCreateImage ( motionpicsSize, src->depth, src->nChannels);
    compare_pic = cvCreateImage ( cvSize(100,100), IPL_DEPTH_8U, 1);
    resized = cvCreateImage ( motionpicsSize, src->depth, src->nChannels);
    movingAverage = cvCreateImage( motionpicsSize, IPL_DEPTH_32F, src->nChannels);
    dst = cvCreateImage( motionpicsSize, IPL_DEPTH_8U, 1 );
    facegrey = cvCreateImage( cvSize(src->width, src->height), IPL_DEPTH_8U, 1 );
    boolimage = new bool*[src->height];
    env.envmap = new pixel*[src->height];
    for (int i = 0; i < src->height; i++)
    {
        boolimage[i] = new bool[src->width];
    }
    for (int i = 0; i < src->height; i++)
    {
        env.envmap[i] = new pixel[src->width];
    }

    recognitionInProgress = false;
    presenceCounter = 0;
    overdetect = false;
    currentcascade = 0;
    env.tabsize = motionpicsSize.width*motionpicsSize.height;
    env.global_avg = 150;
    env.checked = false;
    fun.fun = 0.0;
    fun.funcounter = 0;
    fun.newfun = 0;

    HRDWR.pid = getpid();
    first = true;
    sleep = false;
    halted = false;
    tmp_halted = false;
    deactiveworking = false;
    timer = 0;
    prevmax = 0;
    deactive_timer = 0;
    delay = 100;
    sleepdelay = 500;
    fps = 25;

    if (faceDetectEnabled)
    {
        for (int i=0;i<faceCascade.size();i++)
        {
            if( !faceCascade[i] )
            {
                faceCascade.erase(faceCascade.begin()+i);
                cerr << "Couldnt load Face detector\n";
                i--;
            }
        }
        if (faceCascade.size() == 0)
        {
            cerr << "Warning: Could not load specified cascades - trying to fuck this situation like a boss...\n";
            rescue_cascades();
        }
        if (faceCascade.size() == 0)
        {
            faceDetectEnabled = false;
            cerr << "Error: Beeing like a boss failed - could not find any cascade - disabling recognition\n";
        }
        faceAreas = new vector <CvRect> [faceCascade.size()];

        if (faceRecognitionEnabled)
        {
            hungarianInput = new int *[maxFacesPerImg];
            for (int i = 0; i < maxFacesPerImg; i++)
            {
                hungarianInput[i] = new int[maxFacesPerImg];
            }
        }
    }

}

void camcapture::rescue_cascades()
{
    string emergency_cascades[4];
    emergency_cascades[0] = "haarcascade_frontalface_alt_tree.xml";
    emergency_cascades[1] = "haarcascade_frontalface_alt.xml";
    emergency_cascades[2] = "haarcascade_frontalface_alt2.xml";
    emergency_cascades[3] = "haarcascade_frontalface_default.xml";

    string possible_dir = "/usr/share/opencv/haarcascades/";

    for (int i = 0; i<4; i++)
    {
        faceCascade.push_back((CvHaarClassifierCascade*)cvLoad(&(possible_dir + emergency_cascades[i])[0], 0, 0, 0));
    }
    for (int i=0;i<faceCascade.size();i++)
    {
        if( !faceCascade[i] )
        {
            faceCascade.erase(faceCascade.begin()+i);
            i--;
        }
    }

    if (faceCascade.size() == 0)
    {
        cerr << "Couldnt load Face detector in " << possible_dir << "\n";
        possible_dir = "/usr/share/OpenCV/haarcascades/";

        for (int i = 0; i<4; i++)
        {
            faceCascade.push_back((CvHaarClassifierCascade*)cvLoad(&(possible_dir + emergency_cascades[i])[0], 0, 0, 0));
        }
        for (int i=0;i<faceCascade.size();i++)
        {
            if( !faceCascade[i] )
            {
                faceCascade.erase(faceCascade.begin()+i);
                i--;
            }
        }

        if (faceCascade.size() == 0)
        {
            cerr << "Couldnt load Face detector in " << possible_dir << "\n";
            possible_dir = "/usr/share/OpenCV/data/haarcascades/";

            for (int i = 0; i<4; i++)
            {
                faceCascade.push_back((CvHaarClassifierCascade*)cvLoad(&(possible_dir + emergency_cascades[i])[0], 0, 0, 0));
            }
            for (int i=0;i<faceCascade.size();i++)
            {
                if( !faceCascade[i] )
                {
                    faceCascade.erase(faceCascade.begin()+i);
                    i--;
                }
            }
        }

        if (faceCascade.size() == 0)
            cerr << "Couldnt load Face detector in " << possible_dir << "\n";
    }

    if (faceCascade.size()!=0)
    {
        cerr << "Success!\n" <<
                "    _____________   \n" <<
                "  _/             \\  \n" <<
                " /     ____________ \n" <<
                "|  ===(    \\    \\  )\n" <<
                "|      \\____\\/\\___/ \n" <<
                " \\_    ,_____     / \n" <<
                "   \\___'_________/  \n" <<
                "\n" <<
                "               LIKE A BOSS...\n\n";
        Configuration::getInstance()->setValue("cam.system.face_cascades_dir", &possible_dir[0]);
    }
}

IplImage* camcapture::get_motionpics(double tolerance, IplImage *input)
{
    if (first)
    {
        difference = cvCloneImage(input);
        temp = cvCloneImage(input);
        cvConvertScale(input, movingAverage, 1.0, 0.0);
        first = false;
    }
    else
    {
        cvRunningAvg(input, movingAverage, tolerance, NULL);
    }

    cvConvertScale(movingAverage,temp, 1.0, 0.0);
    cvAbsDiff(input,temp,difference);
    cvCvtColor( difference, dst, CV_RGB2GRAY );
    cvThreshold(dst, dst, 70, 255, CV_THRESH_BINARY);
    return dst;
}

bool** camcapture::img2bool(IplImage *input)
{
    int step = input->widthStep;
    uchar *data = ( uchar* )input->imageData;

    //bool *output[motionpicsSize.height][motionpicsSize.width];

    motioncounter = 0;
    for( int i = 0; i < motionpicsSize.height; i++ )
    {
        for( int j = 0 ; j < motionpicsSize.width; j++ )
        {
            boolimage[i][j] = data[i*step + j];
            if (data[i*step + j] > 0)
                motioncounter++;
        }
    }
    return boolimage;
}


vector<CvRect> camcapture::detectFaceInImage(IplImage *inputImg, CvHaarClassifierCascade* cascade)
{
        // Smallest face size.
        CvSize minFeatureSize = cvSize(inputImg->width*minfacesize, inputImg->height*minfacesize);
        // Only search for 1 face.
        int flags = CV_HAAR_DO_ROUGH_SEARCH;
        // How detailed should the search be.
        float search_scale_factor = 1.1f;
        CvMemStorage* storage;
        double t;
        CvSize size;
        int i, ms, nFaces;
        vector <CvRect> retvec;

        storage = cvCreateMemStorage(0);
        cvClearMemStorage( storage );

        retvec.clear();
        // Detect all the faces in the greyscale image.
        t = (double)cvGetTickCount();
        facerects = cvHaarDetectObjects( inputImg, cascade, storage,
                        search_scale_factor, 3, flags, minFeatureSize);
        t = (double)cvGetTickCount() - t;
        ms = cvRound( t / ((double)cvGetTickFrequency() * 1000.0) );
        nFaces = facerects->total;
        //printf("Face Detection took %d ms and found %d objects\n", ms, nFaces);
        cerr << "Detected " << nFaces << " faces witch method: " << currentcascade+1 << "\n";

        // Get the first detected face (the biggest).
        if (nFaces > 0)
        {
            for (int j = 0; j<nFaces; j++)
            {
                retvec.push_back(*(CvRect*)cvGetSeqElem( facerects, j ));
            }
            retvec = mergePartialFaces(retvec, minMergeArea);
        }

        cvReleaseMemStorage( &storage );
        //cvReleaseHaarClassifierCascade( &cascade );

        return retvec;	// Return the biggest face found, or (-1,-1,-1,-1).
}


vector <CvRect> camcapture::mergePartialFaces(vector<CvRect> input, double minMatchPerc)
{
    //cerr << "start of merging...\n";
    vector <pair<int, int> > matched;
    matched.clear();
    for (int i = 0; i<input.size(); i++)
    {
        for(int j = i;j<input.size(); j++)
        {
            if ((input[i].x < input[j].x && input[i].x+input[i].width > input[j].x ) ||
                (input[j].x+input[j].height < input[i].x+input[i].height && input[j].x+input[j].height > input[i].x ) ||
                (input[j].x < input[i].x && input[j].x+input[j].width > input[i].x ) ||
                (input[i].x+input[i].height < input[j].x+input[j].height && input[i].x+input[i].height > input[j].x ) )
            {
                if ((input[i].y < input[j].y && input[i].y+input[i].height > input[j].y ) ||
                    (input[j].y+input[j].height < input[i].y+input[i].height && input[j].y+input[j].height > input[i].y ) ||
                    (input[j].y < input[i].y && input[j].y+input[j].height > input[i].y ) ||
                    (input[i].y+input[i].height < input[j].y+input[j].height && input[i].y+input[i].height > input[j].y ) )
                {
                    if (i < j)
                    {
                        matched.push_back(make_pair(i, j));
                    }
                    else if (j < i)
                        matched.push_back(make_pair(j, i));
                }
            }
        }
    }

    //if (input.size() > 1)
    //{
    //    cerr << "CAUGHT\n";
    //    cerr << input[0].x << " " << input[0].width << "\n";
    //    cerr << input[1].x << " " << input[1].width << "\n";
    //}

    //cerr << "merging...\n";

    vector <int> useless;
    useless.clear();
    for (int i = 0; i<matched.size(); i++)
    {
        pair <int, int> duplicated;
        if (input[matched[i].ST].x < input[matched[i].ND].x)
            duplicated.first = -input[matched[i].ND].x;
        else
            duplicated.first = -input[matched[i].ST].x;
        if (input[matched[i].ST].x+input[matched[i].ST].width < input[matched[i].ND].x+input[matched[i].ND].width)
            duplicated.first += input[matched[i].ST].x+input[matched[i].ST].width;
        else
            duplicated.first += input[matched[i].ND].x+input[matched[i].ND].width;

        if (input[matched[i].ST].y < input[matched[i].ND].y)
            duplicated.second = -input[matched[i].ST].y;
        else
            duplicated.second = -input[matched[i].ND].y;
        if (input[matched[i].ST].y+input[matched[i].ST].height < input[matched[i].ND].y+input[matched[i].ND].height)
            duplicated.second += input[matched[i].ST].y+input[matched[i].ST].height;
        else
            duplicated.second += input[matched[i].ND].y+input[matched[i].ND].height;

        if ((duplicated.first > input[matched[i].ST].width*minMatchPerc || duplicated.first > input[matched[i].ND].width*minMatchPerc) &&
            (duplicated.second > input[matched[i].ST].height*minMatchPerc || duplicated.second > input[matched[i].ND].height*minMatchPerc) )
        {
            if (input[matched[i].ST].x > input[matched[i].ND].x)
                input[matched[i].ST].x = input[matched[i].ND].x;
            if (input[matched[i].ST].x+input[matched[i].ST].width < input[matched[i].ND].x+input[matched[i].ND].width)
                input[matched[i].ST].width = (input[matched[i].ND].x+input[matched[i].ND].width) - input[matched[i].ST].x;

            if (input[matched[i].ST].y > input[matched[i].ND].y)
                input[matched[i].ST].y = input[matched[i].ND].y;
            if (input[matched[i].ST].y+input[matched[i].ST].height < input[matched[i].ND].y+input[matched[i].ND].height)
                input[matched[i].ST].height = (input[matched[i].ND].y+input[matched[i].ND].height) - input[matched[i].ST].y;
            useless.push_back(matched[i].ND);
            //cerr << "merged\n";
        }
        //else
            //cerr << "not merged\n";
    }
    for (int i = 0; i<useless.size(); i++)
    {
        input.erase(input.begin() + useless[i] - i);
        //cerr << "ERASED\n";
    }
    //cerr << "end of merging\n";

    return input;
}


vector <CvRect> camcapture::generateAvgRect(vector<CvRect> input[], int size)
{
    vector <CvRect> retvec;
    retvec.clear();
    vector <CvRect> tmp;
    for (int i = 0; i < input[0].size(); i++)
    {
        //cerr << "firststep in " << i << " picture of vector 0\n";
        tmp.clear();
        tmp.push_back(input[0][i]);
        for (int j = 1; j<size;j++)
        {
            //cerr << "step2 in " << j << " second vector \n";
            for (int k = 0 ;k < input[j].size();k++)
            {
                //cerr << "step3 in " << k << " picture - size of" << input[j].size() << "\n";
                if (compareRect(input[0][i], input[j][k], minSizeMatch, minPosMatch))
                {
                    //cerr << "MATCH!\n";
                    tmp.push_back(input[j][k]);
                    input[j].erase(input[j].begin()+k);
                    break;
                }
                //else
                    //cerr << "-";
            }
        }
        if (tmp.size() == size)
        {
            //cerr << "MAPPED TRUE FACE\n";
            retvec.push_back(input[0][i]);
            for (int i = 1; i < size ; i++)
                retvec[retvec.size()-1].x += tmp[i].x;
            retvec[retvec.size()-1].x /=size;
            if (retvec[retvec.size()-1].x < 0)
                retvec[retvec.size()-1].x = 0;
            for (int i = 1; i < size ; i++)
                retvec[retvec.size()-1].y += tmp[i].y;
            retvec[retvec.size()-1].y /=size;
            if (retvec[retvec.size()-1].y < 0)
                retvec[retvec.size()-1].y = 0;
            for (int i = 1; i < size ; i++)
                retvec[retvec.size()-1].width += tmp[i].width;
            retvec[retvec.size()-1].width /=size;
            if (retvec[retvec.size()-1].x + retvec[retvec.size()-1].width > facegrey->width)
                retvec[retvec.size()-1].width = facegrey->width-retvec[retvec.size()-1].x;
            for (int i = 1; i < size ; i++)
                retvec[retvec.size()-1].height += tmp[i].height;
            retvec[retvec.size()-1].height /=size;
            if (retvec[retvec.size()-1].y + retvec[retvec.size()-1].height > facegrey->height)
                retvec[retvec.size()-1].height = facegrey->height-retvec[retvec.size()-1].y;
        }
    }
    //cerr << "TOTAL TRUEFACE " << retvec.size() << "\n";
    return retvec;
}

bool camcapture::compareRect(CvRect a, CvRect b, double size_precision, double pos_precision)
{
    if (a.width <= b.width*(1.0+size_precision) || b.width <= a.width*(1.0+size_precision))
    {
        if (a.height <= b.height*(1.0+size_precision) || b.height <= a.height*(1.0+size_precision))
        {
            if (abs((a.x + a.width/2) - (b.x + b.width/2)) < facegrey->width*pos_precision && abs((a.y + a.height/2) - (b.y + b.height/2)) < facegrey->height*pos_precision )
                return true;
        }
    }
    return false;
}

void camcapture::unrotate(vector<IplImage *> input, CvHaarClassifierCascade* cascade)
{
    // Smallest face size.
    CvSize minFeatureSize = cvSize(10, 10);
    // Only search for 1 face.
    int flags = CV_HAAR_DO_ROUGH_SEARCH;
    // How detailed should the search be.
    float search_scale_factor = 1.1f;
    CvMemStorage* storage;
    double t;
    CvSeq* rects;
    int ms, nEyes;
    vector<pair <pair <int,int>, pair <int,int> > > newrotvec;
    newrotvec.clear();

    storage = cvCreateMemStorage(0);
    cvClearMemStorage( storage );

    for (int i=0;i<input.size() && correctcascade[4];i++)
    {
        // Detect all the faces in the greyscale image.
        t = (double)cvGetTickCount();

        rects = cvHaarDetectObjects( input[i], cascade, storage,
                        search_scale_factor, 2, flags, minFeatureSize);

        t = (double)cvGetTickCount() - t;
        ms = cvRound( t / ((double)cvGetTickFrequency() * 1000.0) );

        nEyes = rects->total;
        //printf("Face Detection took %d ms and found %d objects\n", ms, nFaces);
        //cerr << "Detected " << nEyes << " eyes\n";
        if (nEyes == 2)
        {
            newrotvec.push_back(make_pair(make_pair((*(CvRect*)cvGetSeqElem( rects, 0 )).x +(*(CvRect*)cvGetSeqElem( rects, 0 )).width/2 , (*(CvRect*)cvGetSeqElem( rects, 0 )).y +(*(CvRect*)cvGetSeqElem( rects, 0 )).height/2), make_pair((*(CvRect*)cvGetSeqElem( rects, 1 )).x +(*(CvRect*)cvGetSeqElem( rects, 1 )).width/2 , (*(CvRect*)cvGetSeqElem( rects, 1 )).y +(*(CvRect*)cvGetSeqElem( rects, 1 )).height/2)));
            cvRectangle(
                        input[i],
                        cvPoint((*(CvRect*)cvGetSeqElem( rects, 0 )).x, (*(CvRect*)cvGetSeqElem( rects, 0 )).y),
                        cvPoint((*(CvRect*)cvGetSeqElem( rects, 0 )).x + (*(CvRect*)cvGetSeqElem( rects, 0 )).width, (*(CvRect*)cvGetSeqElem( rects, 0 )).y + (*(CvRect*)cvGetSeqElem( rects, 0 )).height),
                        CV_RGB(255, 255, 255),
                        1, 8, 0
                    );
            cvRectangle(
                        input[i],
                        cvPoint((*(CvRect*)cvGetSeqElem( rects, 1 )).x, (*(CvRect*)cvGetSeqElem( rects, 1 )).y),
                        cvPoint((*(CvRect*)cvGetSeqElem( rects, 1 )).x + (*(CvRect*)cvGetSeqElem( rects, 1 )).width, (*(CvRect*)cvGetSeqElem( rects, 1 )).y + (*(CvRect*)cvGetSeqElem( rects, 1 )).height),
                        CV_RGB(255, 255, 255),
                        1, 8, 0
                    );
        }
        else if (i < rotvec.size())
            newrotvec.push_back(rotvec[i]);
        else
            newrotvec.push_back(make_pair(make_pair(-1, -1), make_pair(-1, -1)));

        if (newrotvec[i].ST.ST != -1)
        {
            //cerr << "ROTATING\n";
            double x = rotvec[i].ST.ST-rotvec[i].ND.ST;
            if (x<0)
                x = -x;
            double y = rotvec[i].ST.ND-rotvec[i].ND.ND;
            if (rotvec[i].ST.ST > rotvec[i].ND.ST)
                y = -y;
            double d = sqrt(x*x + y*y);
            float m[6];
            CvMat M = cvMat(2, 3, CV_32F, m);
            int w = input[i]->width;
            int h = input[i]->height;
            m[0] = (float)( cos(y/d) );
            m[1] = (float)( sin(x/d) );
            m[3] = -m[1];
            m[4] = m[0];
            m[2] = w*0.5f;
            m[5] = h*0.5f;

            //cerr << m[0] << " " << m[1] << "\n";
            // Make a spare image for the result
            CvSize sizeRotated;
            sizeRotated.width = cvRound(w);
            sizeRotated.height = cvRound(h);

            // Rotate
            IplImage *imageRotated = cvCreateImage( sizeRotated,
                    input[i]->depth, input[i]->nChannels );

            // Transform the image
            cvGetQuadrangleSubPix( input[i], imageRotated, &M);
            input[i] = imageRotated;
        }
        rotvec = newrotvec;

        // Get the first detected face (the biggest).
    }

    cvReleaseMemStorage( &storage );
    //cvReleaseHaarClassifierCascade( &cascade );
}


vector<IplImage*> camcapture::cropImages(IplImage *input, vector<CvRect> region)
{
        //faceimg.clear();
        vector <IplImage*> retvec;
        IplImage *imageCropped;
        IplImage *croptemp = cvCreateImage(cvSize(input->width, input->height), input->depth, input->nChannels);
        cvCopy(input, croptemp);

        // Set the desired region of interest.
        for (int i = 0; i < region.size(); i++)
        {
            cvSetImageROI(croptemp, region[i]);
            // Copy region of interest into a new iplImage and return it.
            imageCropped = cvCreateImage(cvSize(region[i].width, region[i].height), croptemp->depth, croptemp->nChannels);
            cvCopy(croptemp, imageCropped);
            retvec.push_back(cvCreateImage(cvSize(100,100), imageCropped->depth, imageCropped->nChannels));// Copy just the region.
            cvResize(imageCropped, retvec[i]);
            cvEqualizeHist(retvec[i], retvec[i]);
            cvReleaseImage(&imageCropped);
        }

        cvReleaseImage(&croptemp);
        return retvec;
}


int camcapture::searchFace(IplImage *input, Ptr<FaceRecognizer> inputModel, double precision)
{
    if (ccap.faceRecognitionFirstRun)
        return -1;
    Mat converted(input);
    int predictedLabel = -1;
    inputModel->predict(converted, predictedLabel, precision);
    return predictedLabel;
}


/*

  hungarian input:

        R
    [1][2][3][4]
  L [2]
    [3]
    [4]

  */


vector <PII> camcapture::trackFaces(vector<CvRect> inputL, vector<CvRect> inputR, int maxDist, double ignoreDist)
{
    int newsize = maxFacesPerImg;
    bool toresize = false;
    if (inputL.size() > newsize)
    {
        toresize = true;
        newsize = inputL.size();
    }
    if (inputR.size() > newsize)
    {
        toresize = true;
        newsize = inputR.size();
    }
    if (toresize)
    {
        for (int i = 0; i < maxFacesPerImg; i++)
        {
            for (int j = 0; j < maxFacesPerImg; j++)
            {
                delete(&hungarianInput[i][j]);
            }
            delete(&hungarianInput[i]);
        }
        delete (&hungarianInput);
        maxFacesPerImg = newsize;
        hungarianInput = new int * [maxFacesPerImg];
        for (int i = 0; i < maxFacesPerImg; i++)
        {
            hungarianInput[i] = new int[maxFacesPerImg];
        }

    }

    vector <pair < int, int > > retvec (0);
    if (inputL.size() == 0 && inputR.size() == 0)
        return retvec;
    if (inputR.size() == 1 && inputL.size() == 1)
    {
        if (sqrt( ((inputR[0].x+inputR[0].width/2)-(inputL[0].x+inputL[0].width/2))*((inputR[0].x+inputR[0].width/2)-(inputL[0].x+inputL[0].width/2)) +
                  ((inputR[0].y+inputR[0].height/2)-(inputL[0].y+inputL[0].height/2))*((inputR[0].y+inputR[0].height/2)-(inputL[0].y+inputL[0].height/2)) ) > (double)maxDist*ignoreDist)
        {
            retvec.push_back(make_pair (-1, 0));
            retvec.push_back(make_pair (0, -1));
        }
        else
            retvec.push_back(make_pair (0, 0));
        return retvec;
    }
    if (inputR.size() == 1 && inputL.size() == 0)
    {
        retvec.push_back(make_pair(-1, 0));
        return retvec;
    }
    if (inputR.size() == 0 && inputL.size() == 1)
    {
        retvec.push_back(make_pair(0, -1));
        return retvec;
    }

    cerr << "Beginning standard matching process:\nCounting distances...";

    for (int i = 0; i < inputL.size(); i++)
    {
        for (int j = 0; j < inputR.size(); j++)
        {
            hungarianInput[i][j] = sqrt( ((inputR[j].x+inputR[j].width/2)-(inputL[i].x+inputL[i].width/2))*((inputR[j].x+inputR[j].width/2)-(inputL[i].x+inputL[i].width/2)) +
                                         ((inputR[j].y+inputR[j].height/2)-(inputL[i].y+inputL[i].height/2))*((inputR[j].y+inputR[j].height/2)-(inputL[i].y+inputL[i].height/2)) );
            if ((double)hungarianInput[j][i] < (double)maxDist*ignoreDist)
                hungarianInput[i][j] = (int)HRDWR.equalize(0, maxDist, hungarianInput[j][i], 1.5);
            else //blacklist element:
                hungarianInput[i][j] = maxDist;
        }
    }

    cerr << "DUN!\nbaking hungarian vector:\n";

    if (inputL.size() < inputR.size())
    {
        cerr << "switched to L < R...\nResizing to nxn matrix...";
        for (int i = inputL.size(); i < inputR.size(); i++)
        {
            for (int j = 0; j < inputR.size(); j++)
            {
                hungarianInput[i][j] = maxDist;
            }
        }
        cerr << "DUN!\nGetting return statement from hungarian method...";
        retvec = hungarian::hungarian(inputR.size(), inputR.size(), hungarianInput);
        cerr << "DUN!\nConverting to -1 notation...";
        // convert to correct notation:
        for (int i = 0; i < retvec.size(); i++)
        {
            if (retvec[i].ST >= inputL.size())
                retvec[i].ST = -1;
        }
        cerr << "DUN!\neliminating blacklisted...\n";
    }
    else
    {
        cerr << "switched to L >= R...\nResizing to nxn matrix...";
        for (int i = inputR.size(); i < inputL.size(); i++)
        {
            for (int j = 0; j < inputL.size(); j++)
            {
                hungarianInput[j][i] = maxDist;
            }
        }
        cerr << "DUN!\nGetting return statement from hungarian method...";
        retvec = hungarian::hungarian(inputL.size(), inputL.size(), hungarianInput);
        cerr << "DUN!\nConverting to -1 notation...";
        // convert to correct notation:
        for (int i = 0; i < retvec.size(); i++)
        {
            if (retvec[i].ND >= inputR.size())
                retvec[i].ND = -1;
        }
        cerr << "DUN!\neliminating blacklisted...\n";
    }


    cerr << "input vector:\n";
    for (int i = 0; i < retvec.size(); i++)
    {
        cerr << retvec[i].ST << " -> " << retvec[i].ND << "\n";
    }
    //eliminate blacklisted
    cerr << "beginning...\n";
    for (int i = 0; i < retvec.size(); i++)
    {
        if (retvec[i].ST != -1 && retvec[i].ND != -1)
        {
            if (hungarianInput[retvec[i].ST][retvec[i].ND] == maxDist)
            {
                cerr << "erasing...";
                retvec.push_back(make_pair(retvec[i].ST, -1));
                retvec.push_back(make_pair(-1, retvec[i].ND));
                retvec.erase(retvec.begin()+i);
                i--;
                cerr << "DUN!\n";
            }
        }
    }
    return retvec;

}


/*

  face 1: [0][0][1][0][2]
  face 2: [1][1][0][1][0]
  face 3:             [1]

          [0][0][0][0][0]
          [1][1][2][1][1]
                      [-1]


  0  1
  1  2
  2 -1
  3  0

  0  1
  1  2
  2  0
  -1 3



  */

bool camcapture::addFaceData(vector<IplImage *> input, vector<PII> inputmatches, vector<vector <int> > *prevrecords)
{
    //if there is no faces on image - clear everything
    if (inputmatches.size() == 0)
    {
        prevrecords[0].clear();
        prevrecords[1].clear();
        newFacesImgs.clear();
        faceSamples.clear();
        return 0;
    }
    //baking faces paths vectors
    cerr << "Baking faces paths:\n";
    cerr << inputmatches.size() << "\n" << inputmatches[0].ST << " " << inputmatches[0].ND << "\n";

    cerr << "prevrec size: " << prevrecords[0].size() << "\n";
    for (int i = 0; i<prevrecords[0].size(); i++)
    {
        // search existing faces

        int searchfor = prevrecords[0][i][prevrecords[0][i].size()-1];
        for (int j = 0; j < inputmatches.size(); j++)
        {
            if (inputmatches[j].ST == searchfor)
            {
                if (inputmatches[j].ND != -1)
                {
                    //if success - erase old records and matching pair, and add new face position and face prediction index

                    if (prevrecords[0][i].size() == maxRecognitionBufferSize)
                    {
                        cerr << "erasing old records...";
                        prevrecords[0][i].erase(prevrecords[0][i].begin()+0);
                        prevrecords[1][i].erase(prevrecords[1][i].begin()+0);
                        cerr << "DUN!\n";
                    }
                    faceSamples[i]++;
                    prevrecords[0][i].push_back(inputmatches[j].ND);
                    prevrecords[1][i].push_back(searchFace(input[inputmatches[j].ND], facesModel, faceRecognisePrecision));
                    inputmatches.erase(inputmatches.begin() + j);
                    break;
                }
                else
                {
                    //if failed - delete face record path and pair

                    for (int j = 0; j < newFacesImgs.size(); j++)
                    {
                        if (newFacesImgs[j].ST == i)
                        {
                            newFacesImgs.erase(newFacesImgs.begin()+j);
                            break;
                        }
                    }
                    faceSamples.erase(faceSamples.begin()+i);
                    prevrecords[0].erase(prevrecords[0].begin()+i);
                    prevrecords[1].erase(prevrecords[1].begin()+i);
                    inputmatches.erase(inputmatches.begin() + j);
                    i--;
                    break;
                }
            }
        }
    }
    cerr << "result " << prevrecords[0].size() << "\n";
    // if there are still untracked faces...
    cerr << "Tracking for new faces\n";

    if (inputmatches.size() != 0)
    {
        for (int i = 0; i < inputmatches.size(); i++)
        {
            if (inputmatches[i].ST == -1)
            {
                // add them if everything works correctly

                cerr << "addin new record...";
                vector <int> newface (0);
                prevrecords[0].push_back(newface);
                prevrecords[1].push_back(newface);
                prevrecords[0][prevrecords[0].size()-1].push_back(inputmatches[i].ND);
                prevrecords[1][prevrecords[1].size()-1].push_back(searchFace(input[inputmatches[i].ND], facesModel, faceRecognisePrecision));
                faceSamples.push_back(1);
                cerr << "DUN!:\n";
                cerr << prevrecords[0][prevrecords[0].size()-1][prevrecords[0][prevrecords[0].size()-1].size() -1] << " " <<
                        prevrecords[1][prevrecords[1].size()-1][prevrecords[1][prevrecords[1].size()-1].size() -1] << " " <<
                        faceSamples[faceSamples.size()-1] << "\n";
            }
            else
            {
                cerr << "We just don't know what went wrong! There is face which should be tracked before...\n";
                return 0;
            }
        }
    }
    for (int i = 0; i < prevrecords[0].size(); i++)
    {
        for (int j = 0; j < prevrecords[0][i].size(); j++)
            cerr << "[" << prevrecords[0][i][j] << "]";
        cerr << "\n";
    }
    cerr << "\n";
    for (int i = 0; i < prevrecords[1].size(); i++)
    {
        for (int j = 0; j < prevrecords[1][i].size(); j++)
            cerr << "[" << prevrecords[1][i][j] << "]";
        cerr << "\n";
    }

    //get average recognision if there is at least minimal samplaes number
    cerr << "recognition\n";

    vector <int> avgRecognitions (0);
    for (int i = 0; i < prevrecords[1].size(); i++)
    {
        cerr << "reco step 1...";
        if (prevrecords[1][i].size() == maxRecognitionBufferSize)
        {
            vector <pair <int, int> > counter (0);
            for (int j = 0; j < prevrecords[1][i].size(); j++)
            {
                bool present = false;
                for (int k = 0; k < counter.size(); k++)
                {
                    if (counter[k].ST == prevrecords[1][i][j])
                    {
                        counter[k].ND++;
                        present = true;
                        break;
                    }
                }
                if (!present)
                    counter.push_back(make_pair(prevrecords[1][i][j], 1));
            }
            pair <int, int> max = make_pair(0, 0);
            for (int j = 0; j < counter.size(); j++)
            {
                if (counter[j].ND > max.ND)
                    max = counter[j];
            }
            if (max.ND > maxRecognitionBufferSize/2)
                avgRecognitions.push_back(max.ST);
            else
                avgRecognitions.push_back(-2);
        }
        else
            avgRecognitions.push_back(-2);

        cerr << "DUN!\nreco step 2...";

        if ((prevrecords[1][i][prevrecords[1][i].size()-1] == -1 && prevrecords[1][i].size() < maxRecognitionBufferSize) || avgRecognitions[i] == -1)
        {
            if (prevrecords[1][i][prevrecords[1][i].size()-1] == -1 && prevrecords[1][i].size() < maxRecognitionBufferSize + newFaceOverdetectSkipSamples && prevrecords[1][i].size() > newFaceOverdetectSkipSamples)
                overdetect = true;
            bool present = false;
            for (int j = 0; j < newFacesImgs.size(); j++)
            {
                if (newFacesImgs[j].ST == i)
                {
                    newFacesImgs[j].ND.push_back(*cvCloneImage(input[prevrecords[0][i][prevrecords[0][i].size()-1]]));
                    present = true;
                }
            }
            if (!present)
            {
                vector <IplImage> newface (0);
                newFacesImgs.push_back(make_pair(i, newface));
                newFacesImgs[newFacesImgs.size()-1].ND.push_back(*cvCloneImage(input[prevrecords[0][i][prevrecords[0][i].size()-1]]));
            }
        }
        cerr << "DUN!\n";
    }

    cerr << "\n";
    for (int i = 0; i < avgRecognitions.size(); i++)
        cerr << "[" << avgRecognitions[i] << "] (" << faceSamples[i] << ")\n";


    bool toreload = false;

    // chceck if face save record is necessary:
    cerr << "savin\'\n";

    for (int i = 0; i < faceSamples.size(); i++)
    {
        if (faceSamples[i] >= faceImageDropDelay)
        {
            if (avgRecognitions[i] != -2 && avgRecognitions[i] != -1 && avgRecognitions[i] == prevrecords[1][i][prevrecords[1][i].size()-1])
            {
                faceSamples[i] = 0;
                facesBank.push_back(Mat (input[prevrecords[0][i][prevrecords[0][i].size()-1]]));
                facesBankIndex.push_back(avgRecognitions[i]);
                facesBankQuantities[avgRecognitions[i]]++;
                stringstream ss, ss2;
                ss << avgRecognitions[i];
                ss2 << facesBankQuantities[avgRecognitions[i]];
                HRDWR.set_file(ccap.facesBankPath + ss.str() + "/" + "size", ss2.str());
                cerr << "No new face detected, face " << ss.str() << " bank size changed to " << ss2.str() << "\n";

                stringstream ss3, ss4;
                ss3 << (facesBankQuantities[facesBankIndex[facesBankIndex.size()-1]] - 1);
                ss4 << facesBankIndex[facesBankIndex.size()-1];
                const string path = string (ccap.facesBankPath + ss4.str() + "/" + ss3.str() + ".jpg");
                imwrite(path, norm_0_255(ccap.facesBank[facesBank.size()-1].reshape(1, ccap.facesBank[facesBank.size()-1].rows)));
                cerr << "File :" << path << " saved\n";
                toreload = true;
            }
        }
    }


    //search for filled new face bases:
    cerr << "savin\'2\n";

    for (int i = 0; i < newFacesImgs.size(); i++)
    {
        if (newFacesImgs[i].ND.size() == maxRecognitionBufferSize + newFaceOverdetectSkipSamples)
        {
            cerr << "Detected new face:\n";
            stringstream ss, ss2, ss3;
            ss << facesBankQuantities.size();
            if ( -1 == mkdir ( &(ccap.facesBankPath + ss.str() + "/")[0], S_IRUSR | S_IWUSR | S_IXUSR ) and errno != EEXIST  )
            {
                cerr << "Couldn't create faces dir - disabling recognition";
                ccap.faceRecognitionEnabled = false;
            }
            ss3 << maxRecognitionBufferSize;
            HRDWR.set_file(ccap.facesBankPath + ss.str() + "/" + "size", ss3.str());
            facesBankQuantities.push_back(maxRecognitionBufferSize);
            ss2 << facesBankQuantities.size();
            HRDWR.set_file(ccap.facesBankPath + "size", ss2.str());
            cerr << "sizeof new face set to " << ss3.str() << ", new dir name: " << ss.str() << ". Bank size changed to: " << ss2.str() << "\n";
            for (int j = newFaceOverdetectSkipSamples; j < maxRecognitionBufferSize + newFaceOverdetectSkipSamples; j++)
            {
                facesBank.push_back(Mat (&newFacesImgs[i].ND[j]));
                facesBankIndex.push_back(facesBankQuantities.size()-1);
                stringstream ss, ss2;
                ss << (j-newFaceOverdetectSkipSamples);
                ss2 << (facesBankQuantities.size()-1);
                const string path = string (ccap.facesBankPath + ss2.str() + "/" + ss.str() + ".jpg");
                imwrite(path, norm_0_255(Mat (&newFacesImgs[i].ND[j]).reshape(1, Mat (&newFacesImgs[i].ND[j]).rows)));
                cerr << "File :" << path << " saved\n";
            }
            toreload = true;
        }
    }

    if (toreload)
    {
        cerr << "Reloading...";
        faceRecognitionFirstRun = false;
        facesModel = createEigenFaceRecognizer(0, ccap.faceRecognizerTreshold);
        facesModel->train(facesBank, facesBankIndex);
        return true;
    }
    else
        return false;
/*
    for (int i = 0; i < input.size(); i++)
    {
        int result = searchFace(input[i], facesModel, faceRecognisePrecision);
        cerr << "recognised: " << searchFace(input[i], facesModel, faceRecognisePrecision) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 0.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 0.05) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 0.1) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 0.2) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 0.5) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 0.8) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 1.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 2.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 5.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 15.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 50.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 150.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 250.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 500.0) << "\n";
        cerr << "recognised: " << searchFace(input[i], facesModel, 5000.0) << "\n\n";

        facesBank.push_back(Mat (input[i]));
        if (result == -1)
        {
            cerr << "Detected new face:\n";
            stringstream ss, ss2;
            ss << facesBankQuantities.size();
            if ( -1 == mkdir ( &(ccap.facesBankPath + ss.str() + "/")[0], S_IRUSR | S_IWUSR | S_IXUSR ) and errno != EEXIST  )
            {
                cerr << "Couldn't create faces dir - disabling recognition";
                ccap.faceRecognitionEnabled = false;
            }
            HRDWR.set_file(ccap.facesBankPath + ss.str() + "/" + "size", string ("1"));
            facesBankIndex.push_back(facesBankQuantities.size());
            facesBankQuantities.push_back(1);
            ss2 << facesBankQuantities.size();
            HRDWR.set_file(ccap.facesBankPath + "size", ss2.str());
            cerr << "sizeof new face set to 1, new dir name: " << ss.str() << ". Bank size changed to: " << ss2.str() << "\n";
        }
        else
        {
            facesBankIndex.push_back(result);
            facesBankQuantities[result]++;
            stringstream ss, ss2;
            ss << result;
            ss2 << facesBankQuantities[result];
            HRDWR.set_file(ccap.facesBankPath + ss.str() + "/" + "size", ss2.str());
            cerr << "No new face detected, face " << ss.str() << " bank size changed to " << ss2.str() << "\n";
        }
        facesModel = createEigenFaceRecognizer(0, ccap.faceRecognizerTreshold);
        facesModel->train(facesBank, facesBankIndex);
        stringstream ss, ss2;
        ss << (facesBankQuantities[facesBankIndex[facesBankIndex.size()-1]] - 1);
        ss2 << facesBankIndex[facesBankIndex.size()-1];
        const string path = string (ccap.facesBankPath + ss2.str() + "/" + ss.str() + ".jpg");
        imwrite(path, norm_0_255(ccap.facesBank[facesBank.size()-1].reshape(1, ccap.facesBank[facesBank.size()-1].rows)));
        cerr << "File :" << path << " saved\n";
    }*/
}


void camcapture::faceprocessing(IplImage *source)
{

    // Perform face detection on the input image, using the given Haar classifier
    if (currentcascade == faceCascade.size())
    {
        overdetect = false;
        addFaceData(faceimg, trackFaces(faceRectsPrev,avgRects, sqrt(facegrey->width*facegrey->width + facegrey->height*facegrey->height), faceTrackMaxDist/100.0), &pathRecords);
        faceRectsPrev = avgRects;
        currentcascade = -1;
        recognitionInProgress = false;
    }
    else
    {
        if (currentcascade == 0)
            cvCvtColor( src, facegrey, CV_RGB2GRAY );
        recognitionInProgress = true;
        for (int i = 0; i < faceAreas[currentcascade].size(); i++)
            delete(&faceAreas[currentcascade][i]);
        faceAreas[currentcascade] = detectFaceInImage(facegrey, faceCascade[currentcascade]);
    }
    if (currentcascade == faceCascade.size()-1)
    {
        avgRects = generateAvgRect(faceAreas, faceCascade.size());
        for (int i = 0; i < faceimg.size(); i++)
        {
            if (faceimg[i] != 0)
                cvReleaseImage(&faceimg[i]);
        }
        faceimg = cropImages(facegrey, avgRects);
        for (int i = 0; i<faceCascade.size();i++)
        {
            faceAreas[i].clear();
        }

        newFace = make_pair(-1, -1);
        if (faceimg.size() != 0)
        {
            if (sleep)
                newFaceLookAtRemained = 0;
            if (faceRectsPrev.size() == 0)
            {
                newFaceLookAtRemained = newFaceLookAtTimeMin + (rand() % (newFaceLookAtTimeMax - newFaceLookAtTimeMin));
                newFace.ST = 0;
            }
            else if (newFaceLookAtRemained > 0)
            {
                newFace.ST = 0;
                newFaceLookAtRemained--;
            }
        }

        if (newFace.ST != -1)
            newFace = make_pair(100-(100*(avgRects[newFace.ST].x+avgRects[newFace.ST].width/2))/facegrey->width, (100*(avgRects[newFace.ST].y+avgRects[newFace.ST].height/2))/facegrey->height);

        if (!faceRecognitionEnabled)
            currentcascade = 0;
        else
            currentcascade++;

        if (faceimg.size() > 0)
        {
            facePresent = true;
            presenceCounter = presenceBufferSize*2;
        }
        else
        {
            facePresent = false;
            if (presenceCounter > 0)
                presenceCounter--;
        }
        //unrotate(faceimg, faceCascade[4]);
        if (!ccap.faceRecognitionEnabled)
        {
            recognitionInProgress = false;
            overdetect = false;
        }
    }
    else
        currentcascade++;
    cerr << currentcascade << " cascade\n";
}

vector <plama> camcapture::splash_detect(bool **input, int min_splash_size)
{
    bool odwiedzone[motionpicsSize.height][motionpicsSize.width];
    plamy.clear();
    REP(x,motionpicsSize.height)
        {
            REP(y,motionpicsSize.width)
            {
                odwiedzone[x][y]=0;
            }
        }
    VPII mozna;
    FOR(x,-1,1)
    {
        FOR(y,-1,1)
        {
            mozna.PB(MP(x,y));
        }
    }

    REP(x,motionpicsSize.height)
    {
        REP(y,motionpicsSize.width)
        {
            if (input[x][y]==1 && !odwiedzone[x][y])
            {
                plama edek;

                VPII stos;
                stos.PB(MP(x,y));

                odwiedzone[x][y]=1;
                while(!stos.empty())
                {
                    PII c=stos.back();
                    stos.pop_back();
                    edek.punkty.PB(c);

                    REP(z,mozna.size())
                    {
                        PII tmp=MP(c.ST+mozna[z].ST,c.ND+mozna[z].ND);
                        if (tmp.ST>=0 && tmp.ST<motionpicsSize.height && tmp.ND>=0 && tmp.ND<motionpicsSize.width && !odwiedzone[tmp.ST][tmp.ND] && input[tmp.ST][tmp.ND]==1)
                        {
                            odwiedzone[tmp.ST][tmp.ND]=1;
                            stos.PB(tmp);
                        }
                    }
                }
                plamy.PB(edek);
            }
        }
    }

    for (int i = 0; i < plamy.size(); i++)
    {
        if (plamy[i].punkty.size() < min_splash_size)
        {
            plamy.erase(plamy.begin()+i);
            i--;
        }
    }
    return plamy;
}

bool camcapture::motion_detect(vector<plama> plamy)
{
    bool catched = false;
    vector < pair <PII, PII> > cmidpoints;

    for (int i = 0; i<plamy.size();i++)
    {
        pair<PII, PII> tmp;
        tmp.ST.ST = tmp.ST.ND = tmp.ND.ST = tmp.ND.ND = 0;
        for (int j = 0; j<plamy[i].punkty.size(); j++)
        {
            tmp.ST.ST += plamy[i].punkty[j].ST;
            tmp.ST.ND += plamy[i].punkty[j].ND;
            //motionmap[plamy[i].punkty[j].ST][plamy[i].punkty[j].ND] = i;
        }
        tmp.ST.ST/=plamy[i].punkty.size();
        tmp.ST.ND/=plamy[i].punkty.size();
        cmidpoints.PB(tmp);
    }

    if (plamy.size() == 0)
        midpoints.clear();
    plamy.clear();

    midpoints.PB(cmidpoints);
    if (midpoints.size() > 2)
    {
        if (midpoints.size() > max_buff)
            midpoints.erase(midpoints.begin());
        for (int i = 1; i < midpoints.size(); i++)
        {
            bucket.clear();
            PII tmp (0,0);
            if (midpoints[i-1].size() > midpoints[i].size())
            {
                for (int j = 0; j < midpoints[i-1].size(); j++)
                {
                    int prevdist = midpoints[i-1][j].ND.ND*(midpoints.size()-1);
                    midpoints[i-1][j].ND.ST = -1;
                    midpoints[i-1][j].ND.ND = max_dist;
                    for (int k = 0; k < midpoints[i].size();k++)
                    {
                        int tmpdist = sqrt((midpoints[i-1][j].ST.ST - midpoints[i][k].ST.ST)*(midpoints[i-1][j].ST.ST - midpoints[i][k].ST.ST) + (midpoints[i-1][j].ST.ND - midpoints[i][k].ST.ND)*(midpoints[i-1][j].ST.ND - midpoints[i][k].ST.ND));
                        if (tmpdist < midpoints[i-1][j].ND.ND)
                        {
                            midpoints[i-1][j].ND.ST = k;
                            midpoints[i-1][j].ND.ND = tmpdist;
                        }
                    }
                    if (midpoints[i-1][j].ND.ST == -1)
                        midpoints[i-1][j].ND.ND = 0;
                    else
                    {
                        midpoints[i-1][j].ND.ND += prevdist;
                        midpoints[i-1][j].ND.ND /= midpoints.size();
                    }
                }
                bucket.resize(midpoints[i].size());
                for (int j = 0; j < midpoints[i].size(); j++)
                    bucket[j] = 0;
                for (int j = 0; j < midpoints[i-1].size(); j++)
                {
                    if (midpoints[i-1][j].ND.ST != -1)
                        bucket[midpoints[i-1][j].ND.ST]++;
                }
                for (int j = 0; j < midpoints[i-1].size(); j++)
                {
                    if (midpoints[i-1][j].ND.ST != -1)
                        midpoints[i][midpoints[i-1][j].ND.ST].ND.ND+=midpoints[i-1][j].ND.ND;
                }
                for (int j = 0; j < midpoints[i].size(); j++)
                {
                    if (bucket[j]!=0)
                        midpoints[i][j].ND.ND/=bucket[j];
                }
            }
            else
            {
                for (int j = 0; j < midpoints[i].size(); j++)
                {
                    //int prevdist = midpoints[i][j].ND.ND;
                    midpoints[i][j].ND.ST = -1;
                    midpoints[i][j].ND.ND = max_dist;
                    for (int k = 0; k < midpoints[i-1].size();k++)
                    {
                        int tmpdist = sqrt((midpoints[i][j].ST.ST - midpoints[i-1][k].ST.ST)*(midpoints[i][j].ST.ST - midpoints[i-1][k].ST.ST) + (midpoints[i][j].ST.ND - midpoints[i-1][k].ST.ND)*(midpoints[i][j].ST.ND - midpoints[i-1][k].ST.ND));
                        if (tmpdist < midpoints[i][j].ND.ND && tmpdist > min_dist);
                        {
                            midpoints[i][j].ND.ST = k;
                            midpoints[i][j].ND.ND = tmpdist;
                        }
                    }
                    //midpoints[i][j].ND.ND += prevdist;
                }
                bucket.resize(midpoints[i-1].size());
                for (int j = 0; j < midpoints[i-1].size(); j++)
                    bucket[j] = 0;
                for (int j = 0; j < midpoints[i].size(); j++)
                {
                    if (midpoints[i][j].ND.ST != -1)
                        bucket[midpoints[i][j].ND.ST]++;
                }
                prevdists.clear();
                prevdists.resize(midpoints[i-1].size());
                for (int j = 0; j < midpoints[i-1].size(); j++)
                    prevdists[j] = midpoints[i-1][j].ND.ND;
                for (int j = 0; j < midpoints[i].size(); j++)
                {
                    if (midpoints[i][j].ND.ST != -1)
                        midpoints[i-1][midpoints[i][j].ND.ST].ND.ND+=midpoints[i][j].ND.ND;// + prevdists[midpoints[i][j].ND.ST];
                }
                for (int j = 0; j < midpoints[i-1].size(); j++)
                {
                    if (bucket[j]!=0)
                        midpoints[i-1][j].ND.ND/=bucket[j];
                }
                for (int j = 0; j < midpoints[i].size(); j++)
                {
                    if (midpoints[i][j].ND.ST != -1)
                    midpoints[i][j].ND.ND=midpoints[i-1][midpoints[i][j].ND.ST].ND.ND;
                }
            }
        }
        int biggest = 0;
        int biggestnum = -1;

        if (midpoints[midpoints.size()-2].size() > midpoints[midpoints.size()-1].size())
        {
            for (int i = 0; i < midpoints[midpoints.size()-1].size(); i++)
            {
                if (midpoints[midpoints.size()-2][i].ND.ND > biggest)
                {
                    biggest = midpoints[midpoints.size()-2][i].ND.ND;
                    biggestnum = i;
                }
            }
            if (biggestnum != -1 && biggest - prevmax > catch_speed)
            {
                //cout << prevmax << " " << midpoints[midpoints.size()-2][biggestnum].ND.ST << " " << midpoints[midpoints.size()-2][biggestnum].ND.ND << " " << midpoints[midpoints.size()-1][biggestnum].ST.ST << "x" << midpoints[midpoints.size()-1][biggestnum].ST.ND << "\n";
                prevmax = biggest;
                timer = 0;
                catched = true;
            }
        }

        else
        {
            for (int i = 0; i < midpoints[midpoints.size()-2].size(); i++)
            {
                if (midpoints[midpoints.size()-1][i].ND.ND > biggest)
                {
                    biggest = midpoints[midpoints.size()-1][i].ND.ND;
                    biggestnum = i;
                }
            }
            if (biggestnum != -1 && biggest - prevmax > catch_speed)
            {
                //cout << prevmax << " " << midpoints[midpoints.size()-1][biggestnum].ND.ST << " " << midpoints[midpoints.size()-1][biggestnum].ND.ND << " " << midpoints[midpoints.size()-1][biggestnum].ST.ST << "x" << midpoints[midpoints.size()-1][biggestnum].ST.ND << "\n" ;
                prevmax = biggest;
                timer = 0;
                catched = true;
            }
        }
        if (biggestnum != -1)
        {
            motionpos.ND = midpoints[midpoints.size()-1][biggestnum].ST.ST;
            motionpos.ST = midpoints[midpoints.size()-1][biggestnum].ST.ND;
        }

        if (timer == forget_timer)
        {
            prevmax = 0;
        }
        else
            timer++;
    }
    if (catched)
        return 1;
    else
        return 0;
}

void camcapture::sleepdetect()
{
    if (1000*motioncounter / (motionpicsSize.height*motionpicsSize.width) < deactive_perc && deactive_timer < deactive_delay && !deactiveworking)
    {
        deactivetimer.start();
        deactiveworking = true;
    }
    if (1000*motioncounter / (motionpicsSize.height*motionpicsSize.width) > active_perc)
    {
        sleep = false;
        deactiveworking = false;
    }
    if (deactivetimer.elapsed() > 1000*deactive_delay && deactiveworking)
        sleep = true;
}

void camcapture::optimize(int last_delay)
{
    fps = 1000.0/(double)last_delay;
    if (!sleep)
    {
        if (HRDWR.owncpu < ccap.max_cpu_usage)
        {
            if (ccap.delay - ccap.delayunit > ccap.mindelay)
                ccap.delay-=ccap.delayunit;
        }
        else
            ccap.delay+=ccap.delayunit;

        if (cpu.load > ccap.deactive_global_cpu_load)
        {
            tmp_halted = true;
        }
        else
            tmp_halted = false;

        if (fps < ccap.min_active_fps)
        {
            if (ccap.retry_times > 0)
            {
                if (!fps_adaptation_timer.isValid())
                    fps_adaptation_timer.restart();
                else if (fps_adaptation_timer.elapsed() > fps_adaptation_time)
                {
                    ccap.retry_times--;
                    tmp_halted = true;
                }
            }
            else
                halted = true;
        }
        else
            fps_adaptation_timer.invalidate();
    }
    else
    {
        if (HRDWR.owncpu < ccap.sleep_cpu_usage)
        {
            if (ccap.sleepdelay - ccap.delayunit > ccap.minsleepdelay)
                ccap.sleepdelay-=ccap.delayunit;
        }
        else
        {
            ccap.sleepdelay+=ccap.delayunit;
        }

        if (cpu.load > ccap.deactive_global_cpu_load)
        {
            tmp_halted = true;
        }
        else
            tmp_halted = false;

        if (fps < ccap.min_sleep_fps)
        {
            if (ccap.retry_times > 0)
            {
                if (!fps_adaptation_timer.isValid())
                    fps_adaptation_timer.restart();
                else if (fps_adaptation_timer.elapsed() > fps_adaptation_time)
                {
                    ccap.retry_times--;
                    tmp_halted = true;
                }
            }
            else
                halted = true;
        }
        fps_adaptation_timer.invalidate();
    }
}

double camcapture::averagecalc(double ifps)
{
    double tmp = (reference_sleep_average - reference_active_average)/(double)(reference_fps - reference_sleepfps);
    if (!sleep)
        tmp = (reference_active_average + (tmp*(double)(reference_fps-fps)));
    else
        tmp = (reference_sleep_average - (tmp*(double)(fps - reference_sleep_average)));
    if (tmp > 0)
        return tmp;
    else
        return 0.01;
}

pixel** camcapture::img2env(IplImage *input)
{
    int step = input->widthStep;
    uchar *data = ( uchar* )input->imageData;

    //bool *output[motionpicsSize.height][motionpicsSize.width];

    for( int i = 0; i < motionpicsSize.height; i++ )
    {
        for( int j = 0 ; j < motionpicsSize.width; j++ )
        {
            env.envmap[i][j].B = data[i*step + j*3 + 0];
            env.envmap[i][j].G = data[i*step + j*3 + 1];
            env.envmap[i][j].R = data[i*step + j*3 + 2];
            //boolimage[i][j] = data[i*step + j];
            //if (data[i*step + j] > 0)
            //    motioncounter++;
        }
    }
    return env.envmap;
}

void camcapture::envread(pixel **input)
{
    env.yellowcounter = 0;
    env.redcounter = 0;
    env.greencounter = 0;
    env.bluecounter = 0;
    env.pinkcounter = 0;
    env.lightcounter = 0;
    env.darkcounter = 0;
    env.greycounter = 0;
    double nextgavg = 0;
    for( int i = 0; i < motionpicsSize.height; i++ )
    {
        for( int j = 0 ; j < motionpicsSize.width; j++ )
        {
            unsigned short avg = ((env.envmap[i][j].R*env.R_correct)/100 + (env.envmap[i][j].G*env.G_correct)/100 + (env.envmap[i][j].B*env.B_correct)/100)/3;
            char sign = '-';
            if (avg > 255-(env.min_tolerance + (1.0 - env.global_avg/255.0)*(double)(env.max_tolerance-env.min_tolerance)))
                sign = '#';
            else if (avg < env.min_tolerance + (env.global_avg/255.0)*(double)(env.max_tolerance-env.min_tolerance))
                sign = ' ';
            else
            {
                if ((env.envmap[i][j].R*env.R_correct)/100 > avg)
                {
                    if ((env.envmap[i][j].R*env.R_correct)/100 > (1.25*env.envmap[i][j].B*env.B_correct)/100) //pink or yellow
                    {
                        if ((env.envmap[i][j].R*env.R_correct)/100 > (1.5*env.envmap[i][j].G*env.G_correct)/100)
                        {
                            sign = 'R';
                        }
                        else
                            sign = 'Y';
                    }
                    else if ((env.envmap[i][j].R*env.R_correct)/100 > (1.5*env.envmap[i][j].G*env.G_correct)/100)
                    {
                        if ((env.envmap[i][j].R*env.R_correct)/100 > (1.25*env.envmap[i][j].B*env.B_correct)/100)
                        {
                            sign = 'R';
                        }
                        else
                            sign = 'P';
                    }
                }
                if ((env.envmap[i][j].B*env.B_correct)/100 > avg) // pink or green
                {
                    if ((env.envmap[i][j].B*env.B_correct)/100 > (1.25*env.envmap[i][j].G*env.G_correct)/100)
                    {
                        if ((env.envmap[i][j].B*env.B_correct)/100 > (1.25*env.envmap[i][j].R*env.R_correct)/100)
                        {
                            sign = 'B';
                        }
                        else
                            sign = 'P';
                    }
                    else if ((env.envmap[i][j].B*env.B_correct)/100 > (1.25*env.envmap[i][j].R*env.R_correct)/100)
                    {
                        if ((env.envmap[i][j].B*env.B_correct)/100 > (1.25*env.envmap[i][j].G*env.G_correct)/100)
                        {
                            sign = 'B';
                        }
                        else
                            sign = 'G';
                    }
                }
                if ((env.envmap[i][j].G*env.G_correct)/100 > avg)
                {
                    if ((env.envmap[i][j].G*env.G_correct)/100 > (1.2*env.envmap[i][j].R*env.R_correct)/100) // blue or yellow
                    {
                        if ((env.envmap[i][j].G*env.G_correct)/100 > (0.9*env.envmap[i][j].B*env.B_correct)/100)
                            sign = 'G';
                        else
                            sign = 'B';
                    }
                    else if ((env.envmap[i][j].G*env.G_correct)/100 > (1.2*env.envmap[i][j].B*env.B_correct)/100)
                    {
                        if ((env.envmap[i][j].G*env.G_correct)/100 > (env.envmap[i][j].R*env.R_correct)/100)
                            sign = 'G';
                        else
                            sign = 'Y';
                    }
                }
            }
            nextgavg+=avg;
            if (sign == 'Y')
                env.yellowcounter++;
            if (sign == 'R')
                env.redcounter++;
            if (sign == 'G')
                env.greencounter++;
            if (sign == 'B')
                env.bluecounter++;
            if (sign == 'P')
                env.pinkcounter++;
            if (sign == '#')
                env.lightcounter++;
            if (sign == ' ')
                env.darkcounter++;
            if (sign == '-')
                env.greycounter++;
        }
    }
    env.Rperc = 100*env.redcounter/env.tabsize;
    env.Yperc = 100*env.yellowcounter/env.tabsize;
    env.Gperc = 100*env.greencounter/env.tabsize;
    env.Bperc = 100*env.bluecounter/env.tabsize;
    env.Pperc = 100*env.pinkcounter/env.tabsize;
    env.Lperc = 100*env.lightcounter/env.tabsize;
    env.Dperc = 100*env.darkcounter/env.tabsize;
    env.Hperc = 100*env.greycounter/env.tabsize;
    env.global_avg=nextgavg/env.tabsize;
    env.colindex = 0;
    double max = 0.0;
    if (env.Rperc > max)
    {
        env.colindex = 1;
        max = env.Rperc;
    }
    if (env.Yperc > max)
    {
        env.colindex = 2;
        max = env.Yperc;
    }
    if (env.Gperc > max)
    {
        env.colindex = 3;
        max = env.Gperc;
    }
    if (env.Bperc > max)
    {
        env.colindex = 4;
        max = env.Bperc;
    }
    if (env.Pperc > max)
    {
        env.colindex = 5;
        max = env.Pperc;
    }
    if (env.Lperc > max)
    {
        env.colindex = 6;
        max = env.Lperc;
    }
    if (env.Dperc > max)
    {
        env.colindex = 7;
        max = env.Dperc;
    }
    if (env.Hperc > max)
    {
        env.colindex = 8;
        max = env.Hperc;
    }
    env.checked = true;
}

void camcapture::funcalc()
{
    bool newenv = 0;
    if (!sleep)
    {
        if (!fun.funtimer.isValid())
            fun.funtimer.restart();
    }
    else
    {
        if (fun.funtimer.isValid())
        {
            fun.funcounter+=fun.funtimer.elapsed();
            fun.funtimer.invalidate();
        }
    }
    if (fun.funchunktimer.elapsed()/1000 > fun.funchunk)
    {
        //cerr << "CHUNK!\n";
        //cerr << sleep << " oldfuncounter: " << fun.funcounter << " ";
        fun.funchunktimer.restart();
        if (fun.funtimer.isValid())
        {
            fun.funcounter+=fun.funtimer.elapsed();
            fun.funtimer.invalidate();
        }
        fun.newfun=(double)fun.funcounter/(double)fun.funchunk/1000;
        if (fun.newfun > fun.minfun/100.0 || newenv)
        {
            fun.totforgettimer.restart();
            fun.fun+=fun.newfun*((double)fun.funchunk/300.0);
            //cerr << "GOOD fun:" << fun.fun << " ";
        }
        else
            fun.fun*=fun.forgetcalm/100.0;
        if (fun.totforgettimer.elapsed()/1000 > fun.totforget)
            fun.fun = 0.0;
       // cerr << "funcounter: " << fun.funcounter << " newfun: " << fun.newfun << "\n";
        fun.funcounter = 0;
    }
}

int camcapture::screensaver_management()
{
    cerr << presenceCounter << " : presence counter\n";
    if (!sleep)
    {
        presenceCounter = presenceBufferSize*2;
        return -1;
    }

    if (faceDetectInSleep)
    {
        if (facePresent)
        {
            presenceCounter = presenceBufferSize*2;
            return -1;
        }
        if (!facePresent)
        {
            if (presenceCounter <= 0)
            {
                return 1;
            }
            if (presenceCounter < presenceBufferSize)
            {
                overdetect = true;
                return 1;
            }
            else if (presenceCounter > 0)
            {
                overdetect = true;
            }
        }
    }
    return 0;
}

bool camcapture::main()
{

    bool retstat = false;
    get_image();
    if (env.timer.elapsed() > env.delay*1000)
    {
        envread(img2env(resized));
        env.timer.restart();
    }
    if (motion_detect(splash_detect(img2bool(get_motionpics(averagecalc(fps), resized)), min_splash_size)))
        retstat = true;
    sleepdetect();
    funcalc();
    if (faceDetectEnabled && ((!sleep || faceDetectInSleep) || recognitionInProgress || overdetect ))
    {
        if (( (!sleep && (detectionTimer.elapsed() >= faceDetectDelay)) || (sleep && (detectionTimer.elapsed() >= faceDetectSleepDelay))) || recognitionInProgress || overdetect )
        {
            faceprocessing(src);
            detectionTimer.restart();
        }
    }
    //cerr << faceDetectMisses << "\n";
    //static int counter = 0;


    if (debug)
    {
        cvShowImage("DISPA", dst);
        cvShowImage("DISPB", resized);
        /*for (int i = 0; i < faceimg.size(); i++)
        {
            stringstream ss;
            ss << i;
            //counter++;
            cvShowImage(&("DISP" + ss.str())[0] , faceimg[i]);
            //ss << "." << counter;
            //cvSaveImage(&("./test/test"+ ss.str() + ".jpg")[0] ,faceimg[i]);
        }*/
        for (int i = 0; i < newFacesImgs.size(); i++)
        {
            stringstream ss;
            ss << i;
            //counter++;
            cvShowImage(&("DISP" + ss.str())[0] , &newFacesImgs[i].ND[newFacesImgs[i].ND.size()-1]);
            //ss << "." << counter;
            //cvSaveImage(&("./test/test"+ ss.str() + ".jpg")[0] ,faceimg[i]);
        }
    }

    return retstat;
}

void camcapture::init_debug()
{
    cvNamedWindow("DISPA", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("DISPB", CV_WINDOW_AUTOSIZE);
}

camthread::camthread( eyes_view * neyes )
{
    timer = new QTimer ( this );
    timer->setInterval ( 100 );
    Configuration * cfg = Configuration::getInstance ();
    eyes = neyes;

    ccap.enabled                    = cfg->lookupValue ( "cam.enabled",                              true );
    ccap.forget_timer               = cfg->lookupValue ( "cam.system.forget_timer",                    10 );
    ccap.min_splash_size            = cfg->lookupValue ( "cam.system.min_splash_size",                 40 );
    ccap.max_buff                   = cfg->lookupValue ( "cam.system.max_buff",                        10 );
    ccap.max_dist                   = cfg->lookupValue ( "cam.system.max_dist",                        40 );
    ccap.min_dist                   = cfg->lookupValue ( "cam.system.min_dist",                        10 );
    ccap.catch_speed                = cfg->lookupValue ( "cam.system.catch_speed",                     50 );
    ccap.motionpicsSize.width       = cfg->lookupValue ( "cam.system.width",                          320 );
    ccap.motionpicsSize.height      = cfg->lookupValue ( "cam.system.height",                         180 );
    ccap.active_perc                = cfg->lookupValue ( "cam.user.activate_motion_percentage",        10 );
    ccap.deactive_perc              = cfg->lookupValue ( "cam.user.deactive_motion_percentage",         5 );
    ccap.deactive_delay             = cfg->lookupValue ( "cam.user.deactive_delay",                    10 );
    ccap.delayunit                  = cfg->lookupValue ( "cam.system.framerate_delay_unit",             5 );
    ccap.max_cpu_usage              = cfg->lookupValue ( "cam.user.max_cpu_usage",                     15 );
    ccap.sleep_cpu_usage            = cfg->lookupValue ( "cam.user.sleep_cpu_usage",                    5 );
    ccap.reference_fps              = cfg->lookupValue ( "cam.system.reference_framerate",             15 );
    ccap.reference_sleepfps         = cfg->lookupValue ( "cam.system.reference_sleep_fps",            0.5 );
    ccap.reference_active_average   = cfg->lookupValue ( "cam.system.active_average",                0.02 );
    ccap.reference_sleep_average    = cfg->lookupValue ( "cam.system.deactive_average",               0.2 );
    ccap.min_active_fps             = cfg->lookupValue ( "cam.user.min_active_fps",                     1 );
    ccap.min_sleep_fps              = cfg->lookupValue ( "cam.user.min_sleep_fps",                    0.5 );
    ccap.fps_adaptation_time        = cfg->lookupValue ( "cam.system.fps_adaptation_time",          10000 );
    ccap.retry_times                = cfg->lookupValue ( "cam.system.temphalt_retry_times",            80 );
    ccap.deactive_global_cpu_load   = cfg->lookupValue ( "cam.system.cpu_halt_load",                   80 );
    ccap.tmp_deactive_timer         = cfg->lookupValue ( "cam.system.cpu_halt_retry_timer",         10000 );
    ccap.mindelay                   = cfg->lookupValue ( "cam.system.min_fps_delay",                   50 );
    ccap.minsleepdelay              = cfg->lookupValue ( "cam.system.min_sleepfps_delay",             150 );
    ccap.operationsarea.ST          = cfg->lookupValue ( "cam.user.view_area_percentage_X",            80 );
    ccap.operationsarea.ND          = cfg->lookupValue ( "cam.user.view_area_percentage_Y",            60 );
    ccap.debug                      = cfg->lookupValue ( "cam.system.showdebug",                    false );
    ccap.env.max_tolerance          = cfg->lookupValue ( "cam.system.env_max_tolerance",               30 );
    ccap.env.min_tolerance          = cfg->lookupValue ( "cam.system.env_min_tolerance",                5 );
    ccap.env.B_correct              = cfg->lookupValue ( "cam.system.env_B_correction",              90.0 );
    ccap.env.G_correct              = cfg->lookupValue ( "cam.system.env_G_correction",             100.0 );
    ccap.env.R_correct              = cfg->lookupValue ( "cam.system.env_R_correction",             100.0 );
    ccap.env.delay                  = cfg->lookupValue ( "cam.system.env_reload_delay",                30 );
    ccap.fun.forgetcalm             = cfg->lookupValue ( "cam.system.fun_calm_percentage",           50.0 );
    ccap.fun.funchunk               = cfg->lookupValue ( "cam.system.fun_chunk_size",                 300 );
    ccap.fun.totforget              = cfg->lookupValue ( "cam.system.fun_total_forget_time",          900 );
    ccap.fun.minfun                 = cfg->lookupValue ( "cam.system.fun_minfun",                    75.0 );
    ccap.minfacesize                = cfg->lookupValue ( "cam.system.min_face_size",                  0.1 );
    ccap.minMergeArea               = cfg->lookupValue ( "cam.system.min_merge_area",                 0.4 );
    ccap.minPosMatch                = cfg->lookupValue ( "cam.system.min_position_match",             0.1 );
    ccap.minSizeMatch               = cfg->lookupValue ( "cam.system.min_size_match",                 0.5 );
    ccap.faceDetectEnabled          = cfg->lookupValue ( "cam.system.face_detect_enabled",           true );
    ccap.faceDetectInSleep          = cfg->lookupValue ( "cam.system.face_detect_when_sleep",        true );
    ccap.faceDetectDelay            = cfg->lookupValue ( "cam.system.face_detect_delay",             2500 );
    ccap.faceDetectSleepDelay       = cfg->lookupValue ( "cam.system.face_detect_sleep_delay",      15000 );
    ccap.presenceBufferSize         = cfg->lookupValue ( "cam.system.face_detect_presence_buffer_size",10 );
    ccap.deactivate_screensaver     = cfg->lookupValue ( "cam.system.screensaver_deactivate",        true );
    ccap.turnoff_screen             = cfg->lookupValue ( "cam.system.screensaver_turn_off_screen",  false );
    ccap.activate_screensaver       = cfg->lookupValue ( "cam.system.screensaver_activate",         false );
    ccap.newFaceLookAtTimeMin       = cfg->lookupValue ( "cam.system.new_face_look_at_min_time",        2 );
    ccap.newFaceLookAtTimeMax       = cfg->lookupValue ( "cam.system.new_face_look_at_max_time",        6 );
    ccap.cascadesPath               = cfg->lookupValue ( "cam.system.face_cascades_dir", "/usr/share/OpenCV/haarcascades/");
    int * cascadesnum = new int;
    *cascadesnum                    = cfg->lookupValue ( "cam.system.face_cascades_number",             4 );
    string emergency_cascades[4];
    emergency_cascades[0] = "haarcascade_frontalface_alt_tree.xml";
    emergency_cascades[1] = "haarcascade_frontalface_alt.xml";
    emergency_cascades[2] = "haarcascade_frontalface_alt2.xml";
    emergency_cascades[3] = "haarcascade_frontalface_default.xml";
    for (cascadesnum; *cascadesnum>0;(*cascadesnum)--)
    {
        stringstream ss;
        ss << *cascadesnum;
        string temp = (ccap.cascadesPath + cfg->lookupValue ( &("cam.system.face_cascade" + ss.str())[0], &emergency_cascades[*cascadesnum-1][0] ));
        ccap.faceCascade.push_back((CvHaarClassifierCascade*)cvLoad( &temp[0], 0, 0, 0));
    }
    delete(cascadesnum);
    ccap.faceRecognitionEnabled     = cfg->lookupValue ( "cam.system.face_recognition_enabled",     false );
    ccap.facesBankPath              = cfg->lookupValue ( "cam.system.faces_bank_dir",            "./faces");
    ccap.faceRecognisePrecision     = cfg->lookupValue ( "cam.system.face_recognition_precision",     0.0 );
    ccap.faceRecognizerTreshold     = cfg->lookupValue ( "cam.system.face_recognition_treshold",   3500.0 );
    ccap.faceImageDropDelay         = cfg->lookupValue ( "cam.system.face_recognise_drop_delay",       15 );
    ccap.newFaceOverdetectSkipSamples  = cfg->lookupValue ( "cam.system.face_recognise_new_face_record_delay",       2 );
    ccap.maxRecognitionBufferSize   = cfg->lookupValue ( "cam.system.max_recognition_buffer_size",     10 );
    ccap.maxFacesPerImg             = cfg->lookupValue ( "cam.system.faces_per_frame_matrix_size",     10 );
    ccap.faceTrackMaxDist           = cfg->lookupValue ( "cam.system.face_track_max_match_dist",     10.0 );
    ccap.facesBankPath = "./faces/";

    if (!ccap.enabled)
        ccap.faceDetectEnabled = false;

    if (!ccap.faceDetectEnabled)
        ccap.faceRecognitionEnabled = false;

    if ( access ( &ccap.facesBankPath[0], R_OK | X_OK ) == -1 )
    {
        if ( -1 == mkdir ( &ccap.facesBankPath[0], S_IRUSR | S_IWUSR | S_IXUSR ) and errno != EEXIST  )
        {
            cerr << "Couldn't create faces dir - disabling recognition";
            ccap.faceRecognitionEnabled = false;
        }
    }

    if (ccap.faceRecognitionEnabled)
    {
        ccap.facesModel = createEigenFaceRecognizer(0, ccap.faceRecognizerTreshold);
        string temp = HRDWR.get_file(&(ccap.facesBankPath + "size")[0] );
        int * faces = new int;
        if (temp != "")
        {
             *faces = atoi(&temp[0]);
             ccap.facesBankQuantities.resize(*faces);
             for (int i = 0; i < *faces; i++ )
             {
                 stringstream ss;
                 ss << i;
                 temp = HRDWR.get_file(&(ccap.facesBankPath + ss.str() + "/" + "size")[0] );
                 if (temp != "")
                 {
                     ccap.facesBankQuantities[i]=atoi(&temp[0]);
                     for (int j = 0; j < atoi(&temp[0]);j++)
                     {
                         stringstream ss2;
                         ss2 << j;
                         ccap.facesBank.push_back(cv::imread(ccap.facesBankPath + ss.str() + "/" + ss2.str()+".jpg", 0));
                         if (ccap.facesBank[ccap.facesBank.size()-1].empty())
                         {
                             cerr << "Empty image loaded... removing from data set\n";
                             ccap.facesBank.erase(ccap.facesBank.begin() + ccap.facesBank.size() - 1);
                             ccap.facesBankQuantities[i]--;
                         }
                         else
                            ccap.facesBankIndex.push_back(i);
                     }
                 }
                 else
                 {
                     ccap.facesBankQuantities[i]=0;
                     cerr << "Bank directory corrupted - setting dir " << i << " to empty dir";
                     HRDWR.set_file(ccap.facesBankPath + ss.str() + "/" + "size", string ("0"));
                 }
             }
             if (ccap.facesBank.size() > 0)
             {
                 ccap.faceRecognitionFirstRun = false;
                 ccap.facesModel->train(ccap.facesBank, ccap.facesBankIndex);
             }
             else
                 ccap.faceRecognitionFirstRun = true;
        }
        else
        {
            cerr << "Faces bank size reading failed - creating new bank";
            HRDWR.set_file(ccap.facesBankPath + "size", string ("0"));
            faces = 0;
            ccap.facesBank.clear();
            ccap.faceRecognitionFirstRun = true;
        }

        cerr << "After data reading:\n" <<
                " faces: " << ccap.facesBankQuantities.size() << "\n";
        for (int i = 0 ; i < ccap.facesBankQuantities.size(); i++)
        {
            cerr << " face " << i << " contains " << ccap.facesBankQuantities[i] << " faces\n";
        }

        delete (faces);
    }

    if (ccap.enabled)
    {
        if (ccap.cam_init())
        {
            ccap.init_motionpics();
            if (ccap.debug)
                ccap.init_debug();
        }
        else
            ccap.enabled = false;
    }
}

void camthread::run()
{
    if (ccap.enabled)
    {
        connect ( timer, SIGNAL ( timeout() ), this, SLOT ( tick() ) );
        cerr << "Capture thread started.\n";
        timer->start();
        speedmeter.start();
        ccap.fps_adaptation_timer.start();
        ccap.env.timer.start();
        ccap.fun.funchunktimer.start();
        ccap.fun.funtimer.start();
        ccap.fun.totforgettimer.start();
    }
}

void camthread::tick()
{
    if (!ccap.halted)
    {
        if(ccap.main())
        {

            ccap.motionpos.ST = 100-(100* ccap.motionpos.ST)/(ccap.motionpicsSize.width);
            ccap.motionpos.ND = (100* ccap.motionpos.ND)/(ccap.motionpicsSize.height);
            eyes->look_at(ccap.motionpos.ST, ccap.motionpos.ND, ccap.operationsarea);
        }
        else if (ccap.newFace.first != -1)
            eyes->look_at(ccap.newFace.ST, ccap.newFace.ND, ccap.operationsarea);
        ccap.optimize(speedmeter.elapsed());
        if (!ccap.sleep)
            timer->setInterval ( ccap.delay );
        else
        {
            timer->setInterval ( ccap.sleepdelay );
        }
    }
    if (ccap.tmp_halted)
    {
        timer->setInterval ( ccap.tmp_deactive_timer );
        cerr << "camera capture temponary halted because of big system cpu load\nCapture restart in: " << ccap.tmp_deactive_timer << " miliseconds...\n";
        ccap.fps_adaptation_timer.invalidate();
    }
    if (ccap.halted)
    {
        disconnect(timer, 0, 0, 0);
        cvReleaseCapture(&ccap.cam);
        cerr << "System is too slow for capture with specified maximum cpu loads - capture deactivated permanently\n";
    }
    speedmeter.start();
}
