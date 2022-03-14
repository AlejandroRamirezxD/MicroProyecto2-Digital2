/*
 * File:   EUSART.c
 * Author: RAMIR
 *
 * Created on March 6, 2022, 11:02 PM
 */

#include "EUSART.h"
//EUSART_C(0,1,1,25,0,1,0,1,1,0); // Library functions 
// Configuracion de EUSART
void EUSART_C ( uint8_t sinc, uint8_t brgh, uint8_t brg16, uint8_t spbrg, 
                uint8_t spbrgh, uint8_t spen, uint8_t rx9, uint8_t cren,
                uint8_t txen, uint8_t tx9){
    // 9600 baudios
    // Configuracion de TX y RX
    TXSTAbits.SYNC = sinc;  // Asincronico
    TXSTAbits.BRGH = brgh;  // High speed 
    BAUDCTLbits.BRG16 = brg16; // Generador de 16 bits
    
    // SPBRGH:SPBRG = 25
    SPBRG  = spbrg;
    SPBRGH = spbrgh;
    
    RCSTAbits.SPEN = spen; // Serial port enable RX/DT y TX/CK
    RCSTAbits.RX9  = rx9; // Recibir 8 bits
    RCSTAbits.CREN = cren; // Activar recepcion
    
    TXSTAbits.TXEN = txen; // active trasmi
    TXSTAbits.TX9  = tx9;   
}

void EUSART_Imprimir_String(char string[]){
    for(int i = 0; i<strlen(string); i++) // Ir de 0 hasta la longitud del string
    {
        TXREG = string[i];    // Mostrar cada char del string de 1 en 1
        __delay_ms(1);     // Delay entre cada char mostrado
    }
    return;
}
