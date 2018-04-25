#include "lpc17xx_pinsel.h"
#include "global.h"


void pin_Configuration(void)
{
  // déclaration des structures de configuration
	PINSEL_CFG_Type conf;
	PINSEL_CFG_Type maconfig1;
	PINSEL_CFG_Type maconfig2;
	
	//config SDA0 (PORT0.27)
	conf.Portnum = PINSEL_PORT_0;
	conf.Pinnum = PINSEL_PIN_27;
	conf.Funcnum = PINSEL_FUNC_1;
	conf.Pinmode = PINSEL_PINMODE_TRISTATE;
	PINSEL_ConfigPin(&conf);
	//config SCL0 (PORT0.28)
	conf.Pinnum=PINSEL_PIN_28;
	conf.Pinmode=PINSEL_PINMODE_PULLUP;
	PINSEL_ConfigPin(&conf);
	
	
	// configuration des GPIOs
	GPIO_SetDir(1,1<<9,1);
	GPIO_SetDir(0,1<<3,1);
	
	// configuration des pinsel
	maconfig2.Portnum = PINSEL_PORT_1;
	maconfig2.Pinnum = PINSEL_PIN_9;
	maconfig2.Funcnum = PINSEL_FUNC_0;
	maconfig2.Pinmode = PINSEL_PINMODE_PULLDOWN;
	maconfig2.OpenDrain = PINSEL_PINMODE_NORMAL;
  PINSEL_ConfigPin(&maconfig2);
	// configuration des pinsel
	maconfig1.Portnum = PINSEL_PORT_0;
	maconfig1.Pinnum =PINSEL_PIN_9;
	maconfig1.Funcnum = PINSEL_FUNC_0;
	maconfig1.Pinmode =PINSEL_PINMODE_PULLDOWN;
	maconfig1.OpenDrain = PINSEL_PINMODE_NORMAL;
  PINSEL_ConfigPin(&maconfig1);
	
}
