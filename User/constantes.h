#ifndef CONSTANTES_H
#define CONSTANTES_H


typedef enum {jaune,vert,rouge,bleu} touche ;


typedef enum {
	WAIT,   
	WRITE_DATA_EEPROM,
	READ_DATA_EEPROM,		
	CLAVIER_JEU,	
	JOUER_SEQUENCE,	
	JOUER_SEQUENCEHS,	
	HIGHSCORE,		
} e_mode;
/*
typedef enum {
	START = 0x00000001,     	//bit1
	WRITE_DATA_EEPROM = 0x00000002,	 	//bit2 : ecrit score et sequence 
	READ_DATA_EEPROM = 0x00000004,		//bit3 : remplace score et sequence par ceux du highscore
	CLAVIER_JEU = 0x00000008,					//bit4
	JOUER_SEQUENCE = 0x00000010,				//bit5
	HIGHSCORE = 0x00000020,						//bit6
} e_mode;
*/
// mettez dans ce fichier toutes les constantes et définitions 
// des nouveaux types que vous souhaitez


#define NOTE_GRAVE 4
#define NOTE_AIGUE 5

#endif









