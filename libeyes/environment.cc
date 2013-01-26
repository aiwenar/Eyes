#include "environment.hh"
#include "debug.hxx"

#include <QtGlobal>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// OS_LINUX currently covers unix*

#ifdef OS_LINUX
# include <unistd.h>
# include <sys/stat.h>
#endif

Environment * Environment::_instance = 0;
Environment::OS Environment::os =
#ifdef OS_LINUX
    linux
#else
    other
#endif
    ;

Environment * Environment::instance ()
{
  if ( _instance == 0 )
    _instance = new Environment ();
  return _instance;
}

Environment::Environment ()
{
  if ( os == other )
    warning << "unknown OS, Eyes may not work correctly.\n";
  ready = false;
}

void Environment::fail_errno ( const char * msg )
{
  int errno_orig = errno;
  error << msg
        << "\n        [errno " << errno_orig << " : "
        << strerror ( errno_orig ) << " ]\n";
  exit ( 1 );
}

const char * Environment::gethome ()
{
#ifdef OS_LINUX
  return getenv ( "HOME" );
#endif
}

std::string Environment::eyeshome ()
{
#ifdef OS_LINUX
  return std::string ( getenv ( "HOME" ) ) + "/.eyes/";
#endif
}

void Environment::setup ()
{
  if ( ready )
    return;

  info << "setting up environment.\n";

  std::string usrhome = gethome ();

#ifdef OS_LINUX
  usrhome += "/.eyes";
  if ( -1 == access ( usrhome.c_str (), F_OK ) )
    if ( -1 == mkdir ( usrhome.c_str (), 0777 ) )
      fail_errno ( "cannot create ~/.eyes ." );
  std::string imgcache = usrhome + "/imgcache";
  if ( -1 == access ( imgcache.c_str (), F_OK ) )
    if ( -1 == mkdir ( imgcache.c_str (), 0777 ) )
      fail_errno ( "cannot create ~/.eyes/imgcache ." );
#endif

  ready = true;
}

std::string Environment::setup_imgcache ( const char * theme, int w, int h )
{
  std::string cache = gethome ();

#ifdef OS_LINUX
  cache += "/.eyes/imgcache/";
  cache += theme;
  cache += '/';
  if ( -1 == access ( cache.c_str (), F_OK ) )
    if ( -1 == mkdir ( cache.c_str (), 0777 ) )
      fail_errno ( (std::string ( "cannot create " ) + cache + " .").c_str () );
  cache += w;
  cache += 'x';
  cache += h;
  cache += '/';
  if ( -1 == access ( cache.c_str (), F_OK ) )
    if ( -1 == mkdir ( cache.c_str (), 0777 ) )
      fail_errno ( (std::string ( "cannot create " ) + cache + " .").c_str () );
#endif

  return cache;
}
