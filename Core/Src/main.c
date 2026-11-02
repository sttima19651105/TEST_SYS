/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
USART_HandleTypeDef husart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_ADC3_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI3_Init(void);
static void MX_USART2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */
/* ���������  ����������  ��������  PWR control*/
void    COMMAND_LINE_parse(const char *data_recieve);

/* Precise delay for 100MHz CPU clock */
void delay_us(uint32_t us);
/* Send 4 bits to OLED considering the mirrored PCB routing */
void OLED_Nibble(uint8_t n);
/* Send 1 byte (Command or Data) over 4-bit interface */
void OLED_Write(uint8_t data, uint8_t is_data);
/* Initialize OLED (WS0010 / RS0010 controller) */
void OLED_Init(void);
void OLED_Print(char* str,int i_long);
void OLED_GoToLine(uint8_t line, uint8_t column);

void reset_all_LED(void);

void Start_PWR_neg6V(void); //  ���������  �������   -6V 
void Start_PWR_50V(void); //  ���������  �������   50V
void Start_PWR(void); //  ���������  ������� 
uint8_t init_PWR(void);  //  ��������� ������� �������  response  0 - all is OK
                     //                                      1 - -6V  problem   error
                     //                                      2 - 50V problem    error
										 //                                      4 -  8V  problem    warning 

uint16_t check_STATIC_CURR(void); // ��������� �������� ��������  ����������� ����� 
														 //                      response  0 0 - all is OK
                     //                                        00000000 00000001 - curr stat MOD  >  max
                     //                                        00000000 00000010 - curr stat VP11 >  max
                     //                                        00000000 00000100 - curr stat VP12 >  max
                     //                                        00000000 00001000 - curr stat VP13 >  max
                     //                                        00000000 00010000 - curr stat VP21 >  max
                     //                                        00000000 00100000 - curr stat VP22 >  max
                     //                                        00000000 01000000 - curr stat VP23 >  max
                     //                                        00000001 00000000 - curr stat MOD  <  min (no connect)
                     //                                        00000010 00000000 - curr stat VP11 <  min
                     //                                        00000100 00000000 - curr stat VP12 <  min
                     //                                        00001000 00000000 - curr stat VP13 <  min
                     //                                        00010000 00000000 - curr stat VP21 <  min
                     //                                        00100000 00000000 - curr stat VP22 <  min
                     //                                        01000000 00000000 - curr stat VP23 <  min
										 
uint16_t check_DYNAM_CURR(void); // ��������� �������� ��������  ������������  ����� 
														 //                      response  0 0 - all is OK
                     //                                        00000000 00000001 - curr stat MOD  >  max
                     //                                        00000000 00000010 - curr stat VP11 >  max
                     //                                        00000000 00000100 - curr stat VP12 >  max
                     //                                        00000000 00001000 - curr stat VP13 >  max
                     //                                        00000000 00010000 - curr stat VP21 >  max
                     //                                        00000000 00100000 - curr stat VP22 >  max
                     //                                        00000000 01000000 - curr stat VP23 >  max
                     //                                        00000001 00000000 - curr stat MOD  <  min (no connect)
                     //                                        00000010 00000000 - curr stat VP11 <  min
                     //                                        00000100 00000000 - curr stat VP12 <  min
                     //                                        00001000 00000000 - curr stat VP13 <  min
                     //                                        00010000 00000000 - curr stat VP21 <  min
                     //                                        00100000 00000000 - curr stat VP22 <  min
                     //                                        01000000 00000000 - curr stat VP23 <  min
										 
/* DAC function prototypes  ���������� �������� �������� ---------------------------------------------------------*/

static void SET_GATE_LEV(uint8_t NB_DAC,uint8_t NVAL_DAC); //  ������  �� ������ NB_DAC  ������� �������� NVAL_DAC  set Level Volt Gate  DAC number NB_DAC
	
static void SET_GATEs_LEV(uint8_t *Gate_VAL);	 // ������  �� ��� �������� �������� �� ������� Gate_VAL
	

char strCMD[4];

 ///  msr Channals var ///
 
uint16_t adc1Data[ADC1_CHANELS_NUM];  // 
uint16_t adc2Data[ADC2_CHANELS_NUM];
uint16_t adc3Data[ADC3_CHANELS_NUM];

float adc1CLB[ADC1_CHANELS_NUM] = {	1.0, 1.0,1.0,1.0,1.0,1.0,1.0,1.0};
float adc2CLB[ADC2_CHANELS_NUM] = {	3.140625, 3.0,2.871428,2.871428,2.95588,3.140625,3.140625,1.0};;
float adc3CLB[ADC3_CHANELS_NUM] = {	1.0,1.0,1.0,40.32258,40.650406,39.370078,1.0,1.0};

float adc1Voltage[ADC1_CHANELS_NUM];
float adc2Voltage[ADC2_CHANELS_NUM];
float adc3Voltage[ADC3_CHANELS_NUM];

volatile  uint8_t ADC1_Data_Ready =0;
volatile  uint8_t ADC1_Read_cnt =0;


volatile  uint8_t ADC2_Data_Ready =0;
volatile  uint8_t ADC3_Data_Ready =0;

int ind_MODE_1str = 0;
int ind_MODE_2str = 2;
int ind_MODE_3str = 4;
int ind_MODE_4str = 5;



int i_ADC; 

uint16_t count_ADC_interval = 100; ///  10ms 

// simulation VIDEO_2 signal var


volatile  uint8_t curent_NB_bits;
volatile  uint8_t CountBITs ;
volatile  uint8_t CountWORLDs ;
volatile  int  OutDATA_int[8];
volatile  uint16_t  OutDATA[8]= {0x5500,0x18,0,8,0,0,0,0xAA};
volatile  uint8_t PIN_STATUS;

volatile  uint8_t video2_count; 

	        int var_CNT_base;
volatile   uint8_t  En_VIDEO2_Flag = 0;

volatile  uint8_t  RESPOND_NMEA_TRANS[6][SIZE_BUFF_NMEA];

volatile uint16_t  BUFF_ENTRY_POS[6];

// commands control  var


volatile  uint8_t COMMAND_BUFF_NMEA[SIZE_BUFF_NMEA];
volatile  uint8_t   Command_TEST=0;
volatile int COMMAND_READY_Flag;

volatile int COUNT_time_test;
int COMMAND_BUFF_Index;

int i_dig = 0;//  nb of a number of a digit 
int i_CH_NB = 4;//  nb of a number of a digit 
int  nb_PIN; // nd  PORTs PIN
GPIO_PinState  	port_bit_set;
int i_dig_DAC = 0;//  nb of a digit

char NB_DAC[3] = {};
int i_NB_DAC;



/* DAC variables ---------------------------------------------------------*/
float V_zatvf[8] = {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
int V_zatv[8];


int V_zatv_MOD = 0;  //  Lev V  lach MOD
uint8_t V_zatv_VP11 = 0;  //  Lev V  lach MOD
uint8_t V_zatv_VP12 = 0;  //  Lev V  lach MOD
uint8_t V_zatv_VP13 = 0;  //  Lev V  lach MOD
uint8_t V_zatv_VP21 = 0;  //  Lev V  lach MOD
uint8_t V_zatv_VP22 = 0;  //  Lev V  lach MOD
uint8_t V_zatv_VP23 = 0;  //  Lev V  lach MOD
int Led_i = 0;

int NEXT_STEP_flag_11 = 0;   //  
int NEXT_STEP_flag_12 = 0;   //  

int NEXT_STEP_flag_21 = 0;   //  

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_SPI3_Init();
  MX_USART2_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
	/*  Start process  msr  analog chanals  */
	

	
  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)adc1Data,ADC1_CHANELS_NUM);
  HAL_ADC_Start_DMA(&hadc2,(uint32_t*)adc2Data,ADC2_CHANELS_NUM);
  HAL_ADC_Start_DMA(&hadc3,(uint32_t*)adc3Data,ADC3_CHANELS_NUM);
	
	/*  Start process  simulations  VIDEO -signal  */
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_Base_Start_IT(&htim1);
	
  HAL_TIM_OC_Start(&htim1,1);
  HAL_TIM_OC_Start_IT(&htim1,1);
	
 	 __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
	
 	HAL_GPIO_WritePin(GPIOD, TEST_STROBE_Pin, GPIO_PIN_SET);    // STROBE_Pin - out "1"
 	HAL_GPIO_WritePin(GPIOD, TEST_VIDEO2_Pin, GPIO_PIN_RESET); // VIDEO2_Pin  - out "0")
	En_VIDEO2_Flag = 1;	
	
	OLED_Init();
	reset_all_LED();
		
	OLED_GoToLine(1,5);
	HAL_Delay(5);

//	OLED_Print("Hello",5);
//	HAL_Delay(5);

	NEXT_STEP_flag_11 = 0;
	NEXT_STEP_flag_12 = 0;
	COMMAND_READY_Flag = 0;
	//Step  1:   Output  string   $RS TXD READY1*, $RS TXD READY6*
int i_ind;
//				for (i_ind = 0; i_ind<20; i_ind++)
	while( !(NEXT_STEP_flag_11)) // && !(NEXT_STEP_flag_12))
				{
					
					if( COMMAND_READY_Flag ) 
				{
					COMMAND_LINE_parse((const char*)COMMAND_BUFF_NMEA);// parsing command line
				}
				
				
				
			if( (huart1.RxState) == HAL_UART_STATE_READY)
				{

					HAL_UART_Receive_IT(&huart1, (uint8_t *)COMMAND_BUFF_NMEA,1);
				
					}
				}
				
					
					BUFF_ENTRY_POS[0] = sprintf((char*)RESPOND_NMEA_TRANS[0],"$RS TXD READY6*\r\n"); // USART 6
					BUFF_ENTRY_POS[1] = sprintf((char*)RESPOND_NMEA_TRANS[1],"$RS TXD READY1*\r\n"); // USART 1
//					COMMAND_LINE_parse((const char*)COMMAND_BUFF_NMEA);// parsing command line
						
			
					HAL_UART_Transmit_IT(&huart6,(const uint8_t *) RESPOND_NMEA_TRANS[0], BUFF_ENTRY_POS[0]); //  
					HAL_UART_Transmit_IT(&huart1,(const uint8_t *) RESPOND_NMEA_TRANS[1], BUFF_ENTRY_POS[1]);
					HAL_Delay(500);
					HAL_Delay(500);
					HAL_Delay(500);
					HAL_Delay(500);
					HAL_Delay(500);
					HAL_Delay(500);
					
//				}
//	while(1)
	
//	{
				
				if( COMMAND_READY_Flag ) 
				{
					COMMAND_LINE_parse((const char*)COMMAND_BUFF_NMEA);// parsing command line
				}
			if( (huart1.RxState) == HAL_UART_STATE_READY)
				{

					HAL_UART_Receive_IT(&huart1, (uint8_t *)COMMAND_BUFF_NMEA,1);
				
					}
				}
				
					if(NEXT_STEP_flag_11 == 1)
					{
							BUFF_ENTRY_POS[1] = sprintf((char*)RESPOND_NMEA_TRANS[1],"$RS RXD READY1*\r\n"); // USART 1
							HAL_UART_Transmit_IT(&huart1,(const uint8_t *) RESPOND_NMEA_TRANS[1], BUFF_ENTRY_POS[1]);
						  
					}
					if(NEXT_STEP_flag_21 == 1)
					{
							BUFF_ENTRY_POS[0] = sprintf((char*)RESPOND_NMEA_TRANS[0],"$RS TXD READY1*\r\n"); // USART 1
							HAL_UART_Transmit_IT(&huart1,(const uint8_t *) RESPOND_NMEA_TRANS[0], BUFF_ENTRY_POS[0]);
						  
					}
					
					
					while( !(NEXT_STEP_flag_21)) 
					{
						
				if( COMMAND_READY_Flag ) 
				{
					COMMAND_LINE_parse((const char*)COMMAND_BUFF_NMEA);// parsing command line
				}
			if( (huart1.RxState) == HAL_UART_STATE_READY)
				{

					HAL_UART_Receive_IT(&huart1, (uint8_t *)COMMAND_BUFF_NMEA,1);
				
					}
				}
						
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}




void    COMMAND_LINE_parse(const char *data_recieve)
{

 	    
switch(data_recieve[1] )
              {
								
                case 'P':// Command PORT 
                case 'p':
									
										
										if(data_recieve[6] & 0x1)
											port_bit_set = GPIO_PIN_SET;
										else
											port_bit_set = GPIO_PIN_RESET;
											
								
										nb_PIN = (data_recieve[3] - '0') *10;
										nb_PIN += (data_recieve[4] - '0') ;
													switch(data_recieve[2] ) 
													{
														case 'A':  //   main menu
											
														break ;
														case 'B':  //   main menu
											
														break ;
														case 'C':  //   main menu
											
														break ;
														break ;
														case 'G':  //   main menu
															break ;
														case 'D':  //   main menu
											
													{
//															switch(nb_PIN)
//															{
//																case 8:
//																if(port_bit_set)
//																		{
//																			GPIOG->BSRR = POW_LOW_Pin;
//																		}
//																else
//																		{
//																			GPIOG->BSRR = (uint32_t)POW_LOW_Pin << 16U;
//																		}

//														    break;
//																case 9:
//																				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, port_bit_set);

//														    break;
//																case 10:
//																				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, port_bit_set);

//														    break;
//																case 11:
//																				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, port_bit_set);

//														    break;
//																case 12 :
//																				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, port_bit_set);

//														    break;
//																case 13:
//																	
//																				  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, port_bit_set);

//														    break;
//															}
//																											COMMAND_READY_Flag = 0; // cjvvfnd is reset    

															
															}
														break ;
														case 'E':  //   main menu
											
														break ;
														case 'F':  //   main menu
											
														break ;
													}
													
													
								
								break;
								
	              case 'M':  //   MODE
										switch(data_recieve[2] )
										{
											case '0':  //   main menu
//                    reset = 1;  //перейти в главное меню
												COMMAND_READY_Flag = 0; // cjvvfnd is reset    
                    
											break ;
                
										case '1':
										break ;
										}
										
                 break ;
										
										
	              case 'D':  //   New  DACs  val comand
	              case 'd':  //   
									if((data_recieve[2] == 'A')&&(data_recieve[3] == 'C')){
//										data_recieve[0] = data_recieve[1] = data_recieve[2] = data_recieve[3] = '_';

										sscanf(data_recieve,"%s %f %f %f %f %f %f %f %f",strCMD,&V_zatvf[0],&V_zatvf[1],&V_zatvf[2],&V_zatvf[3],&V_zatvf[4],&V_zatvf[5],&V_zatvf[6],&V_zatvf[7]);
										
												for(i_NB_DAC = 0; i_NB_DAC <8; i_NB_DAC++)
											{
//												
														V_zatv[i_NB_DAC] =(int)( (V_zatvf[i_NB_DAC]*256)/5); 
															SET_GATE_LEV((uint8_t)i_NB_DAC,V_zatv[i_NB_DAC]);
 
//														i_dig++;
											
//										i_CH_NB = i_CH_NB + i_dig +1;
//										i_dig = 0;
//										
//										V_zatv[i_NB_DAC] = atoi((const char*)NB_DAC);
										COMMAND_READY_Flag = 0;	
										
									}
								}
		                 break ;
	              case 'I':  //   New  indscator string  val comand
	              case 'i':  //   
									if((data_recieve[2] == 'n')&&(data_recieve[3] == 'd')){

										sscanf(data_recieve,"%s %d %d %d %d",strCMD,&ind_MODE_1str,&ind_MODE_2str,&ind_MODE_3str,&ind_MODE_4str);
											COMMAND_READY_Flag = 0;	
									
									}
								
		                 break ;
								
	              case 'T':  //   New  DACs  val comand
	              case 't':  //   
									if((data_recieve[2] == 'I')&&(data_recieve[3] == 'M')){

										sscanf(data_recieve,"%s %x %x %x %x %x %x %x %x",strCMD,&OutDATA_int[0],&OutDATA_int[1],&OutDATA_int[2],&OutDATA_int[3],&OutDATA_int[4],&OutDATA_int[5],&OutDATA_int[6],&OutDATA_int[7]);
												for(i_NB_DAC = 0; i_NB_DAC <8; i_NB_DAC++)
												{
														OutDATA[i_NB_DAC] =(uint16_t)OutDATA_int[i_NB_DAC]; 
												}
										
										COMMAND_READY_Flag = 0;	
								}
		                 break ;
								
                case 'R': // $RS RXD TEST1*
                    if(   strstr(data_recieve, "RS RXD TEST1")) 
										{
											COMMAND_READY_Flag = 0;
											NEXT_STEP_flag_11 = 1;
										}
										  
                   break ;
 							

//                     case 'O':
//                       if(data_recieve[3] == 'n')
//                         EnDACinit = 1;
//                       else
//                       if(data_recieve[3] == 'f')
//                         EnDACinit = 0;
//                         
//                      data_recieve[2] = ' ';
//                   break ;
//                   
//                     case 'I':  // read  DIR flash
//                       if(data_recieve[3] == 'R')
//                         DIR_read = 1;
//                                             start = 1;
//                    menu5_on = 0;
//                    current_mode_of_work = 0;
//                    STATUS = 1;

//                      data_recieve[2] = ' ';
//                   break ;
//                   
//                   
//                  }
//                 flg_DAC_corr = 1;
//                break ;
//              case 'M':  //   MODE
//                switch(data_recieve[2] )
//                {
//                  case '0':  //   main menu
////                    reset = 1;  //перейти в главное меню
//                    start = 1;
//                    menu5_on = 0;
//                    current_mode_of_work = 0;
//                    STATUS = 1;
//                    
//                  break ;
//                
//                  case '1':
//                  case '2':
//                  case '3':
//                  case '4':
//                  case '5':
//                  case '6':
//#if  MAGNET  == 0                  
//                  case '7':
//#endif                    
//                    if(current_count_of_marker == 4)
//                    {
//                      menu5_on = 0;
//                      cursor_menu5 = END_CURSOR_MENU5;
//                    }
//                    current_count_of_marker = (data_recieve[2] - '1');
//                    if(current_mode_of_work == 0)
//                      current_mode_of_work = 1;
////                    else
////                      current_mode_of_work = 0;
//                    start = 1;
//                    STATUS = 0;
//                flg_cmd = 1;

//                  break ;
//                }
//                  break ;
//              case 'A':  //   MODE
//                if((current_mode_of_work == 0)||((current_count_of_marker != 3)&&(current_mode_of_work == 1)))
//                {
//                current_average = (data_recieve[2] - '0');
//               
//		switch (current_average)
//		{
//                  case 0:
//                        current_average_int = 1;
//                  break;
//                  case 1:
//                        current_average_int = FRQ_Hz;	// 10Hz/2 2006  Bylo = 5;
//                  break;
//                  case 2:
//                        current_average_int = FRQ_Hz * 10;	// 10Hz/10 2006  Bylo = 25;
//                  break;
//                  case 3:
//                        current_average_int = FRQ_Hz * 60;	// 10Hz/100 2006  Bylo = 250;
//                  break;
//                  default:
//                    current_average_int = 1;
//                    current_average = 0;
//                  break;
//                }
//                start_M5();
//                
//                }  
//                  break ;
//#if   MAGNET != 1       
//              case 'E':  //   MODE
//                  
//                if((current_mode_of_work == 0)||((current_count_of_marker != 3)&&(current_mode_of_work == 1)))
//               {
//                switch(data_recieve[2] )
//                {
//                case '2':
//                  current_Electrometer = 0;
//                  break;
//                case '1':
//                  current_Electrometer = 1;
//                  
//                  break;
//                case 'O':
//                case 'o':
//                  if((data_recieve[3] == 'f') && ( data_recieve[4] == 'f'))
//                  current_Electrometer = 2;
//                  
//                  break;
//                }
//                start_M5();
//               }
//                  break ;
//              case 'F':  //   MODE
//                val_days_file_en = data_recieve[2] - '0';
//                start_M5();
//                  break ;
//               
//#endif                
//              case 's':  //   MODE
//              case 'S':  //   MODE
//                Servis_Status = data_recieve[2] - '0';
//                start_M5();
//              break ;
//              
//              case 'c':  //   MODE
//              case 'C':  //   MODE
//                if((data_recieve[2]== 'Y')||(data_recieve[2]== 'y') ) 
//                current_comp_Y_OFF = data_recieve[2] - '0';
//                start_M5();
//              break ;
//               
//             case 'l':  //   MODE  Level  PWR  control
//             case 'L':  //    MODE  Level  PWR  control
//                      data_recieve[1] = ' ';
//                 
//                
//               if((data_recieve[2] == 'l')||(data_recieve[2] == 'L'))
//               {
//                 data_recieve[2] = ' ';
//                 PWR_LOW = atof(data_recieve);
//                 fl_new_param = 1;
//               }
//                if((data_recieve[2] == 'h')||(data_recieve[2] == 'H'))
//               {
//                 data_recieve[2] = ' ';
//                 PWR_HIGH = atof(data_recieve);
//                 fl_new_param = 1;
//               }
//             break ;

//          
//              case 'R':  //   MODE
//          
//          if(
//             (data_recieve[2] == 'S') &&
//             (data_recieve[3] == 'T') )
//          {
//            if((data_recieve[4] == 'B') )
//              STATUS = 2;
//            else
//              STATUS = 3;
//           start = 1;
//                    menu5_on = 0;
//                    current_mode_of_work = 0;
//         }
//          break;
              
//#endif
//          
//          
//         count_receive = 0; 
//         data_recieve[0]= data_recieve[1] = data_recieve[2] = data_recieve[3] = data_recieve[4] =data_recieve[5] =
//         data_recieve[6] =data_recieve[7] = data_recieve[8] =data_recieve[9] =data_recieve[10] = 
//          ' ';
//        }
//        else
//	count_receive++;

        
        }
			}

	void reset_all_LED(void)
{	
//			N6V_R;
//			P_CURR_R;	
//			C50V_R;
//			Overheating_R;
//			P_KSHN_R;
//			POW_LOW_R;
}		
							
/* 
 * Hardware Mapping (Port G):
 * PG5..PG2 -> D4..D7 (Hardware mirrored: D7->PG2, D6->PG3, D5->PG4, D4->PG5)
 * PG6      -> RW (Read/Write)
 * PG7      -> EN (Enable)
 * PG8      -> RS (Register Select)
 */

/* Precise delay for 100MHz CPU clock */
void delay_us(uint32_t us) {
    uint32_t count = us * 21; 
    while(count--) __NOP();
}

/* Send 4 bits to OLED considering the mirrored PCB routing */
void OLED_Nibble(uint8_t n) {
    // Clear PG2-PG7 (Bits 20-23 in BSRR)
//      GPIOG->BSRR = (0xF0 << 16); 
			GPIOG->BSRR = GPIO_BSRR_BR2;  
			GPIOG->BSRR = GPIO_BSRR_BR3;  
			GPIOG->BSRR = GPIO_BSRR_BR4;  
			GPIOG->BSRR = GPIO_BSRR_BR5;  
	
	
	
    // Software bit-reversal to match mirrored hardware:
    // Bit 0 (D4) -> PG7, Bit 1 (D5) -> PG6, Bit 2 (D6) -> PG5, Bit 3 (D7) -> PG4
    uint32_t mirrored = 0;
    if (n & 0x01) mirrored |= GPIO_BSRR_BS2; // D4 to PG5
    if (n & 0x02) mirrored |= GPIO_BSRR_BS3; // D5 to PG4
    if (n & 0x04) mirrored |= GPIO_BSRR_BS4; // D6 to PG3
    if (n & 0x08) mirrored |= GPIO_BSRR_BS5; // D7 to PG2
    
    GPIOG->BSRR = mirrored;
    
    // Strobe the Enable pin
//    EN_1;
//    delay_us(2); 
//    EN_0;
//    delay_us(2);
}

/* Send 1 byte (Command or Data) over 4-bit interface */
void OLED_Write(uint8_t data, uint8_t is_data) {
//    if(is_data) RS_1; else RS_0;
//    RW_0; 
//    
//    OLED_Nibble(data >> 4);   // High nibble first
//    OLED_Nibble(data & 0x0F); // Low nibble second
//    delay_us(50);             // Wait for OLED internal processing
}

/* Initialize OLED (WS0010 / RS0010 controller) */
void OLED_Init(void) {
    // Enable Port G clock
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    
    // Set PG0, PG2, PG3, PG4-PG7 as Output (01)
    GPIOG->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 |
                      GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6 | GPIO_MODER_MODER7| GPIO_MODER_MODER8);
    GPIOG->MODER |=  (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 |
                      GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0| GPIO_MODER_MODER8_0);

    delay_us(100000); // Power-on stabilization (100ms)

    // Force 4-bit mode initialization sequence
    OLED_Nibble(0x03); delay_us(5000);
    OLED_Nibble(0x02); delay_us(2000); 
	
	    OLED_Write(0xff, 0); 
	    OLED_Write(0xff, 0); 
	    OLED_Write(0xff, 0); 
	    OLED_Write(0xff, 0); 
	    OLED_Write(0xff, 0); 


    // Function Set: 4-bit mode, 2 lines, 5x8 font
    OLED_Write(0x28, 0); 
    // Display Control: Display ON, Cursor OFF, Blink OFF
		
		
    OLED_Write(0x0C, 0); 
    // Clear Display
		
		
		
    OLED_Write(0x01, 0); 
    delay_us(2000);
    // Entry Mode: Auto-increment cursor, No shift
    OLED_Write(0x06, 0); 
}

void OLED_Print(char* str,int i_long) {
    while(i_long--) OLED_Write((uint8_t)*str++, 1);
}		
/**
 * @brief Set the cursor to a specific line and column.
 * @param line: 1 to 4
 * @param column: 0 to 19 (for 20x4) or 0 to 15 (for 16x4)
 */
void OLED_GoToLine(uint8_t line, uint8_t column) {
    uint8_t address;

    switch (line) {
        case 1:
            address = 0x00 + column;
            break;
        case 2:
            address = 0x40 + column;
            break;
        case 3:
            address = 0x14 + column; // Use 0x10 for some 16x4 models
            break;
        case 4:
            address = 0x54 + column; // Use 0x50 for some 16x4 models
            break;
        default:
            address = 0x00;
            break;

    }

    // Command 0x80 sets the DDRAM address
    OLED_Write(0x80 | address, 0);
}


// Процедура  установки значений  ЦАПа с номером NB_DAC    значением  NVAL_DAC  
static void SET_GATE_LEV(uint8_t NB_DAC,uint8_t NVAL_DAC)
{
	uint16_t word_DAC;
	uint8_t  word_DAC_A[2];
	word_DAC = 0;
	word_DAC = (((uint16_t)NB_DAC)<<12)|((((uint16_t)NVAL_DAC)<< 4 )&0x0ff0); //((uint16_t)NB_DAC)<<12)|(
	word_DAC_A[0] =(uint8_t)(word_DAC & 0x00ff);
	word_DAC_A[1] =(uint8_t)((word_DAC & 0xff00)>>8);
	HAL_GPIO_WritePin(GPIOE, CSDAC_REF_Pin|LDAC_REF_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	
  HAL_SPI_Transmit(&hspi3,word_DAC_A,1,1);
//	hspi3.Instance->DR = word_DAC;
//	while(flag_SPI3)
//		flag_SPI3  = (hspi3.Instance->SR & 0x0080)>>7;
	
	//hspi3.Instance->CR1
	
	HAL_GPIO_WritePin(GPIOE, LDAC_REF_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, CSDAC_REF_Pin|LDAC_REF_Pin, GPIO_PIN_SET);


}	
			
		
/* Процедуры  управления  питанием  PWR control*/


void Start_PWR_neg6V(void){//  процедура  запуска   -6V 
	
	
}	
void Start_PWR_50V(void)//  процедура  запуска   50V
{
}	
void Start_PWR(void)//  процедура  запуска 
{
}	
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 8;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ENABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 8;
  hadc2.Init.DMAContinuousRequests = DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 4;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 5;
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_15;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ENABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 6;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_84CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  husart2.Instance = USART2;
  husart2.Init.BaudRate = 115200;
  husart2.Init.WordLength = USART_WORDLENGTH_8B;
  husart2.Init.StopBits = USART_STOPBITS_1;
  husart2.Init.Parity = USART_PARITY_NONE;
  husart2.Init.Mode = USART_MODE_TX_RX;
  husart2.Init.CLKPolarity = USART_POLARITY_LOW;
  husart2.Init.CLKPhase = USART_PHASE_1EDGE;
  husart2.Init.CLKLastBit = USART_LASTBIT_DISABLE;
  if (HAL_USART_Init(&husart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CSDAC_REF_Pin|LDAC_REF_Pin|RESET_OUT_Pin|RST_LEVEL_Pin
                          |Transmission_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, H_VOLT_MOD_DIS_Pin|H_VOLT_VP1_DIS_Pin|H_VOLT_DIS_VP2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, TEST_STROBE_Pin|TEST_VIDEO2_Pin|READY_Pin|FAILURE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : CSDAC_REF_Pin LDAC_REF_Pin H_VOLT_DIS_VP2_Pin RESET_OUT_Pin
                           RST_LEVEL_Pin Transmission_Pin */
  GPIO_InitStruct.Pin = CSDAC_REF_Pin|LDAC_REF_Pin|H_VOLT_DIS_VP2_Pin|RESET_OUT_Pin
                          |RST_LEVEL_Pin|Transmission_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : VIDEO2_Pin */
  GPIO_InitStruct.Pin = VIDEO2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VIDEO2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PF12 Protection_by_KSXN_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_12|Protection_by_KSXN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_C_D_Pin OLED_E_Pin OLED_RW_Pin OLED_D7_Pin
                           OLED_D6_Pin OLED_D5_Pin OLED_D4_Pin POW_50P_ROC_Pin */
  GPIO_InitStruct.Pin = OLED_C_D_Pin|OLED_E_Pin|OLED_RW_Pin|OLED_D7_Pin
                          |OLED_D6_Pin|OLED_D5_Pin|OLED_D4_Pin|POW_50P_ROC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : H_VOLT_MOD_DIS_Pin H_VOLT_VP1_DIS_Pin */
  GPIO_InitStruct.Pin = H_VOLT_MOD_DIS_Pin|H_VOLT_VP1_DIS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : ENABLE_Pin POW_START_Pin RESET_HI_Pin */
  GPIO_InitStruct.Pin = ENABLE_Pin|POW_START_Pin|RESET_HI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : Hand_Start_Tr_Pin */
  GPIO_InitStruct.Pin = Hand_Start_Tr_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Hand_Start_Tr_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 Protection_KSHN_Pin Overheating_Pin Protection_CURR_Pin
                           Contr_50V_Pin Contr_n6V_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_8|Protection_KSHN_Pin|Overheating_Pin|Protection_CURR_Pin
                          |Contr_50V_Pin|Contr_n6V_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : TEST_STROBE_Pin TEST_VIDEO2_Pin READY_Pin FAILURE_Pin */
  GPIO_InitStruct.Pin = TEST_STROBE_Pin|TEST_VIDEO2_Pin|READY_Pin|FAILURE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : WORK_Pin */
  GPIO_InitStruct.Pin = WORK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(WORK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : STROBE_Pin */
  GPIO_InitStruct.Pin = STROBE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(STROBE_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
