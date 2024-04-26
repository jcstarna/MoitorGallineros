////////////////////////////////////////////////////////////////////////////////
////                  RFcomm Maestro
////FECHA:11/07/2019
////REVISION: 1.00
////AUTOR:CRTARG
////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
//  NOTAS REVISIONES:
//
/*
Ver: 1.00
   Version inicial
*/
//OPCIONES DE CONFIGURACION
#DEFINE useModbus
//#define useRXString
//#define useCustom

#include <16F1713.h>
#fuses INTRC_IO,NOWDT,noprotect,NOLVP,NOBROWNOUT,NODEBUG,PUT,NOMCLR
#use delay(clock=4000000)
#pragma use fast_io(A)
#pragma use fast_io(B)
#pragma use fast_io(C)
//#pin_select 
//#use rs232(baud=2400, BITS=8, xmit=PIN_C6, rcv=PIN_C7, enable=PIN_C5, ERRORS,STREAM=COMM)// RS232 Estándar
#pin_select U1RX = PIN_C7
#pin_select U1TX = PIN_C6
//#use rs232(UART1)
#use rs232(baud=19200, BITS=8, xmit=PIN_C6, rcv=PIN_C7, ERRORS)// RS232 Estándar enable=PIN_C5

#include <MasterRX.h>
//#include <inputs.c>

// librerias modbus
#ifdef useModbus
   #include "modbus.h"
   #include "modbus.c"
#endif
// recepcion de texto Ej, balanzas
#ifdef useRXString
   //#include "txtProtocol.c"
#endif


// INTERRUPCIONES /////////////////////////////////////////////////////////////
#INT_RDA
void RX_isr(){
char rcvchar=0x00;
rcvchar=0x00;                        // Inicializo caracter recibido
   if(kbhit()){                           // Si hay algo pendiente de recibir ...
      rcvchar=getc();                     // lo descargo y ...
      ModBusRX(rcvchar);                // lo añado al buffer y ...
   }
   setup_timer_2 (T2_DIV_BY_16, 0xF0,3); //setup_timer_2 (T2_DIV_BY_4, 250, 16);
}


#int_TIMER2
void TMR2_isr(){
int16 CRC=0;
int16 CRC_Leido=0;
   IF(fParami){                        //Calculo checksum de dato recibido
         CRC=CRC_Calc(rxbuff,rxpuntero-3); //
         CRC_Leido= make16(rxBuff[rxpuntero-2],rxBuff[rxpuntero-1]);
         if(CRC_Leido==CRC){
            flagcommand=1;
         }
         setup_timer_2(T2_DISABLED,0,1);
         rxpuntero=0;
         }
   else{
         inicbuffRX();
         rxpuntero=0;
         }
}


#INT_TBE
void TX_isr(){
   ModBusTX();
}


#int_TIMER0
void RTCC_isr() {             // Interrupción Timer 0
    if (--r500ms==0){
       rOscilador++;
       output_toggle(pLED);
       r500ms=k05s;
       if (!fRxPend){ 
          if(--rPolTime==0){
             fTXModbus=1;//si no hay encuesta en curso, pide nueva
             rPolTime=kPolTime; 
             fTimeOut=0;
             //output_low(pR6);//(pBuzzer);
             }
          rTimeOut=kTimeOut;
       }
       else{
          //si esta esperando respuesta, cuenta time out
          if(--rTimeOut==0){
             rTimeOut=kTimeOut; 
             fTimeOut=1;
             fRxPend=0; 
             //output_high(pR6);//(pBuzzer);
          }
       }
    }
    if (input(pReset))
    {
        if(--rInmuni==0)
        {
            rInmuni=kInmuni;
            fReset=1;
        }
    }
    else
    {
        rInmuni=kInmuni;
        fReset=0;
    }
}
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//  FUNCIONES
////////////////////////////////////////////////////////
// LOGICA ALARMA
void AlarmLogic (void)
{
    int8 i =0;
    int16 Registro=0;
    //si hay reset presionado, apaga la campana
    if (fReset) fCampana=0;

    //analizo estado de cada esclavo, si entra en falla, activa campana
    for (i=0; i < CantEsclavos; i++)
    {
        if ((slaveList[i].prevStatus != slaveList[i].status) && (slaveList[i].status == 0))    //reviso estado de la comunicacion
        {
                fCampana=1;
                slaveStatus[i]=1;   //alarma aparecida
        }
        slaveList[i].prevStatus = slaveList[i].status;
        
        //detecta cambios en los registros del esclavo
        //deteccion flanco de bajada del cualquier bit del registro 0 de cada esclavo configurado
        slaveList[i].slaveFP[0] = ~slaveList[i].slaveRegs[0] & slaveList[i].slaveEst[0];
        slaveList[i].slaveEst[0] = slaveList[i].slaveRegs[0];
        
        if ( slaveList[i].slaveFP[0] != 0)
        {
            fCampana = 1;
            slaveStatus[i]=1;   //alarma aparecida
        }
    }

}

///////////////////////////////////////////////////////////
// logica para indicar el estado de cada esclavo
///////////////////////////////////////////////////////////
// apagado, esclavo normal
// titilando, alarma no reconocida
// encendido, alarma activa y reconocida por el operario
// pR7 esclavo 1
// pR6 esclavo 2
// pR5 esclavo 3
void LedsLogic(void)
{
    int i =0;
    for (i=0; i < CantEsclavos; i++)
    {
        if (fReset && (slaveStatus[i]==1))
            slaveStatus[i]=2;
        if ((slaveStatus[i]==2) && (slaveList[i].slaveRegs[0] == 4) && (slaveList[i].status == 1))
            slaveStatus[i]=0;
    }
    //led esclavo 1
    if (slaveStatus[0] == 1)//si el esclavo tiene alarma aparecida
    {
        if(bit_test(rOscilador,0))
            output_low(pR7);
        else
            output_high(pR7);
    }
    else if (slaveStatus[0] == 2)
        output_low(pR7);
    else if (slaveStatus[0] == 0)
        output_high(pR7);
    
     //led esclavo 2
    if (slaveStatus[1] == 1)//si el esclavo tiene alarma aparecida
    {
        if(bit_test(rOscilador,0))
            output_low(pR6);
        else
            output_high(pR6);
    }
    else if (slaveStatus[1] == 2)
        output_low(pR6);
    else if (slaveStatus[1] == 0)
        output_high(pR6);  
    
    //led esclavo 3
    if (slaveStatus[2] == 1)//si el esclavo tiene alarma aparecida
    {
        if(bit_test(rOscilador,0))
            output_low(pR5);
        else
            output_high(pR5);
    }
    else if (slaveStatus[2] == 2)
        output_low(pR5);
    else if (slaveStatus[2] == 0)
        output_high(pR5);
    
    if (fcampana )
    {
        if (bit_test(rOscilador,1))
            output_high(pBuzzer);
        else
            output_low(pBuzzer);
    }
    else
        output_low(pBuzzer);
}



////////////////////////////////////////////////////////
//                         MAIN
////////////////////////////////////////////////////////
void Main(){
int8 rTemp=0;
INT16 CHECKSUM=0;
//++++++++++++++++++Configura puertos analogicos++++++++++++++++++++++++++++++
setup_timer_0(RTCC_INTERNAL|RTCC_DIV_16);  //interrumpe cada 1.024ms
setup_timer_2(T2_DIV_BY_4,0xF0,10);       //control de inactividad del bus
setup_adc_ports( ADC_OFF );               //no analogicas
setup_comparator(NC_NC_NC_NC);            //no comparadores

//Configuracion de puertos 
set_tris_a(0B00010000);
set_tris_b(0B00000001);
set_tris_c(0B11000000);

output_a(0);
output_b(0);
output_c(0);

//Inicio Variables
rTmr10ms=k10ms;
rTmr05s=k05s;
r600ms=k06s;
flagcommand=0;
inicbuffRX();

////seteo interrupciones
   enable_interrupts(INT_rda);
   enable_interrupts(int_TIMER2);                  // Habilita Interrupción TIMER2   
   enable_interrupts(INT_TIMER0);   
   enable_interrupts(global);   

   output_low(pEN_RX);
   output_high(pR1);
   output_high(pR2);
   output_high(pR3);
   output_high(pR4);
   output_high(pR5);
   output_high(pR6);
   output_high(pR7);

   
   slaveList[0].slaveAdd=1;
   slaveList[0].startAdd=0;
   slaveList[0].cantReg=1;
   slaveList[0].maxFail=5;
   slaveList[0].failCount=5;
   //slaveList[0].slaveRegs = &rSlave0;
   
   slaveList[1].slaveAdd=2;
   slaveList[1].startAdd=0;
   slaveList[1].cantReg=1;
   slaveList[1].maxFail=5;
   slaveList[1].failCount=5;
   //slaveList[1].slaveRegs = &rSlave1;
   
   slaveList[2].slaveAdd=3;
   slaveList[2].startAdd=0;
   slaveList[2].cantReg=1;
   slaveList[2].maxFail=5;
   slaveList[2].failCount=5;
   
   slaveList[0].prevStatus = slaveList[0].status;
   slaveList[1].prevStatus = slaveList[1].status;
   slaveList[2].prevStatus = slaveList[2].status;
   
   MBMasterStatus=StandBy;
   actSlave=0;
   fRxPend=0;
   output_high(pBuzzer);
   delay_ms(100);
   output_low(pBuzzer);
   
while(true){
        MBMaster_exe();
        //si el esclavo esta en falla, dejar encendido led
        
        //logica alarma
        AlarmLogic();
        
        //leds
        LedsLogic();
       
    }
}