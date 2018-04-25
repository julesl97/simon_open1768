//===========================================================//
// Projet Micro - INFO1 - ENSSAT - S2 2018							 //
//===========================================================//
// File                : Programme de départ
// Hardware Environment: Open1768	
// Build Environment   : Keil µVision
//===========================================================//

#include "lpc17xx_gpio.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_libcfg_default.h"
#include "lpc17xx_timer.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"


#include "global.h"
#include "globaldec.h" // fichier contenant toutes les déclarations de variables globales
#include <stdio.h>

	
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
		JouerNote(200000, s[i]);
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
						sprintf(chaine,"Jouer ou Highscore ? 3  ");
						LCD_write_english_string(32,30,chaine,White,Blue);
		for(i=0; i<NB_COLORS; i++){
			dessiner_rect(colorsPos[i][0],colorsPos[i][1],colorsPos[i][2],colorsPos[i][3],2,1,Black,colors[0][i]);
	  }
		
		pin_Configuration();
		
	  touch_init(); // init pinsel tactile et init tactile 
	  
		mode = WAIT;
		start = TRUE;
    while(1){
		switch(mode){
			case CLAVIER_JEU:
				timeSinceLastInput=0;
				mode = WAIT;
				touch_read();
				if(!etaitAppuye){
					int note=getNumeroNote();
					if(start){
						if(note<4){
							start=FALSE;
							mode=JOUER_SEQUENCE;
							//srand(touch_x+touch_y);
							sequence[0]=touch_x%4; // #random
							score=0;
							avancement=0;
							start=FALSE;
						}
						else{
							mode = READ_DATA_EEPROM;
						}
					}
					else if(note<4){
						GPIO_SetValue(0,(1<<3));
						JouerNote(200000, note);
						dessiner_rect(colorsPos[note][0],colorsPos[note][1],colorsPos[note][2],colorsPos[note][3],2,1,White,colors[1][note]);
						attentems(200);
						dessiner_rect(colorsPos[note][0],colorsPos[note][1],colorsPos[note][2],colorsPos[note][3],2,1,Black,colors[0][note]);
						if(sequence[avancement]==note && score<MAX_SCORE-1){ //si la note est correct et que le score est suffisament petit
							avancement++;
							if(avancement>score+1) exit(42); //problème 
							if(avancement==score+1){
								score++;
								sequence[score]=touch_x%4;
								avancement=0;
								JouerNote(500000, NOTE_AIGUE);
								attentems(1000);
								mode = JOUER_SEQUENCE;
							}
						}else{
							JouerNote(500000, NOTE_GRAVE);
							attentems(1500);
							mode = WRITE_DATA_EEPROM;
						}
					}
				}
				etaitAppuye=TRUE;
				break;
			case JOUER_SEQUENCE:
				GPIO_SetValue(0,(1<<3));
				sprintf(chaine,"Ecouter attentivement...");
				LCD_write_english_string(32,30,chaine,White,Blue);
				jouerSequence(score+1, sequence);
				timeSinceLastInput=0;
				mode = WAIT;
				break;
			case JOUER_SEQUENCEHS:
				GPIO_SetValue(0,(1<<3));
				jouerSequence(highscore, sequenceHS);
				mode = WAIT;
				break;
			case WRITE_DATA_EEPROM:
				if(score >= highscore){
					write_dataI2C(0x00, &score, 1);
					write_dataI2C(0x01, sequence, score);
				}
						sprintf(chaine,"Jouer ou Highscore ? 2  ");
						LCD_write_english_string(32,30,chaine,White,Blue);
				mode = WAIT;
				start = TRUE;
				break;
			case READ_DATA_EEPROM:
				
				read_dataI2C(0x00, &highscore, 1);
				read_dataI2C(0x01, sequenceHS, highscore);
				if(highscore>MAX_SCORE) exit(41); //problème de mémoire
				mode = HIGHSCORE;
				break;
			case HIGHSCORE:
				sprintf(chaine,"Highscore : %d          ", highscore);
				LCD_write_english_string(32,30,chaine,White,Blue);
				mode = JOUER_SEQUENCEHS;
				break;
			case WAIT:
				if(timeSinceLastInput/20 > 3000 && start==FALSE){
					GPIO_SetValue(0,(1<<3));	 // eteint la led 
					sprintf(chaine,"3 sec ecoulees          ");
					LCD_write_english_string(32,30,chaine,Red,Blue);
					JouerNote(500000, NOTE_GRAVE);
					attentems(1500);
					mode = WRITE_DATA_EEPROM;
				}
				if ((GPIO_ReadValue(0)&(1<<3)) != 0 ){
					GPIO_ClearValue(0,(1<<3));	
					if(start){
						sprintf(chaine,"Jouer ou Highscore ?    ");
						LCD_write_english_string(32,30,chaine,White,Blue);
					}else{
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
