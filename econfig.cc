/* Eyes
 * Copyright (C) 2011  Krzysztof Mędrzycki, Damian Chiliński
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

#include "econfig.hxx"
#include <debug.hxx>

#include <iostream>

using namespace std;

Configuration * Configuration::getInstance ()
{
    if ( _cfg == 0 )
        _cfg = new Configuration ();
    return _cfg;
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
        return def;
    }
    return tmp;
}

bool Configuration::setValue (const char *path, bool value)
{
    cfg.lookup ( path ) = value;
    return true;
}

bool Configuration::setValue ( const char *path, char value )
{
    cfg.lookup ( path ) = value;
    return true;
}

bool Configuration::setValue ( const char *path, const char *value )
{
    cfg.lookup ( path ) = value;
    return false;
}

bool Configuration::setValue ( const char *path, int value )
{
    cfg.lookup ( path ) = value;
    return false;
}

void Configuration::save ()
{
    cfg.writeFile ( "config.cfg" );
}
