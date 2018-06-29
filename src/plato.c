/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * plato.c - main program
 */

#include <serial.h>
#include <stdbool.h>
#include "protocol.h"
#include "terminal.h"
#include "welcome.h"
#include "screen.h"
#include "touch.h"
#include "keyboard.h"
#include "io.h"
#include "config.h"
#include "splash.h"
#include "config.h"

extern ConfigInfo config;
uint8_t already_started=false;
uint8_t running=false;
uint8_t restart=true;

/**
 * greeting(void) - Show terminal greeting
 */
void greeting(void)
{
  ShowPLATO(splash,sizeof(splash));
  terminal_initial_position();
}

void main(void)
{
  while (restart==true)
    {
      screen_init();
      config_init();
      terminal_init();
      greeting();
      screen_beep();
      io_init();
      //      touch_init();
      
      already_started=true;
      running=true;
      
      // And do the terminal
      // Yes, I know this is weird, but since this is a tightly running loop
      // The execution time of the loop must be minimized -Thom
      if (config.io_mode==IO_MODE_SERIAL)
	{
	  while (running==true)
	    {
	      io_recv_serial();
	      keyboard_main();
	      //      touch_main();
	    }
	}
      else if (config.io_mode==IO_MODE_ETHERNET)
	{
	  while (running==true)
	    {
	      io_recv_ethernet();
	      keyboard_main();
	      // touch_main();
	    }
	}
      
      screen_done();
      touch_done();
      io_done();
    }
  
  return;
}
