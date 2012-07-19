/* Eyes
 * Copyright (C) 2011, 2012  Krzysztof Mędrzycki, Damian Chiliński
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui>
#include <QApplication>

#include <iostream>
#include <string>
#include <libconfig.h++>

#include "window.hxx"
#include "debug.hxx"
#include "eyes.hxx"
#include "configurator.hxx"

using namespace std;
using namespace libconfig;

static const char help [] = {
  "usage: eyes [ARGUMENT [OPTION]]\n\n"
  "Arguments:\n"
  "\t--help\t\t-h\t\tPrint this text and exit.\n"
  "\t--version\t-v\t\tPrint version and exit.\n"
  "\t--color=COLOR\t-c COLOR\tSet eyes color to COLOR.\n"
  "\t--config\t\t\tOpen configuration, then quit.\n"
  "\t\t\t\-C\t\tCore only mode.\n"
  "\nSee --help colors for colors.\n"
};

static const char about [] = {
  "eyes Copyright (C) 2011, 2012  Krzysztof Mędrzycki, Damian Chiliński\n"
  "This program comes with ABSOLUTELY NO WARRANTY.\n"
  "This is free software, and you are welcome to redistribute it\n"
  "under certain conditions.\n"
};

static const char help_colors [] = {
    "Avalible colors are:\n"
};

int eye_swL,
    eye_shL,
    eye_swR,
    eye_shR,
    eye_mwL,
    eye_mhL,
    eye_mwR,
    eye_mhR,
    eyes_w,
    eyes_h;

void on_destroy ();

int main ( int argc, char ** argv )
{
  for ( int i=1 ; i<argc ; ++i )
  {
    QString arg ( argv[i] );
    if ( arg == "-C" )
    {
      QCoreApplication app ( argc, argv );
      info << "(main) starting eyes::core only mode.";
      Core * core = new Core ();
      core->core_only_mode = true;
      core->load_config ();
      core->run ();
      return app.exec ();
    }
  }

  QApplication app ( argc, argv );

  QStringList arg = app.arguments ();
  QString color = "";

  for ( int i = 0 ; i < arg.size () ; i++ )
  {
    if ( arg.at ( i ) == "-h" or arg.at ( i ) == "--help" )
      {
        if ( arg.size () > 2 )
        {
          if ( arg.at ( i+1 ) == "colors" )
          {
            cout << help_colors;
            Config cfg;
            try
            {
              cfg.readFile ( "colors.cfg" );
            }
            catch ( libconfig::ParseException e )
            {
              cout << e.what () << '\n' << e.getError () << '\n';
            }
            std::string color;
            int count;
            cfg.lookupValue ( "colors.count", count );
            cout << count << '\n';
            for ( int j=0 ; j<count ; j++ )
            {
              cfg.lookupValue ( ( QString ( "colors.list." ) + j ).toStdString (), color );
              cout << "\t" << color << "\n";
            }
            exit ( 0 );
          }
        }
        cout << help;
        exit ( 0 );
      }
    else if ( arg.at ( i ) == "-v" or arg.at ( i ) == "--version" )
    {
      cout << "eyes " << verstr << '\n';
      exit ( 0 );
    }
    else if ( arg.at ( i ) == "--about" )
    {
      cout << about;
      exit ( 0 );
    }
    else if ( arg.at ( i ).split ( "=" )[0] == "--color" )
      color = arg.at ( i ).split ( "=" )[1];
    else if ( arg.at ( i ) == "-c" )
      color = arg.at ( ++i );
    else if ( arg.at ( i ) == "--config" )
    {
      info << "(main) starting eyes::configmode...\n";
      Configurator * conf = new Configurator ();
      conf->show ();
      return app.exec ();
    }
  }

  info << "(main) starting eyes...\n";
  eyes_window win ( color );
  win.show ();

  return app.exec ();
}
