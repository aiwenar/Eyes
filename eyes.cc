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
#include "tm.hh"
#include "debug.hxx"

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

const double versiond = 0.100000;
const char *verstr = "0.10.0 alpha";
extern camcapture ccap;

bool is_finished;
bool images_ready;

eyes_view::eyes_view ( QWidget * parent,/* QString ncolor, */double size_m )
: QWidget ( parent )
, pics    ()
, eyes    ()
{
    info << "(eyes) preparing...\n";
    layers = new _layer[NUM_LAYERS];
    for ( int i=0 ; i<NUM_LAYERS ; ++i ) layers[i].drawable = false;
    px = MM_NO_MOTION;
    px = MM_NO_MOTION;
    win = parent;
    spec = "spec";
    eye = "eye_04";
    face = "slp_10";
    face_next = "slp_10";
    size_multiplier = size_m;
    // loading configuration
    info << "(eyes) loading config...\n";
    Configuration * cfg = Configuration::getInstance ();
    ThemeManager * tm = ThemeManager::instance ();
    theme = cfg->lookupValue ( ".ui.theme", "default" );
    tm->load ( theme.toStdString () );
    string scolor;
    scolor  =cfg->lookupValue ( ".ui.color",                    "green"         );
    eye_swL = tm->lookupValue ( ".ui.eyeL.size_X",              60              )*size_multiplier;
    eye_shL = tm->lookupValue ( ".ui.eyeL.size_Y",              eye_swL         )*size_multiplier;
    eye_mwL = tm->lookupValue ( ".ui.eyeL.mirror_size_X",       9               )*size_multiplier;
    eye_mhL = tm->lookupValue ( ".ui.eyeL.mirror_size_Y",       eye_mwL         )*size_multiplier;
    eye_swR = tm->lookupValue ( ".ui.eyeR.size_X",              eye_swL         )*size_multiplier;
    eye_shR = tm->lookupValue ( ".ui.eyeR.size_Y",              eye_shL         )*size_multiplier;
    eye_mwR = tm->lookupValue ( ".ui.eyeR.mirror_size_X",       eye_mwL         )*size_multiplier;
    eye_mhR = tm->lookupValue ( ".ui.eyeR.mirror_size_Y",       eye_mhL         )*size_multiplier;
    epx1    = tm->lookupValue ( ".ui.eyeL.posX",                46              )*size_multiplier; //.14375; //46;
    epx2    = tm->lookupValue ( ".ui.eyeR.posX",                223             )*size_multiplier; //.665625; //213;
    mpx1    = tm->lookupValue ( ".ui.eyeL.mirror_pos_X",        83              )*size_multiplier;//.259375;
    mpx2    = tm->lookupValue ( ".ui.eyeR.mirror_pos_X",        (int)(mpx1+(epx2-epx1)) )*size_multiplier;//.7875; //252;
    epy1    = tm->lookupValue ( ".ui.eyeL.posY",                10              )*size_multiplier;//.125; //10;
    epy2    = tm->lookupValue ( ".ui.eyeR.posY",                epy1            )*size_multiplier;//.125; //10;
    mpy1    = tm->lookupValue ( ".ui.eyeL.mirror_pos_Y",        (double)24      )*size_multiplier;//.3; //24;
    mpy2    = tm->lookupValue ( ".ui.eyeR.mirror_pos_Y",        mpy1            )*size_multiplier;//.3; //24;
    color   = tm->color       ( scolor.c_str () ).c_str ();
    dual_eyes = ThemeManager::instance ()->dual ();
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
    open_images ( color.toStdString ().c_str () );
    set_layer ( SLEEPY, "tired_03" );
    toggle_layer ( SLEEPY, true );
    core->load_config ();
    area = new QPixmap ( eyes_w, eyes_h );
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
void _som ( int i, int max ) // I have no idea why i've named it like that.
{
    i ++;
    cerr << "\r\e[K" << _s;
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

#include "img_fileinfo.hh"

void eyes_view::load ( QString folder, QString alt, const char * suffix, const _img_loadinfo files[], int num )
{
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
    int rescaled = access ( ( alt+files[i].file+suffix+".png" ).toStdString ().c_str (), F_OK|R_OK );
    if ( rescaled == 0 )
      file->load ( alt+files[i].file+suffix+".png" );
    else
    {
      if ( -1 == access ( ( folder + files[i].file + suffix + ".png" ).toStdString ().c_str (), F_OK | R_OK ) )
      {
        cerr << "\r\e[K";
        info << "(eyes) file " << ( theme + files[i].file + suffix + ".png" ).toStdString () << " missing.\n";
        no_file = true;
        delete file;
        file = new QPixmap ( 1, 1 );
        QColor transparent ( 0, 0, 0, 0 );
        file->fill ( transparent );
        //break;
      }
      else
      {
        ++numrescaled;
        file->load ( folder + files[i].file + suffix + ".png" );
        QPixmap * filetmp = new QPixmap ( file->scaled ( eyes_w, eyes_h, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation ) );
        delete file;
        file = filetmp;
        file->save ( alt + files[i].file + suffix + ".png" );
      }
    }
    if ( file == 0 )
    {
      warning << "QPixmap associated with file " << files[i].file << " is nil from unknow reason, skipping...\n";
      file = new QPixmap ( 1, 1 );
    }
    for ( int j=0 ; j<files[i].numnames ; ++j )
      pics.insert ( files[i].names[j], file );
  }

  if ( numrescaled > 0 )
    info << "rescaled and saved " << numrescaled << " images.\n";
  if ( no_file )
  {
    //c_main.cancel ();
    //exit ( 2 );
  }
}

void eyes_view::open_images ( const char * color )
{
  _s = "(eyes) loading images...  ";
  const char * ctheme = Configuration::getInstance ()->lookupValue ( ".ui.theme", "default" );
  std::ostringstream oss1, oss2;
  oss1 << "./themes/" << ctheme << '/';
  theme = oss1.str ().c_str ();
  // don't know why, ctheme is modified
  ctheme = Configuration::getInstance ()->lookupValue ( ".ui.theme", "default" );
  if ( access ( theme.toStdString().c_str(), R_OK | X_OK ) == -1 )
  {
    error << "(eyes) theme `" << ctheme << "` does not exists. Trying default one...\n";
    theme = "./themes/default/";
    ctheme = "default";
  }
  oss2 << "./imagetmp/" << ctheme << '/' << eyes_w << 'x' << eyes_h << '/';
  QString alt = oss2.str ().c_str ();

  const _img_loadinfo * eyefls = ( dual_eyes ? eyefiles_dual : eyefiles );
  int numeyes = 10; if ( dual_eyes ) numeyes *= 2;  // KEEP THESE
  load ( theme, alt, "", files, 278 );              //   NUMBERS
  load ( theme, alt, color, eyefls, numeyes );      // = CORRECT =

  images_ready = true;
}

void eyes_view::paintEvent ( QPaintEvent * event )
{
    if (eye.isEmpty())
    {
        eye = "eye_06";
        warning << "eye variable was not set correctly!\n";
    }
    if (face.isEmpty())
    {
        face = "slp_10";
        warning << "face variable was not set correctly!\n";
    }
    QPainter paint ( this );
    QPainter parea ( area );
    area->fill ( QColor ( 0, 0, 0 ) );
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, *pics.value ( face+"_a" ) );
    parea.drawPixmap ( epx1, epy1, eye_swL, eye_shL, *pics.value ( eye + "_L" ) );
    parea.drawPixmap ( epx2, epy2, eye_swR, eye_shR, *pics.value ( eye + "_R" ) );
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, *pics.value ( face+"_s" ) );
    parea.drawPixmap ( int(mpx1), int(mpy1), eye_mwL, eye_mhL, *pics.value ( spec ) );
    parea.drawPixmap ( int(mpx2), int(mpy2), eye_mwR, eye_mhR, *pics.value ( spec ) );
    parea.setCompositionMode(QPainter::CompositionMode_Screen);
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, merge_mirrors(*pics.value ( face+"_m" )) );
    parea.setCompositionMode(QPainter::CompositionMode_SourceOver);
    parea.drawPixmap ( 0, 0, eyes_w, eyes_h, *pics.value ( face+"_o" ) );
    parea.end ();
    area->setMask ( pics[face+"_a"]->mask () );
    for ( int i=0 ; i<NUM_LAYERS ; i++ )
      if ( layers[i].drawable )
        paint.drawPixmap ( 0, 0, eyes_w, eyes_h, *pics.value ( layers[i].face ) );
    paint.drawPixmap ( 0, 0, eyes_w, eyes_h, *area );
}

QPixmap eyes_view::merge_mirrors(QPixmap mask)
{
    QImage workspace = mask.toImage();
    QImage alpha = workspace.alphaChannel();
    char *data = ccap.mir.mirrorL->imageData;
    int h = ccap.mir.mirrorL->height;
    int w = ccap.mir.mirrorL->width;

    for (int y = 0; y < h; y++, data += ccap.mir.mirrorR->widthStep)
    {
        for (int x = 0; x < w; x++)
        {
            int a = qAlpha(workspace.pixel(x+ccap.mir.paintcornerL.ST, y+ccap.mir.paintcornerL.ND))*ccap.mir.alphacorrection;
            if (a>255)
                a=255;
            unsigned char r, g, b;
            r = data[x * 3 + 2];
            b = data[x * 3 + 1];
            g = data[x * 3];
            if (a!=0)
                workspace.setPixel(x+ccap.mir.paintcornerL.ST, y+ccap.mir.paintcornerL.ND, qRgba((r*a)/255, (g*a)/255, (b*a)/255, a));
            else
                workspace.setPixel(x+ccap.mir.paintcornerL.ST, y+ccap.mir.paintcornerL.ND, qRgba(0, 0, 0, a));
        }
    }

    data = ccap.mir.mirrorR->imageData;
    h = ccap.mir.mirrorR->height;
    w = ccap.mir.mirrorR->width;

    for (int y = 0; y < h; y++, data += ccap.mir.mirrorR->widthStep)
    {
        for (int x = 0; x < w; x++)
        {
            int a = qAlpha(workspace.pixel(x+ccap.mir.paintcornerR.ST, y+ccap.mir.paintcornerR.ND))*ccap.mir.alphacorrection;
            if (a>255)
                a=255;
            unsigned char r, g, b;
            r = data[x * 3 + 2];
            b = data[x * 3 + 1];
            g = data[x * 3];
            if (a!=0)
                workspace.setPixel(x+ccap.mir.paintcornerR.ST, y+ccap.mir.paintcornerR.ND, qRgba((r*a)/255, (g*a)/255, (b*a)/255, a));
            else
                workspace.setPixel(x+ccap.mir.paintcornerR.ST, y+ccap.mir.paintcornerR.ND, qRgba(0, 0, 0, a));
        }
    }
    return QPixmap::fromImage(workspace);
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

void eyes_view::look_at ( int px, int py, pair<int, int> operationsarea, int looktime )
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

  looker->interrupt ( totX, totY, looktime );
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
