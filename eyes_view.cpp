#include "eyes_view.h"
#include "eyes_info.h"

#include <QPainter>

#include <iostream>

using namespace std;

#define MM_NO_MOTION 10000

static char   folder[]    = "./pics/";
static char * files[]     = {
    "clap_01_a", "clap_01_m", "clap_01_o", "clap_01_s",         // 4
    "clap_02_a", "clap_02_m", "clap_02_o", "clap_02_s",         // 8
    "clap_03_a", "clap_03_m", "clap_03_o", "clap_03_s",         // 12
    "clap_04_a", "clap_04_m", "clap_04_o", "clap_04_s",         // 16
    "clap_05_a", "clap_05_m", "clap_05_o", "clap_05_s",         // 20
    "cusual_01_a", "cusual_01_m", "cusual_01_o", "cusual_01_s", // 24
    "eye_01_n", "eye_02_n", "eye_03_n", "eye_04_n", "eye_05_n", // 29
    "eye_06_n", "eye_07_n", "eye_08_n", "eye_09_n", "eye_10_n", // 34
    "tired_01", "tired_02", "tired_03",                         // 37
    "spec"                                                      // 38
};

eyes_view::eyes_view ( QWidget * parent ) : QWidget ( parent )
{
    px = MM_NO_MOTION;
    px = MM_NO_MOTION;
    epx1 = 46;
    epx2 = 213;
    epy = 10;
    setMinimumSize ( EYES_W, EYES_H );
    setMaximumSize ( EYES_W, EYES_H );
    spec = "spec";
    eye = "eye_06_n";
    out = "cusual_01_o";
    shadow = "cusual_01_s";
}

eyes_view::~eyes_view ()
{
                delete &eye, &pics, &px, &py, &epx1, &epx2, &epy;
}

void eyes_view::open_images ( QString color )
{
    for ( int i=0 ; i<38 ; i++ )
    {
        pics.insert ( QString ( files[i] ), QPixmap ( QString ( folder ) + files[i] + ".png" ) );
        if ( pics[QString ( files[i] ) ].isNull () )
        {
            cerr << "[error :] file " << ( QString ( folder ) + files[i] + ".png" ).toStdString () << " is nil.\n";
        }
    }
}

void eyes_view::paintEvent ( QPaintEvent * event )
{
    QPainter paint ( this );
    paint.drawPixmap ( epx1, epy, EYE_S, EYE_S, pics[eye] );
    paint.drawPixmap ( epx2, epy, EYE_S, EYE_S, pics[eye] );
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, pics[shadow] );
    //paint.drawPixmap ( 0, 0, EYES_W, EYES_H, pics[spec] );
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, pics[out] );
}

void eyes_view::mousePressEvent ( QMouseEvent * ev )
{
    ev->ignore ();
}

void eyes_view::mouseMoveEvent ( QMouseEvent * ev )
{
    ev->accept ();
    if ( px == MM_NO_MOTION or py == MM_NO_MOTION )
    {
        px = ev->x ();
        py = ev->y ();
        return;
    }
    int dx = ( ( px > ev->x () ? -1 : 1 ) * px ) + ev->x ();
    int dy = ( ( py > ev->y () ? -1 : 1 ) * py ) + ev->y ();
    move ( dx, dy );
    repaint ();
}

void eyes_view::closeEvent ( QCloseEvent * ev )
{
    ev->accept ();
}

int eyes_view::heightForWidth ( int w ) const
{
    return ( EYES_H );
}

QVariant eyes_view::inputMethodQuery ( Qt::InputMethodQuery query ) const
{
    return ( nil );
}

QSize eyes_view::sizeHint () const
{
    return ( QSize ( EYES_W, EYES_H ) );
}
