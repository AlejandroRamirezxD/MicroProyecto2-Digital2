/* 
 * File:   I2C.c
 * Author: Alejandro Ramirez Morales
 * Comments: Configuracion rapida de I2C
 */

#include "I2C.h"

// Configuracion del Maestro
void I2C_Maestro(const unsigned long SCL){
    // Declarar como entradas SDA y SCL
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
    
//--SSPCON2-----------------------------Registro de control 2 
    
    SSPCON2bits.GCEN    = 0;   // Deshabilitar la escucha general
    SSPCON2bits.ACKSTAT = 0;// Sin bit de acknowledge acknowledge en envio
    // Probar este bit
    SSPCON2bits.ACKDT   = 0;// Activa Acknowledge en recepcion
    SSPCON2bits.ACKEN   = 0;// Inactiva Secuencia acknowledge inactiva
    SSPCON2bits.RCEN    = 0;// Inactiva recepcion
    SSPCON2bits.PEN     = 0;// Inactiva condicion de stop
    SSPCON2bits.RSEN    = 0;// Inactiva condicion de reinicio
    SSPCON2bits.SEN     = 0;// Inactiva SCL
    
//--SSPCON------------------------------Registro de control 1
    SSPCONbits.WCOL     = 0; // Sin colision
    SSPCONbits.SSPOV    = 0; // Sin overflow
    SSPCONbits.SSPEN    = 1; // Activa SDA y SCL
    SSPCONbits.CKP      = 0; // No se toma en cuenta en Master (Maestro)
    //SSPM = 1000, pues clock = fosc/(4*(SSPADD+1) - Maestro
    SSPCONbits.SSPM3    = 1; 
    SSPCONbits.SSPM2    = 0;
    SSPCONbits.SSPM1    = 0;
    SSPCONbits.SSPM0    = 0;
    
//--SSPADD------------------------------Registro de baud rate
    SSPADD = (_XTAL_FREQ/(4*SCL))-1;
    
//--SSPSTAT-----------------------------Registro de control 0
    SSPSTAT = 0;
    
}

// Esperar las condiciones para la comunicacion
void I2C_Espera_Maestro(){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

// Inicializar la comunicacion serial
void I2C_Inicio_Maestro(){
    I2C_Espera_Maestro();  // Esperar las condiciones apropiadas
    SSPCON2bits.SEN = 1;   // Habilita el SCL del i2c 
}

// Reiniciar I2C 
void I2C_Reinicio_Maestro(){
    I2C_Espera_Maestro();  // Esperar las condiciones apropiadas
    SSPCON2bits.RSEN = 1;  // Reinicia la comunicacion i2c
}

// Detiene comunicaon
void I2C_Detener_Maestro(){
    I2C_Espera_Maestro();  // Esperar las condiciones apropiadas
    SSPCON2bits.PEN = 1;   // Inactiva comunicacion i2c
}

// Escribe a los esclavos
void I2C_Escribe_Maestro(unsigned byteEnvio){
     I2C_Espera_Maestro();  // Esperar las condiciones apropiadas
     SSPBUF = byteEnvio;
}

unsigned short I2C_Lectura_Maestro(unsigned short Ack_Secuencia){
    unsigned short Valor_Leido;
    I2C_Espera_Maestro();   // Esperar las condiciones apropiadas
    SSPCON2bits.RCEN = 1;   // Habilita la recepcion
    I2C_Espera_Maestro();   // Esperar las condiciones apropiadas
    Valor_Leido = SSPBUF;
    I2C_Espera_Maestro();   // Esperar las condiciones apropiadas
    
    if(Ack_Secuencia == 1){
        SSPCON2bits.ACKDT = 0;  // Habilita acknowledge en recepcion
    }else{
        SSPCON2bits.ACKDT = 1;  // Inactiva acknowledge en recepcion
    }
    SSPCON2bits.ACKDT = 1;  // Inactiva acknowledge en recepcion
    return Valor_Leido;   // Regresa valor del byte leido
}

void I2C_Incio_Esclavo(uint8_t address){
    SSPADD = address;
    SSPCONbits.SSPEN = 1;       // Activa SDA y SCL
    SSPCONbits.CKP   = 1;       // Habilita recepcion SCL
    SSPCONbits.SSPM = 0b0110;   // Salve mode, en adress de 7bits
    
    SSPCON2bits.SEN  = 1;       // Habilita el SCL del i2c
    SSPSTATbits.SMP  = 0;       // Slew rate para 400kH
    SSPSTATbits.CKE  = 1;        // Rising edfe of sck
            
    TRISCbits.TRISC3 = 1;  
    TRISCbits.TRISC4 = 1;
}