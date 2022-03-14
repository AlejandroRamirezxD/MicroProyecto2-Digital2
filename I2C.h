/* 
 * File:   I2C.h
 * Author: Alejandro Ramirez Morales
 * Comments: Configuracion rapida de I2C
 */

#ifndef I2C_H
#define	I2C_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h> // utilizar uint
#include <string.h> // medir cadenas
#include <pic16f887.h>

#define _XTAL_FREQ 1000000

void I2C_Maestro(const unsigned long SCL);
void I2C_Espera_Maestro(void);
void I2C_Inicio_Maestro(void);
void I2C_Reinicio_Maestro(void);
void I2C_Detener_Maestro(void);
void I2C_Escribe_Maestro(unsigned byteEnvio);
unsigned short I2C_Lectura_Maestro(unsigned short Ack_Secuencia);
void I2C_Incio_Esclavo(uint8_t address);
#endif	/* I2C_H */ 

