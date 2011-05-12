#include "econfig.hxx"

#include <iostream>

using namespace std;

eConfig::eConfig ( QString fname )
{
    cfg.readFile ( fname );
}

bool eConfig::lookupValue ( char * path, bool def )
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

char * eConfig::lookupValue ( char * path, char * def )
{
    char * tmp;
    if ( not cfg.lookupValue ( path, tmp ) )
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

char eConfig::lookupValue ( char * path, char def )
{
    char * tmp;
    if ( not cfg.lookupValue ( path, tmp ) )
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

int eConfig::lookupValue ( char * path, int def )
{
    int * tmp;
    if ( not cfg.lookupValue ( path, tmp ) )
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
