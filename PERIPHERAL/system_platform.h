#ifndef __SYSTEM_PLATFORM_H__
#define __SYSTEM_PLATFORM_H__


//#include "main.h"


#define EZ261
//#define EZ261




#ifdef EZ261  /************************  EZ261 *************************/



	#define SCL_GPIO GPIOB
	#define SCL_PIN GPIO_Pin_6
	#define SDA_GPIO GPIOB
	#define SDA_PIN GPIO_Pin_7
		
	#define SENSE_PWEN_GPIO GPIOB
	#define SENSE_PWEN_PIN GPIO_Pin_14

	#define M25P80_PWEN_GPIO GPIOB
	#define M25P80_PWEN_PIN GPIO_Pin_15

	#define CCS811_PWEN_GPIO GPIOB
	#define CCS811_PWEN_PIN GPIO_Pin_10

	#define WIFI_PWEN_GPIO GPIOB
	#define WIFI_PWEN_PIN GPIO_Pin_2

	#define LED_GREEN_GPIO GPIOB
	#define LED_GREEN_PIN  GPIO_Pin_1

	#define LED_RED_GPIO GPIOB
	#define LED_RED_PIN  GPIO_Pin_11

	#define WIFI_NETWORK_CONNECT_GPIO GPIOA
	#define WIFI_NETWORK_CONNECT_PIN  GPIO_Pin_8
	
	#define M25P80_NSS_GPIO GPIOB
	#define M25P80_NSS_PIN  GPIO_Pin_12
	
	
//	#define SPIx                             SPI1
	#define SPIx_CLK_ENABLE()                RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE)
	#define SPIx_SCK_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

	#define SPIx_MISO_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
	#define SPIx_MOSI_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

//	#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
//	#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

	/* Definition for SPIx Pins */
	#define SPIx_SCK_PIN                     GPIO_Pin_5
	#define SPIx_SCK_GPIO_PORT               GPIOA
//	#define SPIx_SCK_AF                      GPIO_AF0_SPI1

	#define SPIx_MISO_PIN                    GPIO_Pin_6
	#define SPIx_MISO_GPIO_PORT              GPIOA
//	#define SPIx_MISO_AF                     GPIO_AF0_SPI1

	#define SPIx_MOSI_PIN                    GPIO_Pin_7
	#define SPIx_MOSI_GPIO_PORT              GPIOA
//	#define SPIx_MOSI_AF                     GPIO_AF0_SPI1


	#define RESET_IOPORT                                GPIOC
	#define RESET_PIN                                   GPIO_Pin_13

	#define NSS_IOPORT                                  GPIOA
	#define NSS_PIN                                     GPIO_Pin_4  

	
	#define DIO0_IOPORT                                 GPIOB
	#define DIO0_PIN                                    GPIO_Pin_9

	#define DIO1_IOPORT                                 GPIOB
	#define DIO1_PIN                                    GPIO_Pin_8

	#define DIO2_IOPORT                                 GPIOB
	#define DIO2_PIN                                    GPIO_Pin_5

	#define DIO3_IOPORT                                 GPIOB
	#define DIO3_PIN                                    GPIO_Pin_4

	#define DIO4_IOPORT                                 GPIOB
	#define DIO4_PIN                                    GPIO_Pin_3

	#define DIO5_IOPORT                                 GPIOA
	#define DIO5_PIN                                    GPIO_Pin_15

//	#define RXTX_IOPORT                                 
//	#define RXTX_PIN                                    FEM_CTX_PIN

		
#endif

#ifdef EZ265		/************************  EZ265 *************************/

	#define SCL_GPIO GPIOB
	#define SCL_PIN GPIO_Pin_6
	#define SDA_GPIO GPIOB
	#define SDA_PIN GPIO_Pin_7
		
	#define SENSE_PWEN_GPIO GPIOE
	#define SENSE_PWEN_PIN GPIO_Pin_12

	#define M25P80_PWEN_GPIO GPIOB
	#define M25P80_PWEN_PIN GPIO_Pin_2

	#define CCS811_PWEN_GPIO GPIOA
	#define CCS811_PWEN_PIN GPIO_Pin_4

	#define WIFI_PWEN_GPIO GPIOH
	#define WIFI_PWEN_PIN GPIO_Pin_9

	#define WIFI_NETWORK_CONNECT_GPIO GPIOH
	#define WIFI_NETWORK_CONNECT_PIN  GPIO_Pin_10

	#define M25P80_NSS_GPIO GPIOD
	#define M25P80_NSS_PIN  GPIO_Pin_13
	
	
//	#define SPIx                             SPI1
	#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
	#define SPIx_SCK_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
	#define SPIx_MISO_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)
	#define SPIx_MOSI_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

	#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
	#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

	#define SPIx_SCK_PIN                     GPIO_Pin_5
	#define SPIx_SCK_GPIO_PORT               GPIOA
	#define SPIx_SCK_AF                      GPIO_AF0_SPI1

	#define SPIx_MISO_PIN                    GPIO_Pin_14
	#define SPIx_MISO_GPIO_PORT              GPIOE
	#define SPIx_MISO_AF                     GPIO_AF2_SPI1

	#define SPIx_MOSI_PIN                    GPIO_Pin_15
	#define SPIx_MOSI_GPIO_PORT              GPIOE
	#define SPIx_MOSI_AF                     GPIO_AF2_SPI1
	
	

	#define RESET_IOPORT                                GPIOD
	#define RESET_PIN                                   GPIO_Pin_15

	#define NSS_IOPORT                                  GPIOD
	#define NSS_PIN                                     GPIO_Pin_14     

	#define DIO0_IOPORT                                 GPIOD
	#define DIO0_PIN                                    GPIO_Pin_11

	#define DIO1_IOPORT                                 GPIOD
	#define DIO1_PIN                                    GPIO_Pin_10

	#define DIO2_IOPORT                                 GPIOE
	#define DIO2_PIN                                    GPIO_Pin_9

	#define DIO3_IOPORT                                 GPIOE
	#define DIO3_PIN                                    GPIO_Pin_10

	#define DIO4_IOPORT                                 GPIOE
	#define DIO4_PIN                                    GPIO_Pin_7

	#define DIO5_IOPORT                                 GPIOE
	#define DIO5_PIN                                    GPIO_Pin_8

	#define RXTX_IOPORT                                 
	#define RXTX_PIN                                    FEM_CTX_PIN
	
#endif



#define SENSE_PWEN_OFF    
#define SENSE_PWEN_ON   


#define M25P80_PWEN_OFF    
#define M25P80_PWEN_ON     

#define LORA_PWEN_OFF    
#define LORA_PWEN_ON    


#define VOC_PWEN_OFF    
#define VOC_PWEN_ON    


#define WIFI_PWEN_OFF   
#define WIFI_PWEN_ON    




#endif // ___SYSTEM_PLATFORM_H___
