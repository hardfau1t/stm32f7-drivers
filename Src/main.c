#include "stm32f767xx.h" 
#include "string.h" 
#include "stm32f767xx_gpio.h"
#include "stm32f767xx_spi.h"

void delay(){
	for(int i = 0; i<300000; i++){
		__asm__("nop");
	}
}

/* 
 * PB4 MISO AF6
 * PB5 MOSI AF6
 * PB3 SCK AF6
 * PA4 NSS  AF6
 */

void gpio_init(){
    GPIO_Handle_t spi_sck, spi_mosi, spi_miso, spi_nss;
    spi_sck.GPIOx   = GPIOE;
    spi_miso.GPIOx  = GPIOE;
    spi_mosi.GPIOx  = GPIOE;
    spi_nss.GPIOx   = GPIOE;

    spi_sck.GPIOx->MODER = GPIO_OUTPUT;
    spi_miso.GPIOx->MODER = GPIO_OUTPUT;
    spi_mosi.GPIOx->MODER = GPIO_OUTPUT;
    spi_nss.GPIOx->MODER = GPIO_OUTPUT;
    
    spi_sck.PinConfig.GPIO_PinMode = GPIO_ALTERNATE_FUNCTION;
    spi_miso.PinConfig.GPIO_PinMode = GPIO_ALTERNATE_FUNCTION;
    spi_mosi.PinConfig.GPIO_PinMode = GPIO_ALTERNATE_FUNCTION;
    spi_nss.PinConfig.GPIO_PinMode = GPIO_ALTERNATE_FUNCTION;

    spi_sck.PinConfig.GPIO_PinAltMode   = AF5;
    spi_miso.PinConfig.GPIO_PinAltMode  = AF5;
    spi_mosi.PinConfig.GPIO_PinAltMode  = AF5;
    spi_nss.PinConfig.GPIO_PinAltMode   = AF5;

    spi_sck.PinConfig.GPIO_PinNumber    = 2;
    spi_miso.PinConfig.GPIO_PinNumber   = 5;
    spi_mosi.PinConfig.GPIO_PinNumber   = 6;
    spi_nss.PinConfig.GPIO_PinNumber    = 4;

    spi_sck.PinConfig.GPIO_OPType = GPIO_PUSHPULL;
    spi_miso.PinConfig.GPIO_OPType = GPIO_PUSHPULL;
    spi_mosi.PinConfig.GPIO_OPType = GPIO_PUSHPULL;
    spi_nss.PinConfig.GPIO_OPType = GPIO_PUSHPULL;

    spi_sck.PinConfig.GPIO_PinSpeed =   GPIO_VERY_HIGH_SPEED;
    spi_miso.PinConfig.GPIO_PinSpeed =  GPIO_VERY_HIGH_SPEED;
    spi_mosi.PinConfig.GPIO_PinSpeed =  GPIO_VERY_HIGH_SPEED;
    spi_nss.PinConfig.GPIO_PinSpeed =   GPIO_VERY_HIGH_SPEED;

    RCC_GPIO_ClkCtrl(RCC_GPIOE, SET);
    GPIO_Init(&spi_sck);
    GPIO_Init(&spi_mosi);
    GPIO_Init(&spi_miso);
    GPIO_Init(&spi_nss);
}

void spi_init(){
    RCC_SPI_ClkCtrl(RCC_SPI4, SET);
    SPI_Handle_t handler;
    handler.handle = SPI4;
    handler.config.bus_config = SPI_Bus_Full_duplex;
    handler.config.device_mode = SPI_Mode_Master;
    handler.config.speed = SPI_pclk_div_by_2;
    handler.config.cpha = SPI_CPHA_First;
    handler.config.cpol = SPI_CPOL_LOW;
    handler.config.ds = 8;
    handler.config.ssm = SPI_SSM_RESET;
    SPI_Init(&handler);
}

int main(void)
{
    gpio_init();

    spi_init();
    char buffer[] = "Hello hi";
    uint32_t size = strlen(buffer);
    SPI_Control(SPI3, SET);

	while(1){
		SPI_SendData(SPI3, (uint8_t *)buffer, size);
		while( SPI_Status(SPI3, SPI_Status_BSY) );
		SPI_Control(SPI3, RESET);
		for(int i=0; i<1000000; i++);
        __asm__("nop");
	}
}