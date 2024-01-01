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

#if !defined(NIX_TEST)
#include "avr.h"
#else
#include "nix.h"
#endif

#define LED_PORT PORTB
#define LED_ID   PB0
#define BUT_PIN  PIND
#define BUT_ID   PD4

#define true      1
#define false     0

struct cRGB led[LED_COUNT];
struct cRGB new_led[LED_COUNT];

int debounce_count = 50;

// TODO: Add better debouncer?
int button_pressed(int reset_press, int delay) {
    static int press_count = 0;

    if (reset_press) press_count = 0;

    if (delay <= 0) delay = 1;

    while (delay > 0) {
        // If pressed
        if ((BUT_PIN & (1<<BUT_ID)) != (1<<BUT_ID)) {
            press_count += 1;
        } else {
            press_count = 0;
        }

        if (press_count > debounce_count) return true;

        _delay_ms(1);
        delay--;
    }

    return false;
}

void fire_staff(void) {
    printf("fire_staff()\n");

    int i = 0;
    int n = 0;
    struct cRGB new = {0, 0, 0};

    for (n = 0; n < LED_COUNT; ++n) {
        led[n].g = 0; led[n].r = 0; led[n].b = 0;
    }

    for (new.g = 0; new.g < 255; /* ++new.g */) {
        for (n = LED_COUNT-1; n > 0; --n) {
            led[n].r = led[n-1].r;
            led[n].g = led[n-1].g;
            led[n].b = led[n-1].b;
        }

        led[0].r = new.r;
        led[0].g = new.g;
        led[0].b = new.b;
        ws2812_setleds(led, LED_COUNT);
        _delay_ms(1);
        if (new.g + 6 > 255) break;
        if (new.g + 6 < new.g) break;
        new.g += 6;
    }
    _delay_ms(80);

    for (; new.g > 0; --new.g) {
            for (n = LED_COUNT-1; n > 0; --n) {
                led[n].r = led[n-1].r;
                led[n].g = led[n-1].g;
                led[n].b = led[n-1].b;
            }

            led[0].r = new.r;
            led[0].g = new.g;
            led[0].b = new.b;
            ws2812_setleds(led, LED_COUNT);
            _delay_ms(2);
    }
}

void idle_init(int led) {
    static int chaos = 0;

    int n = 0;
    int n_min = 0;
    int n_max = LED_COUNT - 1;

    int seed = 16 + chaos;

    if (++chaos > LED_COUNT) chaos = 0;

    if (led < LED_COUNT) n_min = n_max = led;

    for (n = n_min; n <= n_max; ++n) {
        new_led[n].r = 0;
        new_led[n].g = ( seed    %  4) *  1;
        new_led[n].b = ( (((seed+1) % 16) *  1) / 2);
        if (new_led[n].b >= 16) new_led[n].b = 0;

        ++seed;
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

    for (n = 0; n < LED_COUNT; ++n) {
        if (
            led[n].r == new_led[n].r &&
            led[n].g == new_led[n].g &&
            led[n].b == new_led[n].b
        ) idle_init(n);

        led[n].r +=
            (
                (new_led[n].r > led[n].r) ? 1 :
                    ((new_led[n].r < led[n].r) ? -1 : 0)
            );

        led[n].g +=
            (
                (new_led[n].g > led[n].g) ? 1 :
                    ((new_led[n].g < led[n].g) ? -1 : 0)
            );

        led[n].b +=
            (
                (new_led[n].b > led[n].b) ? 1 :
                    ((new_led[n].b < led[n].b) ? -1 : 0)
            );
    }

    ws2812_setleds(led, LED_COUNT);
    return;




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

    // Setup idle
    idle_init(LED_COUNT);

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

//        ws2812_setleds(led, 1);

        if (!button_pressed(1, 200)) continue;

        // Button pressed
        fire_staff();

        // Reset idle animation
        idle_staff(1);

    } // Loop forever!

}
