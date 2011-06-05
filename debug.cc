#include <debug.hxx>

Debug::Debug ()
{
}

Debug::Debug ( char * nfile, char * ntype )
{
    fbuf.open ( nfile, ios_base::out );
    stream = new iostream ( fbuf );
    type = ntype;
}

Debug::~Debug ()
{
    file.close ();
}

void Debug::open ( char * nfile )
{
    file.close ();
    file.open ( nfile, ios_base::out );
}

void Debug::set_str ( char * ntype )
{
    type = ntype;
}

Debug info;
Debug warning;
Debug error;
