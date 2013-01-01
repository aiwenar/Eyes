#include "ostream.hh"
#include "configuration.hxx"

#include <iostream>

OStream::OStream ( const char * path )
{
  enabled = true;
  enabled = Configuration::getInstance ()->lookupValue ( (QString(".debug.")+path).toStdString().c_str(), true );
}

OStream& OStream::operator << ( const double d )
{
  if ( not enabled ) return *this;
  std::clog << d;
  return *this;
}

OStream& OStream::operator << ( const char * s )
{
  if ( not enabled ) return *this;
  std::clog << s;
  return *this;
}

OStream& OStream::operator << ( const char c )
{
  if ( not enabled ) return *this;
  std::clog << c;
  return *this;
}

OStream& OStream::operator << ( const int i )
{
  if ( not enabled ) return *this;
  std::clog << i;
  return *this;
}

OStream& OStream::operator << ( const bool b )
{
  if ( not enabled ) return *this;
  std::clog << ( b ? "true" : "false" );
  return *this;
}

OStream& OStream::operator << ( const std::string &s )
{
  if ( not enabled ) return *this;
  std::clog << s;
  return *this;
}

OStream& OStream::operator << ( const QString& s )
{
  if ( not enabled ) return *this;
  std::clog << s.toStdString ();
  return *this;
}

namespace eyes_dbg
{

OStream oinfo    ( "info" );
OStream oerror   ( "error" );
OStream owarning ( "warning" );

}
