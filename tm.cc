#include "tm.hh"
#include "debug.hxx"
#include "configuration.hxx"

#include <QApplication>

ThemeManager * ThemeManager::_instance = 0;

ThemeManager::ThemeManager ()
{
  _dual = false;
}

void ThemeManager::load ( std::string name )
{
  t_name = name;
  cfg.read ( ( std::string ( "./themes/" ) + t_name + "/theme.cfg" ).c_str () );
  if ( not cfg.get ( ".theme.dual", _dual ) )
  {
    warning << "(ThemeManager) missing .theme.dual . Suposing that singular.\n";
    _dual = false;
  }
}

bool ThemeManager::lookupValue ( const char * path, bool def )
{
  bool ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret;
}


int ThemeManager::lookupValue ( const char * path, int def )
{
  long long ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret;
}

char ThemeManager::lookupValue ( const char * path, char def )
{
  std::string ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret[0];
}

double ThemeManager::lookupValue ( const char * path, double def )
{
  double ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return ret;
}

char * ThemeManager::lookupValue ( const char * path, const char * def )
{
  std::string ret;
  if ( not cfg.get ( path, ret ) )
    return Configuration::getInstance ()->lookupValue ( path, def );
  return (char*)ret.c_str ();
}

std::string ThemeManager::color ( std::string name )
{
  std::string ret, def;
  if ( not cfg.get ( std::string ( ".colors.list." ) + name, ret ) )
  {
    warning << "(ThemeManager) color `" << name << "` not on list, falling to default.\n";
    if ( not cfg.get ( ".colors.default", def ) or
         not cfg.get ( std::string ( ".colors.list." ) + def, ret ) )
    {
      error << "(ThemeManager) cannot get default color name or suffix for theme '"
            << t_name << "'.\n";
      qApp->quit ();
    }
  }
  return ret;
}
