#include "econfig.hxx"
#include <debug.hxx>

#include <iostream>

using namespace std;

eConfig::eConfig ( QString fname )
{
    try
    {
        cfg.readFile ( fname.toStdString ().c_str () );
    }
    catch ( libconfig::ParseException e )
    {
        error << "Exception thrown while parsing config:\n";
        cerr << e.what ();
    }
}

bool eConfig::lookupValue ( const char * path, bool def )
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

char * eConfig::lookupValue ( const char * path, char * def )
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

char eConfig::lookupValue ( const char * path, char def )
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

int eConfig::lookupValue ( const char * path, int def )
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

Config * eConfig::libconfigConfig()
{
    return &cfg;
}
