#ifndef _eyes_animation_hxx
#define _eyes_animation_hxx

/**
 * Class represented eyes animation.
 */
class animation
{
public:
    /**
     * Construct animation for \p nsize frames.
     * @param nsize number of frames in animation.
     */
                animation ( int nsize );
    /**
     * Add frame to animation.
     * @param f frame.
     */
    animation * add_frame ( char * f );

    /// List of animation frames.
    char ** frames;
    /// Noumber of frames in animation.
    int     size;
private:
    int     act_frame;
};

#endif //eyes_animation_hxx
