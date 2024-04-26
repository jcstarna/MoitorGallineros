////////////////////////////////////////////////////////////////////////////////
////                       IMPLEMENTACION MODBUS
////                    VARIABLES, CONSTANTES, FUNCIONES.
////FECHA:  07/12/09
////REVISION: 0
////AUTOR:  CONTROLARG
////////////////////////////////////////////////////////////////////////////////

//Defines
#define CantEsclavos 3

enum MBMaster_Stauts
{
    standBy = 0,
    sendCMD,
    waitResp,
    parseResp,
    nextStatus
}MBMasterStatus;

//ESTRUCTURAS
typedef struct str_Slave
{
    int     slaveAdd;   //esclavo numero
    int16   startAdd;
    int16   cantReg ;
    int     failCount;
    int     maxFail;
    byte    status;
    byte    prevStatus;
    int16   *slaveRegs[5];
    int16   *slaveFP[5];
    int16   *slaveEst[5];
};

struct str_Slave SlaveList[CantEsclavos];
int8   slaveStatus[CantEsclavos]; //guardo estado de cada esclavo (alarma aparecida, reconocida, etc)


//Constantes comando trama modbus
int const   ModbusAdd=0;               //posicion de la direccion en la trama modbus
int const   ModbusCmd=1;               //posicion del comando en la trama modbus
int const   ModbusIni=2;               //posicion del primer registro solicitado en trama modbus 2 bytes
int const   ModbusCant=4;              //posicion de la cantidad de registros en la trama modbus 2 bytes
int const   ModbusBCount=2;            //posicion de la cantidad de bytes en la respuesta modbus
int const   lenbuff=40;
int const   kIntentos=3;
int const   kTimeOut=2;                //constante de tiempo para timeout, multiplo de timer 0
int const   kPolTime=3;

char  Txbuff[lenbuff];                   // Buffer
char  Rxbuff[lenbuff];                   // Buffer
int   rxPuntero=0x00;                    // Índice: siguiente char en cbuff
char  rcvchar=0x00;                      // último caracter recibido
int   txlen=0;
int   txpoint=0;
int   actSlave=0;                       //esclavo actual encuestando
int8  CRC8=0; 
int8  ModbusAddress=0;                 //direccion del esclavo
int8  nroIntentos=kIntentos;           //nro intentos antes de falla
int8  rTimeOut=kTimeOut;               //tiempo de espera de respuesta
int8  rPolTime=kPolTime;               //tiempo entre encuestas
int16 CHECKSUM;

int1  flagcommand=0;                    // Flag para indicar comando disponible
int1  fParami =0;                       // Indica que el comando es par este esclavo
int1  fFinTrama=0;                      // Paso el tiempo de espera para considerar que termino la trama 3 bit time para RTU 
int1  fTXModbus=0;                      // flag ecuestar esclavo
int1  fRxPend=0;                        // espera respuesta del esclavo
int1  fTimeOUT=0;                       // falla time out

int16 rSlave0[2];
int16 rSlave1[2];
int16 rSlave2[2];

//FUNCIONES
char CRC8 (char Dato);
void ModBusRX (char data);
void ParseMDBSlave (struct str_Slave* Slave);
void ModBusRX (void);
void ModBus_Exep(int SlvAdd,int ExepNro);
void Modbus_RespOK(int SlvAdd, int Cmd);
void inicbuffTx(void);                   // Borra buffer
void inicbuffRx(void);                   // Borra buffer
