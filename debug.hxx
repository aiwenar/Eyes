#ifndef _eyes_debug_hxx
#define _eyes_debug_hxx

#include <defines.hxx>

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Debug
{
public:
         Debug  ();
         Debug  ( char * stream, char * type );
         ~Debug ();

    void open       ( char * nstream );
    void set_str    ( char * ntype );

    Debug& operator<< (  const char* str )
    {
        string s ( str );
        if ( message )
        {
            stream << '[' << type << " :] ";
            message = false;
        }
        if ( s[s.size()-1] == '\n' )
            message = true;
        stream << s;
        return *this;
    }
    Debug& operator<< ( int in )
    {
        stream << in;
        return *this;
    }
    Debug& operator<< ( char c )
    {
        if ( c == '\n' )
        {
            message = false;
            stream << c;
        }
        stream.flush ();
        return *this;
    }
    Debug& operator<< ( double d )
    {
        stream << d;
        return *this;
    }
    Debug& operator<< ( bool b )
    {
        stream << "b:" << ( b ? "true" : "false" );
        return *this;
    }
    Debug& operator<< ( string s )
    {
        if ( message )
        {
            stream << '[' << type << " :] ";
            message = false;
        }
        if ( s[s.size()-1] == '\n' )
        {
            message = true;
            stream.flush ();
        }
        stream << s;
        return *this;
    }
private:
    bool        message;
    char      * type;
    streambuf   fbuf;
    iostream    &stream;
};

extern Debug info;
extern Debug warning;
extern Debug error;

#endif //eyes_debug_hxx
