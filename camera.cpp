#include "camera.hxx"
#include "hardware.hxx"
#include "core.hxx"
#include <unistd.h>

camcapture ccap;
extern hardware HRDWR;
extern percental cpu;

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
    cvResize(cvQueryFrame(cam), resized);
    return resized;
}

void camcapture::init_motionpics()
{
    difference = cvCreateImage ( motionpicsSize, src->depth, src->nChannels);
    resized = cvCreateImage ( motionpicsSize, src->depth, src->nChannels);
    movingAverage = cvCreateImage( motionpicsSize, IPL_DEPTH_32F, src->nChannels);
    dst = cvCreateImage( motionpicsSize, IPL_DEPTH_8U, 1 );
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
            fps_adaptation_timer.restart();
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
        cerr << "CHUNK!\n";
        cerr << sleep << " oldfuncounter: " << fun.funcounter << " ";
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
            cerr << "GOOD fun:" << fun.fun << " ";
        }
        else
            fun.fun*=fun.forgetcalm/100.0;
        if (fun.totforgettimer.elapsed()/1000 > fun.totforget)
            fun.fun = 0.0;
        cerr << "funcounter: " << fun.funcounter << " newfun: " << fun.newfun << "\n";
        fun.funcounter = 0;
    }
}

bool camcapture::main()
{

    bool retstat = false;
    ccap.get_image();
    if (ccap.env.timer.elapsed() > ccap.env.delay*1000)
    {
        ccap.envread(ccap.img2env(resized));
        ccap.env.timer.restart();
    }
    if (ccap.motion_detect(ccap.splash_detect(ccap.img2bool(ccap.get_motionpics(ccap.averagecalc(fps), resized)), ccap.min_splash_size)))
        retstat = true;
    ccap.sleepdetect();
    funcalc();

    if (ccap.debug)
    {
        cvShowImage("DISPA", ccap.dst);
        cvShowImage("DISPB", ccap.resized);
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
    ccap.retry_times                = cfg->lookupValue ( "cam.system.temphalt_retry_times",             8 );
    ccap.deactive_global_cpu_load   = cfg->lookupValue ( "cam.system.cpu_halt_load",                   80 );
    ccap.tmp_deactive_timer         = cfg->lookupValue ( "cam.system.cpu_halt_retry_timer",         30000 );
    ccap.mindelay                   = cfg->lookupValue ( "cam.system.min_fps_delay",                   50 );
    ccap.minsleepdelay              = cfg->lookupValue ( "cam.system.min_sleepfps_delay",             150 );
    ccap.operationsarea.ST          = cfg->lookupValue ( "cam.user.view_area_percentage_X",            80 );
    ccap.operationsarea.ND          = cfg->lookupValue ( "cam.user.view_area_percentage_Y",            60 );
    ccap.debug                      = cfg->lookupValue ( "cam.system.showdebug",                    false );
    ccap.enabled                    = cfg->lookupValue ( "cam.ccap.enabled",                         true );
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
        cerr << "System is too slow for caption with specified maximum cpu loads - capture deactivated permanently\n";
    }
    speedmeter.start();
}
