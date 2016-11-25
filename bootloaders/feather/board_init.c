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

#include <sam.h>
#include "board_definitions.h"

/**
 * \brief system_init() configures the needed clocks and according Flash Read Wait States.
 * At reset:
 * - OSC8M clock source is enabled with a divider by 8 (1MHz).
 * - Generic Clock Generator 0 (GCLKMAIN) is using OSC8M as source.
 * We need to:
 * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator), will be used as DFLL48M reference.
 * 2) Put XOSC32K as source of Generic Clock Generator 1
 * 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
 * 4) Enable DFLL48M clock
 * 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
 * 6) Modify PRESCaler value of OSCM to have 8MHz
 * 7) Put OSC8M as source for Generic Clock Generator 3
 */
// Constants for Clock generators
#define GENERIC_CLOCK_GENERATOR_MAIN      (0u)
#define GENERIC_CLOCK_GENERATOR_XOSC32K   (1u)
#define GENERIC_CLOCK_GENERATOR_OSCULP32K (2u) /* Initialized at reset for WDT */
#define GENERIC_CLOCK_GENERATOR_OSC8M     (3u)
// Constants for Clock multiplexers
#define GENERIC_CLOCK_MULTIPLEXER_DFLL48M (0u)

void board_init(void)
{


#if !defined(CRYSTALLESS)

  /* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
  NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;

  /* Turn on the digital interface clock */
  PM->APBAMASK.reg |= PM_APBAMASK_GCLK;


  /* ----------------------------------------------------------------------------------------------
   * 1) Enable XOSC32K clock (External on-board 32.768Hz oscillator)
   */
  SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_STARTUP( 0x6u ) | /* cf table 15.10 of product datasheet in chapter 15.8.6 */
                         SYSCTRL_XOSC32K_XTALEN | SYSCTRL_XOSC32K_EN32K;
  SYSCTRL->XOSC32K.bit.ENABLE = 1; /* separate call, as described in chapter 15.6.3 */

  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_XOSC32KRDY) == 0 )
  {
    /* Wait for oscillator stabilization */
  }

  /* Software reset the module to ensure it is re-initialized correctly */
  /* Note: Due to synchronization, there is a delay from writing CTRL.SWRST until the reset is complete.
   * CTRL.SWRST and STATUS.SYNCBUSY will both be cleared when the reset is complete, as described in chapter 13.8.1
   */
  GCLK->CTRL.reg = GCLK_CTRL_SWRST;

  while ( (GCLK->CTRL.reg & GCLK_CTRL_SWRST) && (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY) )
  {
    /* Wait for reset to complete */
  }

  /* ----------------------------------------------------------------------------------------------
   * 2) Put XOSC32K as source of Generic Clock Generator 1
   */
  GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_XOSC32K ); // Generic Clock Generator 1

  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
  {
    /* Wait for synchronization */
  }

  /* Write Generic Clock Generator 1 configuration */
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_XOSC32K ) | // Generic Clock Generator 1
                      GCLK_GENCTRL_SRC_XOSC32K | // Selected source is External 32KHz Oscillator
    //                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
                      GCLK_GENCTRL_GENEN;

  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
  {
    /* Wait for synchronization */
  }

  /* ----------------------------------------------------------------------------------------------
   * 3) Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0 (DFLL48M reference)
   */
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GENERIC_CLOCK_MULTIPLEXER_DFLL48M ) | // Generic Clock Multiplexer 0
                      GCLK_CLKCTRL_GEN_GCLK1 | // Generic Clock Generator 1 is source
                      GCLK_CLKCTRL_CLKEN;

  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
  {
    /* Wait for synchronization */
  }

  /* ----------------------------------------------------------------------------------------------
   * 4) Enable DFLL48M clock
   */

  /* DFLL Configuration in Closed Loop mode, cf product datasheet chapter 15.6.7.1 - Closed-Loop Operation */

  /* Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905 */
  SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;

  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
  {
    /* Wait for synchronization */
  }

  SYSCTRL->DFLLMUL.reg = SYSCTRL_DFLLMUL_CSTEP( 31 ) | // Coarse step is 31, half of the max value
                         SYSCTRL_DFLLMUL_FSTEP( 511 ) | // Fine step is 511, half of the max value
                         SYSCTRL_DFLLMUL_MUL( (VARIANT_MCK/VARIANT_MAINOSC) ); // External 32KHz is the reference

  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
  {
    /* Wait for synchronization */
  }

  /* Write full configuration to DFLL control register */
  SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_MODE | /* Enable the closed loop mode */
                           SYSCTRL_DFLLCTRL_WAITLOCK |
                           SYSCTRL_DFLLCTRL_QLDIS; /* Disable Quick lock */

  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
  {
    /* Wait for synchronization */
  }

  /* Enable the DFLL */
  SYSCTRL->DFLLCTRL.reg |= SYSCTRL_DFLLCTRL_ENABLE;

  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKC) == 0 ||
          (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLLCKF) == 0 )
  {
    /* Wait for locks flags */
  }

  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY) == 0 )
  {
    /* Wait for synchronization */
  }

  /* ----------------------------------------------------------------------------------------------
   * 5) Switch Generic Clock Generator 0 to DFLL48M. CPU will run at 48MHz.
   */
  GCLK->GENDIV.reg = GCLK_GENDIV_ID( GENERIC_CLOCK_GENERATOR_MAIN ); // Generic Clock Generator 0

  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
  {
    /* Wait for synchronization */
  }

  /* Write Generic Clock Generator 0 configuration */
  GCLK->GENCTRL.reg = GCLK_GENCTRL_ID( GENERIC_CLOCK_GENERATOR_MAIN ) | // Generic Clock Generator 0
                      GCLK_GENCTRL_SRC_DFLL48M | // Selected source is DFLL 48MHz
//                      GCLK_GENCTRL_OE | // Output clock to a pin for tests
                      GCLK_GENCTRL_IDC | // Set 50/50 duty cycle
                      GCLK_GENCTRL_GENEN;

  while ( GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY )
  {
    /* Wait for synchronization */
  }


  /*
   * Now that all system clocks are configured, we can set CPU and APBx BUS clocks.
   * These values are normally the ones present after Reset.
   */
  PM->CPUSEL.reg  = PM_CPUSEL_CPUDIV_DIV1;
  PM->APBASEL.reg = PM_APBASEL_APBADIV_DIV1_Val;
  PM->APBBSEL.reg = PM_APBBSEL_APBBDIV_DIV1_Val;
  PM->APBCSEL.reg = PM_APBCSEL_APBCDIV_DIV1_Val;
#else

  /////////////////////////////////////////

  /* Set 1 Flash Wait State for 48MHz, cf tables 20.9 and 35.27 in SAMD21 Datasheet */
  NVMCTRL->CTRLB.bit.RWS = NVMCTRL_CTRLB_RWS_HALF_Val;

  ////////////////////////////////////////
  uint32_t temp_genctrl_config = 0;

  // ENABLE OUTPUT OF GENCLK 0 on D2 (PA14)
  // Write to the PINCFG register to enable the peripheral multiplexer
  PORT->Group[0].PINCFG[14].reg = 1;
  // Enable peripheral function H for PA14, refer chapter I/O Multiplexing in the device datasheet
  PORT->Group[0].PMUX[7].bit.PMUXE = 7;

  // ENABLE OUTPUT OF GENCLK 1 on D5 (PA15)
  // Write to the PINCFG register to enable the peripheral multiplexer
  PORT->Group[0].PINCFG[15].reg = 1;
  // Enable peripheral function H for PA15, refer chapter I/O Multiplexing in the device datasheet
  PORT->Group[0].PMUX[7].bit.PMUXO = 7;

  // ENABLE OUTPUT OF GENCLK 2 on D11 (PA16) - should be OSC8M @ 8MHz
  // Write to the PINCFG register to enable the peripheral multiplexer
  PORT->Group[0].PINCFG[16].reg = 1;
  // Enable peripheral function H for PA16, refer chapter I/O Multiplexing in the device datasheet
  PORT->Group[0].PMUX[8].bit.PMUXE = 7;


  /////////////////////////////////////////////////

  /* Set OSC8M prescalar to divide by 1, now gclk0 is @ 8mhz */
  SYSCTRL->OSC8M.bit.PRESC = 0;


  /* ----------------------------------------------------------------------------------------------
   * 1) Enable OSC32K clock (Internal 32.768Hz oscillator)
   */
  
  uint32_t calib = (*((uint32_t *) SYSCTRL_FUSES_OSC32K_ADDR) & SYSCTRL_FUSES_OSC32K_Msk) >> SYSCTRL_FUSES_OSC32K_Pos;

  SYSCTRL->OSC32K.reg = SYSCTRL_OSC32K_CALIB(calib) | SYSCTRL_OSC32K_STARTUP( 0x6u ) | // cf table 15.10 of product datasheet in chapter 15.8.6 
    SYSCTRL_OSC32K_EN32K | SYSCTRL_OSC32K_ENABLE;

  while ( (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC32KRDY) == 0 ); // Wait for oscillator stabilization 

  /* ----------------------------------------------------------------------------------------------
   * 2) Put OSC32K as source of Generic Clock Generator 1
   */

  GCLK_GENCTRL_Type genctrl={0};
  uint32_t temp_genctrl;

  GCLK->GENCTRL.bit.ID = 1; // Read GENERATOR_ID - GCLK_GEN_1
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); // wait for data to be ready

  temp_genctrl = GCLK->GENCTRL.reg;
  genctrl.bit.SRC = GCLK_GENCTRL_SRC_OSC32K_Val;     // gclk 1 is now = osc32k
  genctrl.bit.GENEN = 1;
  genctrl.bit.RUNSTDBY = 0;
  genctrl.bit.OE = 1;                               // output on GCLK_IO[1] pin for debugging

  GCLK->GENCTRL.reg = (genctrl.reg | temp_genctrl); // set it!

  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);


  /* Configure OSC8M as source for GCLK_GEN 2 */
  GCLK->GENCTRL.bit.ID = 2; // Read GENERATOR_ID - GCLK_GEN_2 
  
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY); // wait for data to be ready
  
  temp_genctrl = GCLK->GENCTRL.reg;
  genctrl.bit.SRC = GCLK_GENCTRL_SRC_OSC8M_Val;     // gclk 2 is now = osc8m
  genctrl.bit.GENEN = 1;
  genctrl.bit.RUNSTDBY = 0;
  genctrl.bit.OE = 1;                               // output on GCLK_IO[2] pin for debugging
  GCLK->GENCTRL.reg = (genctrl.reg | temp_genctrl); // set it!

  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

  // Turn on DFLL
  SYSCTRL_DFLLCTRL_Type dfllctrl_conf = {0};
  SYSCTRL_DFLLVAL_Type dfllval_conf = {0};
  uint32_t coarse =( *((uint32_t *)(NVMCTRL_OTP4)
		       + (NVM_SW_CALIB_DFLL48M_COARSE_VAL / 32))
		     >> (NVM_SW_CALIB_DFLL48M_COARSE_VAL % 32))
    & ((1 << 6) - 1);
  if (coarse == 0x3f) {
    coarse = 0x1f;
  }
  uint32_t fine =( *((uint32_t *)(NVMCTRL_OTP4)
		     + (NVM_SW_CALIB_DFLL48M_FINE_VAL / 32))
		   >> (NVM_SW_CALIB_DFLL48M_FINE_VAL % 32))
    & ((1 << 10) - 1);
  if (fine == 0x3ff) {
    fine = 0x1ff;
  }
  dfllval_conf.bit.COARSE  = coarse;
  dfllval_conf.bit.FINE    = fine;
  dfllctrl_conf.bit.USBCRM = 1;  // usb correction
  dfllctrl_conf.bit.BPLCKC = 0;
  dfllctrl_conf.bit.QLDIS  = 0;
  dfllctrl_conf.bit.CCDIS  = 1;
  dfllctrl_conf.bit.ENABLE = 1;

  SYSCTRL->DFLLCTRL.bit.ONDEMAND = 0;
  while (!(SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_DFLLRDY));
  SYSCTRL->DFLLMUL.reg = 48000;
  SYSCTRL->DFLLVAL.reg = dfllval_conf.reg;
  SYSCTRL->DFLLCTRL.reg = dfllctrl_conf.reg;
  
  //
  GCLK_CLKCTRL_Type clkctrl={0};
  uint16_t temp;
  GCLK->CLKCTRL.bit.ID = 2; // GCLK_ID - DFLL48M Reference 
  temp = GCLK->CLKCTRL.reg;
  clkctrl.bit.CLKEN = 1;
  clkctrl.bit.WRTLOCK = 0;
  clkctrl.bit.GEN = GCLK_CLKCTRL_GEN_GCLK0_Val;
  GCLK->CLKCTRL.reg = (clkctrl.reg | temp);

  // Configure DFLL48M as source for GCLK_GEN 0
  GCLK->GENCTRL.bit.ID = 0; // GENERATOR_ID - GCLK_GEN_0
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
  temp_genctrl = GCLK->GENCTRL.reg;
  genctrl.bit.SRC = GCLK_GENCTRL_SRC_DFLL48M_Val;
  genctrl.bit.GENEN = 1;
  genctrl.bit.RUNSTDBY = 0;
  genctrl.bit.OE = 1;
  GCLK->GENCTRL.reg = (genctrl.reg | temp_genctrl);
  while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

#endif

}
