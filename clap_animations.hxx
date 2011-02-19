#ifndef _eyes_clap_animations_hxx
#define _eyes_clap_animations_hxx

#include <QMap>
#include "defines.hxx"
#include "animation.hxx"

// dodawaj animki z nazwami anim_<name>_[<nr>_]_<open/close>
// a potem updatuj w register_animations



void register_animations ( QMap <QString,animation*> * map )
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
    map->insert ( "bul_09_close", ( new animation ( 3 ) )->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_09_open", ( new animation ( 2 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" ) );
    map->insert ( "bul_10_close", ( new animation ( 3 ) )->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_10_open", ( new animation ( 2 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" ) );
    map->insert ( "bul_11_close", ( new animation ( 3 ) )->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_11_open", ( new animation ( 2 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" ) );
    map->insert ( "bul_12_close", ( new animation ( 3 ) )->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_12_open", ( new animation ( 2 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" ) );
    map->insert ( "bul_13_close", ( new animation ( 3 ) )->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_13_open", ( new animation ( 2 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" ) );
    map->insert ( "bul_14_close", ( new animation ( 3 ) )->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_14_open", ( new animation ( 2 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" ) );
    map->insert ( "bul_15_close", ( new animation ( 3 ) )->add_frame ( "clap_03" )->add_frame ( "clap_04" )->add_frame ( "clap_05" ) );
    map->insert ( "bul_15_open", ( new animation ( 2 ) )->add_frame ( "clap_04" )->add_frame ( "clap_03" ) );
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
}

#endif //eyes_clap_animations_hxx
