#ifndef _eyes_clap_animations_hxx
#define _eyes_clap_animations_hxx

#include <QMap>
#include "defines.hxx"
#include "animation.hxx"

// dodawaj animki z nazwami anim_<name>_[<nr>_]_<open/close>
// a potem updatuj w register_animations

static animation anim_bul_01_close = {
    5, { "clap_01", "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_01_open = {
    4, { "clap_04", "clap_03", "clap_02", "clap_01" }
};

static animation anim_bul_02_close = {
    5, { "clap_01", "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_02_open = {
    4, { "clap_04", "clap_03", "clap_02", "clap_01" }
};

static animation anim_bul_03_close = {
    5, { "clap_01", "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_03_open = {
    4, { "clap_04", "clap_03", "clap_02", "clap_01" }
};

static animation anim_bul_04_close = {
    5, { "clap_01", "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_04_open = {
    4, { "clap_04", "clap_03", "clap_02", "clap_01" }
};

static animation anim_bul_05_close = {
    5, { "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_05_open = {
    3, { "clap_04", "clap_03", "clap_02" }
};

static animation anim_bul_06_close = {
    4, { "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_06_open = {
    3, { "clap_04", "clap_03", "clap_02" }
};

static animation anim_bul_07_close = {
    4, { "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_07_open = {
    3, { "clap_04", "clap_03", "clap_02" }
};

static animation anim_bul_08_close = {
    4, { "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_08_open = {
    3, { "clap_04", "clap_03", "clap_02" }
};

static animation anim_bul_09_close = {
    3, { "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_09_open = {
    2, { "clap_04", "clap_03" }
};

static animation anim_bul_10_close = {
    3, { "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_10_open = {
    2, { "clap_04", "clap_03" }
};

static animation anim_bul_11_close = {
    3, { "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_11_open = {
    2, { "clap_04", "clap_03" }
};

static animation anim_bul_12_close = {
    3, { "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_12_open = {
    2, { "clap_04", "clap_03" }
};

static animation anim_bul_13_close = {
    3, { "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_13_open = {
    2, { "clap_04", "clap_03" }
};

static animation anim_bul_14_close = {
    3, { "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_14_open = {
    2, { "clap_04", "clap_03" }
};

static animation anim_bul_15_close = {
    3, { "clap_03", "clap_04", "clap_05" }
};

static animation anim_bul_15_open = {
    2, { "clap_04", "clap_03" }
};

static animation anim_bul_16_close = {
    0, { }
};

static animation anim_bul_16_open = {
    0, { }
};

static animation anim_sh_02_close = {
    0, { }
};

static animation anim_sh_02_open = {
    7, { "clap_04", "clap_03", "clap_02", "clap_01", "cusual_01", "sh_02", "sh_01" }
};

static animation anim_sh_01_close = {
    6, { "cusual_01", "clap_01", "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_sh_01_open = {
    0, { }
};

static animation anim_cusual_01_close = {
    5, { "clap_01", "clap_02", "clap_03", "clap_04", "clap_05" }
};

static animation anim_cusual_01_open = {
    4, { "clap_04", "clap_03", "clap_02", "clap_01" }
};

static animation anim_slp_01_close = {
    5, { "slp_06", "slp_07", "slp_08", "slp_09", "slp_10" }
};


static animation anim_slp_01_open = {
    4, { "slp_09", "slp_08", "slp_07", "slp_06" }
};

static animation anim_slp_02_close = {
    5, { "slp_06", "slp_07", "slp_08", "slp_09", "slp_10" }
};

static animation anim_slp_02_open = {

    4, { "slp_09", "slp_08", "slp_07", "slp_06" }
};

static animation anim_slp_03_close = {
    5, { "slp_06", "slp_07", "slp_08", "slp_09", "slp_10" }
};

static animation anim_slp_03_open = {
    4, { "slp_09", "slp_08", "slp_07", "slp_06" }
};

static animation anim_slp_04_close = {
    5, { "slp_06", "slp_07", "slp_08", "slp_09", "slp_10" }
};

static animation anim_slp_04_open = {
    4, { "slp_09", "slp_08", "slp_07", "slp_06" }
};


static animation anim_slp_05_close = {
    5, { "slp_06", "slp_07", "slp_08", "slp_09", "slp_10" }
};

static animation anim_slp_05_open = {
    4, { "slp_09", "slp_08", "slp_07", "slp_06" }

};

static animation anim_slp_06_close = {
    4, { "slp_07", "slp_08", "slp_09", "slp_10" }
};

void register_animations ( QMap <QString,animation*> * map )
{
    map->insert ( "slp_06_close", ( new animation ( 4 ) )->add_frame ( "slp_07" )->add_frame ( "slp_08" )->add_frame ( "slp_09" )->add_frame ( "slp_10" ) );
    map->insert ( "slp_06_open", ( new animation ( 3 ) )->add_frame ( "slp_09" )->add_frame ( "slp_08" )->add_frame ( "slp_07" ) );
    map->insert ( "slp_10_close", ( new animation ( 0 ) ) );
    map->insert ( "slp_10_open", ( new animation ( 0 ) ) );
}

#endif //eyes_clap_animations_hxx
