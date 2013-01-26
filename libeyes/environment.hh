#pragma once

#include <string>

class Environment
{
public:
  static Environment * instance ();

  enum OS
  {
    linux, // currently covers unix*
    other
  };

  void          setup           ();
  std::string   setup_imgcache  ( const char * theme, int w, int h );
  std::string   eyeshome        ();
  const char  * gethome         ();

private:
  Environment ();

  static Environment  * _instance;
  static OS             os;

  void fail_errno ( const char * msg );

  bool  ready;
};
