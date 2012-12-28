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

#ifndef _eyes_debug_hxx
#define _eyes_debug_hxx

#define info    eyes_dbg::oinfo<<"[info: ] "
#define error   eyes_dbg::oerror<<"[\033[31merror\033[0m :] "
#define warning eyes_dbg::owarning<<"[\033[33mwarning\033[0m :] "

#include "ostream.hh"

namespace eyes_dbg
{

extern  OStream oinfo;
extern  OStream owarning;
extern  OStream oerror;

}

#endif //eyes_debug_hxx
