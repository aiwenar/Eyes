#include "econfig.hxx"

#include <iostream>

using namespace std;

eConfig::eConfig ( QString fname )
{
    cfg.readFile ( fname.toStdString ().c_str () );
}

bool eConfig::lookupValue ( const char * path, bool def )
{
    bool tmp;
    if ( not cfg.lookupValue ( path, tmp ) )
    {
        cerr << "[\033[31merror\033[0m :] lookup for: "
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
        cerr << "[\033[31merror\033[0m :] lookup for: "
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
        cerr << "[\033[31merror\033[0m :] lookup for: "
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
        cerr << "[\033[31merror\033[0m :] lookup for: "
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
