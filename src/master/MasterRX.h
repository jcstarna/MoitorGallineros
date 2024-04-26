////////////////////////////////////////////////////////////////////////////////
////                         RFCOMM
//                            VARIABLES
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

//ENTRADAS DIGITALES
#define pLed1       PIN_E3            //MCLR
#Define pLed2       PIN_A0            //ENTRADA ANALOGICA
#Define pLed3       PIN_A1            //ENTRADA ANALOGICA
#Define pLed4       PIN_A2            //ENTRADA ANALOGICA
#Define pReset      PIN_B0

//SALIDAS A RELE
#Define pR7       PIN_A3            //ENTRADA ANALOGICA
#Define pR6       PIN_A5            //SALIDA DIGITAL
#Define pR4       PIN_A6            //SALIDA DIGITAL
#Define pR5       PIN_A7            //SALIDA DIGITAL
#Define pR3       PIN_C0
#Define pR2       PIN_C1
#Define pR1       PIN_C2
#Define pBuzzer   PIN_C3

//LED TEST
#Define pLED        PIN_C4          //ESTADO COMUNICACION

//COMM
#Define pEN_RX      PIN_C5
//C6
//C7

//PUERTO B
//#Define pB0         PIN_B0
//#Define pB1         PIN_B1
//#Define pB2         PIN_B2
//#Define pB3         PIN_B3
//#Define pB4         PIN_B4
//#Define pB5         PIN_B5
//#Define pB6         PIN_B6
//#Define pB7         PIN_B7

int const k100ms=100;
int const k10ms=10;
int const k05s=50;//50;
int const k06s=60;



//CONSTANTES
int const kInmuni=10;

//VARIABLES
//int f10ms=k10ms;
//int r100ms=k100ms;
int16 r500ms=k05s;
int16 r600ms=k06s;
int rInmuni=kInmuni;

//Tiempos
int rTmr10ms=0;//contador para llegar a 10ms
int rTmr05s=0; //contador para llegar a 500ms
int rTiempos=0;//registros con bits de tiempo
int rBTAl=1;   //Base de tiempo para alarma  
int rAuxBTAl=0;//auxiliar
int rOscilador=0;

int rSpTmrBus=5;  //tiempo de inact del bus, depende del baud rate
int rMuxDigito=0; //digito a cargar

INT8 rTeclasCrudas=0;
int8 rDmil=0;
int8 rUmil=0;
int8 rCent=0;
int8 rDec =0;
int8 rUni=0;
int8 rDispFun=0;  //funcion del display
int8 rEstAlarma=0;

//FLAGS
int1  fIni=0;
int1  fContar=0; ///habilita contar tiempo
int1  fpIni=0;    //para detectar flanco positivo
int1  fActDisp=0;
int1  fReset=0;
int1  fCampana=0;


