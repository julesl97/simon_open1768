//===========================================================//
// Projet Micro - INFO1 - ENSSAT - S2 2018							 //
//===========================================================//
// File                : Programme de départ
// Hardware Environment: Open1768	
// Build Environment   : Keil µVision
//===========================================================//

#include "lpc17xx_libcfg_default.h"
#include "touch\ili_lcd_general.h"
#include "touch\lcd_api.h"
#include "affichagelcd.h"
#include "touch\touch_panel.h"


#include "globaldec.h" // fichier contenant toutes les déclarations de variables globales
#include "global.h"
#include <stdio.h>
#include <stdlib.h>

void pin_Configuration(void);
void T1_Init(void);
void T0_Init(void);
void JouerNote(int dureeEnUs, int numero_note);
int getNumeroNote();

void read_dataI2C(uint32_t addr, uint8_t* dataArray, uint32_t nbBytes);
void write_dataI2C(uint32_t addr, uint8_t* dataArray, uint32_t nbBytes);

const int colors[][4] = {
	{Yellow, Green, Blue, Red},
	{0xFFE3, 0x1FE3, 0x187F, 0xF863}
};
const int colorsPos[][4] = {
	{10,60,110,110},
	{120,60,110,110},
	{10,170,110,110},
	{120,170,110,110}
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
int main(void){	  
	uint8_t score=0;
	uint8_t highscore=0;
	uint8_t sequence[256];
	uint8_t sequenceHS[256];
	int i;
	
	pin_Configuration();
	T1_Init();
	T0_Init();
	NVIC_EnableIRQ(TIMER1_IRQn);
	NVIC_EnableIRQ(TIMER0_IRQn);
	//important initialisation de l'ecran après les timers
	lcd_Initializtion(); // init pinsel ecran et init LC
	touch_init(); // init pinsel tactile et init tactile 

	// Init(); // init variable globales et pinsel son et IR
	// affichage de l'écran maitre

	sprintf(chaine,"Entrez une sequence      ");
	LCD_write_english_string(32,30,chaine,White,Blue);
	dessiner_rect(10,60,110,110,2,1,Black,Yellow);
	dessiner_rect(120,60,110,110,2,1,Black,Green);
	dessiner_rect(10,170,110,110,2,1,Black,Blue);
	dessiner_rect(120,170,110,110,2,1,Black,Red);

	init_i2c_eeprom();
	
	score = 0;
	mode = WAIT;
	start = TRUE;
	i=0;  //avancement sequence
	

	while(1){
		switch(mode){
			case CLAVIER_JEU:
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
							start=FALSE;
						}
						else{
							mode = READ_DATA_EEPROM;
						}
					}
					else if(note<4){
						JouerNote(200000, note);
						dessiner_rect(colorsPos[note][0],colorsPos[note][1],colorsPos[note][2],colorsPos[note][3],2,1,White,colors[1][note]);
						attentems(200);
						dessiner_rect(colorsPos[note][0],colorsPos[note][1],colorsPos[note][2],colorsPos[note][3],2,1,Black,colors[0][note]);
						if(sequence[i]==note){
							i++;
							if(i==score+1 && score!=255){
								score++;
								sequence[score]=touch_x%4;
								i=0;
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
				jouerSequence(score, sequence);
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
				mode = WAIT;
				start = TRUE;
				break;
			case READ_DATA_EEPROM:
				read_dataI2C(0x00, &highscore, 1);
				read_dataI2C(0x01, sequenceHS, highscore);
				mode = HIGHSCORE;
				break;
			case HIGHSCORE:
				sprintf(chaine,"Highscore : %d           ", highscore);
				LCD_write_english_string(32,40,chaine,White,Blue);
				mode = JOUER_SEQUENCEHS;
				break;
			case WAIT:
				GPIO_ClearValue(0,(1<<3));
				break;
			default:
				break;
		}
	}
}



//---------------------------------------------------------------------------------------------	
#ifdef  DEBUG
void check_failed(uint8_t *file, uint32_t line) {while(1);}
#endif
