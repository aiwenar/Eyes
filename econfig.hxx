#ifndef _eyes_econfig_hxx
#define _eyes_econfig_hxx

#include <libconfig.h++>
#include <QString>
#include <QMap>

using namespace libconfig;

class eConfig
{
public:
    eConfig                         ( QString fname );
    bool        lookupValue         ( const char * path, bool def=false );
    int         lookupValue         ( const char * path, int def=0 );
    char        lookupValue         ( const char * path, char def='\0' );
    char      * lookupValue         ( const char * path, char * def="" );
    Config    * libconfigConfig    ();
private:
    Config cfg;
};

class Configuration
{
public:
    static  Configuration   *   getInstance ();
    bool        lookupValue         ( const char * path, bool def=false );
    int         lookupValue         ( const char * path, int def=0 );
    char        lookupValue         ( const char * path, char def='\0' );
    char      * lookupValue         ( const char * path, char * def="" );
private:
    Configuration ();
    Config  cfg;
};

#endif //eyes_econfig_hxx
