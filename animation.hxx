#ifndef _eyes_animation_hxx
#define _eyes_animation_hxx

class animation
{
public:
                animation ( int nsize );
    animation * add_frame ( char * f );

    char ** frames;
    int     size;
private:
    int     act_frame;
};

#endif //eyes_animation_hxx
