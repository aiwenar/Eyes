/* Eyes
 * Copyright (C) 2011, 2012  Krzysztof Mędrzycki, Damian Chiliński
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "cdbg.hxx"
#include "core.hxx"
#include "hardware.hxx"

#include <iostream>
#include <unistd.h>

using namespace std;

extern percental  cpu;
extern percental  memory;
extern percental  battery;
extern unital     temperature;
extern timal      times;
extern timal      energy;
extern bul        bulwers;
extern auto_calc  autocalc;
extern QTime      mousetime;
extern mouse_actions  mousea;
extern unsigned short battery_state;
extern unsigned short mod_bat_plug;
extern bool           once_plugged;
extern hardware       HRDWR;
extern unsigned int   prev_bat_plug;
extern int            core_step;

/*

This is concept debug layout:


############################################################################################################################
# step:____   special:__    basic         mods       mouse debug | EQzone: _______ (__)              | wake up | autocalc  |
# ---------------------------------------------------------------| final value: ___%                 |---------|-----------|
# cpu:  | mem:  | temp: | battery:   | cpu:  load: | X:___ Y:___ | [x10%]                            | remain: | rem. ____ |
#       |       |       | ]]]]]]>><< |             |    <===>    | 10 |[]                            | _______ | common    |
# p1 b1 | m1 b1 | t1 b1 |------------| mem:  load: |-------------| 9  |[] []                         | active: | ___       |
# p2 b2 | m2 b2 | t2 b2 | time:__    |             | friendship: | 8  |[] [] []                      | _______ |   rare:   |
# p3 b3 | m3 b3 | t3 b3 | ========== | temp: val:  | ______ <___ | 7  |[] [] [] []                   | reason: | low  high |
# p4 b4 | m4 b4 | t4 b4 |------------|             |-------------| 6  |[] [] [] [] []                | _______ | ___   ___ |
# p5 b5 | m5 b5 | t5 b5 | energy:__  | batt: perc: | delay:      | 5  |[] [] [] [] [] []             |---------| anglelow  |
# p6 b6 | m6 b6 | t6 b6 | ========== |             | ___________ | 4  |[] [] [] [] [] [] []          | flue: _ |  _______  |
# p7 b7 | m7 b7 | t7 b7 |------------| time: val:  | result:     | 3  |[] [] [] [] [] [] [] []       |---------| anglehigh |
# p8 b8 | m8 b8 | t8 b8 | RISE||CALM |             | ____[Mpx/s] | 2  |[] [] [] [] [] [] [] [] []    | buffer: |  _______  |
# p9 b9 | m9 b9 | t9 b9 | __________ | enrg: val:  |-------------| 1  |[] [] [] [] [] [] [] [] [] [] | _______ | multlow   |
# p0 b0 | m0 b0 | t0 b0 | __________ |             | mouse.mod:  | 0  |[] [] [] [] [] [] [] [] [] [] | amplit. | (A)  ____ |
#       |       |       | next_wall: | plug: val:  | ____        |    +----------------------------- | __º __º | multhigh  |
# ===== | ===== | ===== | __________ |             | buffers:___ |     01 __ __ __ __ __ __ __ __ __ |  <__º>  | (A)  ____ |


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

cdbg::cdbg ( Core * c ) :
  core  ( c ),
  max_s ( 0 ),
  max_EQ( 6 ),
  timer ()
{
    if (!core->cdbg_enabled)
        return;
  if ( cpu.buffered and cpu.buff_size > max_s )
    max_s = cpu.buff_size;
  if ( memory.buffered and memory.buff_size > max_s )
    max_s = memory.buff_size;
  if ( temperature.buffered and temperature.buff_size > max_s )
    max_s = temperature.buff_size;
  if ( cpu.EQsize > max_EQ )
    max_EQ = cpu.EQsize;
  if ( memory.EQsize > max_EQ )
    max_EQ = memory.EQsize;
  if ( battery.EQsize > max_EQ )
    max_EQ = battery.EQsize;

  cout << "\033[40m\033[37m\n";
  for ( int i = max_s+4 ; i>1 ; --i ) cout << '\n';
  for ( int i=max_s+5 ; i>1 ; --i )   cout << "\033[1A";
  for ( int i=64 ; i>0 ; --i )        cout << '-';
  sleep(2);
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
  cout << "\033[13C";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1B\033[1D|";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1A";
  cout << "\033[14C";
  for (int i = max_s+4; i>0; i--)    cout << "\033[1B\033[1D|";
  for (int i = max_s+5; i>0; i--)    cout << "\033[1A";
  cout << "\033[14C";
  cout << "\033[1D|";
  for (int i = max_s+5; i>0; i--)    cout << "\033[1B\033[1D|";
  for (int i = max_s+5; i>0; i--)    cout << "\033[1A";
  cout << "\n\033[1A";
  for (int i = max_s+5; i>0; i--)
  {
      cout << "\033[71C";
      for (int i = max_EQ; i>=0; i--)     cout << "   ";
      cout << "|\n";
  }
  cout << "\033[71C";
  for (int i = max_EQ; i>=0; i--)     cout << "   ";
  cout << "|";
  for (int i = max_s+2; i>0; i--)    cout << "\033[1A";
  for (int i = 11; i>0; i--)         cout << "\033[69C|\n";
  cout << "\033[69C|";
  for (int i = max_EQ; i>=0; i--)     cout << "---";
  cout << "\n\033[1A\033[69C+";
  for (int i = 11+3; i>0; i--)    cout << "\033[1A";
  cout << "\n\033[" << 75+max_EQ*3 << "C---------"
          "\033[7B\033[9D---------\033[2B\033[9D---------\n\033[11A";
  cout << "\n\033[1A\033[" << 84+max_EQ*3 << "C";
  for (int i = max_s+5; i>0; i--)    cout << "|\033[1B\033[1D";
  cout << "|\033[1D";
  for (int i = max_s+5; i>0; i--)    cout << "\033[1A";

  cout << "\n\033[1A step:        special:\033[2B\033[21D"
          "cpu:\033[4Cmem:\033[4Ctemp:\033[3Cbattery:"
          "\033[8D\033[3Btime:\033[5D\033[3Benergy:\033[7D"
          "\033[6Bnext_wall:\n\033[15A\033[22C      basic     "
          "   mods       mouse debug \033[1C EQzone:         "
          "(  )\033[" << max_EQ*3 - 11 << "C wake up \n\033[65C"
          " final value:    %\n\033[65C [x10%]\033[" << max_EQ*3 + 4 << "Creason:\n"
          "\033[65C 10 \n"
          "\033[65C  9 \033[" << max_EQ*3 + 7 << "Cactive:\n"
          "\033[65C  8 \n"
          "\033[65C  7 \033[" << max_EQ*3 + 7 << "Cremain:\n"
          "\033[65C  6 \n"
          "\033[65C  5 \n"
          "\033[65C  4 \033[" << max_EQ*3 + 7 << "Cflue:\n"
          "\033[65C  3 \n"
          "\033[65C  2 \033[" << max_EQ*3 + 7 << "Cbuffer:\n"
          "\033[65C  1 \n"
          "\033[65C  0 \033[" << max_EQ*3 + 7 << "Camplit.\n\033[14A";
  if (autocalc.enabled)
  {
      cout << "\033[" << max_EQ*3 + 85 << "C autocalc  |";
      for (int i = max_s+5; i>0; i--)    cout << "\033[1B\033[1D|";
      for (int i = max_s+5; i>0; i--)    cout << "\033[1A";
      cout << "\n\033";
      cout << "\033[" << max_EQ*3 + 85 << "C-----------\n"
              "\033[" << max_EQ*3 + 85 << "C rem.      \n"
              "\033[" << max_EQ*3 + 85 << "C common    \n\n"
              "\033[" << max_EQ*3 + 85 << "C   rare:   \n"
              "\033[" << max_EQ*3 + 85 << "C low  high \n\n"
              "\033[" << max_EQ*3 + 85 << "C anglelow  \n\n"
              "\033[" << max_EQ*3 + 85 << "C anglehigh \n\n"
              "\033[" << max_EQ*3 + 85 << "C multlow   \n"
              "\033[" << max_EQ*3 + 85 << "C ( )       \n"
              "\033[" << max_EQ*3 + 85 << "C multhigh  \n"
              "\033[" << max_EQ*3 + 85 << "C ( )       \n\033[16A\n\033[1A";
  }
  if (core->hdbg_enabled)
      cout << "\033[" << max_EQ*3 + 85 << "C HARDLY DEBUG MODE:";
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
    if (!core->cdbg_enabled)
        return;
  cout << "\033[1;33m\033[2A\033[6C"
       << core_step
       << "\n\033[1A\033[22C0\n\033[1B\n\n";
  if (cpu.buffered)
  {
      for (unsigned short i = 0; i< cpu.buff_size; i++)
      {
          if (i != cpu.current_probe)
          {
              if (cpu.probes[i] <= cpu.stable - cpu.loseless)
              {
                  if (cpu.probes[i] < 10)
                    cout << "\033[1;32m  "  << (unsigned short)cpu.probes[i] << "\n";
                  else
                    cout << "\033[1;32m "  << (unsigned short)cpu.probes[i] << "\n";
              }
              else if (cpu.probes[i] >= cpu.stable + cpu.loseless)
              {
                  if (cpu.probes[i] < 10)
                    cout << "\033[1;31m  " << (unsigned short)cpu.probes[i] << "\n";
                  else
                    cout << "\033[1;31m " << (unsigned short)cpu.probes[i] << "\n";
              }
              else
              {
                  if (cpu.probes[i] < 10)
                    cout << "\033[1;30m  " << (unsigned short)cpu.probes[i] << "\n";
                  else
                    cout << "\033[1;30m " << (unsigned short)cpu.probes[i] << "\n";
              }
          }
          else
          {
              if (cpu.probes[i] < 10)
                cout << "\033[1;33m> " << (unsigned short)cpu.probes[i] << "\n";
              else
                cout << "\033[1;33m>" << (unsigned short)cpu.probes[i] << "\n";
          }
      }
      for (unsigned short i = 0; i<= cpu.buff_size; i++)
          cout << "\033[1A";
      cout << "\n";

      for (unsigned short i = 0; i< cpu.buff_size; i++)
      {
          if (i != cpu.current_probe_small)
          {
              if (cpu.sector_small[i] <= cpu.stable - cpu.loseless)
              {
                  if (cpu.sector_small[i] < 10)
                      cout << "\033[1;32m\033[4C" << (unsigned short)cpu.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;32m\033[4C" << (unsigned short)cpu.sector_small[i] << " \n";
              }
              else if (cpu.sector_small[i] >= cpu.stable + cpu.loseless)
              {
                  if (cpu.sector_small[i] < 10)
                      cout << "\033[1;31m\033[4C" << (unsigned short)cpu.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;31m\033[4C" << (unsigned short)cpu.sector_small[i] << " \n";
              }
              else
              {
                  if (cpu.sector_small[i] < 10)
                      cout << "\033[1;30m\033[4C" << (unsigned short)cpu.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;30m\033[4C" << (unsigned short)cpu.sector_small[i] << " \n";
              }
          }
          else
          {
              if (cpu.sector_small[i] < 10)
                  cout << "\033[1;33m\033[4C" << (unsigned short)cpu.sector_small[i] << " <\n";
              else
                  cout << "\033[1;33m\033[4C" << (unsigned short)cpu.sector_small[i] << "<\n";
          }
      }
  }
  else
  {
      if (cpu.load <= cpu.stable - cpu.loseless)
          cout << "\033[1;32m " << (unsigned short)cpu.load << " \n";
      else if (cpu.load >= cpu.stable + cpu.loseless)
          cout << "\033[1;31m " << (unsigned short)cpu.load << " \n";
      else
          cout << "\033[1;30m " << (unsigned short)cpu.load << " \n";
  }

  cout << "\n" << "\033[1C";

  switch ( cpu.frequency )
  {
  case 'l':
      if (cpu.mod <= 0)
      {
          unsigned short s = 5 * (double(-cpu.mod) / double(cpu.lin_num*cpu.steps));
          if (s == 0 && cpu.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }
      else
      {
          unsigned short s = 5 * (double(cpu.mod) / double(cpu.lin_num*cpu.steps));
          if (s == 0 && cpu.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }

  case 'q':
      if (cpu.mod <= 0)
      {
          unsigned short s = 5 * (double(-cpu.mod) / double(cpu.steps*cpu.steps));
          if (s == 0 && cpu.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }
      else
      {
          unsigned short s = 5 * (double(cpu.mod) / double(cpu.steps*cpu.steps));
          if (s == 0 && cpu.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }

  case 'f':
      unsigned short max_mod = 0;
      unsigned short max_mod_prev = 0;
      for (unsigned int i = 0; i <= cpu.steps; i++)
      {
          max_mod_prev = max_mod;
          max_mod = i + max_mod_prev;
      }
      if (cpu.mod <= 0)
      {
          unsigned short s = 5 * (double(-cpu.mod) / double(max_mod));
          if (s == 0 && cpu.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for (unsigned short i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }
      else
      {
          unsigned short s = 5 * (double(cpu.mod) / double(max_mod));
          if (s == 0 && cpu.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }
  }

  if (cpu.buffered)
      for (unsigned short i = 0; i<= cpu.buff_size+1; i++)
          cout << "\033[1A";
  else
      cout << "\033[3A";

  cout << "\033[1A \n\n";

  if (memory.buffered)
  {
      for (unsigned short i = 0; i< memory.buff_size; i++)
      {
          if (i != memory.current_probe)
          {
              if (memory.probes[i] <= memory.stable - memory.loseless)
              {
                  if (memory.probes[i] < 10)
                    cout << "\033[1;32m\033[8C  " << (unsigned short)memory.probes[i] << "\n";
                  else
                    cout << "\033[1;32m\033[8C " << (unsigned short)memory.probes[i] << "\n";
              }
              else if (memory.probes[i] >= memory.stable + memory.loseless)
              {
                  if (memory.probes[i] < 10)
                    cout << "\033[1;31m\033[8C  " << (unsigned short)memory.probes[i] << "\n";
                  else
                    cout << "\033[1;31m\033[8C " << (unsigned short)memory.probes[i] << "\n";
              }
              else
              {
                  if (memory.probes[i] < 10)
                    cout << "\033[1;30m\033[8C  " << (unsigned short)memory.probes[i] << "\n";
                  else
                    cout << "\033[1;30m\033[8C " << (unsigned short)memory.probes[i] << "\n";
              }
          }
          else
          {
              if (memory.probes[i] < 10)
                cout << "\033[1;33m\033[8C> " << (unsigned short)memory.probes[i] << "\n";
              else
                cout << "\033[1;33m\033[8C>" << (unsigned short)memory.probes[i] << "\n";
          }

      }
      for (unsigned short i = 0; i<= memory.buff_size; i++)
          cout << "\033[1A";
      cout << "\n";

      for (unsigned short i = 0; i< memory.buff_size; i++)
      {
          if (i != memory.current_probe_small)
          {
              if (memory.sector_small[i] <= memory.stable - memory.loseless)
              {
                  if (memory.sector_small[i] < 10)
                      cout << "\033[1;32m\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;32m\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
              }
              else if (memory.sector_small[i] >= memory.stable + memory.loseless)
              {
                  if (memory.sector_small[i] < 10)
                      cout << "\033[1;31m\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;31m\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
              }
              else
              {
                  if (memory.sector_small[i] < 10)
                      cout << "\033[1;30m\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                  else
                      cout << "\033[1;30m\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
              }
          }
          else
          {
              if (memory.sector_small[i] < 10)
                  cout << "\033[1;33m\033[12C" << (unsigned short)memory.sector_small[i] << " <\n";
              else
                  cout << "\033[1;33m\033[12C" << (unsigned short)memory.sector_small[i] << "<\n";
          }
      }
  }
  else
  {
      if (memory.load <= memory.stable - memory.loseless)
          cout << "\033[1;32m\033[9C" << (unsigned short)memory.load << " \n";
      else if (memory.load >= memory.stable + memory.loseless)
          cout << "\033[1;31m\033[9C" << (unsigned short)memory.load << " \n";
      else
          cout << "\033[1;30m\033[9C" << (unsigned short)memory.load << " \n";
  }

  cout << "\n" << "\033[9C";
  if (memory.frequency == 'l')
  {
      if (memory.mod <= 0)
      {
          unsigned short s = 5 * (double(-memory.mod) / double(memory.lin_num*memory.steps));
          if (s == 0 && memory.mod < 0)
              s = 1;
          cout << "\033[1;32m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }
      else
      {
          unsigned short s = 5 * (double(memory.mod) / double(memory.lin_num*memory.steps));
          if (s == 0 && memory.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
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
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
      }
      else
      {
          unsigned short s = 5 * (double(memory.mod) / double(memory.steps*memory.steps));
          if (s == 0 && memory.mod > 0)
              s = 1;
          cout << "\033[1;31m";
          for ( unsigned int i = s; i > 0; i--)
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
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
              cout << "=";
          cout << "\033[1;30m";
          for (unsigned short m = 5 - s; m > 0; m--)
              cout << "=";
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
  /*
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
  */

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

  cout << "\033[15A\n\033[1A\033[74C";






  if (core_step % 60 < 15)
  {
      cout << "cpu    " << "\033[2C\033[1;30m";
      spacefill(15 - (core_step%60), 2);
      cout << "\n\033[79C\033[33m" << cpu.calculate() << "\n\n\033[70C";
      for (int i = 10; i>=0; i--)
      {
          for (int j = 0; j<=cpu.EQsize;j++)
          {
              cout << "   ";
          }
          cout << "\n\033[70C";
      }
      cout << "\n\033[12A\033[70C";
      if (!autocalc.cpu_enabled)
      {
          for (int i = 10; i>=0; i--)
          {
              for (int j = 0; j<=cpu.EQsize;j++)
              {
                  if (cpu.EQ[j] >= i*10)
                  {
                      if (cpu.load >= (100/cpu.EQsize)*j && cpu.load < (100/cpu.EQsize)*(j+1))
                          cout << "\033[1;34m[] ";
                      else
                          cout << "\033[1;36m[] ";
                  }
                  else
                      cout << "   ";
              }
              cout << "\033[1;33m\n\033[70C";
          }
      }
      else
      {
          if (core_step % 15 < 5 || autocalc.start_delay != 0)
          {
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=cpu.EQsize;j++)
                  {
                      if (autocalc.cpu_virtualEQ[j] >= i*10)
                      {
                          if (cpu.load >= (100/cpu.EQsize)*j && cpu.load < (100/cpu.EQsize)*(j+1))
                              cout << "\033[1;34m[] ";
                          else
                              cout << "\033[1;36m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else if (core_step % 15 < 10)
          {
              double correction = 100.0/autocalc.cpu_common;
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=cpu.EQsize;j++)
                  {
                      if (autocalc.cpu_freq[j]*correction >= i*10)
                      {
                          if (j == autocalc.cpu_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.cpu_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.cpu_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else
          {
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=cpu.EQsize;j++)
                  {
                      if (autocalc.cpu_curve[j] >= i*10)
                      {
                          if (j == autocalc.cpu_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.cpu_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.cpu_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
      }

      cout << "\n\033[70C";
      for (int i = 0; i<=cpu.EQsize; i++)
      {
          spacefill((100/cpu.EQsize)*i, 3);
      }
      if (autocalc.cpu_enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C";
          spacefill (autocalc.start_delay, 4);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.cpu_common, 3);
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.cpu_exoticlow, 3);
          cout << "\033[2C";
          spacefill (autocalc.cpu_exotichigh, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.cpu_freq_angle_low, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.cpu_freq_angle_high, 3);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_cpu)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.cpu_mult_low, 4);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_cpu)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.cpu_mult_high, 4);
      }
      else if (autocalc.enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C    ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\033[2C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
      }
  }
  else if (core_step % 60 >= 15 && core_step % 60 < 30)
  {
      cout << "memory " << "\033[2C\033[1;30m";
      spacefill(30 - (core_step%60), 2);
      cout << "\n\033[79C\033[33m";
      spacefill (memory.calculate(), 2);
      cout << "\n\n\033[70C";
      for (int i = 10; i>=0; i--)
      {
          for (int j = 0; j<=memory.EQsize;j++)
          {
              cout << "   ";
          }
          cout << "\n\033[70C";
      }
      cout << "\n\033[12A\033[70C";
      if (!autocalc.memory_enabled)
      {
          for (int i = 10; i>=0; i--)
          {
              for (int j = 0; j<=memory.EQsize;j++)
              {
                  if (memory.EQ[j] >= i*10)
                  {
                      if (memory.load >= (100/memory.EQsize)*j && memory.load < (100/memory.EQsize)*(j+1))
                          cout << "\033[1;34m[] ";
                      else
                          cout << "\033[1;36m[] ";
                  }
                  else
                      cout << "   ";
              }
              cout << "\033[1;33m\n\033[70C";
          }
      }
      else
      {
          if (core_step % 15 < 5 || autocalc.start_delay != 0)
          {
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=memory.EQsize;j++)
                  {
                      if (autocalc.memory_virtualEQ[j] >= i*10)
                      {
                          if (memory.load >= (100/memory.EQsize)*j && memory.load < (100/memory.EQsize)*(j+1))
                              cout << "\033[1;34m[] ";
                          else
                              cout << "\033[1;36m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else if (core_step % 15 < 10)
          {
              double correction = 100.0/autocalc.memory_common;
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=memory.EQsize;j++)
                  {
                      if (autocalc.memory_freq[j]*correction >= i*10)
                      {
                          if (j == autocalc.memory_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.memory_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.memory_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else
          {
              double correction = 100.0/autocalc.memory_common;
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=memory.EQsize;j++)
                  {
                      if (autocalc.memory_curve[j] >= i*10)
                      {
                          if (j == autocalc.memory_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.memory_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.memory_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
      }

      cout << "\n\033[70C";
      for (int i = 0; i<=memory.EQsize; i++)
      {
          spacefill((100/memory.EQsize)*i, 3);
      }
      if (autocalc.memory_enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C";
          spacefill (autocalc.start_delay, 4);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.memory_common, 3);
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.memory_exoticlow, 3);
          cout << "\033[2C";
          spacefill (autocalc.memory_exotichigh, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.memory_freq_angle_low, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.memory_freq_angle_high, 3);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_memory)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.memory_mult_low, 4);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_memory)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.memory_mult_high, 4);
      }
      else if (autocalc.enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C    ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\033[2C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
      }
  }
  else if (core_step % 60 >= 30 && core_step % 60 < 45)
  {
      cout << "thermal" << "\033[2C\033[1;30m";
      spacefill(45 - (core_step%60), 2);
      cout << "\n\033[79C\033[33m";
      spacefill(temperature.calculate(), 2);
      cout << "\n\n\033[70C";
      for (int i = 10; i>=0; i--)
      {
          for (int j = 0; j<=temperature.EQsize;j++)
          {
              cout << "   ";
          }
          cout << "\n\033[70C";
      }
      cout << "\n\033[12A\033[70C";
      if (!autocalc.temperature_enabled)
      {
          for (int i = 10; i>=0; i--)
          {
              for (int j = 0; j<=temperature.EQsize;j++)
              {
                  if (temperature.EQ[j] >= i*10)
                  {
                      if (temperature.value >= temperature.EQbegin + ((temperature.EQend-temperature.EQbegin)/temperature.EQsize)*j && temperature.value < temperature.EQbegin + ((temperature.EQend-temperature.EQbegin)/temperature.EQsize)*(j+1))
                          cout << "\033[1;34m[] ";
                      else
                          cout << "\033[1;36m[] ";
                  }
                  else
                      cout << "   ";
              }
              cout << "\033[1;33m\n\033[70C";
          }
      }
      else
      {
          if (core_step % 15 < 5 || autocalc.start_delay != 0)
          {
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=temperature.EQsize;j++)
                  {
                      if (autocalc.temperature_virtualEQ[j] >= i*10)
                      {
                          if (temperature.value >= temperature.EQbegin + ((temperature.EQend-temperature.EQbegin)/temperature.EQsize)*j && temperature.value < temperature.EQbegin + ((temperature.EQend-temperature.EQbegin)/temperature.EQsize)*(j+1))
                              cout << "\033[1;34m[] ";
                          else
                              cout << "\033[1;36m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else if (core_step % 15 < 10)
          {
              double correction = 100.0/autocalc.temperature_common;
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=temperature.EQsize;j++)
                  {
                      if (autocalc.temperature_freq[j]*correction >= i*10)
                      {
                          if (j == autocalc.temperature_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.temperature_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.temperature_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else
          {
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=temperature.EQsize;j++)
                  {
                      if (autocalc.temperature_curve[j] >= i*10)
                      {
                          if (j == autocalc.temperature_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.temperature_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.temperature_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
      }


      cout << "\n\033[70C";
      for (int i = 0; i<=temperature.EQsize; i++)
      {
          spacefill(temperature.EQbegin + ((temperature.EQend-temperature.EQbegin)/temperature.EQsize)*i, 3);
      }
      if (autocalc.temperature_enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C";
          spacefill (autocalc.start_delay, 4);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.temperature_common, 3);
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.temperature_exoticlow, 3);
          cout << "\033[2C";
          spacefill (autocalc.temperature_exotichigh, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.temperature_freq_angle_low, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.temperature_freq_angle_high, 3);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_temperature)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.temperature_mult_low, 4);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_temperature)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.temperature_mult_high, 4);
      }
      else if (autocalc.enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C    ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\033[2C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
      }
  }
  else
  {
      cout << "battery" << "\033[2C\033[1;30m";
      spacefill(60 - (core_step%60), 2);
      cout << "\n\033[79C\033[33m";
      spacefill(battery.calculate(), 2);
      cout << "\n\n\033[70C";
      for (int i = 10; i>=0; i--)
      {
          for (int j = 0; j<=battery.EQsize;j++)
          {
              cout << "   ";
          }
          cout << "\n\033[70C";
      }
      cout << "\n\033[12A\033[70C";
      if (!autocalc.battery_enabled)
      {
          for (int i = 10; i>=0; i--)
          {
              for (int j = 0; j<=battery.EQsize;j++)
              {
                  if (battery.EQ[j] >= i*10)
                  {
                      if (battery.load >= (100/battery.EQsize)*j && battery.load < (100/battery.EQsize)*(j+1))
                          cout << "\033[1;34m[] ";
                      else
                          cout << "\033[1;36m[] ";
                  }
                  else
                      cout << "   ";
              }
              cout << "\033[1;33m\n\033[70C";
          }
      }
      else
      {
          if (core_step % 15 < 5 || autocalc.start_delay != 0)
          {
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=battery.EQsize;j++)
                  {
                      if (autocalc.battery_virtualEQ[j] >= i*10)
                      {
                          if (battery.load >= (100/battery.EQsize)*j && battery.load < (100/battery.EQsize)*(j+1))
                              cout << "\033[1;34m[] ";
                          else
                              cout << "\033[1;36m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else if (core_step % 15 < 10)
          {
              double correction = 100.0/autocalc.battery_common;
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=battery.EQsize;j++)
                  {
                      if (autocalc.battery_freq[j]*correction >= i*10)
                      {
                          if (j == autocalc.battery_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.battery_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.battery_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
          else
          {
              double correction = 100.0/autocalc.battery_common;
              for (int i = 10; i>=0; i--)
              {
                  for (int j = 0; j<=battery.EQsize;j++)
                  {
                      if (autocalc.battery_curve[j] >= i*10)
                      {
                          if (j == autocalc.battery_curmid)
                              cout << "\033[36m[] ";
                          else if (j == autocalc.battery_stablepointlow)
                              cout << "\033[1;31m[] ";
                          else if ( j == autocalc.battery_stablepointhigh)
                              cout << "\033[1;32m[] ";
                          else
                              cout << "\033[35m[] ";
                      }
                      else
                          cout << "   ";
                  }
                  cout << "\033[1;33m\n\033[70C";
              }
          }
      }

      cout << "\n\033[70C";
      for (int i = 0; i<=battery.EQsize; i++)
      {
          spacefill((100/battery.EQsize)*i, 3);
      }
      if (autocalc.battery_enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C";
          spacefill (autocalc.start_delay, 4);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.battery_common, 3);
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.battery_exoticlow, 3);
          cout << "\033[2C";
          spacefill (autocalc.battery_exotichigh, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.battery_freq_angle_low, 3);
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C";
          spacefill (autocalc.battery_freq_angle_high, 3);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_battery)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.battery_mult_low, 4);
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          if (autocalc.auto_battery)
          {
              cout <<  "\033[37m";
          }
          else
              cout << "\033[1;30m";
          cout << "A\033[2C\033[33m";
          spacefill (10*autocalc.battery_mult_high, 4);
      }
      else if (autocalc.enabled)
      {
          cout << "\033[1A\n\033[13A";
          cout << "\033[" << max_EQ*3 + 90 << "C    ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\033[2C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 86 << "C   ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
          cout << "\n\n\033[" << max_EQ*3 + 87 << "C";
          cout << " \033[2C    ";
      }
  }
      cout << "\033[1A\n\033[13A";
      cout << "\n\033[" << max_EQ*3 + 76 << "C";
      if (bulwers.wkup_reason == 0)
          cout << " ---- ";
      else if (bulwers.wkup_reason == 1)
          cout << " time ";
      else if (bulwers.wkup_reason == 2)
          cout << "motion";
      cout << "\n\n\033[" << max_EQ*3 + 76 << "C";
      if (bulwers.wkup_reason == 0)
          cout << " ---- ";
      else if (bulwers.wkup_active == 1)
          cout << " time ";
      else if (bulwers.wkup_active == 2)
          cout << "wakeup";
      else if (bulwers.wkup_active == 3)
          cout << " both ";
      cout << "\n\n\033[" << max_EQ*3 + 76 << "C";
      spacefill (bulwers.current_wkup_delay, 7);
      cout << "\n\n\033[" << max_EQ*3 + 82 << "C";
      if (bulwers.flue)
          cout << "X";
      else
          cout << " ";
      cout << "\n\n\n\033[" << max_EQ*3 + 76 << "C ";
      spacefill(100*(bulwers.fluehighval-bulwers.fluelowval)/bulwers.flueamplitude, 3);
      cout << "%\n\n\033[" << max_EQ*3 + 76 << "C";
      spacefill(bulwers.fluelowval, 2);
      cout << "º ";
      spacefill(bulwers.fluehighval, 2);
      cout << "º";
      cout << "\n\033[" << max_EQ*3 + 76 << "C <";
      spacefill(bulwers.fluehighval-bulwers.fluelowval, 2);
      cout << "º>";
      cout << "\033[1A\n\033[13A";

  if (core->hdbg_enabled)
  {
    spacer = max_EQ*3 + 86;
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
    cout << "bcapa" << " " << HRDWR.battery_capacity;
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
    cout << "tempt" << " " << bulwers.flueamplitude;
    chck_s ();
    cout << "flutm" << " " << bulwers.fluetimer;
    chck_s ();
    cout << "gtflu" << " " << bulwers.flue;
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
