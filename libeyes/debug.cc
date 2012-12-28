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

#include <debug.hxx>

Debug::Debug ()
{
}

Debug::Debug ( char * nfile, char * ntype )
{
    fbuf.open ( nfile, ios_base::out );
    stream = new iostream ( fbuf );
    type = ntype;
}

Debug::~Debug ()
{
    file.close ();
}

void Debug::open ( char * nfile )
{
    file.close ();
    file.open ( nfile, ios_base::out );
}

void Debug::set_str ( char * ntype )
{
    type = ntype;
}

Debug info;
Debug warning;
Debug error;
