#include "econfig.hxx"
#include <debug.hxx>

#include <iostream>

using namespace std;
/*
Configuration::cfgFile::cfgFile ( std::string name )
{
    cfg.readFile ( name.c_str () );
}

Configuration::cfgFile::cfgFile ( const cfgFile & o )
{
    cfg = o.cfg;
}

void Configuration::load ( std::string name )
{
    files.insert ( name, new cfgFile ( name ) );
}
*/
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

Configuration * _cfg;

Configuration::Configuration ()
{
    info << "(Configuration) creating new instance...\n";
    cfg.readFile ( "config.cfg" );
}

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
