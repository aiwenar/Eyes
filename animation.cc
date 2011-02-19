#include <cstdlib>

#include "animation.hxx"
#include "defines.hxx"

animation::animation ( int nsize ) : size ( nsize ), act_frane ( 0 )
{
    frames = new char*[size];
    if ( frames == nil )
    {
        cerr << "[\033[31merror \033[0m:] cannot allocate memory.\n";
        exit ( 12 ); // error code 12 : cannot allocate memory
    }
}

animation * animation::add_frame ( char * f )
{
    if ( act_frame+1 > size )
    {
        cerr << "[\033[33mwarning \033[0m:] adding new frame to animation, but there isn't any free slot.\n";
        return;
    }
    frames[act_frame] = f;
    act_frame ++;
    return ( this );
}
