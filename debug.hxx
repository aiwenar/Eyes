#ifndef _eyes_debug_hxx
#define _eyes_debug_hxx

#include <defines.hxx>

#include <fstream>
#include <string>

using namespace std;

class Debug
{
public:
         Debug  ();
         Debug  ( char * file, char * type );
         ~Debug ();

    void open       ( char * nfile );
    void set_str    ( char * ntype );

    Debug& operator<< (  const char* str )
    {
        string s ( str );
        if ( message )
        {
            file << '[' << type << " :] ";
            message = false;
        }
        if ( s[s.size()-1] == '\n' )
            message = true;
        file << s;
        return *this;
    }
    Debug& operator<< ( int in )
    {
        file << in;
        return *this;
    }
    Debug& operator<< ( char c )
    {
        if ( c == '\n' )
            message = false;
        file << c;
        return *this;
    }
    Debug& operator<< ( double d )
    {
        file << d;
        return *this;
    }
    Debug& operator<< ( bool b )
    {
        file << "b:" << ( b ? "true" : "false" );
        return *this;
    }
    Debug& operator<< ( string s )
    {
        if ( message )
        {
            file << '[' << type << " :] ";
            message = false;
        }
        if ( s[s.size()-1] == '\n' )
            message = true;
        file << s;
        return *this;
    }
private:
    bool    message;
    char  * type;
    fstream file;
};

extern Debug info;
extern Debug warning;
extern Debug error;

#endif //eyes_debug_hxx
