// Fichier contenant les fonctions liees a la configuration du haut parleur.

#include <LPC17xx.H>
#include "lpc17xx_timer.h"
#include "lpc17xx_gpio.h"
#include "global.h"

#define TIMER_TIME_STEP 1					// 1 us deStepTimer
#define TIMER_MATCH_VALUE_1s 50 	//valeur pour avoir 1s


const double TabNote[6]={
	1.0/329.63*1000000,				//MI - periode pour 440Hz en �s
	1.0/349.23*1000000,				//FA - periode pour 393.66Hz en �s
	1.0/392.00*1000000,				//SOL - periode pour 392Hz en �s
	1.0/440.00*1000000,				//LA - periode pour 349.23Hz en �s
	1.0/261.63*1000000,				//DO - periode pour 440Hz en �s
	1.0/493.88*1000000				//SI - periode pour 440Hz en �s
};

int duree = 0;						    	//p�riode de temps durant laquelle la note est jou�e
int demiperiode50us = 0;
int compteur_50us = 0;
int attente=0;

void T0_Init(void)
{
	  //on cr�e une variable contenant la structure d'initialisation du timer
    TIM_TIMERCFG_Type        Timer_Config_Structure;
  // on cr�e une variable contenant la structure d'initialisation d'un match
    TIM_MATCHCFG_Type        Timer_MatchConfig_Structure;
    
    // Timer0  Initialization
    // On remplit la structure de configuration du timer cr��e ci-dessus

    // d�finit le Prescaler en microsecondes
    Timer_Config_Structure.PrescaleOption = TIM_PRESCALE_USVAL; // d�finit le step timer � 1 microseconde           
    Timer_Config_Structure.PrescaleValue    = TIMER_TIME_STEP;                            
    /* On passe les donn�es de la structure de configuration � la fonction TIM_Init pour configurer le Timer. 
		En param�tre, on passe un pointeur sur le Timer0 � configurer, le mode du timer (mode timer) et la structure de configuration.    */
    TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&Timer_Config_Structure);
    
    // Configuration de la structure de configuration des Match channels
    // on configure le channel Math0.0
    Timer_MatchConfig_Structure.MatchChannel = 0;                              
    Timer_MatchConfig_Structure.IntOnMatch   = TRUE;                        // lors d'un match on active les interruptions        
    Timer_MatchConfig_Structure.ResetOnMatch = TRUE;                        // lors d'un match on reset le timer 
    Timer_MatchConfig_Structure.StopOnMatch  = FALSE;                       // lors d'un match on arr�te le timer       
    Timer_MatchConfig_Structure.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;  // lors d'un match on change la valeur de math0.0  
    // on match lorsque le timer � atteind une p�riode de 1ms !
    Timer_MatchConfig_Structure.MatchValue = TIMER_MATCH_VALUE_1s;    
    
 /* On passe les donn�es de la structure de configuration � la fonction TIM_ConfigMatch pour configurer le match. En param�tre, on passe un pointeur sur le Timer0 � configurer et la structure de configuration.    */
	TIM_ConfigMatch(LPC_TIM0,&Timer_MatchConfig_Structure);  
		//initialisation compteur pour interrruptions
	 compteur_50us = 0;
  // On lance le timer 0  
	TIM_Cmd(LPC_TIM0,ENABLE);                                                                    
}

void JouerNote (int dureeEnUs, int numero_note)
{
		duree = dureeEnUs/50;
		demiperiode50us = (int)(TabNote[numero_note]/50/2-1);
}

void attentems(int ms){
	attente=ms*20;
	while(attente>0);
}
/* Cette fonction va permettre le changement de la valeur du bit value du port P1.9 lorsqu�on match. */
void TIMER0_IRQHandler(){
   
	attente--;
	compteur_50us++;
	if (duree>0)
	{
		duree--;
	}
	if(compteur_50us >= demiperiode50us && duree>0)
	{
		compteur_50us = 0;
		
		// si le P1.9 est � 0, on le passe � 1
		if ((GPIO_ReadValue(1)&(1<<9)) == 0 )
		{
			GPIO_SetValue(1,(1<<9));
		}
		//sinon on le remet a 0
		else{
			GPIO_ClearValue(1,(1<<9));
		}
	}
	
  // On remet � z�ro le match register 0 du timer 0
  TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
}
