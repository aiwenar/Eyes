    #include <QtGui>
#include <QApplication>

#include <iostream>
#include <string>

#include "eyes_view.h"
#include "eyes_info.h"

using namespace std;

static const char help [] = {
    "usage: eyes [ARGUMENT [OPTION]]\n\n"
    "Arguments:\n"
    "\t--help -h\t\tPrint this text ang exit.\n"
    "\t--version -v\t\tPrint version and exit.\n"
    "\t--color=COLOR -c COLOR\tSet eyes color to COLOR.\n"
    "\nSee --help colors for colors.\n"
};

static const char help_colors [] = {
    "Avalible colors are:\n"
    "\tgreen ; blue ; brown ; fire ; monochrome ; pink ; WidnowsSpecjal ; ubuntu ; evilgreen\n\n"
};

eyes_view * eyes;

int main ( int argc, char ** argv )
{
  QApplication app ( argc, argv );

  QStringList arg = app.arguments ();
  QString color;

  for ( int i = 0 ; i < arg.size () ; i++ )
  {
      if ( arg.at ( i ) == "-h" or arg.at ( i ) == "--help" )
      {
          if ( arg.at ( i+1 ) == "colors" )
          {
              cout << help_colors;
              exit ( 0 );
          }
          else
          {
            cout << help;
            exit ( 0 );
          }
      }
      else if ( arg.at ( i ) == "-v" or arg.at ( i ) == "--version" )
      {
          cout << "Wersyjka\n";
          exit ( 0 );
      }
      else if ( arg.at ( i ).split ( "=" )[0] == "--color" )
      {
          color = arg.at ( i ).split ( "=" )[1];
      }
      else if ( arg.at ( i ) == "-c" )
      {
          i++;
          color = arg.at ( i );
      }
  }

  QWidget win ( 0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
  win.resize ( EYES_W, EYES_H );
  win.setWindowTitle( "Eyesy" );
  win.setAttribute ( Qt::WA_TranslucentBackground, true );
  win.show ();

  eyes = new eyes_view ( &win, color );
  eyes->show ();
  eyes->repaint ();
  eyes->update ();

  return app.exec ();
}
