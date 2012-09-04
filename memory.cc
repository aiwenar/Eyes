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

#include "memory.hxx"

#include <stdio.h>
#include <endian.h>
#include <stdexcept>

const unsigned short memory::version;

void memory::save ( const char * file, memory * mem )
{
  FILE * memf = fopen ( file, "w" );

  fwrite ( "EYES MEMORY", 11 , 1, memf );

  unsigned short ver = htole16 ( version );
  fwrite ( &ver, 2, 1, memf );

  fclose ( memf );
}

memory * memory::load ( const char * file )
{
  FILE * mem = fopen ( file, "r" );

  fseek ( mem, 12, SEEK_SET );

  unsigned short ver;
  fread ( &ver, 2, 1, mem );
  ver = le16toh ( ver );

  if ( ver > version )
    throw std::runtime_error ( "memory file format newer than supported." );

  fclose ( mem );
}
