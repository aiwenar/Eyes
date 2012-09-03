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

QMap <const char*,Configuration*> Configuration::instances;

Configuration * Configuration::getInstance ( const char * file )
{
  Configuration * cfg = instances[file];
  if ( cfg == 0 )
  {
    cfg = new Configuration ( file );
    instances[file] = cfg;
  }
  return cfg;
}

Configuration::Configuration ( const char * file )
: cfg   ()
, file  ( file )
{
  cfg.read ( file );
  needsave = false;
}

double Configuration::lookupValue ( const char * path, const double def )
{
  double tmp;
  if ( not cfg.get ( path, tmp ) )
  {
    error << "lookup for: "
          << path
          << " that isn't in configuration file. Using default value("
          << def
          << ")\n";
    setValue ( path, def );
    return def;
  }
  return tmp;
}

bool Configuration::lookupValue ( const char * path, bool def )
{
    bool tmp;
    if ( not cfg.get ( path, tmp ) )
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

char * Configuration::lookupValue ( const char * path, const char * def )
{
    std::string tmp;
    if ( not cfg.get ( &(*path), tmp ) )
    {
        error << "lookup for: "
              << path
              << " that isn't any in configuration file. Using default value ("
              << def
              << ")\n";
        setValue ( path, def );
        return const_cast<char*>(def);
    }
    return const_cast<char*>(tmp.c_str ());
}

char Configuration::lookupValue ( const char * path, char def )
{
    std::string tmp;
    if ( not cfg.get ( &(*path), tmp ) )
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
    long long tmp;
    if ( not cfg.get ( &(*path), tmp ) )
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

bool Configuration::setValue ( const char * path, double value )
{
  lookup ( path, sc::Value::t_float ) = value;
  needsave = true;
  return true;
}

bool Configuration::setValue (const char *path, bool value)
{
    lookup ( path, sc::Value::t_boolean ) = value;
    needsave = true;
    return true;
}

bool Configuration::setValue ( const char *path, char value )
{
  char val[2] = {value,'\0'};
  lookup ( path, sc::Value::t_string ) = (char*)val;
  needsave = true;
  return true;
}

bool Configuration::setValue ( const char *path, const char *value )
{
    lookup ( path, sc::Value::t_string ) = value;
    needsave = true;
    return false;
}

bool Configuration::setValue ( const char *path, int value )
{
    lookup ( path, sc::Value::t_number ) = value;
    needsave = true;
    return false;
}

void Configuration::save ()
{
  if ( needsave )
  {
    info << "(Configuration) configuration changed, saving.\n";
    cfg.save ( file );
    needsave = false;
  }
  else
    info << "(Configuration) configuration not changed, skipping save.\n";
}

sc::Value& Configuration::lookup ( const char * path, sc::Value::Type type )
{
  if ( not cfg.exists ( path ) )
  {
    sc::Value * val = &cfg.root ();
    QStringList paths = QString ( path ).split ( "." );
    int i = 0;
    if ( paths[0] == "" )
      i = 1;
    for ( ; i<paths.size ()-1 ; ++i )
    {
      std::string p = ".";
      p += paths[i].toStdString ();
      if ( not val->exists ( p ) )
      {
        sc::Value * val2 = &val->append ( sc::Value::t_group );
        val2->name ( paths[i].toStdString () );
        val = val2;
      }
      else
        val = &((*val)[p]);
    }
    val = &val->append ( type );
    val->name ( paths[paths.size ()-1].toStdString () );
    return *val;
  }
  return cfg[path];
}
