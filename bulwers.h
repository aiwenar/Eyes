#ifndef _bulwers_h
#define _bulwers_h

#define IMG_NORMAL  0

bool wake_up;
bool get_flu;

struct pict_layers
{
int bulwers;
char * outline;
int eye;
int tired;
int hot;
int shy;
int layer2;
int layer3;
int layer4;

friend int operator== (pict_layers& left, int& right );
friend int operator:/ (pict_layers& a, pict_layers& b );

};
int operator:/ (pict_layers& a, pict_layers& b )
{
    if ( a.bulwers == b.bulwers  && strcmp ( x, y ) == 0 && a.eye == b.eye && a.tired == b.tired && a.hot == b.hot && a.shy == b.shy && a.layer1 == b.layer1 && a.layer2 == b.layer2 && a.layer3 == b.layer3 )
        return true;
    else
        return false;
}
int operator== (pict_layers& left, int& right )
{
    if ( left.bulwers == right )
        return true;
    else
        return false;
}

#endif // bulwers_h
