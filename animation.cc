/* Eyes
 * Copyright (C) 2011  Krzysztof Mędrzycki, Damian Chiliński
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

#include <cstdlib>
#include <iostream>

#include "animation.hxx"
#include "defines.hxx"
#include <debug.hxx>

using namespace std;

animation::animation ( int nsize ) : size ( nsize ), act_frame ( 0 )
{
    frames = new char*[size];
    if ( frames == nil )
    {
        error << "(animation) cannot allocate memory.\n";
        exit ( 12 ); // error code 12 : cannot allocate memory
    }
}

animation * animation::add_frame ( char * f )
{
    if ( act_frame+1 > size )
    {
        error << "(animation) adding new frame to animation, but there isn't any free slot.\n";
        return ( this );
    }
    frames[act_frame] = f;
    act_frame ++;
    return ( this );
}
