#include <QtGui>
#include <QApplication>

#include "eyes_view.h"
#include "eyes_info.h"

int main ( int argc, char ** argv )
{
  QApplication app ( argc, argv );

  QWidget win;
  win.resize ( EYES_W, EYES_H );
  win.setWindowTitle( "!Eyesy" );
  win.setAttribute ( Qt::WA_TranslucentBackground, true );
  win.show ();

  eyes_view eyes ( &win );
  eyes.show();

  return app.exec ();
}
