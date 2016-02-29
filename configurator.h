#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
using namespace std;
int wys=0, szer=0;
int SCREEN_WIDTH = 240;
int SCREEN_HEIGHT = 240;
const int SCREEN_BPP = 32;
std::string kolor;
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

void rozdzialka(string path="./themes/AJ/eye_03_L_aj.png")
{
  ifstream tmp;
  string command, w, s;
  szer=0;
  wys=0;
  command="file "+path+" >./rozdz.tmp";
  system(command.c_str());
  tmp.open("./rozdz.tmp");
  string tmp2;
  getline(tmp, tmp2);
  string tmp3=tmp2.substr(tmp2.find("PNG image data, ")+16);
  string final=tmp3.substr(0, (tmp3.find(", ")));
  s=final.substr(0, final.find("x")-1);
  w=final.substr(final.find("x")+2);
  int pow=1;
  for(int i=1; i<=s.size(); i++)
  {
    if(i-1)
    {
      pow*=10;
    }
    szer+=(s[s.size()-i]-48)*pow;
  }
  pow=1;
  for(int i=1; i<=w.size(); i++)
  {
    if(i-1)
    {
      pow*=10;
    }
    wys+=(w[w.size()-i]-48)*pow;
  }
  system("rm ./rozdz.tmp");
}
int getch()
{
  struct termios oldt,
      newt;
  int            ch;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}
SDL_Surface *load_image(std::string filename)
{
  SDL_Surface* loadedImage = NULL;
  SDL_Surface* optimizedImage = NULL;
  loadedImage=IMG_Load(filename.c_str());
  if(loadedImage!=NULL)
  {
    optimizedImage=SDL_DisplayFormat(loadedImage);
    SDL_FreeSurface(loadedImage);
  }
  return optimizedImage;
}

void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination)
{
  SDL_Rect offset;
  offset.x = x;
  offset.y = y;
  SDL_BlitSurface(source, NULL, destination, &offset);
}

bool init()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    return false;
  }
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if(screen == NULL)
  {
    return false;
  }
  SDL_WM_SetCaption("Preview", NULL);
  return true;
}
void clean_up()
{
  SDL_FreeSurface(image);
  SDL_Quit();
}
int wyswietl(string path="./themes/default/eye_03_n.png", int szer=240, int wys=240)
{
  SCREEN_WIDTH = szer;
  SCREEN_HEIGHT = wys;

  if(init() == false)
  {
    return 1;
  }
  image = load_image(path.c_str());
  if(image == NULL)
  {
    return 1;
  }
  apply_surface(0, 0, image, screen);
  if(SDL_Flip(screen) == -1)
  {
    return 1;
  }
  SDL_Delay(1500);
  clean_up();
  return 0;
}
