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

#include "eyes.hxx"
#include "tm.hh"
#include <debug.hxx>

#include <iostream>

using namespace std;

eyes_looker::eyes_looker ( eyes_view * neyes )
{
    info << "(looker) preparing...\n";
    eyes = neyes;
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( look() ) );
    timer->setInterval ( ( qrand () % 30 + 1 )*200 );
    info << "(looker) loading config...\n";
    Configuration * cfg = Configuration::getInstance ();
    ThemeManager * tm = ThemeManager::instance ();
    min_dx = cfg->lookupValue ( ".ui.looker.delta_x.min",    0   )*eyes->size_multiplier;
    max_dx = cfg->lookupValue ( ".ui.looker.delta_x.max",    14  )*eyes->size_multiplier;
    min_dy = cfg->lookupValue ( ".ui.looker.delta_y.min",    0   )*eyes->size_multiplier;
    max_dy = cfg->lookupValue ( ".ui.looker.delta_y.max",    10  )*eyes->size_multiplier;
    bmin_x = tm ->lookupValue ( ".ui.looker.bounds_x.min",   40  )*eyes->size_multiplier;
    bmax_x = tm ->lookupValue ( ".ui.looker.bounds_x.max",   54  )*eyes->size_multiplier;
    bmin_y = tm ->lookupValue ( ".ui.looker.bounds_y.min",   4   )*eyes->size_multiplier;
    bmax_y = tm ->lookupValue ( ".ui.looker.bounds_y.max",   14  )*eyes->size_multiplier;
    min_dl = cfg->lookupValue ( ".ui.looker.delay.min",      1   )*eyes->size_multiplier;
    max_dl = cfg->lookupValue ( ".ui.looker.delay.max",      30  )*eyes->size_multiplier;
    info << "(looker) ready!\n";
}

void eyes_looker::run ()
{
    info << "(looker) working...\n";
    timer->start ();
}

void eyes_looker::look ()
{
  int dx = ( (qrand()%100+1) > 50 ? -1 : 1 )*(qrand() % max_dx + min_dx);
  int dy = ( (qrand()%100+1) > 50 ? -1 : 1 )*(qrand() % max_dy + min_dy);
  look_at ( dx, dy );
  timer->setInterval ( ( qrand() % max_dl + min_dl ) * 200 );
  eyes->repaint ();
}

void eyes_looker::interrupt ( int x, int y )
{
  x = bmin_x + x*(bmax_x-bmin_x)/100;
  y = bmin_y + y*(bmax_y-bmin_y)/100;
  look_at ( x-eyes->get_eyes_x1 (), y-eyes->get_eyes_y1 () );
  timer->stop();
  timer->start( 3000 );
  eyes->repaint ();
}

void eyes_looker::look_at ( int dx, int dy )
{
  int px1 = eyes->get_eyes_x1 ();
  int px2 = eyes->get_eyes_x2 ();
  int py1 = eyes->get_eyes_y1 ();
  int py2 = eyes->get_eyes_y2 ();
  double mpx1 = eyes->get_mirror_x1 ();
  double mpx2 = eyes->get_mirror_x2 ();
  double mpy1 = eyes->get_mirror_y1 ();
  double mpy2 = eyes->get_mirror_y2 ();
  if ( px1+dx < bmin_x or px1+dx > bmax_x )
      dx = 0;
  if ( py1+dy < bmin_y or py1+dy > bmax_y )
      dy = 0;
  eyes->set_eyes_position ( px1+dx, px2+dx, py1+dy, py2+dy );
  eyes->set_mirror_position ( mpx1+(dx!=0?(double(dx)/3):0), mpx2+(dx!=0?(double(dx)/3):0), mpy1+(dy!=0?(double(dy)/3):0), mpy2+(dy!=0?(double(dy)/3):0) );
}
