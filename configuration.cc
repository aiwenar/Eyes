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

#include "configuration.hxx"
#include <debug.hxx>
#include <QStringList>
#include <QString>
#include <iostream>

using namespace std;

Configuration * Configuration::_cfg = 0;

Configuration * Configuration::getInstance ()
{
    if ( _cfg == 0 )
        _cfg = new Configuration ();
    return _cfg;
}

Configuration::Configuration ()
{
  try
  {
    cfg.readFile ( "config.cfg" );
  }
  catch ( ... )
  {
    warning << "(Configuration) configuration read failure, falling back to emergency values.\n";
  }
  needsave = false;
}

bool Configuration::lookupValue ( const char * path, bool def )
{
    bool tmp;
    if ( not cfg.lookupValue ( path, tmp ) )
    {
        error << "lookup for: "
              << path
              << " that isn't any in configuration file. Using default value ("
              << ( def ? "true" : "false" )
              << ")\n";
        setValue ( path, def );
        return def;
    }
    return tmp;
}

char * Configuration::lookupValue ( const char * path, char * def )
{
    std::string tmp;
    if ( not cfg.lookupValue ( &(*path), tmp ) )
    {
        error << "lookup for: "
              << path
              << " that isn't any in configuration file. Using default value ("
              << def
              << ")\n";
        setValue ( path, def );
        return def;
    }
    return const_cast<char*>(tmp.c_str ());
}

char Configuration::lookupValue ( const char * path, char def )
{
    std::string tmp;
    if ( not cfg.lookupValue ( &(*path), tmp ) )
    {
        error << "lookup for: "
              << path
              << " that isn't any in configuration file. Using default value ("
              << def
              << ")\n";
        setValue ( path, def );
        return def;
    }
    return tmp[0];
}

int Configuration::lookupValue ( const char * path, int def )
{
    int tmp;
    if ( not cfg.lookupValue ( &(*path), tmp ) )
    {
        error << "lookup for: "
              << path
              << " that isn't any in configuration file. Using default value ("
              << def
              << ")\n";
        setValue ( path, def );
        return def;
    }
    return tmp;
}

bool Configuration::setValue (const char *path, bool value)
{
    lookup ( path, libconfig::Setting::TypeBoolean ) = value;
    needsave = true;
    return true;
}

bool Configuration::setValue ( const char *path, char value )
{
    lookup ( path, libconfig::Setting::TypeString ) = value;
    needsave = true;
    return true;
}

bool Configuration::setValue ( const char *path, const char *value )
{
    lookup ( path, libconfig::Setting::TypeString ) = value;
    needsave = true;
    return false;
}

bool Configuration::setValue ( const char *path, int value )
{
    lookup ( path, libconfig::Setting::TypeInt ) = value;
    needsave = true;
    return false;
}

void Configuration::save ()
{
  if ( needsave )
  {
    info << "(Configuration) configuration changed, saving.\n";
    cfg.writeFile ( "config.cfg" );
    needsave = false;
  }
  else
    info << "(Configuration) configuration not changed, skipping save.\n";
}

libconfig::Setting& Configuration::lookup ( const char * path, libconfig::Setting::Type type )
{
  if ( not cfg.exists ( path ) )
  {
    libconfig::Setting * set = &cfg.getRoot ();
    QStringList paths = QString ( path ).split ( "." );
    for ( int i=0 ; i<paths.size () ; ++i )
    {
      if ( not set->exists ( paths[i].toStdString () ) )
        set->add ( paths[i].toStdString (), i+1 == paths.size() ? type : libconfig::Setting::TypeGroup );
      set = &((*set)[paths[i].toStdString ()]);
    }
    return *set;
  }
  return cfg.lookup ( path );
}
