/*
  Archivo:  LAB1.c
  Autor:    Alejandro Ramirez Morales
  Creado:   10/feb/22
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
#include "LCD.h"
#include "I2C.h"
#include "EUSART.h"
/*
 +----------------------------------------------------------------------------+
 |                             VARIABLES GLOBALES                             |
 +----------------------------------------------------------------------------+
 */
#define _XTAL_FREQ 4000000

char Mensaje1 [] = " msnm:  Color:";
char Mensaje2 [] = "         ";
char Verde    [] = "Green  ";
char Amarillo [] = "Yellow ";
char Rojo     [] = "Red    ";
char Apagados [] = "Led off";
//char AlturaMSM[];

//uint8_t  Alti_Lec_Lista;
int  Alti_Sup;
uint8_t  Alti_Inf;
int Alti_Tot;
uint8_t bandera = 0;
char data;
char estadoLeds = '4';
uint8_t temperatura = 23;

struct digitos{
    uint8_t milesima;
    uint8_t centena;
    uint8_t decena;
    uint8_t unidad;
}Altitud,Altidud_Ascci;

//Status del sensor,utilizar estructura para analizar bit a bit con facilidad.
typedef union{
    uint8_t byte;
    struct {
        unsigned b0:1;
        unsigned b1:1;
        unsigned b2:1;
        unsigned b3:1;
        unsigned b4:1;
        unsigned b5:1;
        unsigned b6:1;
        unsigned b7:1;
    };
}byte_t;
byte_t Alti_Lec_Lista;
/*
 +----------------------------------------------------------------------------+
 |                          PROTOTIPOS DE FUNCIONES                           |
 +----------------------------------------------------------------------------+
 */
void setup(void);
void actualizar_cadena(uint8_t Pos_Ini_Cadena,char Texto_Sum[],
                       char Texto_Base[]);
void Divisor(int valor, uint8_t *unidades, uint8_t *decena, uint8_t *centena,
             uint8_t *milesima);
void actualizar_cadena(uint8_t posI, char refresco[], char mensaje[]);
void MensajeLCD (void);
void MensajeLCDAma (void);
void MensajeLCDRojo (void);
void MensajeLCDVerde (void);
void MensajeLCDApagados (void);
/*
 +----------------------------------------------------------------------------+
 |                               INTERRUPCIONES                               |
 +----------------------------------------------------------------------------+
 */

void __interrupt() isr(void){
    if(PIR1bits.RCIF){
        estadoLeds = RCREG;
    }
    PIR1bits.RCIF = 0;
}

/*
 +----------------------------------------------------------------------------+
 |                                   LOOP                                     |
 +----------------------------------------------------------------------------+
 */

void main(void) {
    setup();        // Initial values
    __delay_ms(10);
    LCD_Inicar();   // Initialize LCD
    LCD_Set_Cursor(0,0);
    LCD_Imprimir_Texto(Mensaje1);
       
//---------------------------------------------------- Configuracion Altimetro
    // OSR = 128 y seleccion altimetro
    
    I2C_Inicio_Maestro();
    I2C_Escribe_Maestro(0xC0);
    I2C_Escribe_Maestro(0x26);
    I2C_Escribe_Maestro(0xB8);
    I2C_Detener_Maestro();
    __delay_ms(200);
    
    // Activar banderas
    I2C_Inicio_Maestro();
    I2C_Escribe_Maestro(0xC0);
    I2C_Escribe_Maestro(0x13);
    I2C_Escribe_Maestro(0x07);
    I2C_Detener_Maestro();
    __delay_ms(200);
    
    // Activar altimetro
    I2C_Inicio_Maestro();
    I2C_Escribe_Maestro(0xC0);
    I2C_Escribe_Maestro(0x26);
    I2C_Escribe_Maestro(0xB9);
    I2C_Detener_Maestro();
    __delay_ms(200);
    
    //Lec_Lista b3 del status del sensor, data nueva para analizar
    I2C_Inicio_Maestro();
    I2C_Escribe_Maestro(0xC0);
    I2C_Escribe_Maestro(0x06);
    I2C_Inicio_Maestro();
    I2C_Escribe_Maestro(0xC1);
    Alti_Lec_Lista.byte = I2C_Lectura_Maestro(0);
    I2C_Detener_Maestro();
    __delay_ms(200);
    
        
    while(1){
        //Lec_Lista b3 del status del sensor, data nueva para analizar
        I2C_Inicio_Maestro();
        I2C_Escribe_Maestro(0xC0);
        I2C_Escribe_Maestro(0x06);
        I2C_Inicio_Maestro();
        I2C_Escribe_Maestro(0xC1);
        Alti_Lec_Lista.byte = I2C_Lectura_Maestro(0);
        I2C_Detener_Maestro();
        __delay_ms(200);    
        
        if(Alti_Lec_Lista.b3 == 1){
            //Alti_Lec_Lista
            I2C_Inicio_Maestro();
            I2C_Escribe_Maestro(0xC0);
            I2C_Escribe_Maestro(0x01);
            I2C_Inicio_Maestro();
            I2C_Escribe_Maestro(0xC1);
            Alti_Sup = I2C_Lectura_Maestro(0);
            I2C_Detener_Maestro();
            __delay_ms(200);

            I2C_Inicio_Maestro();
            I2C_Escribe_Maestro(0xC0);
            I2C_Escribe_Maestro(0x02);
            I2C_Inicio_Maestro();
            I2C_Escribe_Maestro(0xC1);
            Alti_Inf = I2C_Lectura_Maestro(0);
            I2C_Detener_Maestro();
            __delay_ms(200); 
        }

        if(estadoLeds == '1'){
            MensajeLCDRojo(); 
        }
        else if(estadoLeds == '2'){
            MensajeLCDAma();         
        }
        else if(estadoLeds == '3'){
            MensajeLCDVerde();
        }
        else if(estadoLeds == '4'){
            MensajeLCDApagados ();            
        }
                        
        Alti_Tot = (Alti_Sup <<8 | Alti_Inf)+100;
        //Alti_Tot = 1234;
        Divisor(Alti_Tot,&Altitud.unidad,&Altitud.decena,&Altitud.centena,
                &Altitud.milesima);
        
        if(PIR1bits.TXIF){
            while(!TRMT);
            TXREG= Altitud.milesima+48;
            
            while(!TRMT);
            //__delay_ms(1);
            TXREG= Altitud.centena+48;
            
            while(!TRMT);
            //__delay_ms(1);
            TXREG= Altitud.decena+48;
            
            while(!TRMT);
            //__delay_ms(1);
            TXREG= Altitud.unidad+48;
            
            while(!TRMT);
            //__delay_ms(1);
            TXREG= ' ';
            //__delay_ms(1);
        }
        /*
        
        // Sensor de temperatura
        I2C_Inicio_Maestro();
        I2C_Escribe_Maestro(0x91);
        temperatura = I2C_Lectura_Maestro(0);
        I2C_Detener_Maestro();
        __delay_ms(200);
        
        */
        
        // PIC ESCLAVO ENVIAR LOS VALORES 
        I2C_Inicio_Maestro();
        I2C_Escribe_Maestro(0x50);
        I2C_Escribe_Maestro(estadoLeds);
        I2C_Detener_Maestro();
        __delay_ms(200);
       
        // Recibir valores del PIC esclavo (Valores recibidos no son utilizados)
        I2C_Inicio_Maestro();
        I2C_Escribe_Maestro(0x51);
        PORTE = I2C_Lectura_Maestro(0);
        I2C_Detener_Maestro();
        __delay_ms(200);
        PORTB++;  
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
      
    TRISAbits.TRISA1 = 0;  // PORTA out
    TRISB = 0;  // PORTB out
    TRISD = 0;  // PORTC out
    TRISE = 0;  // PORTC out
    
    PORTAbits.RA1 = 0;  // PORTA 0
    PORTB = 0;  // PORTB 0
    PORTD = 0;  // PORTC 0
     PORTE = 0;  // PORTC 0
    
    // Interrupciones
    INTCONbits.GIE  = 1;        // Interrupciones globales activadas
    INTCONbits.PEIE = 1;        // Interrupciones perifericas activadas
    PIR1bits.RCIF   = 0;        // Bandera de recepcion de EUSART
    PIE1bits.RCIE   = 1;        // Interrupciones de recepcion EUSART
    
    // Clock
    OSCCONbits.IRCF = 0b110;    // 1MHz
    OSCCONbits.SCS = 1;         // Use the internal clock
    
    // I2C configuration
    I2C_Maestro(400000);
    
    
    // EUSART CONF
    EUSART_C(0,1,1,8,0,1,0,1,1,0); // Library functions     
    return;
}

void Divisor(int valor, uint8_t *unidades, uint8_t *decena, uint8_t *centena,
             uint8_t *milesima){
    *unidades = valor%10;           // Modulo gives the first digit
    int cociente0 = (int)(valor/10);// Get the last two digits
    *decena = cociente0%10;         // Remainder of the second one digit
    
    int dobleCentena = (int)(cociente0/10);// First two digit of division
    int cociente1 = (int)(dobleCentena/10);// Get the first two digits
    *centena  = (dobleCentena%10); // Remainder of the second one digit
    *milesima = cociente1%10;      // Get the first digit
}

void MensajeLCDAma (void){
    LCD_Set_Cursor(0,0);   
    LCD_Imprimir_Texto(Mensaje1);  
    LCD_Set_Cursor(1,0);
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(Altitud.milesima+48); 
    LCD_Imprimir_Char(Altitud.centena+48);
    LCD_Imprimir_Char(Altitud.decena+48); 
    LCD_Imprimir_Char(Altitud.unidad+48); 
    LCD_Imprimir_Char('m');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char('A'); 
    LCD_Imprimir_Char('m');
    LCD_Imprimir_Char('a'); 
    LCD_Imprimir_Char('r'); 
    LCD_Imprimir_Char('i');
    LCD_Imprimir_Char('l');
    LCD_Imprimir_Char('l');
    LCD_Imprimir_Char('o');  
}
void MensajeLCDRojo (void){
    LCD_Set_Cursor(0,0);  
    LCD_Imprimir_Texto(Mensaje1);  
    LCD_Set_Cursor(1,0);
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(Altitud.milesima+48); 
    LCD_Imprimir_Char(Altitud.centena+48);
    LCD_Imprimir_Char(Altitud.decena+48); 
    LCD_Imprimir_Char(Altitud.unidad+48); 
    LCD_Imprimir_Char('m');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char('R'); 
    LCD_Imprimir_Char('o');
    LCD_Imprimir_Char('j'); 
    LCD_Imprimir_Char('o'); 
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' '); 
}
void MensajeLCDVerde (void){
    LCD_Set_Cursor(0,0);  
    LCD_Imprimir_Texto(Mensaje1);  
    LCD_Set_Cursor(1,0);
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(Altitud.milesima+48); 
    LCD_Imprimir_Char(Altitud.centena+48);
    LCD_Imprimir_Char(Altitud.decena+48); 
    LCD_Imprimir_Char(Altitud.unidad+48); 
    LCD_Imprimir_Char('m');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char('V'); 
    LCD_Imprimir_Char('e');
    LCD_Imprimir_Char('r'); 
    LCD_Imprimir_Char('d'); 
    LCD_Imprimir_Char('e');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(' '); 
}
void MensajeLCDApagados (void){
    LCD_Set_Cursor(0,0); 
    LCD_Imprimir_Texto(Mensaje1);  
    LCD_Set_Cursor(1,0);
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char(Altitud.milesima+48); 
    LCD_Imprimir_Char(Altitud.centena+48);
    LCD_Imprimir_Char(Altitud.decena+48); 
    LCD_Imprimir_Char(Altitud.unidad+48); 
    LCD_Imprimir_Char('m');
    LCD_Imprimir_Char(' ');
     LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char('L'); 
    LCD_Imprimir_Char('e');
    LCD_Imprimir_Char('d'); 
    LCD_Imprimir_Char('s'); 
    LCD_Imprimir_Char(' ');
    LCD_Imprimir_Char('O');
    LCD_Imprimir_Char('f');
    LCD_Imprimir_Char('f'); 
}