#ifndef _eyes_clap_animations_hxx
#define _eyes_clap_animations_hxx

#include <QMap>

// dodawaj animki z nazwami anim_<name>_[<nr>_]_<open/close>
// a potem updatuj w register_animations

static char * anim_bul_01_close[] = {
    (char*)5, "clap_01", "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_01_open[] = {
    (char*)4, "clap_04", "clap_03", "clap_02", "clap_01",
};

static char * anim_bul_02_close[] = {
    (char*)5, "clap_01", "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_02_open[] = {
    (char*)4, "clap_04", "clap_03", "clap_02", "clap_01",
};

static char * anim_bul_03_close[] = {
    (char*)5, "clap_01", "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_03_open[] = {
    (char*)4, "clap_04", "clap_03", "clap_02", "clap_01",
};

static char * anim_bul_04_close[] = {
    (char*)5, "clap_01", "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_04_open[] = {
    (char*)4, "clap_04", "clap_03", "clap_02", "clap_01",
};

static char * anim_bul_05_close[] = {
    (char*)5, "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_05_open[] = {
    (char*)3, "clap_04", "clap_03", "clap_02",
};

static char * anim_bul_06_close[] = {
    (char*)4, "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_06_open[] = {
    (char*)3, "clap_04", "clap_03", "clap_02",
};

static char * anim_bul_07_close[] = {
    (char*)4, "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_07_open[] = {
    (char*)3, "clap_04", "clap_03", "clap_02",
};

static char * anim_bul_08_close[] = {
    (char*)4, "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_08_open[] = {
    (char*)3, "clap_04", "clap_03", "clap_02",
};

static char * anim_bul_09_close[] = {
    (char*)3, "clap_03", "clap_04", "clap_05",

};

static char * anim_bul_09_open[] = {
    (char*)2, "clap_04", "clap_03",
};

static char * anim_bul_10_close[] = {
    (char*)3, "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_10_open[] = {
    (char*)2, "clap_04", "clap_03",
};

static char * anim_bul_11_close[] = {
    (char*)3, "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_11_open[] = {
    (char*)2, "clap_04", "clap_03",
};

static char * anim_bul_12_close[] = {
    (char*)3, "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_12_open[] = {
    (char*)2, "clap_04", "clap_03",
};

static char * anim_bul_13_close[] = {
    (char*)3, "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_13_open[] = {
    (char*)2, "clap_04", "clap_03",
};

static char * anim_bul_14_close[] = {
    (char*)3, "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_14_open[] = {
    (char*)2, "clap_04", "clap_03",
};

static char * anim_bul_15_close[] = {
    (char*)3, "clap_03", "clap_04", "clap_05",
};

static char * anim_bul_15_open[] = {
    (char*)2, "clap_04", "clap_03",
};

static char * anim_bul_16_close[] = {
    (char*)0,
};

static char * anim_bul_16_open[] = {
    (char*)0,
};

static char * anim_cusual_01_close[] = {
    (char*)5, "clap_01", "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_cusual_01_open[] = {
    (char*)4, "clap_04", "clap_03", "clap_02", "clap_01",
};

static char * anim_sh_02_close[] = {
    (char*)0,
};

static char * anim_sh_02_open[] = {
    (char*)7, "clap_04", "clap_03", "clap_02", "clap_01", "cusual_01", "sh_02", "sh_01",
};

static char * anim_sh_01_close[] = {
    (char*)6, "cusual_01", "clap_01", "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_sh_01_open[] = {
    (char*)0,
};

static char * anim_cusual_01_close[] = {
    (char*)5, "clap_01", "clap_02", "clap_03", "clap_04", "clap_05",
};

static char * anim_cusual_01_open[] = {
    (char*)4, "clap_04", "clap_03", "clap_02", "clap_01",
};

static char * anim_slp_01_close[] = {
    (char*)5, "slp_06", "slp_07", "slp_08", "slp_09", "slp_10",
};


static char * anim_slp_01_open[] = {
    (char*)4, "slp_09", "slp_08", "slp_07", "slp_06",
};

static char * anim_slp_02_close[] = {
    (char*)5, "slp_06", "slp_07", "slp_08", "slp_09", "slp_10",
};

static char * anim_slp_02_open[] = {

    (char*)4, "slp_09", "slp_08", "slp_07", "slp_06",
};

static char * anim_slp_03_close[] = {
    (char*)5, "slp_06", "slp_07", "slp_08", "slp_09", "slp_10",
};

static char * anim_slp_03_open[] = {
    (char*)4, "slp_09", "slp_08", "slp_07", "slp_06",
};

static char * anim_slp_04_close[] = {
    (char*)5, "slp_06", "slp_07", "slp_08", "slp_09", "slp_10",
};

static char * anim_slp_04_open[] = {
    (char*)4, "slp_09", "slp_08", "slp_07", "slp_06",
};


static char * anim_slp_05_close[] = {
    (char*)5, "slp_06", "slp_07", "slp_08", "slp_09", "slp_10",
};

static char * anim_slp_05_open[] = {
    (char*)4, "slp_09", "slp_08", "slp_07", "slp_06",

};

static char * anim_slp_06_close[] = {
    (char*)4, "slp_07", "slp_08", "slp_09", "slp_10",
};

static char * anim_slp_06_open[] = {
    (char*)3, "slp_09", "slp_08", "slp_07",
};

static char * anim_slp_10_close[] = {
    (char*)0,
};

static char * anim_slp_10_open[] = {
    (char*)0,
};

void register_animations ( QMap <QString,char**> * map )
{
    map->insert ( "bul_01_close", anim_bul_01_close );
    map->insert ( "bul_01_open", anim_bul_01_open );
    map->insert ( "bul_02_close", anim_bul_02_close );
    map->insert ( "bul_02_open", anim_bul_02_open );
    map->insert ( "bul_03_close", anim_bul_03_close );
    map->insert ( "bul_03_open", anim_bul_03_open );
    map->insert ( "bul_04_close", anim_bul_04_close );
    map->insert ( "bul_04_open", anim_bul_04_open );
    map->insert ( "bul_05_close", anim_bul_05_close );
    map->insert ( "bul_05_open", anim_bul_05_open );
    map->insert ( "bul_06_close", anim_bul_06_close );
    map->insert ( "bul_06_open", anim_bul_06_open );
    map->insert ( "bul_07_close", anim_bul_07_close );
    map->insert ( "bul_07_open", anim_bul_07_open );
    map->insert ( "bul_08_close", anim_bul_08_close );
    map->insert ( "bul_08_open", anim_bul_08_open );
    map->insert ( "bul_09_close", anim_bul_09_close );
    map->insert ( "bul_09_open", anim_bul_09_open );
    map->insert ( "bul_10_close", anim_bul_10_close );
    map->insert ( "bul_10_open", anim_bul_10_open );
    map->insert ( "bul_11_close", anim_bul_11_close );
    map->insert ( "bul_11_open", anim_bul_11_open );
    map->insert ( "bul_12_close", anim_bul_12_close );
    map->insert ( "bul_12_open", anim_bul_12_open );
    map->insert ( "bul_13_close", anim_bul_13_close );
    map->insert ( "bul_13_open", anim_bul_13_open );
    map->insert ( "bul_14_close", anim_bul_14_close );
    map->insert ( "bul_14_open", anim_bul_14_open );
    map->insert ( "bul_15_close", anim_bul_15_close );
    map->insert ( "bul_15_open", anim_bul_15_open );
    map->insert ( "bul_16_close", anim_bul_16_close );
    map->insert ( "bul_16_open", anim_bul_16_open );
    map->insert ( "cusual_01_open", anim_cusual_01_open );
    map->insert ( "cusual_01_close", anim_cusual_01_close );
    map->insert ( "slp_01_open", anim_slp_01_open );
    map->insert ( "slp_01_close", anim_slp_01_close );
    map->insert ( "slp_02_open", anim_slp_02_open );
    map->insert ( "slp_02_close", anim_slp_02_close );
    map->insert ( "slp_03_open", anim_slp_03_open );
    map->insert ( "slp_03_close", anim_slp_03_close );
    map->insert ( "slp_04_open", anim_slp_04_open );
    map->insert ( "slp_04_close", anim_slp_04_close );
    map->insert ( "slp_05_open", anim_slp_05_open );
    map->insert ( "slp_05_close", anim_slp_05_close );
    map->insert ( "slp_06_open", anim_slp_06_open );
    map->insert ( "slp_06_close", anim_slp_06_close );
    map->insert ( "slp_10_open", anim_slp_10_open );
    map->insert ( "slp_10_close", anim_slp_10_close );
    map->insert ( "sh_01_open", anim_sh_01_open );
    map->insert ( "sh_01_close", anim_sh_01_close );
    map->insert ( "sh_02_open", anim_sh_02_open );
    map->insert ( "sh_02_close", anim_sh_02_close );
}

#endif //eyes_clap_animations_hxx
