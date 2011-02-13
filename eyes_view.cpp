#include "eyes_view.h"
#include "eyes_info.h"

#include <QPainter>

#include <iostream>
using namespace std;

#define MM_NO_MOTION 10000

eyes_view::eyes_view ( QWidget * parent ) : QWidget ( parent )
{
    px = MM_NO_MOTION;
    px = MM_NO_MOTION;
    epx = 30;
    epy = (EYES_H/2)+(EYE_S/2);
    setMinimumSize ( EYES_W, EYES_H );
    setMaximumSize ( EYES_W, EYES_H );
}

eyes_view::~eyes_view ()
{
		delete out, shadow, mirror, eyein, eye, spec, &px, &py, &epx, &epy;
}

void eyes_view::open_images ( QString folder )
{
    out = new QPixmap ( "out.png" );
    null_protect ( out, "out" );
    shadow = new QPixmap ( "shadow.png" );
    null_protect ( shadow, "shadow" );
    mirror = new QPixmap ( "mirror.png" );
    null_protect ( mirror, "mirror" );
    eyein = new QPixmap ( "eyein.png" );
    null_protect ( eyein, "eye in" );
    eye = new QPixmap ( QString ( "pics/00_eye_01.png" ) );
    null_protect ( eye, "eye" );
    spec = new QPixmap ( "spec.png" );
    null_protect ( spec, "spec" );
}

void eyes_view::paintEvent ( QPaintEvent * event )
{
    QPainter paint ( this );
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, *eyein );
    paint.drawPixmap ( epx-(EYE_S/2), epy-(EYE_S/2), EYE_S, EYE_S, *eye);
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, *shadow );
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, *spec );
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, *out );
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
}

void eyes_view::null_protect ( QPixmap * pix, QString pix_name )
{
    if ( pix->isNull () )
    {
        std::cout << pix_name.toStdString () << " to null.\n";
    }
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
