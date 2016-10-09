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

#pragma once

#include <stackonfigure/config.hh>

class ThemeManager
{
public:
  static inline ThemeManager * instance ()
  {
    if ( _instance == 0 )
      _instance = new ThemeManager ();
    return _instance;
  }

  void load ( std::string name );

  inline bool dual  () { return _dual; }

  /** load value from path from theme.cfg, or from config.cfg if path not present in theme config */
  bool    lookupValue ( const char * path, bool def=false );
  int     lookupValue ( const char * path, int def=0 );
  char    lookupValue ( const char * path, char def='\0' );
  double  lookupValue ( const char * path, double def=0 );
  std::string lookupValue ( const char * path, const char * def );

  std::string color ( std::string name );

private:
  ThemeManager  ();
  static ThemeManager * _instance;

  sc::Config  cfg;
  bool        _dual;
  std::string t_name;
};
