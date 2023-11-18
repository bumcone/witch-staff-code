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

// ATMega32u4 (Pro Micro) powered WS2812b controller

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

#define LED_PIN PB0

struct cRGB led[1];

int main(void) {
    // LED Setup
    DDRB  = 0b00000001; // Set LED pin as OUTPUT
    PORTB = 0b00000000; // Set all pins to LOW

    // Main loop
    while (1) {

        // Colour LED blinky!

PORTB ^= _BV(LED_PIN);
        // Write red
        led[0].r = 255; led[0].g =   0; led[0].b =   0;
        ws2812_setleds(led, 1);
        _delay_ms(500);

PORTB ^= _BV(LED_PIN);
        // Write green
        led[0].r =   0; led[0].g = 255; led[0].b =   0;
        ws2812_setleds(led, 1);
        _delay_ms(500);

PORTB ^= _BV(LED_PIN);
        // Write blue
        led[0].r =   0; led[0].g =   0; led[0].b = 255;
        ws2812_setleds(led, 1);
        _delay_ms(500);

    } // Loop forever!
}
