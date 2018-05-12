#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "global.h"

#define TIMER_TIME_STEP			500			// 500 us de Step Timer precuision de 0.5ms
#define TIMER_MATCH_VALUE_20ms  40 // valeur pour avoir 20000 us 

int compteur_20ms;

int getNumeroNote(){
	if(touch_x>600 && touch_x<2000){
		if(touch_y>700 && touch_y<1800)
			return 2;
		if(touch_y>2000 && touch_y<3000)
			return 0;
	}
	if(touch_x>2100 && touch_x<3600){
		if(touch_y>700 && touch_y<1800)
			return 3;
		if(touch_y>2000 && touch_y<3000)
			return 1;
	}
	return 4;
}
	

void T1_Init(void) //La fonction permet de creer et d'initialiser le TIMER1
{
	TIM_TIMERCFG_Type		Timer_Config_Structure; //Permet de definir le structure en mode timer, a partir du typedef de lpc17xx_timer.h
  TIM_MATCHCFG_Type		Timer_MatchConfig_Structure; //Permet de definir la structure en mode match (savoir dans quel mode va fonctionner le match), a partir de lpc17xx_timer.h
	
	// Timer0  Initialization
	Timer_Config_Structure.PrescaleOption = TIM_PRESCALE_USVAL;	//Le prescale sera en microsecondes, d'apres sa definition dans TIM_TIMERCFG_TYPE				
	Timer_Config_Structure.PrescaleValue	= TIMER_TIME_STEP;		//La valeur du prescale sera egale a TIMER_TIME_STEP, soit 1000 microsecondes (globaldefine.h)			

	TIM_Init(LPC_TIM1, TIM_TIMER_MODE,&Timer_Config_Structure); //On itilialise le TIMER a partir des parametres definis en global et dans les commandes d'avant
	

	// Configuration of Match channels
	Timer_MatchConfig_Structure.MatchChannel = 0;			  //On travaillera sur le MAT1.0										
	Timer_MatchConfig_Structure.IntOnMatch   = TRUE;		//On genere une interruption lorsqu'on match	
	Timer_MatchConfig_Structure.ResetOnMatch = TRUE;		//On genere un reset lorsqu'on match.									
	Timer_MatchConfig_Structure.StopOnMatch  = FALSE;		// On ne stoppe pas le processus lorsqu'on match									
	Timer_MatchConfig_Structure.ExtMatchOutputType = TIM_EXTMATCH_NOTHING; 

	Timer_MatchConfig_Structure.MatchValue = TIMER_MATCH_VALUE_20ms;	//On va match lorsque le timer arrivera a 20ms
	TIM_ConfigMatch(LPC_TIM1,&Timer_MatchConfig_Structure);		//On passe notre structure de configuration pour iniatialiser les matchs.					
	
	
	TIM_Cmd(LPC_TIM1,ENABLE);			//On lance le timer																			
	compteur_20ms = 0;
	etaitAppuye=FALSE;
}

void TIMER1_IRQHandler(){
	compteur_20ms = compteur_20ms + 1;
	
	if (compteur_20ms == 2){ //
		if ((GPIO_ReadValue(0)&(1<<19)) == 0 ){
			mode = CLAVIER_JEU;
		}else{
			etaitAppuye=FALSE;
		}
		compteur_20ms = 0;
	}
	TIM_ClearIntPending(LPC_TIM1, TIM_MR0_INT);
}
