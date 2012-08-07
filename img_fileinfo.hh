#pragma once

struct _img_loadinfo
{
  const char * file;
  const unsigned char numnames;
  const char * names[];
};

static const _img_loadinfo files[] = {
  { "bul_01_a", 1, { "bul_01_a" } }
};

static const _img_loadinfo eyefiles[] = {
  { "eye_01", 2, { "eye_01_L", "eye_01_R" } }
};

static const _img_loadinfo eyefiles_dual[] = {
  { "eye_01_L", 1, { "eye_01_L" } },
  { "eye_01_R", 1, { "eye_01_R" } }
};
