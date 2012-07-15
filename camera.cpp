#include "camera.hxx"

camcapture ccap;

bool camcapture::cam_init()
{
    cam = cvCaptureFromCAM(-1);
    src = cvQueryFrame(cam);
}

IplImage* camcapture::get_image()
{
    return cvQueryFrame(cam);
}

void camcapture::init_motionpics()
{
    difference = cvCreateImage ( motionpicsSize, src->depth, src->nChannels);
    resized = cvCreateImage ( motionpicsSize, src->depth, src->nChannels);
    movingAverage = cvCreateImage( motionpicsSize, IPL_DEPTH_32F, src->nChannels);
    dst = cvCreateImage( motionpicsSize, IPL_DEPTH_8U, 1 );
    boolimage = new bool*[src->height];
    for (int i = 0; i < src->height; i++)
    {
        boolimage[i] = new bool[src->width];
    }

    first = true;
    sleep = false;
    timer = 0;
    prevmax = 0;
    deactive_timer = 0;
}

double camcapture::switchmode()
{
    if (!sleep)
    {
        return 0.020;
    }
    else
    {
        usleep (250000);
        return 0.2;
    }
}

IplImage* camcapture::get_motionpics(double tolerance, IplImage *input)
{
    cvResize(input, resized);
    if (first)
    {
        difference = cvCloneImage(resized);
        temp = cvCloneImage(resized);
        cvConvertScale(resized, movingAverage, 1.0, 0.0);
        first = false;
    }
    else
    {
        cvRunningAvg(resized, movingAverage, tolerance, NULL);
    }

    cvConvertScale(movingAverage,temp, 1.0, 0.0);
    cvAbsDiff(resized,temp,difference);
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

pair<int, int> camcapture::motion_detect(vector<plama> plamy)
{
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
            if (biggestnum != -1 && biggest - prevmax > 80)
            {
                //cout << prevmax << " " << midpoints[midpoints.size()-2][biggestnum].ND.ST << " " << midpoints[midpoints.size()-2][biggestnum].ND.ND << " " << midpoints[midpoints.size()-1][biggestnum].ST.ST << "x" << midpoints[midpoints.size()-1][biggestnum].ST.ND << "\n";
                prevmax = biggest;
                timer = 0;
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
            if (biggestnum != -1 && biggest - prevmax > 80)
            {
                //cout << prevmax << " " << midpoints[midpoints.size()-1][biggestnum].ND.ST << " " << midpoints[midpoints.size()-1][biggestnum].ND.ND << " " << midpoints[midpoints.size()-1][biggestnum].ST.ST << "x" << midpoints[midpoints.size()-1][biggestnum].ST.ND << "\n" ;
                prevmax = biggest;
                timer = 0;
            }
        }
        if (timer == forget_timer)
        {
            prevmax = 0;
        }
        else
            timer++;
    }
}

void camcapture::sleepdetect()
{
    if (1000*motioncounter / (motionpicsSize.height*motionpicsSize.width) < deactive_perc && deactive_timer < deactive_delay)
        deactive_timer++;
    if (1000*motioncounter / (motionpicsSize.height*motionpicsSize.width) > active_perc)
    {
        sleep = false;
        deactive_timer = 0;
    }
    if (deactive_timer == deactive_delay)
        sleep = true;
}

/*
int mane()
{
  ccap.forget_timer = 10;
  ccap.min_splash_size = 40;
  ccap.max_buff = 10;
  ccap.max_dist = 40;
  ccap.min_dist = 10;
  ccap.motionpicsSize.width = 320;
  ccap.motionpicsSize.height = 180;
  ccap.active_perc = 10;
  ccap.deactive_perc = 5;
  ccap.deactive_delay = 600;

  ccap.cam_init();

  const char *window = "Example 1";
  cvNamedWindow(window, CV_WINDOW_AUTOSIZE);
  cvNamedWindow("DISP2", CV_WINDOW_AUTOSIZE);

  ccap.init_motionpics();

  while (cvWaitKey(4) == -1)
  {

      ccap.motion_detect(ccap.splash_detect(ccap.img2bool(ccap.get_motionpics(ccap.switchmode(), ccap.get_image())), ccap.min_splash_size));
      ccap.sleepdetect();
      //ccap.splash_detect(ccap.img2bool(ccap.get_motionpics(0.2, ccap.get_image())), ccap.min_splash_size);

    //usleep(50000);
    cvShowImage(window, ccap.dst);
    cvShowImage("DISP2", ccap.resized);
  }

  cvDestroyAllWindows();
  //cvReleaseCapture(&cam);

  return 0;
}
*/
