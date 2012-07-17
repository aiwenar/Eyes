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
    min_dx = cfg->lookupValue ( "ui.looker.delta_x.min", 0 );
    max_dx = cfg->lookupValue ( "ui.looker.delta_x.max", 14 );
    min_dy = cfg->lookupValue ( "ui.looker.delta_y.min", 0 );
    max_dy = cfg->lookupValue ( "ui.looker.delta_y.max", 10 );
    bmin_x = cfg->lookupValue ( "ui.looker.bounds_x.min", 40 );
    bmax_x = cfg->lookupValue ( "ui.looker.bounds_x.max", 54 );
    bmin_y = cfg->lookupValue ( "ui.looker.bounds_y.min", 4 );
    bmax_y = cfg->lookupValue ( "ui.looker.bounds_y.max", 14 );
    min_dl = cfg->lookupValue ( "ui.looker.delay.min", 1 );
    max_dl = cfg->lookupValue ( "ui.looker.delay.max", 30 );
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
  look_at ( x-eyes->get_eyes_x1 (), y-eyes->get_eyes_y () );
  timer->setInterval ( 2000 );
  eyes->repaint ();
}

void eyes_looker::look_at ( int dx, int dy )
{
  static int px1;
  static int px2;
  static int py;
  static double mpx1;
  static double mpx2;
  static double mpy;
  px1 = eyes->get_eyes_x1 ();
  px2 = eyes->get_eyes_x2 ();
  py = eyes->get_eyes_y ();
  mpx1 = eyes->get_mirror_x1 ();
  mpx2 = eyes->get_mirror_x2 ();
  mpy = eyes->get_mirror_y ();
  if ( px1+dx < bmin_x or px1+dx > bmax_x )
      dx = 0;
  if ( py+dy < bmin_y or py+dy > bmax_y )
      dy = 0;
  eyes->set_eyes_position ( px1+dx, px2+dx, py+dy );
  eyes->set_mirror_position ( mpx1+(dx!=0?(double(dx)/3):0), mpx2+(dx!=0?(double(dx)/3):0), mpy+(dy!=0?(double(dy)/3):0) );
}
