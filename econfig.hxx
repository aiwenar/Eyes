#ifndef _eyes_econfig_hxx
#define _eyes_econfig_hxx

#include <libconfig.h++>
#include <QString>
#include <QMap>

using namespace libconfig;
/*
class Configuration
{
public:
    class cfgFile
    {
    public:
        cfgFile ( std::string name );
        cfgFile ();
        cfgFile ( const cfgFile & );

        int&    operator[]  ( std::string path )
        {

        }

    private:
        Config cfg;
    };

    static  void    load ( std::string name );

    cfgFile&    operator[]  ( QString name )
    {
        return files[name];
    }

private:
    static QMap<QString,cfgFile>   files;
};
*/

/**
 * \p libconfig configuration wrapper.
 */
class eConfig
{
public:

    eConfig                         ( QString fname );
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
    char      * lookupValue         ( const char * path, char * def="\0" );
    /**
     * @return \p libconfig configuration class.
     */
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
