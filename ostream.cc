#include "ostream.hh"
#include "configuration.hxx"

OStream::OStream ( const char * path, const char * prefix ) :
  prefix ( prefix )
{
  Configuration * cfg = Configuration::getInstance ();
  enabled = cfg->lookupValue ( (QString("debug.")+path+".use").toStdString().c_str(), false );
  char * fp = cfg->lookupValue ( (QString("debug.")+path+".out").toStdString().c_str(), "/dev/null" );
  stream.open ( fp, std::fstream::out );
}

void OStream::pstr ( const char * s )
{
  if ( not onmsg )
  {
    stream << prefix;
    onmsg = true;
  }
  stream << s;
}
