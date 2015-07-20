/**
 * File: startup.c
 * Description: Startup initialization code.
 * Author: Vijay Karthick Baskar.
 * License: Assume BSD type license and feel free to use this code.
 *			Sending me a thank you mail will do!
 */

/*
 *------------------------------------------------------------------------------
 * Include Files
 *------------------------------------------------------------------------------
 */

#include <msp430g2553.h>

/*
 *------------------------------------------------------------------------------
 * Private Defines
 *------------------------------------------------------------------------------
 */

/* Settings for various clock frequencies */
#define DCO_8MHZ				(DCO1|DCO0)
#define DCO_16MHZ				(DCO2|DCO1|DCO0)
#define RSEL_8MHZ				(RSEL3|RSEL2|RSEL0)
#define RSEL_16MHZ				(RSEL3|RSEL2|RSEL1|RSEL0)

/* Enable external resistor, as internal resistor is inaccurate */
#define DCO_EXT_RESISTOR_ENABLE	(0x01)

/* Minimum refresh value of the counter used for WDOG */
#define MIN_REFRESH_VALUE		(10)

/*
 *------------------------------------------------------------------------------
 * Private Macros
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * Private Data Types
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * Public Variables
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * Private Variables
 *------------------------------------------------------------------------------
 */

int wdog_counter;

/*
 *------------------------------------------------------------------------------
 * Public Constants
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * Private Constants
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * Private Function Prototypes
 *------------------------------------------------------------------------------
 */

/*
 *------------------------------------------------------------------------------
 * Public Function Definitions
 *------------------------------------------------------------------------------
 */

void disable_watchdog() {
	/* Disable the watchdog */
	WDTCTL = WDTPW | WDTHOLD;
}

void enable_watchdog() {
	/* Enable the watchdog */
	WDTCTL = WDTPW;
}

void clock_init() {
	/* Set the DCO frequency to 8 MHz */
	DCOCTL = DCO_8MHZ;

	/* Disable the external oscillator */
	BCSCTL1 = XT2OFF;

	/* Set the range select to produce 8 MHz frequency clock */
	BCSCTL1 |= RSEL_8MHZ;

	/* Select DCO as the clock source */
	BCSCTL2 &= ~(SELM1 | SELM0);

	/* Select the clock scaling factor as 1 */
	BCSCTL2 &= ~(DIVM1 | DIVM0);

	/* Set the sub module clock source as DCO */
	BCSCTL2 &= ~SELS;

	/* Set scaling factor for sub module clock as 8 to get 1 MHz frequency */
	BCSCTL2 |= (DIVS1 | DIVS0);

	/* Set the DCO resistor to be internal - internal resistor is not accurate */
	BCSCTL2 &= ~DCO_EXT_RESISTOR_ENABLE;
}

void init_wdog() {
	/**
	 * No changes required, as SMCLK is selected with scaler as 32768 = 32.768ms timeout by default
	 * i.e. 1/1MHz * 32768
	 */
}

void reset_wdog() {
	if (wdog_counter < MIN_REFRESH_VALUE) {
		wdog_counter++;
		return;
	}
	else {
		wdog_counter = 0;
		WDTCTL = WDTPW | WDTCNTCL;
	}
}
