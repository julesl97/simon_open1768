#ifndef CONSTANTES_H
#define CONSTANTES_H

#define NOTE_GRAVE 4
#define NOTE_AIGUE 5
#define MAX_SCORE 100

typedef enum {jaune,vert,rouge,bleu,NB_COLORS} touche ;

// mettez dans ce fichier toutes les constantes et définitions 
// des nouveaux types que vous souhaitez


typedef enum {
	WAIT,   
	WRITE_DATA_EEPROM,
	READ_DATA_EEPROM,		
	CLAVIER_JEU,	
	JOUER_SEQUENCE,	
	JOUER_SEQUENCEHS,	
	HIGHSCORE,		
} e_mode;

#endif

