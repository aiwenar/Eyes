#include "eyes_view.h"
#include "eyes_info.h"

#include <QPainter>

#include <iostream>

#define MM_NO_MOTION 10000

eyes_view::eyes_view ( QWidget * parent ) : QWidget ( parent )
{
    g_scene = new QGraphicsScene ( this );
    bulwers = normal;
    px = MM_NO_MOTION;
    px = MM_NO_MOTION;
    setMinimumSize ( EYES_W, EYES_H );
    setMaximumSize ( EYES_W, EYES_H );
}

eyes_view::~eyes_view ()
{
    delete g_scene;
}

void eyes_view::open_images ( QString folder )
{
    images.push_back ( QPixmap ( folder+"/#320_eye_std_01.png" ) );
    if ( images[0].isNull () )
    {
        std::cout << "To jest nUlL ;(\n";
    }
}

void eyes_view::paintEvent ( QPaintEvent * event )
{
    QPainter paint ( this );
    paint.drawPixmap ( 0, 0, EYES_W, EYES_H, images[0] );
    paint.drawText ( 0, 0, "Ala ma kota!" );
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
