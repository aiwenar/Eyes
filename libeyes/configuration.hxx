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

#ifndef _eyes_econfig_hxx
#define _eyes_econfig_hxx

#include <QString>
#include <QMap>
#undef assert /* I have no idea what for, qt is defining 'assert', destroing code of stackonfigured */
#include <stackonfigure/config.hh>

class Configuration
{
public:
  static  Configuration * getInstance ();
  static  Configuration * getInstance ( const char * file );

  /**
   * @return value from \p path if exists, otherwise \p def.
   * @param path path to configuration item.
   * @param def default value, returned if path doesn't exist.
   */
  bool        lookupValue         ( const char * path, bool def=false );
  /**
   * @return value from \p path if exists, otherwise \p def.
   * @param path path to configuration item.
   * @param def default value, returned if path doesn't exist.
   */
  int         lookupValue         ( const char * path, int def=0 );
  /**
   * @return value from \p path if exists, otherwise \p def.
   * @param path path to configuration item.
   * @param def default value, returned if path doesn't exist.
   */
  char        lookupValue         ( const char * path, char def='\0' );
  /**
   * @return value from \p path if exists, otherwise \p def.
   * @param path path to configuration item.
   * @param def default value, returned if path doesn't exist.
   */
  char      * lookupValue         ( const char * path, const char * def="" );
  /**
   * @return value from \p path if exists, otherwise \p def
   * @param path path to configuration item.
   * @param def defaut value, returned if path doesn't exisrs.
   */
  double      lookupValue         ( const char * path, const double def=0 );
  /**
   * Sets value from \p path to \p value.
   * @return true, if set succes, else false.
   * @param path path to value.
   * @param value value to set.
   */
  bool        setValue            ( const char * path, double value );
  /**
   * Sets value from \p path to \p value.
   * @return true, if set succes, else false.
   * @param path path to value.
   * @param value value to set.
   */
  bool        setValue            ( const char * path, bool value );
  /**
   * Sets value from \p path to \p value.
   * @return true, if set succes, else false.
   * @param path path to value.
   * @param value value to set.
   */
  bool        setValue            ( const char * path, int value );
  /**
   * Sets value from \p path to \p value.
   * @return true, if set succes, else false.
   * @param path path to value.
   * @param value value to set.
   */
  bool        setValue            ( const char * path, char value );
  /**
   * Sets value from \p path to \p value.
   * @return true, if set succes, else false.
   * @param path path to value.
   * @param value value to set.
   */
  bool        setValue            ( const char * path, const char * value );
  /** Save changes in configuration */
  void        save                ();

private:
  static QMap <const char*,Configuration*>  instances;

  sc::Value& lookup ( const char * path, sc::Value::Type );

  Configuration ( const char * file );
  ~Configuration();
  sc::Config  cfg;
  bool        needsave;
  char      * file;
};

#endif //eyes_econfig_hxx
