#include <QtGui>
#include <QApplication>
#include <QtConcurrentRun>

#include <iostream>
#include <string>

#include "eyes_view.h"
#include "eyes_info.h"
#include "event.hxx"

using namespace std;

static const char help [] = {
    "usage: eyes [ARGUMENT [OPTION]]\n\n"
    "Arguments:\n"
    "\t--help -h\t\tPrint this text ang exit.\n"
    "\t--version -v\t\tPrint version and exit.\n"
    "\t--color=COLOR -c COLOR\tSet eyes color to COLOR.\n"
};

void core_main ();
EventQueue * evs;

int main ( int argc, char ** argv )
{
  QApplication app ( argc, argv );

  QStringList arg = app.arguments ();
  QString folder ( "green" );

  for ( int i = 0 ; i < arg.size () ; i++ )
  {
      if ( arg.at ( i ) == "-h" or arg.at ( i ) == "--help" )
      {
          cout << help;
          exit ( 0 );
      }
      else if ( arg.at ( i ) == "-v" or arg.at ( i ) == "--version" )
      {
          cout << "Wersyjka\n";
          exit ( 0 );
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

  QWidget win ( 0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
  win.resize ( EYES_W, EYES_H );
  win.setWindowTitle( "!Eyesy" );
  win.setAttribute ( Qt::WA_TranslucentBackground, true );
  win.show ();

  eyes_view * eyes = new eyes_view ( &win );
  eyes->open_images ( folder );
  eyes->show ();
  eyes->repaint ();
  eyes->update ();

  evs = new EventQueue ();
  
  QFuture<void> c_main = QtConcurrent::run ( core_main );

  return app.exec ();
}
