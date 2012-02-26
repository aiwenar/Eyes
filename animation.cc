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

#include <cstdlib>
#include <iostream>

#include "animation.hxx"
#include "defines.hxx"
#include <debug.hxx>

using namespace std;

animation::animation ( int nsize ) : size ( nsize ), act_frame ( 0 )
{
    frames = new const char*[size];
    if ( frames == nil )
    {
        error << "(animation) cannot allocate memory.\n";
        exit ( 12 ); // error code 12 : cannot allocate memory
    }
}

animation * animation::add_frame ( const char * f )
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

void animation::registerAnimations ( QMap <QString,animation*> * map )
{
    map->insert ( "cusual_01_close", ( new animation ( 5 ) )->add_frame ("clap_01")->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "cusual_01_open", ( new animation ( 4 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02")->add_frame("clap_01") );
    map->insert ( "bul_01_close", ( new animation ( 5 ) )->add_frame ("clap_01")->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_01_open", ( new animation ( 4 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02")->add_frame("clap_01") );
    map->insert ( "bul_02_close", ( new animation ( 5 ) )->add_frame ("clap_01")->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_02_open", ( new animation ( 4 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02")->add_frame("clap_01") );
    map->insert ( "bul_03_close", ( new animation ( 5 ) )->add_frame ("clap_01")->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_03_open", ( new animation ( 4 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02")->add_frame("clap_01") );
    map->insert ( "bul_04_close", ( new animation ( 5 ) )->add_frame ("clap_01")->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_04_open", ( new animation ( 4 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02")->add_frame("clap_01") );
    map->insert ( "bul_05_close", ( new animation ( 4 ) )->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_05_open", ( new animation ( 3 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02") );
    map->insert ( "bul_06_close", ( new animation ( 4 ) )->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_06_open", ( new animation ( 3 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02") );
    map->insert ( "bul_07_close", ( new animation ( 4 ) )->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_07_open", ( new animation ( 3 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02") );
    map->insert ( "bul_08_close", ( new animation ( 4 ) )->add_frame ("clap_02")->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_08_open", ( new animation ( 3 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" )->add_frame("clap_02") );
    map->insert ( "bul_09_close", ( new animation ( 6 ) )->add_frame ( "bclap_01" )->add_frame ( "bclap_02" )->add_frame ( "bclap_03" )->add_frame("bclap_04")->add_frame("bclap_05")->add_frame("bclap_06") );
    map->insert ( "bul_09_open", ( new animation ( 5 ) )->add_frame ( "bclap_05" )->add_frame ( "bclap_04" )->add_frame("bclap_03")->add_frame("bclap_02")->add_frame("bclap_01") );
    map->insert ( "bul_10_close", ( new animation ( 6 ) )->add_frame ( "bclap_01" )->add_frame ( "bclap_02" )->add_frame ( "bclap_03" )->add_frame("bclap_04")->add_frame("bclap_05")->add_frame("bclap_06") );
    map->insert ( "bul_10_open", ( new animation ( 5 ) )->add_frame ( "bclap_05" )->add_frame ( "bclap_04" )->add_frame("bclap_03")->add_frame("bclap_02")->add_frame("bclap_01") );
    map->insert ( "bul_11_close", ( new animation ( 6 ) )->add_frame ( "bclap_01" )->add_frame ( "bclap_02" )->add_frame ( "bclap_03" )->add_frame("bclap_04")->add_frame("bclap_05")->add_frame("bclap_06") );
    map->insert ( "bul_11_open", ( new animation ( 5 ) )->add_frame ( "bclap_05" )->add_frame ( "bclap_04" )->add_frame("bclap_03")->add_frame("bclap_02")->add_frame("bclap_01") );
    map->insert ( "bul_12_close", ( new animation ( 6 ) )->add_frame ( "bhclap_01" )->add_frame ( "bhclap_02" )->add_frame ( "bhclap_03" )->add_frame("bhclap_04")->add_frame("bhclap_05")->add_frame("bhclap_06") );
    map->insert ( "bul_12_open", ( new animation ( 5 ) )->add_frame ( "bhclap_05" )->add_frame ( "bhclap_04" )->add_frame("bhclap_03")->add_frame("bhclap_02")->add_frame("bhclap_01") );
    map->insert ( "bul_13_close", ( new animation ( 6 ) )->add_frame ( "bhclap_01" )->add_frame ( "bhclap_02" )->add_frame ( "bhclap_03" )->add_frame("bhclap_04")->add_frame("bhclap_05")->add_frame("bhclap_06") );
    map->insert ( "bul_13_open", ( new animation ( 5 ) )->add_frame ( "bhclap_05" )->add_frame ( "bhclap_04" )->add_frame("bhclap_03")->add_frame("bhclap_02")->add_frame("bhclap_01") );
    map->insert ( "bul_14_close", ( new animation ( 6 ) )->add_frame ( "bhclap_01" )->add_frame ( "bhclap_02" )->add_frame ( "bhclap_03" )->add_frame("bhclap_04")->add_frame("bhclap_05")->add_frame("bhclap_06") );
    map->insert ( "bul_14_open", ( new animation ( 5 ) )->add_frame ( "bhclap_05" )->add_frame ( "bhclap_04" )->add_frame("bhclap_03")->add_frame("bhclap_02")->add_frame("bhclap_01") );
    map->insert ( "bul_15_close", ( new animation ( 6 ) )->add_frame ( "bhclap_01" )->add_frame ( "bhclap_02" )->add_frame ( "bhclap_03" )->add_frame("bhclap_04")->add_frame("bhclap_05")->add_frame("bhclap_06") );
    map->insert ( "bul_15_open", ( new animation ( 5 ) )->add_frame ( "bhclap_05" )->add_frame ( "bhclap_04" )->add_frame("bhclap_03")->add_frame("bhclap_02")->add_frame("bhclap_01") );
    map->insert ( "bul_16_close", ( new animation ( 0 ) ) );
    map->insert ( "bul_16_open", ( new animation ( 0 ) ) );
    map->insert ( "sh_01_close", ( new animation ( 6 ) )->add_frame ("cusual_01")->add_frame("clap_01")->add_frame("clap_02")->add_frame("clap_03")->add_frame("clap_04")->add_frame("clap_05") );
    map->insert ( "sh_01_open", ( new animation ( 0 ) ) );
    map->insert ( "sh_02_close", ( new animation ( 0 ) ) );
    map->insert ( "sh_02_open", ( new animation ( 7 ) )->add_frame ("clap_04")->add_frame("clap_03")->add_frame("clap_02")->add_frame("clap_01")->add_frame("cusual_01")->add_frame("sh_02")->add_frame("sh_01") );
    map->insert ( "slp_01_close", ( new animation ( 5 ) )->add_frame ( "slp_06" )->add_frame ( "slp_07" )->add_frame ( "slp_08" )->add_frame ( "slp_09" )->add_frame ( "slp_10" ) );
    map->insert ( "slp_01_open", ( new animation ( 4 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" )->add_frame( "slp_06" ) );
    map->insert ( "slp_02_close", ( new animation ( 5 ) )->add_frame ( "slp_06" )->add_frame ( "slp_07" )->add_frame ( "slp_08" )->add_frame ( "slp_09" )->add_frame ( "slp_10" ) );
    map->insert ( "slp_02_open", ( new animation ( 4 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" )->add_frame( "slp_06" ) );
    map->insert ( "slp_03_close", ( new animation ( 5 ) )->add_frame ( "slp_06" )->add_frame ( "slp_07" )->add_frame ( "slp_08" )->add_frame ( "slp_09" )->add_frame ( "slp_10" ) );
    map->insert ( "slp_03_open", ( new animation ( 4 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" )->add_frame( "slp_06" ) );
    map->insert ( "slp_04_close", ( new animation ( 5 ) )->add_frame ( "slp_06" )->add_frame ( "slp_07" )->add_frame ( "slp_08" )->add_frame ( "slp_09" )->add_frame ( "slp_10" ) );
    map->insert ( "slp_04_open", ( new animation ( 4 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" )->add_frame( "slp_06" ) );
    map->insert ( "slp_05_close", ( new animation ( 5 ) )->add_frame ( "slp_06" )->add_frame ( "slp_07" )->add_frame ( "slp_08" )->add_frame ( "slp_09" )->add_frame ( "slp_10" ) );
    map->insert ( "slp_05_open", ( new animation ( 4 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" )->add_frame( "slp_06" ) );
    map->insert ( "slp_06_close", ( new animation ( 4 ) )->add_frame ( "slp_07" )->add_frame ( "slp_08" )->add_frame ( "slp_09" )->add_frame ( "slp_10" ) );
    map->insert ( "slp_06_open", ( new animation ( 3 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" ) );
    map->insert ( "slp_10_close", ( new animation ( 0 ) ) );
    map->insert ( "slp_10_open", ( new animation ( 0 ) ) );
    map->insert ( "hpp_01_close", ( new animation ( 7 ) )->add_frame ("hpp_01")->add_frame("hpp_02")->add_frame("hpp_03")->add_frame("hpp_04")->add_frame("hpp_05")->add_frame("hpp_06")->add_frame("hpp_07") );
    map->insert ( "hpp_01_open", ( new animation ( 6 ) )->add_frame("hpp_06")->add_frame("hpp_05")->add_frame("hpp_04")->add_frame("hpp_03")->add_frame("hpp_02")->add_frame("hpp_01") );
    map->insert ( "hpp_02_close", ( new animation ( 6 ) )->add_frame("hpp_02")->add_frame("hpp_03")->add_frame("hpp_04")->add_frame("hpp_05")->add_frame("hpp_06")->add_frame("hpp_07") );
    map->insert ( "hpp_02_open", ( new animation ( 5 ) )->add_frame("hpp_06")->add_frame("hpp_05")->add_frame("hpp_04")->add_frame("hpp_03")->add_frame("hpp_02") );
    map->insert ( "hpp_03_close", ( new animation ( 5 ) )->add_frame("hpp_03")->add_frame("hpp_04")->add_frame("hpp_05")->add_frame("hpp_06")->add_frame("hpp_07") );
    map->insert ( "hpp_03_open", ( new animation ( 4 ) )->add_frame("hpp_06")->add_frame("hpp_05")->add_frame("hpp_04")->add_frame("hpp_03") );
    map->insert ( "hpp_04_close", ( new animation ( 4 ) )->add_frame("hpp_04")->add_frame("hpp_05")->add_frame("hpp_06")->add_frame("hpp_07") );
    map->insert ( "hpp_04_open", ( new animation ( 3 ) )->add_frame("hpp_06")->add_frame("hpp_05")->add_frame("hpp_04") );
    map->insert ( "hit_close", ( new animation ( 2 ) )->add_frame ( "bhclap_02" )->add_frame ( "bul_16" ) );
    map->insert ( "hit_open", ( new animation ( 4 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" )->add_frame ( "slp_07" ) );
    map->insert ( "hpp_continue", ( new animation ( 0 ) ) );
    map->insert ( "hit_continue", ( new animation ( 0 ) ) );
}
