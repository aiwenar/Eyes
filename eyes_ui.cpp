#include <QtGui>
#include <QApplication>
#include <QDebug>
#include <QIODevice>

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

  //QIODevice _iod = new QIODevice ();
  //QDebug dbg = new QDebug( _iod );

  QStringList arg = app.arguments ();
  QString folder ( "green" );
  for ( int i = 0 ; i < arg.size () ; i++ )
  {
      if ( arg.at ( i ) == "--color" )
      {
          folder = arg.at ( i ).split ( "=" )[1];
      }
      else if ( arg.at ( i ) == "-c" )
      {
          i++;
          folder = arg.at ( i );
      }
  }

  qDebug () << folder;

  eyes_view eyes ( &win );
  eyes.show();

  return app.exec ();
}
