#include <QtGui>
#include <QApplication>

#include <iostream>
#include <string>
#include <libconfig.h++>

#include "eyes_view.h"
#include "eyes_info.h"
#include "eyes_cfg.hxx"

using namespace std;
using namespace libconfig;

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
int eye_s,
    eye_m,
    eyes_w,
    eyes_h;

int main ( int argc, char ** argv )
{
  QApplication app ( argc, argv );

  QStringList arg = app.arguments ();
  QString color = "";

  Config set;
  set.readFile ( "./config.cfg" );

  if ( arg.size () > 1 )
  {
    if ( arg.at ( 1 ) == "config" )
    {
      if ( arg.at ( 2 ) == "list" )
        list_keys ( &set );
      else
      {
        if ( not set.exists ( arg.at ( 2 ).toStdString () ) )
        {
          cout << "[\033[31merror \033[0m:] requested path dosn't exist. See `help config` for help.\n";
          exit ( 126 );
        }
        if ( arg.size () > 3 )
        {

        }
        else
        {
          try
          {
            cout << set.lookup ( arg.at ( 2 ).toStdString () ).c_str () << '\n';
          }
          catch ( SettingTypeException e )
          {
            cout << e.what ();
          }
        }
      }
      exit ( 0 );
    }
  }
  for ( int i = 0 ; i < arg.size () ; i++ )
  {
      if ( arg.at ( i ) == "-h" or arg.at ( i ) == "--help" )
      {
          if ( arg.size () > 2 )
          {
            if ( arg.at ( i+1 ) == "colors" )
            {
              cout << help_colors;
              exit ( 0 );
            }
          }
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
          color = arg.at ( i ).split ( "=" )[1];
      }
      else if ( arg.at ( i ) == "-c" )
      {
          i++;
          color = arg.at ( i );
      }
  }

  if ( not set.lookupValue ( "ui.eye.size", eye_s ) or
       not set.lookupValue ( "ui.eye.mirror", eye_m ) or
       not set.lookupValue ( "ui.eyes.width", eyes_w) or
       not set.lookupValue ( "ui.eyes.height", eyes_h ) )
  {
      cerr << "[\033[31merror \033[0m:] one of sections ui.eye ui.eyes in configuration file is wrong.\n";
      exit ( 126 );
  }

  QWidget win ( 0, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint );
  win.resize ( eyes_w, eyes_h );
  win.setWindowTitle( "Eyesy" );
  win.setAttribute ( Qt::WA_TranslucentBackground, true );
  win.show ();

  eyes = new eyes_view ( &win, color );
  eyes->show ();
  eyes->repaint ();
  eyes->update ();

  return app.exec ();
}
