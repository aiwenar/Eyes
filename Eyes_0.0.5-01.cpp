#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <cmath>
#include <time.h>
#include <string.h>


int bulwers = 0;

bool nadmyszkom = false;
bool doExit = false;
int mousew = 0;
int mouseh = 0;
int PLASK = 0;

int bateria      (int bulwers);
int temperatura (int bulwers);
int oczka       (int bulwers);
int rysuj_cucual (int bulwers);
int rysuj_other  (int bulwers);
int flip         ();
int mruganie    (int bulwers);
int rysuj       (int bulwers);
int bulwersThread ( void * unused );
int special ();
int predkosc ();
void srand ( unsigned int seed );
//void SDL_WM_SetCaption ( const char * name, const char * icon );

bool myszka      (bool nadmyszkom);

SDL_Surface * screen,
            * cucual0,
            * cucual1,
            * cucual2,
            * cucual3,
            * cucual4,
            * cucual5,
            * cucual6,
            * cucual7,
            * cucual8,
            * cucual9,
            * cucual10,
            * cucual11,
            * cucual12,
            * bad1,
            * bad2,
            * bad3,
            * bad4,
            * shocked,
            * sleepy,
            * sleepy2,
            * sleepy3,
            * sleepy4,
            * sleepy5,
            * happy,
            * happy2,
            * evil;
SDL_Rect obraz;

using namespace std;

int main ( int argc, char ** argv ) //to MUSI byc w TEJ postaci
{
        //inicjalizacja----------------------------------
        
        if ( SDL_Init ( SDL_INIT_VIDEO ) < 0 )
        {
          cout << "error SDL_Init : " << SDL_GetError () << "\n";
          return 1;
        }
        screen = SDL_SetVideoMode ( 320, 80, 0, SDL_NOFRAME );
        if ( screen == 0 )
        {
          cout << "error SDL_SetVideoMode : " << SDL_GetError () << "\n";
          return 1;
        }
        
        //czynnosci wstepne------------------------------
        
        SDL_WM_SetIcon ( SDL_LoadBMP ( "./pics/ico.bmp" ) , 0 );
        SDL_WM_SetCaption ( "Eyes", 0 );
	srand (time(NULL));
	
        //wczytyeanie obrazków----------------------------
        //for ( int i = 0; i < argc; i++;) {
        
        if ( strcmp ( "-z", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_green/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_green/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_green/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_green/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_green/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_green/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_green/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_green/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_green/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_green/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_green/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_green/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_green/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_green/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_green/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_green/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_green/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_green/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_green/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_green/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_green/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_green/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_green/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_green/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_green/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_green/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-o", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_orange/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_orange/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_orange/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_orange/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_orange/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_orange/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_orange/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_orange/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_orange/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_orange/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_orange/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_orange/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_orange/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_orange/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_orange/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_orange/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_orange/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_orange/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_orange/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_orange/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_orange/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_orange/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_orange/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_orange/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_orange/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_orange/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-p", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_pink/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_pink/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_pink/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_pink/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_pink/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_pink/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_pink/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_pink/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_pink/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_pink/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_pink/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_pink/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_pink/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_pink/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_pink/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_pink/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_pink/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_pink/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_pink/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_pink/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_pink/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_pink/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_pink/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_pink/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_pink/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_pink/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-a", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_aqua/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_aqua/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_aqua/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_aqua/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_aqua/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_aqua/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_aqua/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_aqua/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_aqua/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_aqua/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_aqua/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_aqua/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_aqua/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_aqua/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_aqua/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_aqua/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_aqua/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_aqua/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_aqua/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_aqua/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_aqua/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_aqua/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_aqua/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_aqua/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_aqua/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_aqua/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-n", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-zd", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_dark_green/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_dark_green/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_dark_green/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_dark_green/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_dark_green/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_dark_green/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_dark_green/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_dark_green/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_dark_green/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_dark_green/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_dark_green/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_dark_green/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_dark_green/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_dark_green/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_dark_green/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_dark_green/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_dark_green/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_dark_green/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_dark_green/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_dark_green/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_dark_green/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_dark_green/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_dark_green/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_dark_green/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_dark_green/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_dark_green/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-od", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_dark_orange/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_dark_orange/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_dark_orange/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_dark_orange/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_dark_orange/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_dark_orange/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_dark_orange/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_dark_orange/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_dark_orange/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_dark_orange/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_dark_orange/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_dark_orange/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_dark_orange/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_dark_orange/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_dark_orange/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_dark_orange/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_dark_orange/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_dark_orange/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_dark_orange/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_dark_orange/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_dark_orange/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_dark_orange/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_dark_orange/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_dark_orange/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_dark_orange/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_dark_orange/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-pd", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_dark_pink/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_dark_pink/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_dark_pink/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_dark_pink/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_dark_pink/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_dark_pink/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_dark_pink/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_dark_pink/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_dark_pink/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_dark_pink/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_dark_pink/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_dark_pink/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_dark_pink/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_dark_pink/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_dark_pink/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_dark_pink/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_dark_pink/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_dark_pink/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_dark_pink/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_dark_pink/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_dark_pink/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_dark_pink/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_dark_pink/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_dark_pink/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_dark_pink/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_dark_pink/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-ad", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_dark_aqua/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_dark_aqua/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_dark_aqua/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_dark_aqua/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_dark_aqua/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_dark_aqua/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_dark_aqua/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_dark_aqua/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_dark_aqua/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_dark_aqua/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_dark_aqua/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_dark_aqua/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_dark_aqua/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_dark_aqua/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_dark_aqua/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_dark_aqua/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_dark_aqua/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_dark_aqua/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_dark_aqua/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_dark_aqua/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_dark_aqua/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_dark_aqua/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_dark_aqua/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_dark_aqua/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_dark_aqua/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_dark_aqua/#320_eye_evil_01.png");
        }
        
        if ( strcmp ( "-n", argv[1] ) == 0 ) {
        
        cucual0 = IMG_Load ("./pics_dark/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics_dark/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics_dark/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics_dark/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics_dark/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics_dark/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics_dark/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics_dark/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics_dark/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics_dark/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics_dark/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics_dark/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics_dark/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics_dark/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics_dark/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics_dark/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics_dark/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics_dark/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics_dark/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics_dark/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics_dark/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics_dark/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics_dark/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics_dark/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics_dark/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics_dark/#320_eye_evil_01.png");
        }
        //}
        
        /*else {
        
        cucual0 = IMG_Load ("./pics/#320_eye_std_01.png");
        cucual1 = IMG_Load ("./pics/#320_eye_std_02.png");
        cucual2 = IMG_Load ("./pics/#320_eye_std_03.png");
        cucual3 = IMG_Load ("./pics/#320_eye_std_04.png");
        cucual4 = IMG_Load ("./pics/#320_eye_std_05.png");
        cucual5 = IMG_Load ("./pics/#320_eye_std_06.png");
        cucual6 = IMG_Load ("./pics/#320_eye_std_07.png");
        cucual7 = IMG_Load ("./pics/#320_eye_std_08.png");
        cucual8 = IMG_Load ("./pics/#320_eye_std_09.png");
        cucual9 = IMG_Load ("./pics/#320_eye_std_10.png");
        cucual10 = IMG_Load ("./pics/#320_eye_std_11.png");
        cucual11 = IMG_Load ("./pics/#320_eye_std_12.png");
        cucual12 = IMG_Load ("./pics/#320_eye_std_13.png");
        bad1 = IMG_Load ("./pics/#320_eye_bad_01.png");
        bad2 = IMG_Load ("./pics/#320_eye_bad_02.png");
        bad3 = IMG_Load ("./pics/#320_eye_bad_03.png");
        bad4 = IMG_Load ("./pics/#320_eye_bad_04.png");
        shocked = IMG_Load ("./pics/#320_eye_schocked.png");
        sleepy = IMG_Load ("./pics/#320_eye_slp_01.png");
        happy = IMG_Load ("./pics/#320_eye_hpp_01.png");
        happy2 = IMG_Load ("./pics/#320_eye_hpp_02.png");
        sleepy2 = IMG_Load ("./pics/#320_eye_slp_02.png");
        sleepy3 = IMG_Load ("./pics/#320_eye_slp_03.png");
        sleepy4 = IMG_Load ("./pics/#320_eye_slp_04.png");
        sleepy5 = IMG_Load ("./pics/#320_eye_slp_05.png");
        evil = IMG_Load ("./pics/#320_eye_evil_01.png");
        }*/
        
        
        obraz.x = 0;
        obraz.y = 0;
        obraz.w = 320;
        obraz.h = 80;

        //wątek-------------------------------------
        SDL_Thread * bT;
        bT = SDL_CreateThread ( bulwersThread, 0 );
        if ( bT == 0 )
        {
          cout << "nie udalo sie zainicjowac wątku : " << SDL_GetError () << '\n';
          return 1;
        }
        //----------------------------------------
        SDL_Event ev;
        bool mous = false;
        while ( !doExit )
        {
                rysuj ( bulwers );
                while ( SDL_PollEvent ( &ev ))
                {
                  if ( ev.type == SDL_MOUSEMOTION )
                    mousew = (ev.motion.x);
                    mouseh = (ev.motion.y);
                    predkosc ();
                    //cout << mousew << " " << mouseh << " | ";
                    nadmyszkom = true;
                  if (ev.type != SDL_MOUSEMOTION ) 
                    nadmyszkom = false;
                  if ( ev.type == SDL_QUIT )
                    doExit = true;
                }
        }
        SDL_WaitThread ( bT, 0 );
}

//---------------------------------------------
int bulwersThread ( void * unused )
{
  while ( !doExit )
  {
  if (bulwers == 7 ) bulwers = 0;
    bulwers = temperatura ( bulwers );
    bulwers = bateria ( bulwers );
    bulwers = oczka ( bulwers );
    special ();
    //cout << "bulwers = " << bulwers << '\n';
    SDL_Delay ( 1000 );
  }
}
int special ()
{
static int petla = 0;
petla++;
if (bulwers == 666) bulwers = 0;
if (petla == 666) bulwers = 666;
}
//----------------------------
int predkosc ()
{
static int srednia_predkosc = 0;
static int proba_predkosci = 0;
static int poprzedni_w = 0;
static int poprzedni_h = 0;
static int najwieksza_predkosc = 0;

proba_predkosci++;

if (proba_predkosci < 5)
        {
        srednia_predkosc += (sqrt (((mousew-poprzedni_w)*(mousew-poprzedni_w)) + ((mouseh-poprzedni_h)*(mouseh-poprzedni_h))));
        }
else
        {
        srednia_predkosc = (srednia_predkosc/5);
        if (srednia_predkosc > najwieksza_predkosc)
                najwieksza_predkosc = srednia_predkosc;
                //cout << najwieksza_predkosc << " ";
        if (najwieksza_predkosc > 15)
                PLASK = 1;
                //cout << najwieksza_predkosc << " ";
        if (najwieksza_predkosc <= 15)
                {
                PLASK = 0;
                //cout << najwieksza_predkosc << " ";
                }
        proba_predkosci = 0;
        najwieksza_predkosc = 0;
        srednia_predkosc = 0;
        }
poprzedni_w = mousew;
poprzedni_h = mouseh;
}

//----------------------------

bool myszka (bool nadmyszkom)
{
static bool dlugomyszka = false;
if (nadmyszkom && !dlugomyszka && bulwers <= 3 && PLASK == 0)
        {
        dlugomyszka = true;
        SDL_BlitSurface (happy, 0, screen, &obraz);
        //SDL_Delay (800);
        return true;
        }
if (nadmyszkom && dlugomyszka && bulwers <= 3 && PLASK == 0)
        {
        SDL_BlitSurface (happy2, 0, screen, &obraz);
        return true;
        }
if (nadmyszkom && !dlugomyszka && bulwers > 3 && bulwers != 7 && PLASK == 0)
        {
        dlugomyszka = true;
        SDL_BlitSurface (bad3, 0, screen, &obraz);
        return true;
        }
if (nadmyszkom && dlugomyszka && bulwers > 3 && bulwers != 7 && PLASK == 0)
        {
        SDL_BlitSurface (bad4, 0, screen, &obraz);
        return true;
        }
if (nadmyszkom && PLASK == 1)
        {
        SDL_BlitSurface (bad4, 0, screen, &obraz);
        return true;
        }
if (!nadmyszkom) 
        {
        dlugomyszka = false;
        return false;
        }
}



//---------------------------

int mruganie (int bulwers)
{
//mruganie

        if ((bulwers >=0 && bulwers <= 2) || bulwers == 9)
        {
        SDL_BlitSurface (sleepy, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        SDL_BlitSurface (sleepy2, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        SDL_BlitSurface (sleepy3, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        SDL_BlitSurface (sleepy2, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        SDL_BlitSurface (sleepy, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        }
        if ((bulwers > 2 && bulwers <= 5) || bulwers == 8 || bulwers == 666)
        {
        SDL_BlitSurface (sleepy4, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        SDL_BlitSurface (sleepy5, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        SDL_BlitSurface (sleepy4, 0, screen, &obraz);
        SDL_Flip (screen);
        SDL_Delay (50);
        }
        return bulwers;
}
//--------------------------

int rysuj_cucual (int bulwers)
{
static int lookr = 0;
static int cucual_mrug = rand () % 5;
        //rysowanie minek
        for (int i=0; i<1; i++){
        lookr ++;
        if (lookr < cucual_mrug)
                {
                if (bulwers == 0){
                SDL_BlitSurface (cucual0, 0, screen, &obraz);}
                if (bulwers == 1){
                SDL_BlitSurface (cucual1, 0, screen, &obraz);}
                if (bulwers == 2){
                SDL_BlitSurface (cucual2, 0, screen, &obraz);}
                }
        if (lookr == cucual_mrug)
                {
                static int kolejka = 0;
                static int spojrzenie = 0;
                if ((spojrzenie*(spojrzenie + 3)) < 9){
                        spojrzenie++;
                        if (kolejka == 0)
                        {
                        kolejka++;
                        SDL_BlitSurface (cucual11, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        if (kolejka == 1)
                        {
                        kolejka++;
                        SDL_BlitSurface (cucual9, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        else
                        {
                        kolejka = 0;
                        SDL_BlitSurface (cucual10, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        //cout << spojrzenie << " " << kolejka << endl;
                }
                if ((spojrzenie*(spojrzenie + 3)) == 18){
                        spojrzenie = 0;
                        if (kolejka == 0)
                        {
                        kolejka++;
                        SDL_BlitSurface (cucual4, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        if (kolejka == 1)
                        {
                        kolejka++;
                        SDL_BlitSurface (cucual6, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        else
                        {
                        kolejka = 0;
                        SDL_BlitSurface (cucual5, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        break;
                }
                if ((spojrzenie*(spojrzenie + 3)) >= 9){
                        spojrzenie++;
                        if (kolejka == 0)
                        {
                        kolejka++;
                        SDL_BlitSurface (cucual7, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        if (kolejka == 1)
                        {
                        kolejka++;
                        SDL_BlitSurface (cucual8, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                        else
                        {
                        kolejka = 0;
                        SDL_BlitSurface (cucual3, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                }
                
                else {
                if (kolejka == 0)
                        {
                        kolejka++;
                        SDL_BlitSurface (cucual11, 0, screen, &obraz);
                        //cout << spojrzenie << " " << kolejka << endl;
                        break;
                        }
                else{
                kolejka = 0;
                SDL_BlitSurface (cucual1, 0, screen, &obraz);
                spojrzenie++;
                }
                }
                }
                }
                if (lookr > cucual_mrug){
                lookr = 0;
                cucual_mrug = rand () % 5;
                        if (bulwers == 0 || bulwers == 1){
                        SDL_BlitSurface (cucual0, 0, screen, &obraz);}
                        if (bulwers == 2){
                        SDL_BlitSurface (cucual1, 0, screen, &obraz);}
                }
}
                
//---------------------------

int rysuj_other (int bulwers)
{

                if (bulwers == 3)
                {
                SDL_BlitSurface (bad1, 0, screen, &obraz);
                }
                if (bulwers == 4)
                {
                SDL_BlitSurface (bad2, 0, screen, &obraz);
                }
                if (bulwers == 5)
                {
                SDL_BlitSurface (bad3, 0, screen, &obraz);
                }
                if (bulwers == 6)
                {
                SDL_BlitSurface (bad4, 0, screen, &obraz);
                }
                if (bulwers == 7)
                {
                SDL_BlitSurface (shocked, 0, screen, &obraz);
                }
                if (bulwers == 8)
                {
                SDL_BlitSurface (sleepy, 0, screen, &obraz);
                }
                if (bulwers == 9)
                {
                SDL_BlitSurface (happy, 0, screen, &obraz);
                }
                if (bulwers == 666)
                {
                SDL_BlitSurface (evil, 0, screen, &obraz);
                }
                return bulwers;
}
//----------------------------

int flip ()
{
                SDL_Flip (screen);
}

//----------------------------

int rysuj (int bulwers)
{
static int poprzedni = 0;
static int mrugniecie = 0;
static int losowosc = 25;
mrugniecie++;

if ((mrugniecie >= losowosc || poprzedni != bulwers) && !myszka (nadmyszkom))
        {
        losowosc = ((rand () %30 ) + 5);
        //cout << losowosc << endl;
        poprzedni = bulwers;
        mrugniecie = 0;
        mruganie (bulwers);
        if (bulwers < 3) {rysuj_cucual (bulwers);}
        if (bulwers >=3) {rysuj_other (bulwers);}
        flip ();
}
if (myszka (nadmyszkom)) 
        {flip ();}
if (PLASK == 1) SDL_Delay (800);
SDL_Delay (200);
}
//-----------------------------

int temperatura (int bulwers)
{
        fstream calosc ("/proc/acpi/thermal_zone/TZ00/temperature", fstream::in);

        string texcik;
        while (calosc.good())
                texcik+=calosc.get();

        int tempjuczer=texcik.find_first_of ("temperature");

        int i=0;
        int a=0;
        char temp[27];

        for (;i<27;i++) {
                temp[a] = texcik[25+a];
                a++;
                }

        int nap;
        nap = atoi (temp);
        static int temperaturestate;
        
        if(temperaturestate != nap) {
                temperaturestate = nap;
                if (nap>64) {
                        cout << "SHUT DOWN NOW!!!"<<endl;
                        if (bulwers<6)
                                bulwers = 6;
                }
        }

        if (nap<64 && nap>=62) {
                cout << "CRITICAL TEMPERATURE (" << (nap+273) << " K " << nap << " ºC)"<<endl<<"-_↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑ ↑_-"<<endl<<"-_↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓_-"<<endl;
                if (bulwers<5)
                        bulwers = 5;
        }
        
        if (nap<62 && nap>=59) {
                cout << "WARN!!! Very high temperature! (" << (nap+273) << " K " << nap << " ºC)"<<endl<<"-------"<<endl;
                if (bulwers<4)
                        bulwers = 4;
        }
        
        if (nap<59 && nap>=57) {
                cout << "Risk -- higher temperature (" << (nap+273) << " K " << nap << " ºC)"<<endl<<"-------"<<endl;
                if (bulwers<3)
                        bulwers = 3;
        }
        if (nap<57 && nap>=56) {
                cout << "Temperature OK (" << (nap+273) << " K " << nap << " ºC)"<<endl<<"-------"<<endl;
                if (bulwers<2)
                        bulwers = 2;
        }
        if (nap<56 && nap>=55) {
                cout << "Temperature OK (" << (nap+273) << " K " << nap << " ºC)"<<endl<<"-------"<<endl;
                if (bulwers<1)
                        bulwers = 1;
        }
 
        if (nap<55)
                cout << "Temperature OK" << endl << "-------" << endl;
                
        return bulwers;
}

//--------------------------------

int bateria (int bulwers)
{
        fstream calosc ("/proc/acpi/battery/BAT1/state", fstream::in);

        string texcik;
        while (calosc.good()){
        texcik+=calosc.get();}

        int tempjuczer=texcik.find_first_of ("capacity:");

        if (texcik[25] == 'n')
        {
                cout << "Cioto podlacz baterie!"<< endl <<"→→→→ ×_× ←←←←";
                bulwers =7;
                return bulwers;
        }
        
        static int pluged;
        
        if (texcik[82] == 'c' && pluged !=1)
        {
                pluged = 1;
                cout << "Battery charging" << endl;
                bulwers = 9;
                return bulwers;
        }
        else if (texcik[82] != 'c' && pluged !=0)
        {
                pluged = 0;
                cout << "Battery discharging" << endl;
        }
        if (texcik[82] == 'c')
                return bulwers;

        int i=0;
        int a=0;
        char temp[155];

        for (;i<155;i++)
        {
                temp[a] = texcik[151+a];
                a++;
        }

        int nap;
        nap = atoi (temp);
        static int bateri;
        
        if (bateri!=nap || bulwers <= 2)
        {
                bateri = nap;

                if (nap<=500)
                {
                        cout << "WARN!!! BATERY CRITICAL LOW (" << nap << " mAh)"<<endl;
                        if (bulwers!=7) {
                                bulwers = 8;
                        }
                }
                else if (nap<=1000) {
                        cout << "Low battery (" << nap << " mAh)"<<endl;
                        if (bulwers<2)
                                bulwers = 2;
                }
                else if (nap<=2500) {
                        cout << "Half battery (" << nap << " mAh)"<<endl;
                        if (bulwers<1)
                                bulwers = 1;
                }
        }
        return bulwers;
  }

int oczka (int bulwers)
{
        static int steps(0);
while(steps>0) {
        if (nadmyszkom) steps -=2;
        if (bulwers == 1)  {
                steps--;
                if (steps == 1) {
                         steps = 0;
                         bulwers = 0;
                }
                return bulwers;
        }
        if (bulwers == 2) {
                steps--;
                if (steps == 5)
                        bulwers = 1;
                if (steps < 5)
                        steps=10;
                return bulwers;
         }
        if (bulwers == 3) {
                steps--;
                if (steps == 10)
                        bulwers = 2;
                if (steps < 10)
                        steps=20;
                return bulwers;
        }
        if (bulwers == 4) {
                steps--;
                if (steps == 20)
                       bulwers = 3;
                if (steps < 20)
                        steps=40;
                return bulwers;
        }
        if (bulwers == 5) {
                steps--;
                if (steps == 40)
                        bulwers = 4;
                if (steps < 40)
                        steps=80;
                return bulwers;
        }
        if (bulwers == 6) {
                steps--;
                if (steps == 80)
                        bulwers = 5;
                if (steps < 80)
                        steps=160;
                return bulwers;
        }
        if (bulwers == 7) {
                return bulwers;
        }
        if (bulwers == 8) return bulwers;
        if (bulwers == 9) {
                bulwers = 0;
                return bulwers;
        }
}
        if (bulwers == 0) return bulwers;
        if (bulwers == 1) {
                steps = 5;
                return bulwers;
         }
        if (bulwers == 2) {
                steps = 10;
                return bulwers;
         }
        if (bulwers == 3) {
                steps = 20;
                return bulwers;
         }
        if (bulwers == 4) {
                steps = 40;
                return bulwers;
         }
        if (bulwers == 5) {
                steps = 80;
                return bulwers;
         }
        if (bulwers == 6) {
                steps = 160;
                return bulwers;
         }
        if (bulwers == 7) return bulwers;
        if (bulwers == 8) return bulwers;
        if (bulwers == 9) {
                bulwers = 0;
                return bulwers;}
        if (bulwers == 666) return bulwers;

        else {
        cout << "BUG" << endl;
        SDL_BlitSurface (cucual12, 0, screen, &obraz);
        flip ();
        return 0;
        }
 }
