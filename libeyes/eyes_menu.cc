#include "eyes_menu.hh"

#include <QPainter>
#include <QPaintEvent>

#include "debug.hxx"

eyes_menu::eyes_menu ( QWidget * parent ) :
  QWidget ( parent )
{
  hide ();
  resize ( parent->width (), parent->height () );
  setAttribute ( Qt::WA_MouseTracking, true );
  info << "(eyes_menu) ready!\n";
}

void eyes_menu::paintEvent ( QPaintEvent * ev )
{
  QColor ablack ( 0, 0, 0, 0x60 );
  QColor awhite ( 0xff, 0xff, 0xff, 0x80 );
  QColor ared ( 0xff, 0x11, 0x33, 0x80 );

  QPainter paint ( this );
  paint.fillRect ( 0, 0, width (), height (), ablack );
  // menu
  paint.setPen ( awhite );
  paint.drawText ( 4, 14, "eyes menu" );
  // quit button
  QString sexit ( "quit" );
  if ( quit_hover ) paint.setPen ( ared );
  paint.drawText ( width () - 40, height () - 10, sexit );
  if ( quit_hover ) paint.setPen ( awhite );
  // end
  paint.end ();
}

void eyes_menu::mousePressEvent ( QMouseEvent * ev )
{
  if ( ev->buttons () & Qt::RightButton )
    hide ();
  if ( ev->buttons () & Qt::LeftButton )
  {
    if ( quit_hover ) emit quitting ();
  }
}

void eyes_menu::mouseMoveEvent ( QMouseEvent * ev )
{
  bool n_quit_hover = quit_hover;
  if ( ev->x () < width ()  - 20 and
       ev->y () < height () - 10 and
       ev->x () > width ()  - 40 and
       ev->y () > height () - 20 )
    n_quit_hover = true;
  else
    n_quit_hover = false;
  if ( n_quit_hover != quit_hover )
  {
    quit_hover = n_quit_hover;
    repaint ();
  }
}
