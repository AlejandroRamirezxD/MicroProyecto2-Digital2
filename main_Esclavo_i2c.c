/*
 * File:   main_Esclavo_i2c.c
 * Author: RAMIR
 *
 * Created on March 6, 2022, 10:44 PM
 */
/*
 +----------------------------------------------------------------------------+
 |                           BITS DE CONFIGURACION                            |
 +----------------------------------------------------------------------------+
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*
 +----------------------------------------------------------------------------+
 |                                 LIBRERIAS                                  |
 +----------------------------------------------------------------------------+
 */
#include <xc.h>
#include <stdint.h>
#include <string.h>
#include "I2C.h"
/*
 +----------------------------------------------------------------------------+
 |                             VARIABLES GLOBALES                             |
 +----------------------------------------------------------------------------+
 */
#define _XTAL_FREQ 4000000
#define Led_Rojo PORTAbits.RA1
#define Led_Amar PORTAbits.RA2
#define Led_Verd PORTAbits.RA3
#define TIERRA   PORTAbits.RA0

uint8_t Dato_Leido;
uint8_t EstadoLeds=4;

char Mensaje1 [] = "masl:    Color:";
char Mensaje2 [] = "         ";
char Verde    [] = "Green  ";
char Amarillo [] = "Yellow ";
char Rojo     [] = "Red    ";

/*
 +----------------------------------------------------------------------------+
 |                          PROTOTIPOS DE FUNCIONES                           |
 +----------------------------------------------------------------------------+
 */
void setup(void);

/*
 +----------------------------------------------------------------------------+
 |                               INTERRUPCIONES                               |
 +----------------------------------------------------------------------------+
 */

void __interrupt() isr(void){
    if(PIR1bits.SSPIF   == 1){
        SSPCONbits.CKP  = 0;
        
        // Habilita comunicacion
        if((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            Dato_Leido = SSPBUF;    // Limpiar Buff
            SSPCONbits.SSPOV = 0;   // Sin overflow
            SSPCONbits.WCOL = 0;    // Sin colision
            SSPCONbits.CKP = 1;     // Habilita SCL
        }
        
        // Recibe
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            __delay_us(7);
            Dato_Leido = SSPBUF;    // Limpiar Buff
            __delay_us(2);
            PIR1bits.SSPIF = 0;     // Limpia bandera 
            SSPCONbits.CKP = 1;     // Habilita SCL
            while(!SSPSTATbits.BF); // Esperar toda la recepcion
            EstadoLeds = SSPBUF;         // PORTD igual a valor recibido
            __delay_us(250);
        }
        
        // Envia
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            Dato_Leido = SSPBUF;    // Limpiar Buff
            SSPSTATbits.BF = 0;     // SSPUF vacio, bandera limpia
            SSPBUF = PORTB;         
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;    
        
    }
}

/*
 +----------------------------------------------------------------------------+
 |                                   LOOP                                     |
 +----------------------------------------------------------------------------+
 */
void main(void) {
    setup();        // Initial values
     
    while(1){
          //PORTB = ~PORTB;
         __delay_ms(200);
        if(EstadoLeds == '4'){
            Led_Amar = 0;
            Led_Rojo = 0;
            Led_Verd = 0;
            TIERRA = 0;
        }
        else if(EstadoLeds == '3'){
            Led_Amar = 0;
            Led_Rojo = 0;
            Led_Verd = 1;
            TIERRA = 0;
        }
        else if(EstadoLeds == '2'){
            Led_Amar = 1;
            Led_Rojo = 0;
            Led_Verd = 0;
            TIERRA = 0;
        }
        else if(EstadoLeds == '1'){
            Led_Amar = 0;
            Led_Rojo = 1;
            Led_Verd = 0;
            TIERRA = 0;
        }
    }
}
/*
 +----------------------------------------------------------------------------+
 |                            SETUP Y FUNCIONES                               |
 +----------------------------------------------------------------------------+
 */
void setup(void){    
    // In and outs
    ANSEL = 0;
    ANSELH = 0;
      
    
    TRISAbits.TRISA0    = 0;
    TRISAbits.TRISA1    = 0;
    TRISAbits.TRISA2    = 0;
    TRISAbits.TRISA3    = 0;
    TRISB = 0;  // PORTB out
    TRISE = 0;
    TRISD = 0;  // PORTC out
    
    PORTAbits.RA0   =0;
    PORTAbits.RA1   =0;
    PORTAbits.RA2   =0;
    PORTAbits.RA3   =0;
    PORTB = 0;  // PORTB 0
    PORTD = 0;  // PORTC 0
    PORTE = 0; 
    // Clock
    OSCCONbits.IRCF = 0b110;    // 4MHz
    OSCCONbits.SCS = 1;         // Use the internal clock
    I2C_Incio_Esclavo(0x50);
    
    // EUSART CONF
    //EUSART_C(0,1,1,25,0,1,0,1,1,0); // Library functions     
    
    // ISP 
    I2C_Incio_Esclavo(0x50);
    
    //Interrupciones
    INTCONbits.GIE  = 1;    // Global interruptions 
    INTCONbits.PEIE = 1;    // Peripherals interruptions
    PIE1bits.SSPIE  = 1;    // Interrupcion I2C
    PIR1bits.SSPIF  = 0;    // Bandera I2C
    return;
}