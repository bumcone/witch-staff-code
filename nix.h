/* vim:set ts=4 sw=4 tw=80 et ai si cindent cino=L0,b1,(1s,U1,m1,j1,J1,)50,*90 cinkeys=0{,0},0),0],\:,0#,!^F,o,O,e,0=break:
 */
/**********************************************************************
    Witch Staff
    Copyright (C) 2023 Krayon

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    version 2 ONLY, as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program, in the file COPYING or COPYING.txt; if
    not, see http://www.gnu.org/licenses/ , or write to:
      The Free Software Foundation, Inc.,
      51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 **********************************************************************/

#ifndef   _NIX_H_
#define   _NIX_H_

#include <stdio.h>
#include <unistd.h>
#include "main.h"

#define PD1 1
#define PD4 4
#define PIND pind
#define PORTB portb
#define DDRB ddrb
#define DDRD ddrd

int ddrb = 0;
int ddrd = 0;
int pind = 1;
int portb = 0;

struct cRGB {
    int r;
    int g;
    int b;
};

void _delay_ms(int ms) {
    usleep(ms);
}

void ws2812_setleds(struct cRGB *led, int led_count) {
    int n;

    printf("ws2812_setleds:\n");

    for (n = 0; n < LED_COUNT; ++n) {
        printf("  LED[%2d]: %.3d, %.3d, %.3d\n", n, led[n].r, led[n].g, led[n].b);
    }

    return;
}

#endif /* _NIX_H_ */
