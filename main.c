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

#include "main.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

#define LED_PORT PORTB
#define LED_ID   PB0
#define BUT_PIN  PIND
#define BUT_ID   PD4

#define true      1
#define false     0

struct cRGB led[LED_COUNT];

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
    int n = 0;

    for (n = 0; n < LED_COUNT; ++n) {
        led[n].r = 0; led[n].b = 0;
    }

    for (led[0].g = 0; led[0].g < 255; ++led[0].g) {
        for (n = 1; n < LED_COUNT; ++n) {
            led[n].r = led[0].r;
            led[n].g = led[0].g;
            led[n].b = led[0].b;
        }
        ws2812_setleds(led, LED_COUNT);
//        _delay_ms(1);
    }
    _delay_ms(50);

    for (i = 0; i < 40; ++i) {
        for (n = 0; n < LED_COUNT; ++n) {
            led[n].g = 160;
        }
        ws2812_setleds(led, LED_COUNT);
        _delay_ms(1);

        for (n = 0; n < LED_COUNT; ++n) {
            led[n].g = 200;
        }
        ws2812_setleds(led, LED_COUNT);
        _delay_ms(1);
    }

    for (; led[0].g > 10; --led[0].g) {
        for (n = 1; n < LED_COUNT; ++n) {
            led[n].g = led[0].g;
        }
        ws2812_setleds(led, LED_COUNT);
        _delay_ms(1);
    }
}

void idle_staff(int reset) {
    static int dir = 1;
    int n = 0;

    if (reset) {
        dir = 1;

        for (n = 0; n < LED_COUNT; ++n) {
            led[n].r = 0;
            led[n].g = 0;
            led[n].b = 0;
        }

        return;
    }

    // Idle
    led[0].g += dir;
    if (led[0].g >= MAX_VAL) {
        dir = -1;
    } else if (led[0].g <= MIN_VAL) {
        dir =  1;
    }

    for (n = 1; n < LED_COUNT; ++n) {
        led[n].g = led[0].g;
    }

    ws2812_setleds(led, LED_COUNT);
}

int main(void) {

    // LED Setup
    DDRB  = 0b00000001; // Set LED pin as OUTPUT
    PORTB = 0b00000000; // Set all pins to LOW

    DDRD  = 0b00000000; // Set button as INPUT

    // Reset idle animation
    idle_staff(1);

    // Main loop
    while (1) {

        // Idle animation
        idle_staff(0);

        ws2812_setleds(led, 1);

        if (!button_pressed(200)) continue;

        // Button pressed
        fire_staff();

        // Reset idle animation
        idle_staff(1);

    } // Loop forever!

}
