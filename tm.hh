#pragma once

#include <stackonfigure/config.hh>

class ThemeManager
{
public:
  static inline ThemeManager * instance ()
  {
    if ( _instance == 0 )
      _instance = new ThemeManager ();
    return _instance;
  }

  void load ( std::string name );

  inline bool dual  () { return _dual; }

  /** load value from path from theme.cfg, or from config.cfg if path not present in theme config */
  bool    lookupValue ( const char * path, bool def=false );
  int     lookupValue ( const char * path, int def=0 );
  char    lookupValue ( const char * path, char def='\0' );
  double  lookupValue ( const char * path, double def=0 );
  char *  lookupValue ( const char * path, const char * def );

  std::string color ( std::string name );

private:
  ThemeManager  ();
  static ThemeManager * _instance;

  sc::Config  cfg;
  bool        _dual;
  std::string t_name;
};
