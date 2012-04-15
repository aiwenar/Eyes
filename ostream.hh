#ifndef OSTREAM_HH
#define OSTREAM_HH

#include <fstream>

struct msg { typedef enum { end } type; };

//const msg end = { msg::end };

class OStream
{
public:
  OStream ( const char * path, const char * prefix );

  OStream&  operator<<  ( const char * s )  { pstr ( s ); return *this; }
  OStream&  operator<<  ( const int i )     { pint ( i ); return *this; }

private:
  void pstr ( const char * );
  void pint ( const int );

  std::fstream stream;
  bool         enabled;
  bool         onmsg;
  const char * prefix;
};

#endif // OSTREAM_HH
