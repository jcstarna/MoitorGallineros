////////////////////////////////////////////////////////////////////////////////
////                       IMPLEMENTACION MODBUS
////
////FECHA:  07/12/09
////REVISION: 0
////AUTOR:  CONTROLARG
////////////////////////////////////////////////////////////////////////////////
//
//REVISIONES:
//FECHA: 18/04/11
//Agregado de la funcion 0x10 para escritura de multiples registro
//Probado: OK
//Faltaria hacer comprobacion de datos osea, que lo que se escribio, realmente
//es lo que se queria escribir- comparar registros

//FECHA 10/04/11
//Se agrega la funcion 0x06, escritura de un solo registro
//FECHA 20/08/18
//Se modifica programa para detectar cambio de pagina ne memoria de datos
//si el dato que se pide esta en el aria desde 0x77 hasta 0x0a, se envia
//a la pagina siguiente que comienza en 0x0a

//#include ModBus.h
////////////////////////////////////////////////////////////////////////////////
////                       CALCULO DEL CRC DE O BITS
////////////////////////////////////////////////////////////////////////////////
int16 CRC_Calc (int *MSG,int msgLen){
int16 crc=0xffff;
int i=0 ;   //contador de bytes
int j=0;    //contador de bits
int16 dato=0;
for (i=0;i<=msgLen;i++){   //bucle para cada uno de los bytes del mensaje
   dato=*(msg+i);
   crc=crc^dato;
      for (j=0;j<=7;j++){
         if (shift_right(&crc,2,0))
                  crc=crc^0xA001; //si el bit a la derecha es 1
      }
}
crc = (crc << 8) | (crc >> 8);
fParami=0;
return crc;
}



////////////////////////////////////////////////////////////////////////////////
////                       PROCESAR DATO RECIBIDO
////////////////////////////////////////////////////////////////////////////////
/*Al llegar un dato nuevo al buffer de RX, se envia a esta rutina, si el dato 
  es para este esclavo se lo procesa, de otra forma se rechaza para reducir tiempo
*/
void ModBusRX (char data){
   SET_TIMER2(0);
   if ((rxPuntero==0) & (data==ModbusAddress))fParami=1;
   if(fParami){
      Rxbuff[rxPuntero]=data;
   }
   if (rxPuntero < lenbuff){
         rxpuntero++;
   }

 }



////////////////////////////////////////////////////////////////////////////////
////                       TRANSMITIR BUFFER
////////////////////////////////////////////////////////////////////////////////
/*Al llegar un dato nuevo al buffer de RX, se envia a esta rutina, si el dato 
  es para este esclavo se lo procesa, de otra forma se rechaza para reducir tiempo
*/
void ModBusTX (){
   //if(txlen==0)
      //output_low(pEN_RX);
   if(txlen-->0){
      //output_high(pEN_RX);
      putc(txbuff[txpoint]);
      txpoint++;
   }
   else{
      disable_interrupts(int_tbe); 
      inicbuffTX();
   }
}

////////////////////////////////////////////////////////////////////////////////
////                       PROCESAR COMANDO MODBUS 
////////////////////////////////////////////////////////////////////////////////
/* Utilizado cuando el dispositivo es esclavo modbus
 * 
 * Analiza el buffer de recepcion y elabora respuesta al master.
*/
void ModBus_exe (){
char  comando=0;
int   cant=0;
int   offset=0;
int   *DirIni;
int8 i =0;
int16 CRC=0;
flagcommand=0;
comando = rxBuff[ModBusCmd];  //recupero comando
offset = make16(rxBuff[ModbusIni],rxbuff[ModBusIni+1]);    //recupero direccion del registro como offset del inicio de la memoria
cant = make16(rxBuff[ModbusCant],rxbuff[ModBusCant+1]);    //recupero cantidad de registros para comando 0x03
DirIni = &rTeclasCrudas + offset;

if (DirIni > 0x76 & DirIni < 0xA0) 
   DirIni = 0xA0 + (DirIni-0x77);
switch (comando){
         case 0x03:     //leer registros
            //armo respuesta modubs para comando 0x03
            txbuff[ModbusAdd]=ModbusAddress;                //Direccion esclavo; 
            txbuff[ModbusCmd]=comando;             //Comando
            //busco los datos que esta en memoria
            for (i=1;i<=cant;i++){
                  txbuff[i*2+1]=*(DirIni+1);
                  txbuff[i*2+2]=*(DirIni);
                  DirIni+=2;
                  if (DirIni > 0x76 & DirIni < 0xa0) DirIni = 0xa0;
            }
            txlen=cant*2;
            txbuff[ModbusBCount]=txlen;            //Cantidad de Bytes
            CRC=CRC_Calc(txBuff,txlen+2);          //Calculo checksum del mensaje
            //cargo CRC en buffer
            txBuff[txlen+3]=make8(CRC,1);          //Checksum parte alta
            txBuff[txlen+4]=make8(CRC,0);          //Checksum parte baja
            txlen=txlen+5;                         //Cantidad de bytes en el buffer de transmision
            break;
         case 0x04:
            break;
         case 0x06://Setear un registro
                  //00 esclavo
                  //01 comando
                  //02 DirH
                  //03 DirL
                  //04 DataH
                  //05 DataL                
                  *(DirIni+1) =rxbuff[4];
                  *(DirIni)   =rxbuff[5];
                  //ARmo respuesta que es repetir la pregunta
            Modbus_RespOK(ModbusAddress, comando);           
            break;            
         case 0x10://Setear Multiples Registros
            for (i=1;i<=cant;i++){      
                  *(DirIni+1) =rxbuff[i*2+5];
                  *(DirIni)   =rxbuff[i*2+6];
                  DirIni+=2;
                  if (DirIni > 0x76 & DirIni < 0xa0) DirIni = 0xa0;
            }
            //Armo respuesta
            Modbus_RespOK(ModbusAddress, comando);
            break;            
         case 0x11:
            break;
}
txpoint=0;
enable_interrupts(int_tbe); 
delay_us(1); 
}

//Arma respuesta error de excepcion
void ModBus_Exep(int SlvAdd,int ExepNro){
//int16 CRC=0;
//01 Illegal function
//02 Illegal Data address
//03 Illegal Data Value


}

void Modbus_RespOK(int SlvAdd,int Cmd){
int16 CRC=0;
            txbuff[ModbusAdd]=SlvAdd;                //Direccion esclavo; 
            txbuff[ModbusCmd]=cmd;             //Comando
            txBuff[ModbusIni]=rxBuff[ModbusIni];
            txBuff[ModbusIni+1]=rxBuff[ModbusIni+1];
            txBuff[ModbusCant]=rxBuff[ModbusCant];
            txBuff[ModbusCant+1]=rxBuff[ModbusCant+1];
            CRC=CRC_Calc( txBuff,5);          //Calculo checksum del mensaje
            //cargo CRC en buffer
            txBuff[6]=make8(CRC,1);          //Checksum parte alta
            txBuff[7]=make8(CRC,0);          //Checksum parte baja
            txlen=8;                         //Cantidad de bytes en el buffer de transmision
}

void inicbuffRX(void){                   // Inicia a \0 cbuff -------------------
   int i;
   for(i=0;i<lenbuff;i++){             // Bucle que pone a 0 todos los
      Rxbuff[i]=0x00;                   // caracteres en el buffer
   }
   rxPuntero=0x00;                         // Inicializo el indice de siguiente
}

void inicbuffTX(void){                   // Inicia a \0 cbuff -------------------
   int i;
   for(i=0;i<lenbuff;i++){             // Bucle que pone a 0 todos los
       txbuff[i]=0;
   }
   rxPuntero=0x00;                        // Inicializo el indice de siguiente
                                      // caracter
}


////////////////////////////////////////////////////////
//      interpretea respuesta de esclavos
////////////////////////////////////////////////////////
void ParseMDBSlave (struct str_Slave *Slave){
char  comando=0;
int   cant=0;
//int   offset=0;
int   *DirIni;
int8 i =0;
int  slvAddress=0;
int16 CRC=0;
slvAddress= rxBuff[ModbusAdd];
comando = rxBuff[ModBusCmd];  //recupero comando
//offset = make16(rxBuff[ModbusIni],rxbuff[ModBusIni+1]);    //recupero direccion del registro como offset del inicio de la memoria
cant = rxBuff[ModbusBCount];                                 //recupero cantidad de registros para comando 0x03
//DirIni = &rTeclasCrudas + offset;

switch (comando){
         case 0x03:     //respuesta desde esclavo
             for (i=0; i <= (cant/2) ;i++)
             {
                Slave->slaveregs[i] = make16(rxbuff[i*2+3],rxbuff[i*2+4]);   //(*slaveList[0].slaveRegs)
                //*(SlaveReg->slaveregs+1)=rxbuff[i*2+4]; 
                //rSlave0 = rxbuff[i*2+3];
                //rSlave0 = rxbuff[i*2+4];
             }
                fRxPend=0; 
            break;
         case 0x04:
            break;
         case 0x06://Setear un registro
                  //00 esclavo
                  //01 comando
                  //02 DirH
                  //03 DirL
                  //04 DataH
                  //05 DataL                
                  *(DirIni+1) =rxbuff[4];
                  *(DirIni)   =rxbuff[5];
                  //ARmo respuesta que es repetir la pregunta
            break;            
         case 0x10://Setear Multiples Registros

            //Armo respuesta
            Modbus_RespOK(ModbusAddress, comando);
            break;            
         case 0x11:
            break;
}
delay_us(1); 
}


/*
    Tarea principal maestro modbus
 *  
 * Encuesta uno a uno los esclavos que estan en el array SlaveList
 * 
 */
void MBMaster_exe()
{
    switch (MBMasterStatus)
    {
        case standBy:
            //esperar cierto tiempo antes de siguiente encuensta
            if(fTXModbus){
                output_toggle(pR1);
                MBMasterStatus=sendCMD;
                fTXModbus=0;
            }
            break;
        case sendCMD:
            ModbusAddress=SlaveList[actSlave].slaveAdd;
            txbuff[0]=SlaveList[actSlave].slaveAdd;
            txbuff[1]=3;
            txbuff[2]=(int)(SlaveList[actSlave].startAdd & 0xf0);
            txbuff[3]=(int)(SlaveList[actSlave].startAdd & 0x0f);
            txbuff[4]=(int)(SlaveList[actSlave].cantReg & 0xf0);
            txbuff[5]=(int)(SlaveList[actSlave].cantReg & 0x0f);
            CHECKSUM = CRC_Calc(txBuff,5);        //Calculo checksum del mensaje
            txBuff[6]=make8(CHECKSUM,1);          //Checksum parte alta
            txBuff[7]=make8(CHECKSUM,0);          //Checksum parte baja
            txlen=8;
            txpoint=0;
            enable_interrupts(int_tbe);
            fRxPend=1; 
            MBMasterStatus++;
            break;
        case waitResp:
            if(flagcommand == 1)
            {
                SlaveList[actSlave].failCount = SlaveList[actSlave].maxFail;
                SlaveList[actSlave].Status = 1;     //esclavo respondiendo
                //llamar a interpreter de respuesta
                MBMasterStatus = parseResp;
                flagcommand = 0;
            }
            if(fTimeOUT == 1)
            {
                if (--SlaveList[actSlave].failCount == 0)
                {
                    SlaveList[actSlave].Status = 0;//time falla despues de conteo de fallas
                    SlaveList[actSlave].failCount = SlaveList[actSlave].maxFail;
                }
                MBMasterStatus = nextStatus;
            }
            break;
        case parseResp:
            ParseMDBSlave (&slaveList[actSlave]);//slaveList[actSlave]
            MBMasterStatus = nextStatus;
            break;
        case nextStatus:
            if (++actSlave >= CantEsclavos)
            {
                actSlave = 0;
            }
            MBMasterStatus = standBy;
            break;
          
    }
}
