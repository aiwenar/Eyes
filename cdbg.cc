#include "cdbg.hxx"
#include "core.hxx"
#include "hardware.hxx"

#include <iostream>

using namespace std;

extern percental  cpu;
extern percental  memory;
extern percental  battery;
extern unital     temperature;
extern timal      times;
extern timal      energy;
extern bul        bulwers;
extern QTime      mousetime;
extern mouse_actions  mousea;
extern unsigned short battery_state;
extern unsigned short mod_bat_plug;
extern bool           get_flu;
extern bool           once_plugged;
extern unsigned int   battery_capacity;
extern unsigned int   flu_timer;
extern unsigned int   temp_t;
extern unsigned int   prev_bat_plug;
extern int            core_step;

void spacefill (int input, int lenght)
{
    unsigned int pivot = 1;
    string sign = "";
    if (input < 0)
    {
        lenght--;
        input=-input;
        sign = "-";
    }
    for (int i = 0; i < lenght; i++)
    {
        pivot*=10;
    }
    if (input >= pivot)
    {
        unsigned int deleted = 0;
        while (input >= pivot/100)
        {
            input/=10;
            deleted++;
        }
        cout << sign << input << "^" << deleted;
    }
    else
    {
        cout << sign << input;
        int inleng = 0;
        while (input >= 10)
        {
            input/=10;
            inleng++;
        }
        for (int i = 1; i < lenght-inleng; i++)
        {
            cout << " ";
        }
    }
}

/*

This is concept debug layout:


#######################################################################################################################
# step:____   special:__    basic         mods       mouse debug |
# ---------------------------------------------------------------|
# cpu:  | mem:  | temp: | battery:   | cpu:  load: | X:___ Y:___ |
#       |       |       | ]]]]]]>><< |             |    <===>    |
# p1 b1 | m1 b1 | t1 b1 |------------| mem:  load: |-------------|
# p2 b2 | m2 b2 | t2 b2 | time:__    |             | friendship: |
# p3 b3 | m3 b3 | t3 b3 | ========== | temp: val:  | ______ <___ |
# p4 b4 | m4 b4 | t4 b4 |------------|             |-------------|
# p5 b5 | m5 b5 | t5 b5 | energy:__  | batt: perc: | delay:      |
# p6 b6 | m6 b6 | t6 b6 | ========== |             | ___________ |
# p7 b7 | m7 b7 | t7 b7 |------------| time: val:  | result:     |
# p8 b8 | m8 b8 | t8 b8 | RISE||CALM |             | ____[Mpx/s] |
# p9 b9 | m9 b9 | t9 b9 | __________ | enrg: val:  |-------------|
# p0 b0 | m0 b0 | t0 b0 | __________ |             | mouse.mod:  |
#       |       |       | next_wall: | plug: val:  | ____        |
# ===== | ===== | ===== | __________ |             | buffers:___ |


And for everyone there are terminal instructions for that:


\033 = [Esc] (up in your keyboard top left corner)

 Colors:
 \033[30m set foreground color to black

 \033[31m set foreground color to red

 \033[32m set foreground color to green

 \033[33m set foreground color to yellow

 \033[34m set foreground color to blue

 \033[35m set foreground color to magenta (purple)

 \033[36m set foreground color to cyan

 \033[37m set foreground color to white

 \033[40m set background color to black

 \033[41m set background color to red

 \033[42m set background color to green

 \033[43m set background color to yellow

 \033[44m set background color to blue
MsSQL
 \033[45m set background color to magenta (purple)

 \033[46m set background color to cyan

 \033[47m set background color to white

 \033[1;30m set foreground color to dark gray

 \033[1;31m set foreground color to light red

 \033[1;32m set foreground color to light green

 \033[1;33m set foreground color to yellow

 \033[1;34m set foreground color to light blue

 \033[1;35m set foreground color to light magenta (purple)

 \033[1;36m set foreground color to light cyan

 \033[1;37m set foreground color to white

 \033[1;40m set background color to dark gray

 \033[1;41m set background color to light red

 \033[1;42m set background color to light green

 \033[1;43m set background color to yellow

 \033[1;44m set background color to light blue

 \033[1;45m set background color to light magenta (purple)

 \033[1;46m set background color to light cyan

 \033[1;47m set background color to white

 For other features:
 \033[0m reset; clears all colors and styles (to white on black)

 \033[1m bold on

 \033[3m italics on

 \033[4m underline on

 \033[5m blink on

 \033[7m reverse video on

 \033[8m nondisplayed (invisible)

 \033[x;yH moves cursor to line x, column y

 \033[xA moves cursor up x lines

 \033[xB moves cursor down x lines

 \033[xC moves cursor right x spaces

 \033[xD moves cursor left x spaces

 \033[2J clear screen and home cursor

 END

*/

cdbg::cdbg ( Core * c ) :
  core  ( c ),
  max_s ( 0 ),
  timer ()
{
  if ( cpu.buffered and cpu.buff_size > max_s )
    max_s = cpu.buff_size;
  if ( memory.buffered and memory.buff_size > max_s )
    max_s = memory.buff_size;
  if ( temperature.buffered and temperature.buff_size > max_s )
    max_s = temperature.buff_size;

  cout << "\033[40m\033[37m\n";
  for ( int i = max_s+6 ; i>1 ; --i ) cout << '\n';
  for ( int i=max_s+7 ; i>1 ; --i )   cout << "\033[1A";
  for ( int i=64 ; i>0 ; --i )        cout << '-';
  cout << "\033[40D\033[3B";
  for ( int i=13 ; i>1 ; --i )        cout << '-';
  cout << "\033[13D\033[3B";
  for (int i = 13; i>=1; i--)         cout << "-";
  cout << "\033[13D\033[3B";
  for (int i = 13; i>=1; i--)         cout << "-";
  cout << "\033[6A\033[14C";
  for (int i = 14; i>=1; i--)         cout << "-";
  cout << "\033[14D\033[3B";
  for (int i = 14; i>=1; i--)         cout << "-";
  cout << "\033[14D\033[5B";
  for (int i = 14; i>=1; i--)         cout << "-";
  cout << "\033[56D\033[11A";
  for (int i = max_s + 4; i>0; i--)  cout << "\033[1B\033[1D|";
  for (int i = max_s + 4; i>0; i--)  cout << "\033[1A";
  cout << "\033[8C";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1B\033[1D|";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1A";
  cout << "\033[8C";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1B\033[1D|";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1A";
  cout << "\033[12C\033[1B";
  for (int i = max_s+4; i>0; i--)    cout << "|\033[1B\033[1D";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1A";
  cout << "\033[14C";
  for (int i = max_s+4; i>0; i--)    cout << "|\033[1B\033[1D";
  for (int i = max_s+6; i>0; i--)    cout << "\033[1A";
  cout << "\033[14C";
  for (int i = max_s+6; i>0; i--)    cout << "|\033[1B\033[1D";
  for (int i = max_s+6; i>0; i--)    cout << "\033[1A";
  cout << "\n\033[1A step:        special:\033[2B\033[21D"
          "cpu:\033[4Cmem:\033[4Ctemp:\033[3Cbattery:"
          "\033[8D\033[3Btime:\033[5D\033[3Benergy:\033[7D"
          "\033[6Bnext_wall:\n\033[15A\033[22C      basic     "
          "   mods       mouse debug ";
  if (false)
      cout << "\033[1C HARDLY DEBUG MODE:";
  cout << "\n\n\033[38Ccpu:\n\033[1A\033[44Cload:\n"
          "\n\033[38Cmem:\n\033[1A\033[44Cload:\n\n"
          "\033[38Ctemp:\n\033[1A\033[44Cval:\n\n"
          "\033[38Cbatt:\n\033[1A\033[44Cperc:\n\n"
          "\033[38Ctime:\n\033[1A\033[44Cval:\n\n"
          "\033[38Cenrg:\n\033[1A\033[44Cval:\n\n"
          "\033[38Cplug:\n\033[1A\033[44Cval:\n\033[14A"
          "\n\033[52CX:    Y:\n\n\n\033[52Cfriendship:"
          "\n\n\n\033[52Cdelay:\n\n\033[52Cresult:"
          "\n\033[56C[Mpx/s]\n\n\033[52Cmod:   hit:"
          "\n\n\033[52Cbuffers:\033[14A\n";
  connect ( &timer, SIGNAL ( timeout () ), this, SLOT ( on_timer_tick () ) );
  timer.start ( 1000 );
}

void cdbg::on_timer_tick ()
{
  cout << "\033[1;33m\033[2A\033[6C"
       << core_step
       << "\n\033[1A\033[22C0\n\033[1B\n\n";
  if ( cpu.buffered )
  {
    for ( int i=0 ; i<cpu.buff_size ; ++i )
    {
      if ( i != cpu.current_probe )
      {
        if ( cpu.probes[i] <= cpu.stable - cpu.loseless )
          cout << "\033[1;32m " << cpu.probes[i] << '\n';
        else if ( cpu.probes[i] >= cpu.stable + cpu.loseless )
          cout << "\033[1;31 " << cpu.probes[i] << '\n';
        else
          cout << "\033[1;30m " << cpu.probes[i] << '\n';
      }
      else
        cout << "\033[1;33m>" << (cpu.probes[i]<10?" ":"") << cpu.probes[i] << '\n';
    }
    for ( int i=0 ; i<=cpu.buff_size ; ++i ) cout << "\033[1A";
    cout << '\n';
    for ( int i=0 ; i<cpu.buff_size ; ++i )
    {
      if ( i != cpu.current_probe_small )
      {
        if ( cpu.sector_small[i] <= cpu.stable - cpu.loseless )
          cout << "\033[1;32m\033[4C" << cpu.sector_small[i] << '\n';
        else if ( cpu.sector_small[i] >= cpu.stable + cpu.loseless )
          cout << "\033[1;31m\033[4C" << cpu.sector_small[i] << '\n';
        else
          cout << "\033[1;30m\033[4C" << cpu.sector_small[i] << '\n';
      }
      else
        cout << "\033[1;33m\033[4C" << cpu.sector_small[i] << "<\n";
    }
  }
  else
  {
    if ( cpu.load <= cpu.stable - cpu.loseless )
      cout << "\033[1;32m " << cpu.load << " \n";
    else if ( cpu.load >= cpu.stable + cpu.load )
      cout << "\033[1;31m " << cpu.load << '\n';
    else
      cout << "\033[1;30m " << cpu.load << '\n';
  }
  cout << "\n\033[1C";

  switch ( cpu.frequency )
  {
  case '1':
    if ( cpu.mod <= 0 )
    {
      int s = 5*(double(-cpu.mod)/double(cpu.lin_num*cpu.steps));
      if ( s == 0 and cpu.mod < 0 )
        s = 1;
      cout << "\033[1;32m";
      for ( int i=s ; i>0 ; --i ) cout << '=';
      cout << "\033[1;30m";
      for ( int i=s ; i>0 ; --i ) cout << '=';
    }
    else
    {
      int s = 5*(double(cpu.mod)/double(cpu.lin_num*cpu.steps));
      if ( s == 0 and cpu.mod > 0 )
        s = 1;
      cout << "\033[1;31m";
      for ( int i=s ; i>0 ; --i ) cout << '=';
      cout << "\033[1;30m";
      for ( int i=5-s ; i>0 ; --i ) cout << '=';
    }
    break;

  case 'q':
    if ( cpu.mod <= 0 )
    {
      int s = 5*(double(-cpu.mod)/double(cpu.steps*cpu.stable));
      if ( s == 0 and cpu.mod < 0 )
        s = 1;
      cout << "\033[1;32m";
      for ( int i=s ; i>0 ; --i ) cout << '=';
      cout << "\033[1;30m";
      for ( int i=5 ; i>0 ; --i ) cout << '=';
    }
    else
    {
      int s = 5*(double(cpu.mod)/double(cpu.steps*cpu.stable));
      if ( s == 0 and cpu.mod > 0 )
        s = 1;
      cout << "\033[1;31m";
      for ( int i=s ; i>0 ; --i ) cout << '=';
      cout << "\033[1;30m";
      for ( int i=5-s ; i>0 ; --i ) cout << '=';
    }
    break;

  case 'f':
    int max_mod = 0, max_mod_prev = 0;
    for ( int i=0 ; i<=cpu.steps ; i++ )
    {
      max_mod_prev = max_mod;
      max_mod      = i+max_mod_prev;
    }
    if (cpu.mod <= 0)
    {
        unsigned short s = 5 * (double(-cpu.mod) / double(max_mod));
        if (s == 0 && cpu.mod < 0)
            s = 1;
        cout << "\033[1;32m";
        for (unsigned short i = s; i > 0; i--) cout << "=";
        cout << "\033[1;30m";
        for (unsigned short m = 5 - s; m > 0; m--) cout << "=";
    }
    else
    {
        unsigned short s = 5 * (double(cpu.mod) / double(max_mod));
        if (s == 0 && cpu.mod > 0)
            s = 1;
        cout << "\033[1;31m";
        for ( unsigned int i = s; i > 0; i--) cout << "=";
        cout << "\033[1;30m";
        for (unsigned short m = 5 - s; m > 0; m--) cout << "=";
    }
  }

  cout << "\033[1A \n\n\n\033[9C";
  if (memory.buffered)
  {

      for (unsigned short i = 0; i< memory.buff_size; i++)
      {
          if (i != memory.current_probe)
          {
              if (memory.probes[i] < 10)
              {
                  if (memory.probes[i] <= memory.stable - memory.loseless)
                      cout << "\033[1;32m" << "\033[8C" << "  " << (unsigned short)memory.probes[i] << "\n";
                  else if (memory.probes[i] >= memory.stable + memory.loseless)
                      cout << "\033[1;31m" << "\033[8C" << "  " << (unsigned short)memory.probes[i] << "\n";
                  else
                      cout << "\033[1;30m" << "\033[8C" << "  " << (unsigned short)memory.probes[i] << "\n";
              }
              else
              {
                  if (memory.probes[i] <= memory.stable - memory.loseless)
                      cout << "\033[1;32m" << "\033[8C" << " " << (unsigned short)memory.probes[i] << "\n";
                  else if (memory.probes[i] >= memory.stable + memory.loseless)
                      cout << "\033[1;31m" << "\033[8C" << " " << (unsigned short)memory.probes[i] << "\n";
                  else
                      cout << "\033[1;30m" << "\033[8C" << " " << (unsigned short)memory.probes[i] << "\n";
              }
          }
          else
          {
              if (memory.probes[i] < 10)
                  cout << "\033[1;33m" << "\033[8C" << "> " << (unsigned short)memory.probes[i] << "\n";
              else
                  cout << "\033[1;33m" << "\033[8C" << ">" << (unsigned short)memory.probes[i] << "\n";
          }

      }
      for (unsigned short i = 0; i<= memory.buff_size; i++)
      {
          cout << "\033[1A";
      }
      cout << "\n";

      for (unsigned short i = 0; i< memory.buff_size; i++)
      {
          if (i != memory.current_probe_small)
          {
              if (memory.sector_small[i] <= memory.stable - memory.loseless)
              {
                  if (memory.sector_small[i] < 10)
                      cout << "\033[1;32m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;32m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
              }
              else if (memory.sector_small[i] >= memory.stable + memory.loseless)
              {
                  if (memory.sector_small[i] < 10)
                      cout << "\033[1;31m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;31m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
              }
              else
              {
                  if (memory.sector_small[i] < 10)
                      cout << "\033[1;30m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;30m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
              }
          }
          else
          {
              if (memory.sector_small[i] < 10)
                  cout << "\033[1;33m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " <\n";
              else
                  cout << "\033[1;33m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "<\n";
          }
      }
  }
  else
  {
      if (memory.load <= memory.stable - memory.loseless)
          cout << "\033[1;32m" << "\033[9C" << (unsigned short)memory.load << " \n";
      else if (memory.load >= memory.stable + memory.loseless)
          cout << "\033[1;31m" << "\033[9C" << (unsigned short)memory.load << " \n";
      else
          cout << "\033[1;30m" << "\033[9C" << (unsigned short)memory.load << " \n";
  }

  if (memory.frequency == 'l')
  {
      if (memory.mod <= 0)
      {
          unsigned short s = 5 * (double(-memory.mod) / double(memory.lin_num*memory.steps));
          if (s == 0 && memory.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
      else
      {
          unsigned short s = 5 * (double(memory.mod) / double(memory.lin_num*memory.steps));
          if (s == 0 && memory.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
  }
  if (memory.frequency == 'q')
  {
      if (memory.mod <= 0)
      {
          unsigned short s = 5 * (double(-memory.mod) / double(memory.steps*memory.steps));
          if (s == 0 && memory.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
      else
      {
          unsigned short s = 5 * (double(memory.mod) / double(memory.steps*memory.steps));
          if (s == 0 && memory.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
  }

  if (memory.frequency == 'f')
  {
      unsigned short max_mod = 0;
      unsigned short max_mod_prev = 0;
      for (unsigned int i = 0; i <= memory.steps; i++)
      {
          max_mod_prev = max_mod;
          max_mod = i + max_mod_prev;
      }
      if (memory.mod <= 0)
      {
          unsigned short s = 5 * (double(-memory.mod) / double(max_mod));
          if (s == 0 && memory.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for (unsigned short i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
      else
      {
          unsigned short s = 5 * (double(memory.mod) / double(max_mod));
          if (s == 0 && memory.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
  }

  if (memory.buffered)
  {
      for (unsigned short i = 0; i<= memory.buff_size+1; i++)
      {
          cout << "\033[1A";
      }
  }
  else
      cout << "\033[3A";

  cout << "\033[1A \n\n";

  if (temperature.buffered)
  {
      for (unsigned short i = 0; i< temperature.buff_size; i++)
      {
          if (i != temperature.current_probe)
          {
              if (temperature.probes[i] < 10)
              {
                  if (temperature.probes[i] <= temperature.stable - temperature.loseless)
                      cout << "\033[1;32m" << "\033[16C" << "  " << (unsigned short)temperature.probes[i] << "\n";
                  else if (temperature.probes[i] >= temperature.stable + temperature.loseless)
                      cout << "\033[1;31m" << "\033[16C" << "  " << (unsigned short)temperature.probes[i] << "\n";
                  else
                      cout << "\033[1;30m" << "\033[16C" << "  " << (unsigned short)temperature.probes[i] << "\n";
              }
              else
              {
                  if (temperature.probes[i] <= temperature.stable - temperature.loseless)
                      cout << "\033[1;32m" << "\033[16C" << " " << (unsigned short)temperature.probes[i] << "\n";
                  else if (temperature.probes[i] >= temperature.stable + temperature.loseless)
                      cout << "\033[1;31m" << "\033[16C" << " " << (unsigned short)temperature.probes[i] << "\n";
                  else
                      cout << "\033[1;30m" << "\033[16C" << " " << (unsigned short)temperature.probes[i] << "\n";
              }
          }
          else
          {
              if (temperature.probes[i] < 10)
                  cout << "\033[1;33m" << "\033[16C" << "> " << (unsigned short)temperature.probes[i] << "\n";
              else
                  cout << "\033[1;33m" << "\033[16C" << ">" << (unsigned short)temperature.probes[i] << "\n";
          }
      }
  }
  else
  {
      if (temperature.value <= temperature.stable - temperature.loseless)
          cout << "\033[1;32m" << "\033[16C" << " " << (unsigned short)temperature.value << "\n";
      else if (temperature.value >= temperature.stable + temperature.loseless)
          cout << "\033[1;31m" << "\033[16C" << " " << (unsigned short)temperature.value << "\n";
      else
          cout << "\033[1;30m" << "\033[16C" << " " << (unsigned short)temperature.value << "\n";
  }


  cout << "\n" << "\033[17C";
  if (temperature.frequency == 'l')
  {
      if (temperature.mod <= 0)
      {
          unsigned short s = 5 * (double(-temperature.mod) / double(temperature.steps*temperature.lin_num));
          if (s == 0 && temperature.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
      else
      {
          unsigned short s = 5 * (double(temperature.mod) / double(temperature.steps*temperature.lin_num));
          if (s == 0 && temperature.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
  }
  if (temperature.frequency == 'q')
  {
      if (temperature.mod <= 0)
      {
          unsigned short s = 5 * (double(-temperature.mod) / double(temperature.steps*temperature.steps));
          if (s == 0 && temperature.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
      else
      {
          unsigned short s = 5 * (double(temperature.mod) / double(temperature.steps*temperature.steps));
          if (s == 0 && temperature.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
  }

  if (temperature.frequency == 'f')
  {
      unsigned short max_mod = 0;
      unsigned short max_mod_prev = 0;
      for (unsigned int i = 0; i <= temperature.steps; i++)
      {
          max_mod_prev = max_mod;
          max_mod = i + max_mod_prev;
      }
      if (temperature.mod <= 0)
      {
          unsigned short s = 5 * (double(-temperature.mod) / double(max_mod));
          if (s == 0 && temperature.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for (unsigned short i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
      else
      {
          unsigned short s = 5 * (double(temperature.mod) / double(max_mod));
          if (s == 0 && temperature.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for (unsigned short i = s; i > 0; i--)
          {
              cout << "=";
          }
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
          {
              cout << "=";
          }
      }
  }

  if (temperature.buffered)
  {

      for (unsigned short i = 0; i<= temperature.buff_size+1; i++)
      {
          cout << "\033[1A";
      }
      cout << "\n";

      for (unsigned short i = 0; i< temperature.buff_size; i++)
      {
          if (i != temperature.current_probe_small)
          {
              if (temperature.sector_small[i] <= temperature.stable - temperature.loseless)
              {
                  if (temperature.sector_small[i] < 10)
                      cout << "\033[1;32m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;32m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " \n";
              }
              else if (temperature.sector_small[i] >= temperature.stable + temperature.loseless)
              {
                  if (temperature.sector_small[i] < 10)
                      cout << "\033[1;31m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;31m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " \n";
              }
              else
              {
                  if (temperature.sector_small[i] < 10)
                      cout << "\033[1;30m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;30m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " \n";
              }
          }
          else
          {
              if (temperature.sector_small[i] < 10)
                  cout << "\033[1;33m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " <\n";
              else
                  cout << "\033[1;33m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "<\n";
          }
      }
      for (unsigned short i = 0; i<= temperature.buff_size; i++)
      {
          cout << "\033[1A";
      }
  }
  else
      cout << "\033[3A";

  cout << "\n";

  cout << "\033[25C" << "\033[1;33m" << "\033[1A";
  if (battery_state == 0)
  {
      cout << "<< NONE >>";
  }
  for (unsigned short i = battery.load/10; i > 0; i--)
  {
      cout << "]";
  }
  if (battery_state == 1)
  {
      for (unsigned short i =10 - battery.load/10; i > 0; i--)
      {
          cout << "\033[1;31m" << "O";
      }
  }
  if (battery_state == 2)
  {
      for (unsigned short i =10 - battery.load/10; i > 0; i--)
      {
          cout << "\033[1;32m" << "X";
      }
  }
  if (battery_state == 3)
  {
      for (unsigned short i =10 - battery.load/10; i > 0; i--)
      {
          cout << "\033[1;32m" << ">";
      }
  }
  if (battery_state == 4)
  {
      for (unsigned short i =10 - battery.load/10; i > 0; i--)
      {
          cout << "\033[1;31m" << "<";
      }
  }
  cout << "\n \n" << "\033[30C";
  if (times.mod > 0)
      cout << "\033[1;31m";
  else
      cout << "\033[1;33m";
  cout << "\n";
  cout << "\033[25C";
  cout << "\033[1;31m";
  for (unsigned short i =10*(double(times.mod)/double(times.steps)); i > 0; i--)
  {
      cout << "=";
  }
  cout << "\033[1;30m";
  for (unsigned short i =10 - 10*(double(times.mod)/double(times.steps)); i > 0; i--)
  {
      cout << "=";
  }

  cout << "\n \n" << "\033[32C";
  if (energy.mod > 0)
      cout << "\033[1;31m";
  else
      cout << "\033[1;33m";
  cout << "\n";
  cout << "\033[25C";
  cout << "\033[1;31m";
  for (unsigned short i =10*(double(energy.mod)/double(energy.steps)); i > 0; i--)
  {
      cout << "=";
  }
  cout << "\033[1;30m";
  for (unsigned short i =10 - 10*(double(energy.mod)/double(energy.steps)); i > 0; i--)
  {
      cout << "=";
  }


  cout << "\n \n" << "\033[25C";
  if (bulwers.total_mod > 1)
  {
      cout << "\033[1;31m" << "↑↑↑↑";
      cout << "\033[1;30m" << "||↓↓↓↓";
  }
  else if (bulwers.total_mod < 1)
  {
      cout << "\033[1;30m" << "↑↑↑↑||";
      cout << "\033[1;32m" << "↓↓↓↓";
  }
  else
  {
      cout << "\033[1;30m" << "↑↑↑↑";
      cout << "\033[1;34m" << "||";
      cout << "\033[1;30m" << "↓↓↓↓";
  }
  if (bulwers.total_mod > 1)
      cout << "\033[1;31m";
  else if (bulwers.total_mod < 1)
      cout << "\033[1;32m";
  else
      cout << "\033[1;30m";
  cout << "\n" << "\033[25C" << "          " << "\033[10D" << bulwers.total_mod;
  cout << "\033[1;33m";
  cout << "\n" << "\033[25C" << "          " << "\033[10D" << bulwers.step;
  cout << "\n\n" << "\033[25C" << "          " << "\033[10D";
  if (bulwers.value == 0)
      cout << bulwers.wall_01;
  if (bulwers.value == 1)
      cout << bulwers.wall_02;
  if (bulwers.value == 2)
      cout << bulwers.wall_03;
  if (bulwers.value == 3)
      cout << bulwers.wall_04;
  if (bulwers.value == 4)
      cout << bulwers.wall_05;
  if (bulwers.value == 5)
      cout << bulwers.wall_06;
  if (bulwers.value == 6)
      cout << bulwers.wall_07;
  if (bulwers.value == 7)
      cout << bulwers.wall_08;
  if (bulwers.value == 8)
      cout << bulwers.wall_09;
  if (bulwers.value == 9)
      cout << bulwers.wall_10;
  if (bulwers.value == 10)
      cout << bulwers.wall_11;
  if (bulwers.value == 11)
      cout << bulwers.wall_12;
  if (bulwers.value == 12)
      cout << bulwers.wall_13;
  if (bulwers.value == 13)
      cout << bulwers.wall_14;
  if (bulwers.value == 14)
      cout << bulwers.wall_15;

  cout << "\033[14A";
  if (cpu.mod > 0)
  {
      cout << "\033[1;31m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << cpu.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)cpu.load << "%";
  }
  else if (cpu.mod < 0)
  {
      cout << "\033[1;32m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << cpu.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)cpu.load << "%";
  }
  else
  {
      cout << "\033[1;33m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << cpu.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)cpu.load << "%";
  }

  ////

  if (memory.mod > 0)
  {
      cout << "\033[1;31m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << memory.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)memory.load << "%";
  }
  else if (memory.mod < 0)
  {
      cout << "\033[1;32m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << memory.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)memory.load << "%";
  }
  else
  {
      cout << "\033[1;33m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << memory.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)memory.load << "%";
  }

  ////

  if (temperature.mod > 0)
  {
      cout << "\033[1;31m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << temperature.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << temperature.value << "ºC";
  }
  else if (temperature.mod < 0)
  {
      cout << "\033[1;32m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << temperature.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << temperature.value << "ºC";
  }
  else
  {
      cout << "\033[1;33m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << temperature.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << temperature.value << "ºC";
  }

  ////

  if (battery.mod < 0)
  {
      cout << "\033[1;31m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << -battery.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)battery.load << "%";
  }
  else if (battery.mod > 0)
  {
      cout << "\033[1;32m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << -battery.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)battery.load << "%";
  }
  else
  {
      cout << "\033[1;33m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << battery.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)battery.load << "%";
  }

  ////

  if (times.mod > 0)
  {
      cout << "\033[1;31m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << times.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << times.value << "h";
  }
  else
  {
      cout << "\033[1;33m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << times.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << times.value << "h";
  }

  ////

  if (energy.mod > 0)
  {
      cout << "\033[1;31m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << energy.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (energy.start + energy.wide - energy.value)/3600 << "h";
  }
  else
  {
      cout << "\033[1;33m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << energy.mod;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (energy.start + energy.wide - energy.value)/3600 << "h";
  }

  ////

  if (mod_bat_plug > 0)
  {
      cout << "\033[1;32m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << mod_bat_plug;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << battery_state;
  }
  else
  {
      cout << "\033[1;33m";
      cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << mod_bat_plug;
      cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << battery_state;
  }

  cout << "\033[15A" << "\n\n";
  cout << "\033[52C\033[33m";

  cout << "\033[2C";
  spacefill(mousea.prev_x, 3);

  cout << "\033[3C";
  spacefill(mousea.prev_y, 3);

  cout << "\n\033[55C";
  if(mousetime.elapsed() < mousea.max_delay)
      cout << "\033[37m<===>";
  else
      cout << "\033[1;30m<===>";;
  cout << " \033[33m\n\n\n\033[52C";
  if (bulwers.friendship < 0)
      cout << "\033[31m";
  else
      cout << "\033[32m";
  spacefill(bulwers.friendship, 6);

  cout << "\033[33m\n\033[1A\033[60C";
  if (mousea.mod > 0)
  {
      cout << "\033[32m<";
      spacefill(mousea.goodstep, 3);
  }
  else if (mousea.mod < 0)
  {
      cout << "\033[31m>";
      if (bulwers.friendship < -mousea.heavycalm)
          spacefill(bulwers.friendship/mousea.heavycalm, 3);
      else
          spacefill(mousea.badstep, 3);
  }
  else
      cout << " --";

  cout << "\033[33m\n\n\n\033[52C";
  if (mousetime.elapsed() <= mousea.max_delay)
      spacefill(mousetime.elapsed(), 11);
  else
      cout << "\033[1;30m  ignored  ";

  cout << "\033[33m\n\n\033[52C";
  if (100*mousea.result/mousea.wall >= 80)
      cout << "\033[31m";
  else if (100*mousea.result/mousea.wall <= 20)
      cout << "\033[33m";
  else
      cout << "\033[32m";
  spacefill(mousea.result, 4);

  cout << "\033[33m\n\n\n\033[52C";
  if (mousea.mod < 0)
      cout << "\033[31m";
  else if (mousea.mod == 0)
      cout << "\033[33m";
  else
      cout << "\033[32m";
  spacefill(mousea.mod, 6);
  cout << " ";
  spacefill(mousea.hit_time, 4);

  cout << "\033[33m\n\033[60C";
  spacefill(mousea.tr, 3);

  cout << "\033[15A" << "\n\n";

  if (false)
  {
    spacer = 66;
    line = 0;

    cout << "\033[" << spacer << "C";
    cout << "\033[1;32m";

    cout << "cload" << " " << (unsigned short)cpu.load << " %";
    chck_s ();
    cout << "cstbl" << " " << (unsigned short)cpu.stable << " %";
    chck_s ();
    cout << "closs" << " " << (unsigned short)cpu.loseless;
    chck_s ();
    cout << "clnum" << " " << (unsigned short)cpu.lin_num;
    chck_s ();
    cout << "cfreq" << " " << cpu.frequency;
    chck_s ();
    cout << "mload" << " " << (unsigned short)memory.load << " %";
    chck_s ();
    cout << "mstbl" << " " << (unsigned short)memory.stable << " %";
    chck_s ();
    cout << "mloss" << " " << (unsigned short)memory.loseless;
    chck_s ();
    cout << "mlnum" << " " << (unsigned short)memory.lin_num;
    chck_s ();
    cout << "mfreq" << " " << memory.frequency;
    chck_s ();
    cout << "bload" << " " << (unsigned short)battery.load << " %";
    chck_s ();
    cout << "bstbl" << " " << (unsigned short)battery.stable << " %";
    chck_s ();
    cout << "bloss" << " " << (unsigned short)battery.loseless;
    chck_s ();
    cout << "blnum" << " " << (unsigned short)battery.lin_num;
    chck_s ();
    cout << "bstat" << " " << battery_state;
    chck_s ();
    cout << "bfreq" << " " << battery.frequency;
    chck_s ();
    cout << "pbplg" << " " << prev_bat_plug;
    chck_s ();
    cout << "bcapa" << " " << battery_capacity;
    chck_s ();
    cout << "boplg" << " " << once_plugged;
    chck_s ();
    cout << "tempv" << " " << (unsigned short)temperature.value << "ºC";
    chck_s ();
    cout << "tstbl" << " " << (unsigned short)temperature.stable << "ºC";
    chck_s ();
    cout << "tloss" << " " << (unsigned short)temperature.loseless;
    chck_s ();
    cout << "tlnum" << " " << (unsigned short)temperature.lin_num;
    chck_s ();
    cout << "tunit" << " " << (unsigned short)temperature.unit;
    chck_s ();
    cout << "tfreq" << " " << temperature.frequency;
    chck_s ();
    cout << "tempt" << " " << temp_t;
    chck_s ();
    cout << "flutm" << " " << flu_timer;
    chck_s ();
    cout << "gtflu" << " " << get_flu;
    chck_s ();
    cout << "timev" << " " << (unsigned short)times.value << " h";
    chck_s ();
    cout << "tmstr" << " " << (unsigned short)times.start << " h";
    chck_s ();
    cout << "timew" << " " << (unsigned short)times.wide << " h";
    chck_s ();
    cout << "tmstp" << " " << (unsigned short)times.steps;
    chck_s ();
    cout << "tmlnr" << " " << (unsigned short)times.lin_num;
    chck_s ();
    cout << "timef" << " " << times.frequency;
    chck_s ();
    cout << "nrgvl" << " " << (unsigned short)energy.value;
    chck_s ();
    cout << "nrgst" << " " << (unsigned short)energy.start;
    chck_s ();
    cout << "nrgwd" << " " << (unsigned short)energy.wide;
    chck_s ();
    cout << "nrstp" << " " << (unsigned short)energy.steps;
    chck_s ();
    cout << "nrgln" << " " << (unsigned short)energy.lin_num;
    chck_s ();
    cout << "nrgfq" << " " << energy.frequency;
    chck_s ();
    cout << "creye" << " " << bulwers.eye;
    chck_s ();
    cout << "tmmon" << " " << get_time().month;
    chck_s ();



    cout << "\033[" << line+1 << "A\n";
  }
}


void cdbg::chck_s()
{
    if(line >= max_s+3 && line >= 10 )
    {
        line = 0;
        spacer += 14;
        cout << "\033[1;32m\033[" << max_s+4 << "A\n";
    }
    else
    {
        if (line % 2 == 1)
            cout << "\033[1;32m";
        else
            cout << "\033[1;36m";
        cout << "\n";
        line++;
    }
    cout << "\033[" << spacer << "C";
}
