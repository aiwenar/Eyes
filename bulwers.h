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

};

int operator== (pict_layers& left, int& right )
{
    if ( left.bulwers == right )
        return true;
    else
        return false;
}

#endif // bulwers_h
