/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.
  Copyright (c) 2015 Atmel Corporation/Thibaut VIARD.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "board_driver_led.h"


inline void LED_init(void) { 
  PORT->Group[BOARD_LED_PORT].DIRSET.reg = (1<<BOARD_LED_PIN); 

#if defined(BOARD_RGBLED_CLOCK_PORT)
  // using APA102, set pins to outputs
  PORT->Group[BOARD_RGBLED_CLOCK_PORT].DIRSET.reg = (1<<BOARD_RGBLED_CLOCK_PIN); 
  PORT->Group[BOARD_RGBLED_DATA_PORT].DIRSET.reg = (1<<BOARD_RGBLED_DATA_PIN); 
  // and clock 0x00000 out!


  APA102_set_color(0,0,0);
#endif
}

inline void LEDRX_on(void) {
  PORT->Group[BOARD_LEDRX_PORT].OUTSET.reg = (1<<BOARD_LEDRX_PIN); 

#if defined(BOARD_RGBLED_CLOCK_PORT)
  APA102_set_color(25, 0, 0);
#endif
}

inline void LEDRX_off(void) { 
  PORT->Group[BOARD_LEDRX_PORT].OUTCLR.reg = (1<<BOARD_LEDRX_PIN); 

#if defined(BOARD_RGBLED_CLOCK_PORT)
  APA102_set_color(0, 0, 0);
#endif
}


inline void LEDTX_on(void) { 
  PORT->Group[BOARD_LEDTX_PORT].OUTSET.reg = (1<<BOARD_LEDTX_PIN); 

#if defined(BOARD_RGBLED_CLOCK_PORT)
  APA102_set_color(0, 0, 25);
#endif
}

inline void LEDTX_off(void) { 
  PORT->Group[BOARD_LEDTX_PORT].OUTCLR.reg = (1<<BOARD_LEDTX_PIN); 

#if defined(BOARD_RGBLED_CLOCK_PORT)
  APA102_set_color(0, 0, 0);
#endif
}



#if defined(BOARD_RGBLED_CLOCK_PORT)

void write_apa_byte(uint8_t x) {
  for (int i=0; i<8; i++) {
    if(x & 0x80) 
      PORT->Group[BOARD_RGBLED_DATA_PORT].OUTSET.reg = (1<<BOARD_RGBLED_DATA_PIN);
    else
      PORT->Group[BOARD_RGBLED_DATA_PORT].OUTCLR.reg = (1<<BOARD_RGBLED_DATA_PIN);
    
    PORT->Group[BOARD_RGBLED_CLOCK_PORT].OUTSET.reg = (1<<BOARD_RGBLED_CLOCK_PIN);
    for (uint32_t i=0; i<25; i++) /* 0.1ms */
      __asm__ __volatile__("");
    
    PORT->Group[BOARD_RGBLED_CLOCK_PORT].OUTCLR.reg = (1<<BOARD_RGBLED_CLOCK_PIN);
    for (uint32_t i=0; i<25; i++) /* 0.1ms */
      __asm__ __volatile__("");
    
    x <<= 1;
  }
}

void APA102_set_color(uint8_t red, uint8_t green, uint8_t blue) {
  write_apa_byte(0x0);
  write_apa_byte(0x0);
  write_apa_byte(0x0);
  write_apa_byte(0x0);

  write_apa_byte(0xFF);
  write_apa_byte(blue);
  write_apa_byte(green);
  write_apa_byte(red);

  write_apa_byte(0xFF);
  write_apa_byte(0xFF);
  write_apa_byte(0xFF);
  write_apa_byte(0xFF);

  // set clock port low for 100ms
  for (uint32_t i=0; i<25000; i++) /* 100ms */
     __asm__ __volatile__("");
}
#endif
