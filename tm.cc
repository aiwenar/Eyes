/* This file is part of Eyes.
 *
 * Eyes is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Eyes is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Eyes.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright © 2012 Krzysztof Mędrzycki
 */

#include "tm.hh"
#include "debug.hxx"
#include "configuration.hxx"

#include <QApplication>

ThemeManager * ThemeManager::_instance = 0;

ThemeManager::ThemeManager ()
{
  _dual = false;
}

void ThemeManager::load ( std::string name )
{
  t_name = name;
  cfg.read ( ( std::string ( "./themes/" ) + t_name + "/theme.cfg" ).c_str () );
  if ( not cfg.get ( ".theme.dual", _dual ) )
  {
    warning << "(ThemeManager) missing .theme.dual . Suposing that singular.\n";
    _dual = false;
  }
}

bool ThemeManager::lookupValue ( const char * path, bool def )
{
  bool ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret;
}


int ThemeManager::lookupValue ( const char * path, int def )
{
  long long ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret;
}

char ThemeManager::lookupValue ( const char * path, char def )
{
  std::string ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret[0];
}

double ThemeManager::lookupValue ( const char * path, double def )
{
  double ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret;
}

char * ThemeManager::lookupValue ( const char * path, const char * def )
{
  std::string ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return (char*)ret.c_str ();
}

std::string ThemeManager::color ( std::string name )
{
  std::string ret, def;
  if ( not cfg.get ( std::string ( ".colors.list." ) + name, ret ) )
  {
    warning << "(ThemeManager) color `" << name << "` not on list, falling to default.\n";
    if ( not cfg.get ( ".colors.default", def ) or
         not cfg.get ( std::string ( ".colors.list." ) + def, ret ) )
    {
      error << "(ThemeManager) cannot get default color name or suffix for theme '"
            << t_name << "'.\n";
      qApp->quit ();
    }
  }
  return ret;
}
