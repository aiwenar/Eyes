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

#ifndef CDBG_HXX
#define CDBG_HXX

#include <QTimer>

#include "eyes.hxx"

void spacefill ( int, unsigned short );

class cdbg : public QObject
{
  Q_OBJECT
public:
    cdbg  ( Core * );

    void  chck_s ();
public slots:
    void  on_timer_tick ();

private:
    Core          * core;
    QTimer          timer;
    unsigned  short max_s,
                    max_EQ,
                    line,
                    spacer;
};

#endif // CDBG_HXX
