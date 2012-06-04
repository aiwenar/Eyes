#ifndef OSTREAM_HH
#define OSTREAM_HH

#include <QString>
#include <string>

struct msg { enum { end } type; };

const msg mend = { msg::end };

class OStream
{
public:
  OStream ( const char * path );

  OStream&  operator<<  ( const char * s );
  OStream&  operator<<  ( const char c );
  OStream&  operator<<  ( const int i );
  OStream&  operator<<  ( const double d );
  OStream&  operator<<  ( const msg m );
  OStream&  operator<<  ( const std::string& s );
  OStream&  operator<<  ( const QString& s );

private:
  bool         enabled;
};

#endif // OSTREAM_HH
