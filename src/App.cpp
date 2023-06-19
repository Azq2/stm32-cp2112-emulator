#include "App.h"

#include <algorithm>
#include <climits>

#include "Gpio.h"
#include "utils.h"

#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/dwt.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

void App::initHw() {
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
	
	Gpio::setAllAnalog();
	
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	
	AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON;
	
	delay_init();
	
	// UART
	gpio_set_mode(GPIO_BANK_USART3_TX, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);
	gpio_set_mode(GPIO_BANK_USART3_RX, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART3_RX);
	
	// UART for debug
	rcc_periph_clock_enable(RCC_USART3);
	uart_simple_setup(USART3, 115200, true);
	
	// PC13 - status led
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_OPENDRAIN, GPIO13);
	gpio_set_value(GPIOC, GPIO13, true);
}

int App::run() {
	initHw();
	
	printf("Init usb...\r\n");
	m_usb.init();
	
	//printf("Polling...\r\n");
	while (true) {
		m_usb.poll();
	}
	
	return 0;
}
