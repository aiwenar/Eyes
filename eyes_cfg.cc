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

#include "eyes_cfg.hxx"
#include <debug.hxx>

#include <iostream>

using namespace std;

struct key_info
{
    char * path;
    char * type;
    char * desc;
};

const static key_info keys [] = {
    //path to key             key type   key description
    { "ui",                   "section", "Contains eyes ui settings." },
    { "ui.eye",               "section", "Contains eye parametrs." },
    { "ui.eyes",              "section", "Contains eyes parametrs." },
    { "ui.looker",            "section", "Contains eyes looker parametrs." },
    { "ui.clapper",           "section", "Contains clapper paramters." },
    { "ui.color",             "string",  "Specyfics eyes color." },
    { "ui.eye.size", "        integer",  "Specyfics eye size." },
    { "ui.eye.mirror",        "integer", "Specyfics eye mirror size." },
    { "ui.eyes.width",        "integer", "Specyfics window width {WARNING: currenty not working}" },
    { "ui.eyes.height",       "integer", "Specyfics window height {WARNING: currently not working}" },
    { "ui.looker.min_dx",     "integer", "Specyfics minimal eyes x position." },
    { "ui.looker.max_dx",     "integer", "Specyfics maximal eyes x position." },
    { "ui.looker.min_dy",     "integer", "Specyfics minimal eyes y position." },
    { "ui.looker.max_dx",     "integer", "Specyfics maximal etes y position." },
    { "ui.looker.min_delay",  "integer", "Specyfics minimal delay between looks. (this is (ui.looker.min_delay)*200 milisecounds)" },
    { "ui.looker.max_delay",  "integer", "Specyfics maximal delay between looks. (this is (ui.looker.max_delay_*200 milisecounds)" },
    { "ui.cpaller.min_delay", "integer", "Specyfics minimal delay between claps. (this is (ui.looker.min_delay)*200 milisecounds)" },
    { "ui.clapper.max_delay", "integer", "Specyfics maximal delay between claps. (this is (ui.looker.max_delay_*200 milisecounds)" }
};

const static int keys_count = 19,
                 max_key_string_length = 20;

void list_keys ( Config * set )
{
    for ( int i=0 ; i<keys_count ; i++ )
        cout << keys[i].path << "\033[" << (max_key_string_length-sizeof keys[i].path) << "C" << keys[i].type << '\t' << keys[i].desc << '\n';
}
