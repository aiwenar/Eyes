/* Eyes
 * Copyright (C) 2011, 2012  Krzysztof Mędrzycki, Damian Chiliński
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eyes.hxx"

#include <debug.hxx>

#include <QPainter>
#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

#define MM_NO_MOTION 10000

static const char * files[]     = {
    "bul_01_a", "bul_01_m", "bul_01_o", "bul_01_s",                 //4
    "bul_02_a", "bul_02_m", "bul_02_o", "bul_02_s",                 //8
    "bul_03_a", "bul_03_m", "bul_03_o", "bul_03_s",                 //12
    "bul_04_a", "bul_04_m", "bul_04_o", "bul_04_s",                 //16
    "bul_05_a", "bul_05_m", "bul_05_o", "bul_05_s",                 //20
    "bul_06_a", "bul_06_m", "bul_06_o", "bul_06_s",                 //24
    "bul_07_a", "bul_07_m", "bul_07_o", "bul_07_s",                 //28
    "bul_08_a", "bul_08_m", "bul_08_o", "bul_08_s",                 //32
    "bul_09_a", "bul_09_m", "bul_09_o", "bul_09_s",                 //36
    "bul_10_a", "bul_10_m", "bul_10_o", "bul_10_s",                 //40
    "bul_11_a", "bul_11_m", "bul_11_o", "bul_11_s",                 //44
    "bul_12_a", "bul_12_m", "bul_12_o", "bul_12_s",                 //48
    "bul_13_a", "bul_13_m", "bul_13_o", "bul_13_s",                 //52
    "bul_14_a", "bul_14_m", "bul_14_o", "bul_14_s",                 //56
    "bul_15_a", "bul_15_m", "bul_15_o", "bul_15_s",                 //60
    "bul_16_a", "bul_16_m", "bul_16_o", "bul_16_s",                 //64
    "clap_01_a", "clap_01_m", "clap_01_o", "clap_01_s",             //68
    "clap_02_a", "clap_02_m", "clap_02_o", "clap_02_s",             //72
    "clap_03_a", "clap_03_m", "clap_03_o", "clap_03_s",             //76
    "clap_04_a", "clap_04_m", "clap_04_o", "clap_04_s",             //80
    "clap_05_a", "clap_05_m", "clap_05_o", "clap_05_s",             //84
    "clap_06_a", "clap_06_m", "clap_06_o", "clap_06_s",             //88
    "clap_07_a", "clap_07_m", "clap_07_o", "clap_07_s",             //92
    "clap_08_a", "clap_08_m", "clap_08_o", "clap_08_s",             //96
    "clap_09_a", "clap_09_m", "clap_09_o", "clap_09_s",             //100
    "cusual_01_a", "cusual_01_m", "cusual_01_o", "cusual_01_s",     //104
    "sh_01_a", "sh_01_m", "sh_01_o", "sh_01_s",                     //108
    "sh_02_a", "sh_02_m", "sh_02_o", "sh_02_s",                     //112
    "slp_01_a", "slp_01_m", "slp_01_o", "slp_01_s",                 //116
    "slp_02_a", "slp_02_m", "slp_02_o", "slp_02_s",                 //120
    "slp_03_a", "slp_03_m", "slp_03_o", "slp_03_s",                 //124
    "slp_04_a", "slp_04_m", "slp_04_o", "slp_04_s",                 //128
    "slp_05_a", "slp_05_m", "slp_05_o", "slp_05_s",                 //132
    "slp_06_a", "slp_06_m", "slp_06_o", "slp_06_s",                 //136
    "slp_07_a", "slp_07_m", "slp_07_o", "slp_07_s",                 //140
    "slp_08_a", "slp_08_m", "slp_08_o", "slp_08_s",                 //144
    "slp_09_a", "slp_09_m", "slp_09_o", "slp_09_s",                 //148
    "slp_10_a", "slp_10_m", "slp_10_o", "slp_10_s",                 //152
    "hpp_01_a", "hpp_01_m", "hpp_01_o", "hpp_01_s",                 //156
    "hpp_02_a", "hpp_02_m", "hpp_02_o", "hpp_02_s",                 //160
    "hpp_03_a", "hpp_03_m", "hpp_03_o", "hpp_03_s",                 //164
    "hpp_04_a", "hpp_04_m", "hpp_04_o", "hpp_04_s",                 //168
    "hpp_05_a", "hpp_05_m", "hpp_05_o", "hpp_05_s",                 //172
    "hpp_06_a", "hpp_06_m", "hpp_06_o", "hpp_06_s",                 //176
    "hpp_07_a", "hpp_07_m", "hpp_07_o", "hpp_07_s",                 //180
    "bclap_01_a", "bclap_01_m", "bclap_01_o", "bclap_01_s",         //184
    "bclap_02_a", "bclap_02_m", "bclap_02_o", "bclap_02_s",         //188
    "bclap_03_a", "bclap_03_m", "bclap_03_o", "bclap_03_s",         //192
    "bclap_04_a", "bclap_04_m", "bclap_04_o", "bclap_04_s",         //196
    "bclap_05_a", "bclap_05_m", "bclap_05_o", "bclap_05_s",         //200
    "bclap_06_a", "bclap_06_m", "bclap_06_o", "bclap_06_s",         //204
    "bclap_07_a", "bclap_07_m", "bclap_07_o", "bclap_07_s",         //208
    "bclap_08_a", "bclap_08_m", "bclap_08_o", "bclap_08_s",         //212
    "bclap_09_a", "bclap_09_m", "bclap_09_o", "bclap_09_s",         //216
    "bclap_10_a", "bclap_10_m", "bclap_10_o", "bclap_10_s",         //220
    "bclap_11_a", "bclap_11_m", "bclap_11_o", "bclap_11_s",         //224
    "bhclap_01_a", "bhclap_01_m", "bhclap_01_o", "bhclap_01_s",     //228
    "bhclap_02_a", "bhclap_02_m", "bhclap_02_o", "bhclap_02_s",     //232
    "bhclap_03_a", "bhclap_03_m", "bhclap_03_o", "bhclap_03_s",     //236
    "bhclap_04_a", "bhclap_04_m", "bhclap_04_o", "bhclap_04_s",     //240
    "bhclap_05_a", "bhclap_05_m", "bhclap_05_o", "bhclap_05_s",     //244
    "bhclap_06_a", "bhclap_06_m", "bhclap_06_o", "bhclap_06_s",     //248
    "bhclap_07_a", "bhclap_07_m", "bhclap_07_o", "bhclap_07_s",     //252
    "bhclap_08_a", "bhclap_08_m", "bhclap_08_o", "bhclap_08_s",     //256
    "bhclap_09_a", "bhclap_09_m", "bhclap_09_o", "bhclap_09_s",     //260
    "bhclap_10_a", "bhclap_10_m", "bhclap_10_o", "bhclap_10_s",     //264
    "bhclap_11_a", "bhclap_11_m", "bhclap_11_o", "bhclap_11_s",     //268
    "tired_01", "tired_02", "tired_03",                             //272
    "spec"
};
static const char * eyefiles[]  = {
    "eye_01", "eye_02", "eye_03", "eye_04", "eye_05",   // 5
    "eye_06", "eye_07", "eye_08", "eye_09", "eye_10",   //10
    "eye_01_L", "eye_02_L", "eye_03_L", "eye_04_L", "eye_05_L",   // 15
    "eye_06_L", "eye_07_L", "eye_08_L", "eye_09_L", "eye_10_L",   // 20
    "eye_01_R", "eye_02_R", "eye_03_R", "eye_04_R", "eye_05_R",   // 25
    "eye_06_R", "eye_07_R", "eye_08_R", "eye_09_R", "eye_10_R",   // 30
    "blank"    // 31
};

const double versiond = 0.090105;
const char *verstr = "0.9.1½ alpha";

bool is_finished;
bool images_ready;

QString get_face_suffix ( QString face )
{
    Config cfg;
    std::string ret;
    try
    {
        cfg.readFile ( "colors.cfg" );
        if ( not cfg.lookupValue ( ( QString ( "colors." ) + face + ".suffix" ).toStdString (), ret ) )
        {
            cfg.lookupValue ( "colors.default", ret );
            if ( not cfg.lookupValue ( ( QString ( "colors." ) + ret.c_str () + ".suffix" ).toStdString (), ret ) )
                return QString ( "_g" );
        }
    }
    catch ( libconfig::ParseException e )
    {
        cerr << "Error while parsing configuration file.\n"
              << e.what () << '\n'
              << e.getFile () << '\n'
              << e.getLine () << '\n'
              << e.getError () << '\n';
        return QString ( "_g" );
    }
    return QString ( ret.c_str () );
}

eyes_view::eyes_view ( QWidget * parent, QString ncolor, double size_m ) : QWidget ( parent )
{
    info << "(eyes) preparing...\n";
    layers = new _layer[NUM_LAYERS];
    px = MM_NO_MOTION;
    px = MM_NO_MOTION;
    win = parent;
    spec = "spec";
    eye = "blank";
    face = "slp_10";
    face_next = "slp_10";
    size_multiplier = size_m;
    // loading configuration
    info << "(eyes) loading config...\n";
    Configuration * cfg = Configuration::getInstance ();
    string scolor;
    scolor  = cfg->lookupValue ( "ui.color",                    "green"         );
    eye_swL = cfg->lookupValue ( "ui.eyeL.size_X",              60              )*size_multiplier;
    eye_shL = cfg->lookupValue ( "ui.eyeL.size_Y",              eye_swL         )*size_multiplier;
    eye_mwL = cfg->lookupValue ( "ui.eyeL.mirror_size_X",       9               )*size_multiplier;
    eye_mhL = cfg->lookupValue ( "ui.eyeL.mirror_size_Y",       eye_mwL         )*size_multiplier;
    eye_swR = cfg->lookupValue ( "ui.eyeR.size_X",              eye_swL         )*size_multiplier;
    eye_shR = cfg->lookupValue ( "ui.eyeR.size_Y",              eye_shL         )*size_multiplier;
    eye_mwR = cfg->lookupValue ( "ui.eyeR.mirror_size_X",       eye_mwL         )*size_multiplier;
    eye_mhR = cfg->lookupValue ( "ui.eyeR.mirror_size_Y",       eye_mhL         )*size_multiplier;
    epx1    = cfg->lookupValue ( "ui.eyeL.posX",                46              )*size_multiplier; //.14375; //46;
    epx2    = cfg->lookupValue ( "ui.eyeR.posX",                213             )*size_multiplier; //.665625; //213;
    mpx1    = cfg->lookupValue ( "ui.eyeL.mirror_posX",         83              )*size_multiplier;//.259375;
    mpx2    = cfg->lookupValue ( "ui.eyeR.mirror_posX",         (int)(mpx1+(epx2-epx1)) )*size_multiplier;//.7875; //252;
    epy1    = cfg->lookupValue ( "ui.eyeL.posY",                10              )*size_multiplier;//.125; //10;
    epy2    = cfg->lookupValue ( "ui.eyeR.posY",                epy1            )*size_multiplier;//.125; //10;
    mpy1    = cfg->lookupValue ( "ui.eyeL.mirror_posY",         24              )*size_multiplier;//.3; //24;
    mpy2    = cfg->lookupValue ( "ui.eyeR.mirror_posY",         mpy1            )*size_multiplier;//.3; //24;
    if ( ( color = get_face_suffix ( ncolor ) ) == "NIL" )
        color = get_face_suffix ( QString ( scolor.c_str () ) );
    is_finished = false;
    images_ready = false;
    // initializing submodules
    info << "(eyes) initializing submodules...\n";
    time = QTime::currentTime ();
    qsrand ( (uint)time.msec () );
    setMinimumSize ( eyes_w, eyes_h );
    setMaximumSize ( eyes_w, eyes_h );
    setMouseTracking ( true );
    clapper = new eyes_clapper ( this );
    looker = new eyes_looker ( this );
    core = new Core ( this );
    core->core_only_mode = false;
    camt = new camthread ( this );
    open_images ( color );
    set_layer ( SLEEPY, "tired_03" );
    toggle_layer ( SLEEPY, true );
    core->load_config ();
    area = new QPixmap ( eyes_w, eyes_h );
    QApplication a();
    screensize.first = QApplication::desktop()->width();
    screensize.second = QApplication::desktop()->height();
    clapper->run ();
    looker->run ();
    camt->start(QThread::IdlePriority);
    core->run ();
}

eyes_view::~eyes_view ()
{
    info << "(eyes) destroying eyes.\n";
}

char * _s;
void _som ( int i, int max )
{
    i ++;
    cerr << "\033[2K\033[100D" << _s;
    if ( i < max )
    {
      std::ostringstream ss;
      ss << '[' << i << '/' << max << ']';
      string s = ss.str ();
      cerr << s;
    }
    else
        cerr << "[ done ]\n";
}

bool eyes_view::load ( QString folder, QString alt, QString suffix, const char ** files, int num )
{
  int rn;
  QStringList qsl = alt.split ( '/' );
  QString dir = "./";

  for ( int i=0 ; i<qsl.size () and qsl.at ( i ) != "" ; ++i )
  {
    dir += '/';
    dir += qsl.at ( i );
    if ( -1 == mkdir ( dir.toStdString ().c_str (), S_IRUSR | S_IWUSR | S_IXUSR ) and errno != EEXIST  )
    {
      int errno_orig = errno;
      error << "(eyes) imagetmp folder creation failed with errno: (" << errno_orig << ") " << strerror ( errno_orig ) << ".\n";
      info << i << ' ' << dir << '\n';
      exit ( 2 );
    }
  }
  QPixmap * file;
  int numrescaled = 0;
  bool no_file = false;
  for ( int i=0 ; i<num ; ++i )
  {
    _som ( i, num );
    file = new QPixmap ();
    int rescaled = access ( ( alt + files[i] + suffix + ".png" ).toStdString ().c_str (), F_OK | R_OK );
    if ( rescaled == 0 )
    {
      file->load ( alt + files[i] + suffix + ".png" );
      pics.insert ( files[i], *file );
      delete file;
    }
    else
    {
      if ( -1 == access ( ( folder + files[i] + suffix + ".png" ).toStdString ().c_str (), F_OK | R_OK ) )
      {
        cerr << '\n';
        info << "(eyes) file " << ( theme + files[i] + suffix + ".png" ).toStdString () << " missing.\n";
        no_file = true;
        //break;
        continue;
      }
      ++numrescaled;
      file->load ( folder + files[i] + suffix + ".png" );
      QPixmap filetmp = file->scaled ( eyes_w, eyes_h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation );
      delete file;
      file = &filetmp;
      pics.insert( files[i] + suffix, *file );
      file->save ( alt + files[i] + suffix + ".png" );
    }
  }

  if ( numrescaled > 0 )
    info << "rescaled and saved " << numrescaled << " images.\n";
  if ( no_file )
  {
    return 0;
    //c_main.cancel ();
    //exit ( 2 );
  }
  else
      return 1;
}

void eyes_view::open_images ( QString color )
{
  _s = "(eyes) loading images...  ";
  const char * ctheme = Configuration::getInstance ()->lookupValue ( "ui.theme", "default" );
  std::ostringstream oss1, oss2;
  oss1 << "./themes/" << ctheme << '/';
  theme = oss1.str ().c_str ();
  // don't know why, ctheme is modified
  ctheme = Configuration::getInstance ()->lookupValue ( "ui.theme", "default" );
  if ( access ( theme.toStdString().c_str(), R_OK | X_OK ) == -1 )
  {
    error << "(eyes) theme `" << ctheme << "` does not exists. Trying default one...\n";
    theme = "./themes/default/";
    ctheme = "default";
  }
  oss2 << "./imagetmp/" << ctheme << '/' << eyes_w << 'x' << eyes_h << '/';
  QString alt = oss2.str ().c_str ();

  load ( theme, alt, "", files, 272 );          // KEEP THESE NUMBERS
  if(!load ( theme, alt, color, eyefiles, 10 )) //   == CORRECT ==
      dual_eyes = true;
  else
      dual_eyes = false;
  load ( theme, alt, color, eyefiles, 30 );
  cerr << dual_eyes << "\n\n\n";

  images_ready = true;
}

void eyes_view::paintEvent ( QPaintEvent * event )
{
    QPainter paint ( this );
    QPainter parea ( area );
    area->fill ( QColor ( 0, 0, 0 ) );
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, pics[face+"_a"] );
    if (!dual_eyes)
    {
        parea.drawPixmap ( epx1, epy1, eye_swL, eye_shL, pics[eye] );
        parea.drawPixmap ( epx2, epy2, eye_swR, eye_shR, pics[eye] );
    }
    else
    {
        parea.drawPixmap ( epx1, epy1, eye_swL, eye_shL, pics[eye + "_L"] );
        parea.drawPixmap ( epx2, epy2, eye_swR, eye_shR, pics[eye + "_R"] );
    }
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, pics[face+"_s"] );
    parea.drawPixmap ( int(mpx1), int(mpy1), eye_mwL, eye_mhL, pics[spec] );
    parea.drawPixmap ( int(mpx2), int(mpy2), eye_mwR, eye_mhR, pics[spec] );
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, pics[face+"_m"] );
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, pics[face+"_o"] );
    parea.end ();
    area->setMask ( pics[face+"_a"].mask () );
    for ( int i=0 ; i<NUM_LAYERS ; i++ )
    {
      if ( layers[i].drawable )
      {
        paint.drawPixmap ( 0, 0, eyes_w, eyes_h, pics[layers[i].face] );
      }
    }
    paint.drawPixmap ( 0, 0, eyes_w, eyes_h, *area );
}

void eyes_view::mousePressEvent ( QMouseEvent * ev )
{
    ev->ignore ();
}

void eyes_view::mouseMoveEvent ( QMouseEvent * ev )
{
    ev->ignore ();
    /*if ( px == MM_NO_MOTION or py == MM_NO_MOTION )
    {
        px = ev->globalX ();
        py = ev->globalY ();
        return;
    }*/
    if ( ev->buttons () & Qt::LeftButton )
    {
      win->move ( ev->globalX (), ev->globalY () );
      repaint ();
      return;
    }
    emit mousemoved ( ev->x (), ev->y () );
}

void eyes_view::enterEvent ( QMouseEvent * ev )
{
  if ( ev->buttons () & Qt::LeftButton ) return;
  emit mouseentered ();
}

void eyes_view::closeEvent ( QCloseEvent * ev )
{
    info << "(eyes) close event recived, exiting...\n";
    ev->accept ();
    is_finished = true;
    c_main.waitForFinished ();
    Configuration::getInstance ()->save ();
    cout << "\033[0m";
}

void eyes_view::set_face ( QString nface )
{
    if ( not pics.contains ( nface+"_a" ) )
    {
        warning << "(eyes) setting face to " << nface.toStdString () << " but it not exists.\n";
        return;
    }
    face = nface;
}

void eyes_view::set_eyes ( QString neyes )
{
    eye = neyes;
}

void eyes_view::set_eyes_position ( int nx1, int nx2, int ny1, int ny2 )
{
    epx1 = nx1;
    epx2 = nx2;
    epy1 = ny1;
    epy2 = ny2;
}

void eyes_view::set_mirror_position ( double nx1, double nx2, double ny1, double ny2 )
{
    mpx1 = nx1;
    mpx2 = nx2;
    mpy1 = ny1;
    mpy2 = ny2;
}

void eyes_view::set_animation ( QString start, QString end, int from, int to )
{
    clapper->set_animation ( start, end, from ,to );
}

void eyes_view::interrupt ()// QString start, QString end, int from, int to )
{
  clapper->interrupt ();// start, end, from ,to );
}

void eyes_view::set_layer ( Layers layer, const char * face )
{
  layers[layer].face = face;
}

void eyes_view::toggle_layer ( Layers layer, bool onoff )
{
    layers[layer].drawable = onoff;
}

void eyes_view::look_at ( int px, int py, pair<int, int> operationsarea )
{
  int winx, winy, percL, percR, percU, percD, totX, totY;
  winx = mapToGlobal(pos()).x() + frameGeometry().width()/2;
  winy = mapToGlobal(pos()).y() + frameGeometry().height()/2;

  winx = 100*(winx - screensize.first/2)/(screensize.first/2);
  winy = 100*(screensize.second/2 - winy)/(screensize.second/2);

  percL = 100 - (100 - operationsarea.first)/2 + winx*((100 - operationsarea.first)/2)/100;
  percR = 100 - (100 - operationsarea.first)/2 - winx*((100 - operationsarea.first)/2)/100;
  percD = 100 - (100 - operationsarea.second)/2 + winy*((100 - operationsarea.second)/2)/100;
  percU = 100 - (100 - operationsarea.second)/2 - winy*((100 - operationsarea.second)/2)/100;
  //cerr << percL << " " << percR << " " << percU << " " << percD << "\n";

  totX = 100-percL + px*(operationsarea.first)/100;
  totY = 100-percU + py*(operationsarea.second)/100;

  looker->interrupt ( totX, totY );
}

int eyes_view::heightForWidth ( int w ) const
{
    return ( eyes_h );
}

QVariant eyes_view::inputMethodQuery ( Qt::InputMethodQuery query ) const
{
    return ( nil );
}

QSize eyes_view::sizeHint () const
{
    return ( QSize ( eyes_w, eyes_h ) );
}

QString eyes_view::get_face ()
{
    return ( face );
}

QString eyes_view::get_face_next ()
{
    return ( face_next );
}
