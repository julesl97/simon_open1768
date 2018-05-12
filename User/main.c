//===========================================================//
// Projet Micro - INFO1 - ENSSAT - S2 2018							 //
//===========================================================//
// File                : Projet Fini
// Hardware Environment: Open1768	
// Build Environment   : Keil µVision
// Autors              : Cléis, Emma, Jules
//===========================================================//

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"

#include <stdio.h>

#include "global.h"
#include "globaldec.h" // fichier contenant toutes les déclarations de variables globales
#include "fonctionsSon.h"
#include "fonctionsEcran.h"
#include "fonctionsMemoire.h"
#include "pinconf.h"

	
const int colors[2][4] = {
	{Yellow, Green, Blue, Red}, //couleurs des cases
	{0xFFE3, 0x1FE3, 0x187F, 0xF863} //couleurs plus claires
};
const int colorsPos[4][4] = {
	{10,60,110,110},  	//position de la couleur jaune
	{120,60,110,110},		//position de la couleur vert
	{10,170,110,110},		//position de la couleur bleu
	{120,170,110,110}		//position de la couleur rouge
};

void jouerSequence(int nb, uint8_t* s){
	int i;
	for(i=0; i<nb; i++){
		JouerNote(200, s[i]);
		dessiner_rect(colorsPos[s[i]][0],colorsPos[s[i]][1],colorsPos[s[i]][2],colorsPos[s[i]][3],2,1,White,colors[1][s[i]]);
		attentems(300);
		dessiner_rect(colorsPos[s[i]][0],colorsPos[s[i]][1],colorsPos[s[i]][2],colorsPos[s[i]][3],2,1,Black,colors[0][s[i]]);
	}
}

//===========================================================//
// Function: Main
//===========================================================//
int main(void)
{	  
	uint8_t score=0;
	uint8_t highscore=0;
	uint8_t sequence[MAX_SCORE]; //sequence de note pour le jeu
	uint8_t sequenceHS[MAX_SCORE]; //sequence de note du Highscore
	int avancement=0; //index de la dernière note entrer par le joueur
	int i; //iterateur 
	
	init_i2c_eeprom();
	  // Init(); // init variable globales et pinsel son et IR
	  lcd_Initializtion(); // init pinsel ecran et init LCD
		T1_Init();
		T0_Init();
		NVIC_EnableIRQ(TIMER1_IRQn);
		NVIC_EnableIRQ(TIMER0_IRQn);
	
		// affichage de l'écran maitre
		sprintf(chaine,"Jouer ou Highscore ?    ");
		LCD_write_english_string(32,30,chaine,White,Blue);
		// affichage des 4 carrés
		for(i=0; i<NB_COLORS; i++){
			dessiner_rect(colorsPos[i][0],colorsPos[i][1],colorsPos[i][2],colorsPos[i][3],2,1,Black,colors[0][i]);
	  }
		
		pin_Configuration(); //configuration des entrées/sorties pour le son et l'écran 
		
	  touch_init(); // init pinsel tactile et init tactile 
	  
		mode = WAIT;
		start = TRUE;
    while(1){
		switch(mode){
			case CLAVIER_JEU:
				timeWaiting=0;
				mode = WAIT;
				if(!etaitAppuye){ //si le joueur viens d'appuyer 
					int note;
					touch_read();
					note=getNumeroNote();
					if(start){
						if(note<4){
							start=FALSE;
							mode=JOUER_SEQUENCE;
							//initialisation
							score=0;
							avancement=0;
							//choix de la 1ère note "aléatoirement"
							sequence[0]=touch_x%4;
						}
						else{
							mode = READ_DATA_EEPROM;
						}
					}
					else if(note<4){
						//éteint la led
						GPIO_SetValue(0,(1<<3));
						//on joue la note et change l'affichage
						dessiner_rect(colorsPos[note][0],colorsPos[note][1],colorsPos[note][2],colorsPos[note][3],2,1,White,colors[1][note]);
						JouerNote(200, note);
						attentems(200);
						dessiner_rect(colorsPos[note][0],colorsPos[note][1],colorsPos[note][2],colorsPos[note][3],2,1,Black,colors[0][note]);
						//si la note est correct et que le score est suffisament petit					
						if(sequence[avancement]==note && score<MAX_SCORE-1){ 
							avancement++;
							if(avancement>score+1) while(1); //problème 
							//si le joueur à fini la séquence
							if(avancement==score+1){
								score++;
								avancement=0;
								//choix d'une note "aléatoirement"
								sequence[score]=touch_x%4;
								//on joue la note pour validé la séquence
								JouerNote(500, NOTE_AIGUE);
								attentems(1000);
								
								mode = JOUER_SEQUENCE;
							}
						}else{
							//on joue la note pour signalé une erreur
							JouerNote(500, NOTE_GRAVE);
							attentems(1500);
							
							mode = WRITE_DATA_EEPROM;
						}
					}
				}
				etaitAppuye=TRUE;
				break;
			case JOUER_SEQUENCE:
				//éteint la led
				GPIO_SetValue(0,(1<<3));
				//changement du text à l'écran
				sprintf(chaine,"Ecouter attentivement...");
				LCD_write_english_string(32,30,chaine,White,Blue);
			
				jouerSequence(score+1, sequence);
			
				mode = WAIT;
				break;
			case JOUER_SEQUENCEHS:
				//éteint la led
				GPIO_SetValue(0,(1<<3));
			
				jouerSequence(highscore, sequenceHS);
			
				mode = WAIT;
				break;
			case WRITE_DATA_EEPROM:
				if(score >= highscore){
					//enregistrement du highscore
					write_dataI2C(0x00, &score, 1);
					//enregistrement de la sequence du highscore
					write_dataI2C(0x01, sequence, score);
				}
				
				//changement du text à l'écran
				sprintf(chaine,"Jouer ou Highscore ?    ");
				LCD_write_english_string(32,30,chaine,White,Blue);
				
				//on revient à l'écran principal
				start = TRUE;
				mode = WAIT;
				break;
			case READ_DATA_EEPROM:
				//lecture du highscore
				read_dataI2C(0x00, &highscore, 1);
				//lecture de la sequence du highscore
				read_dataI2C(0x01, sequenceHS, highscore);
				if(highscore>MAX_SCORE) while(1); //problème de mémoire
			
				mode = HIGHSCORE;
				break;
			case HIGHSCORE:
				
				//changement du text à l'écran
				sprintf(chaine,"Highscore : %d          ", highscore);
				LCD_write_english_string(32,30,chaine,White,Blue);
				mode = JOUER_SEQUENCEHS;
				break;
			case WAIT:
				if(timeWaiting/20 > 3000 && start==FALSE){
					// eteint la led
					GPIO_SetValue(0,(1<<3));
					//changement du texte à l'écran
					sprintf(chaine,"3 sec ecoulees          ");
					LCD_write_english_string(32,30,chaine,Red,Blue);
					//on joue la note pour signalé une erreur
					JouerNote(500, NOTE_GRAVE);
					attentems(1500);
					
					mode = WRITE_DATA_EEPROM;
				}
				if ((GPIO_ReadValue(0)&(1<<3)) != 0 ){
					//allume la led
					GPIO_ClearValue(0,(1<<3));
					//point à partir duquel on compte 3s
					timeWaiting=0;					
					if(start){
						//changement du texte à l'écran
						sprintf(chaine,"Jouer ou Highscore ?    ");
						LCD_write_english_string(32,30,chaine,White,Blue);
					}else{
						//changement du texte à l'écran
						sprintf(chaine,"Repeter les/la note(s) !");
						LCD_write_english_string(32,30,chaine,White,Blue);
					}						
				}
				break;
			default:
				break;
		}
	}
} 
	// pour l'instant, le main fait juste quelques inits ...
	 
		
		
		

//---------------------------------------------------------------------------------------------	
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line) {while(1);}
#endif
