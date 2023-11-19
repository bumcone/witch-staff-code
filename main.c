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

#define LED_PORT PORTB
#define LED_ID   PB0
#define BUT_PIN  PIND
#define BUT_ID   PD1

#define true  1
#define false 0

struct cRGB led[1];

// TODO: Add better debouncer?
int button_pressed(int delay) {
    if (delay <= 0) delay = 1;

    while (delay > 0) {
        // If pressed
        if ((BUT_PIN & (1<<BUT_ID)) != (1<<BUT_ID)) {
            _delay_ms(10);
            delay -= 10;

            if ((BUT_PIN & (1<<BUT_ID)) != (1<<BUT_ID)) return true;
        }
    }

    return false;
}

void fire_staff(void) {
    int i = 0;

    led[0].r = 0; led[0].b = 0;

    for (led[0].g = 0; led[0].g < 255; ++led[0].g) {
        ws2812_setleds(led, 1);
//                _delay_ms(1);
    }
    _delay_ms(50);


    for (i = 0; i < 40; ++i) {
        led[0].g = 160; ws2812_setleds(led, 1); _delay_ms(1);
        led[0].g = 200; ws2812_setleds(led, 1); _delay_ms(1);
    }

    for (; led[0].g > 10; --led[0].g) {
        ws2812_setleds(led, 1);
        _delay_ms(1);
    }
}

int main(void) {
    int dir = 1;
    int min_val =  1;
    int max_val =  5;

    // LED Setup
    DDRB  = 0b00000001; // Set LED pin as OUTPUT
    PORTB = 0b00000000; // Set all pins to LOW

    DDRD  = 0b00000000; // Set button as INPUT

    led[0].r = 0; led[0].g = 0; led[0].b = 0;

    // Main loop
    while (1) {

        // Idle
        led[0].g += dir;
        if (led[0].g >= max_val) {
            dir = -1;
        } else if (led[0].g <= min_val) {
            dir =  1;
        }

        ws2812_setleds(led, 1);

        if (!button_pressed(200)) continue;

        // Button pressed
        fire_staff();

        // Reset idle animation
        led[0].g = min_val;
        dir = 1;

    } // Loop forever!
}
