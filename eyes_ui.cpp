#include <QtGui>
#include <QApplication>

#include <iostream>
#include <string>

#include "eyes_view.h"
#include "eyes_info.h"

using namespace std;

int main ( int argc, char ** argv )
{
  QApplication app ( argc, argv );

  QWidget win;
  win.resize ( EYES_W, EYES_H );
  win.setWindowTitle( "!Eyesy" );
  win.setAttribute ( Qt::WA_TranslucentBackground, true );
  win.setAttribute ( Qt::WA_TransparentForMouseEvents, true );
  win.setWindowFlags ( Qt::FramelessWindowHint );
  win.show ();

  QStringList arg = app.arguments ();
  QString folder ( "green" );
  string help ( "" );

  for ( int i = 0 ; i < arg.size () ; i++ )
  {
      if ( arg.at ( i ) == "-h" or arg.at ( i ) == "--help" )
      {
          cout << help;
      }
      else if ( arg.at ( i ).split ( "=" )[0] == "--color" )
      {
          folder = arg.at ( i ).split ( "=" )[1];
      }
      else if ( arg.at ( i ) == "-c" )
      {
          i++;
          folder = arg.at ( i );
      }
  }

  cout << folder.toStdString () << '\n';

  eyes_view eyes ( &win );
  eyes.show();

  return app.exec ();
}
