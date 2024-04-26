/*
 * ESCLAVO MODBUS PARA PROYECTO GALLINEROS
 * 
 * AUTOR: ControlARG
 * Creacion: 17/01/24
 * 
 * Compilador CCS 5.093
 * 
 * Version inicial:
 */

#device PIC12F1572
//includes
#include <12f1572.h>                          // Definiciones del PIC 16F886

#byte OPTION_REG = 0x081
#bit    PS0 = OPTION_REG.0
#bit    PS1 = OPTION_REG.1
#bit    PS2 = OPTION_REG.2
#bit    PSA = OPTION_REG.3
#bit    T0SE = OPTION_REG.4
#bit    T0CS = OPTION_REG.5
#bit    INTEDG = OPTION_REG.6
#bit    RBPU = OPTION_REG.7

//#byte   TXSTA_REG = 0x098
//#bit    TRMT = TXSTA_REG.1


#fuses NOWDT,PROTECT,NOLVP,NOBROWNOUT,NODEBUG,PUT,NOMCLR,INTRC_IO
#use delay(clock=4000000)                     // Oscilador a 4 Mhz
#use rs232(baud=19200, xmit=PIN_A4, rcv=PIN_A5, ERRORS)// RS232 Estándar
//#priority RTCC,RDA,TBE
#pragma use fast_io(A)



// VARIABLES EN RAM ///////////////////////////////////////////////////////////

// Variables PID
//tabla para lectura desde PC//

int16 HR[3]={0};

//PINES
//PIN ALARMA PIN5 RA2
//
#define  pSET_A     PIN_A3   //salida
#define  pInput     PIN_A2   //entrada alarma
#define  pIn1       PIN_A1
#define  pIn0       PIN_A0

////////////////////////////////////////////
// variables teclado
////////////////////////////////////////////
int const kInmuni = 50;
int inmuniIN0 = kInmuni;
int inmuniIN1 = kInmuni;
int inmuniIN2 = kInmuni;

int1 fIn0=0;
int1 fIn1=0;
int1 fIn2=0;

// Declaración de Funciones ///////////////////////////////////////////////////

#include "Modbus.c"  
// INTERRUPCIONES /////////////////////////////////////////////////////////////

#INT_TIMER0
void RTCC_isr() {                // Interrupción Timer 0
    int8 rInputs=0;
    rInputs = input_A() & 7;
    
    if (bit_test(rInputs,0))
    {
        if(--inmuniIN0 ==0)
        {
            fIn0 = 1;
            inmuniIN0 = kInmuni;
        }
        else
        {
            fIn0 = 0;
            inmuniIN0 = kInmuni;
        }
    }
}
         
///////// INTERRUPCION POR RECEPCION SERIE ////////////

#INT_RDA
void rda_isr(){
char rcvchar=0x00;
rcvchar=0x00;                        // Inicializo caracter recibido
   if(kbhit()){                           // Si hay algo pendiente de recibir ...
      rcvchar=getc();                     // lo descargo y ...
      ModBusRX (rcvchar);
   }
   setup_timer_2 (T2_DIV_BY_16, 0xF0,3); //setup_timer_2 (T2_DIV_BY_4, 250, 16);
}

///////// INTERRUPCION POR tx  ////////////
#INT_TBE
void tbe_isr(){
    ModBusTX ();
}

///////// INTERRUPCION  TIMER1 ////////////

#int_TIMER1                            //Timer 1 overflow
void TIMER1_int(){
   clear_interrupt(INT_TIMER1);
}

#INT_TIMER2
void TIMER2_int(){
int16 CRC=0;
int16 CRC_Leido=0;

   if(fParami){                        //Calculo checksum de dato recibido
         CRC=CRC_Calc(Rxbuff,rxPuntero-3); //
         CRC_Leido= make16(Rxbuff[rxPuntero-2],Rxbuff[rxPuntero-1]);
         if(CRC_Leido==CRC){
            flagcommand=1;
         }
         setup_timer_2(T2_DISABLED,0,1);
         rxPuntero=0;
         }
   else{            
         inicbuffRX();
         rxpuntero=0;
         }
}

//Programa Principal /////////////////////////////////////////////////////////

void main() {
//   delay_ms(500);
//++++++++++++++++++Configura puertos analogicos++++++++++++++++++++++++++++++

   setup_adc_ports( NC_NC );                // Ver que puertos van a ser analogicos
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_128);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);  //
   setup_comparator(NC_NC);                 //no comparadores
   
   //Seteos puertos  
   set_tris_a (0b00100011);
   delay_ms(500);
 
   //Seteos puertos  
   set_tris_a (0b00100111);

   clear_interrupt(INT_TIMER1);
   disable_interrupts(INT_TIMER1);

   //ACTIVACION INTERRUPCIONES
   enable_interrupts(INT_TIMER0);                 // Habilita Interrupción TIMER0
   enable_interrupts(INT_TIMER2);   
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);                   // Habilita interrupciones

   ModbusAddress=3; 
   /////////BUCLE PRINCIPAL//////////
   do {
       if(flagcommand)
       {
           ModBus_exe ();
           flagcommand=0;
       }
       HR[0] = (input_a() & 4);

   } while (TRUE);

}
