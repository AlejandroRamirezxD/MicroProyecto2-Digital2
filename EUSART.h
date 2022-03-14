/* 
 * File:   EUSART.h
 * Author: Alejandro Ramirez Morales
 * Comments: Configuracion basica del EUSAR para pic16f887
 */

#ifndef EUSART_H
#define	EUSART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <string.h>
#define _XTAL_FREQ 1000000

void EUSART_C ( uint8_t sinc, uint8_t brgh, uint8_t BRG16, uint8_t spbrg, 
                uint8_t spbrgh, uint8_t spen, uint8_t rx9, uint8_t cren,
                uint8_t txen, uint8_t tx9);
void EUSART_Imprimir_Char(char dato);
void EUSART_Imprimir_String(char string[]);
#endif	/* EUSART_H */ 

