#include "tm.hh"
#include "debug.hxx"

ThemeManager * ThemeManager::_instance = 0;

ThemeManager::ThemeManager ()
{
  _dual = false;
}

void ThemeManager::load ( const char * name )
{
  /*libconfig::Config cfg;
  try
  {
    cfg.readFile ( ( std::string ( "./themes/" ) + name + "/theme.cfg" ).c_str () );
  }
  catch ( libconfig::FileIOException fioe )
  {
    error << "(ThemeManager) could not load theme's theme.cfg:"
          << '\t' << fioe.what () << '\n';
  }*/
}
