/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include "embARC.h"
#include "embARC_debug.h"
#include "dev_gpio.h"

#define LED_TOGGLE_MASK		BOARD_LED_MASK

#define GPIO4B2_0_OFFSET	0
#define GPIO4B2_1_OFFSET	1
#define GPIO4B2_2_OFFSET	2

DEV_GPIO_PTR gpio_4b2;
//hi

ADC_DEFINE(adc_test, ADC_INT_NUM, ADC_CRTL_BASE, NULL);


void arduino_pin_init(void)
{
	io_arduino_config(ARDUINO_PIN_0, ARDUINO_GPIO, IO_PINMUX_ENABLE); //gpio_4b2[0];
	io_arduino_config(ARDUINO_PIN_1, ARDUINO_GPIO, IO_PINMUX_ENABLE); //gpio_4b2[1];
	io_arduino_config(ARDUINO_PIN_2, ARDUINO_GPIO, IO_PINMUX_ENABLE); //gpio_4b2[2];
	io_arduino_config(ARDUINO_PIN_3, ARDUINO_PWM, IO_PINMUX_ENABLE);//pwm timer ch0
	io_arduino_config(ARDUINO_PIN_5, ARDUINO_PWM, IO_PINMUX_ENABLE);//pwm timer ch1
	io_arduino_config(ARDUINO_PIN_6, ARDUINO_PWM, IO_PINMUX_ENABLE);//pwm timer ch2
	io_arduino_config(ARDUINO_PIN_9, ARDUINO_PWM, IO_PINMUX_ENABLE);//pwm timer ch3
//	io_arduino_config(ARDUINO_PIN_AD0, ARDUINO_ADC, IO_PINMUX_ENABLE);//AD0
}

/**
 * \brief	Test hardware board without any peripheral
 */
int main(void)
{
	uint16_t led_toggle_val = LED_TOGGLE_MASK;
/*
	while (1) {
		led_write(led_toggle_val, BOARD_LED_MASK);
		led_toggle_val = ~led_toggle_val;
		board_delay_ms(500, 1);
	}*/
	uint16_t adc_value[6];
	uint32_t value_read = 1;
	//uint32_t buzzer_set = 0;
	//uint32_t LED_set = 0;
	//uint32_t value_set  = 0;
	int32_t init_speed  = 100; //dc
	arduino_pin_init();

	DEV_PWM_TIMER_PTR pwm_timer_test = pwm_timer_get_dev(DW_PWM_TIMER_0_ID);
	pwm_timer_test->pwm_timer_open();

	///////////////////////////////////////////////////////////////
	//smic_adc_open(adc_test); //adc
	///////////////////////////////////////////////////////////////
	gpio_4b2 = gpio_get_dev(DFSS_GPIO_4B2_ID); // get GPIO_4B2 handler

	gpio_4b2->gpio_open(1 << GPIO4B2_0_OFFSET | 1 << GPIO4B2_1_OFFSET | 1 << GPIO4B2_2_OFFSET); // open
	gpio_4b2->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << GPIO4B2_0_OFFSET | 1 << GPIO4B2_2_OFFSET));
	gpio_4b2->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << GPIO4B2_1_OFFSET)); // change GPIO4B2_1_OFFSET dir to input.
	gpio_4b2->gpio_write(0 << GPIO4B2_0_OFFSET, 1 << GPIO4B2_0_OFFSET); // GPIO4B2_0_OFFSET set low output.
	gpio_4b2->gpio_write(0 << GPIO4B2_2_OFFSET, 1 << GPIO4B2_2_OFFSET); // GPIO4B2_2_OFFSET set low output.
	gpio_4b2->gpio_read(&value_read, 1 << GPIO4B2_1_OFFSET);
	value_read = value_read >> GPIO4B2_1_OFFSET;
	board_delay_ms(10,1);

	//connect ARDUINO_PIN_0 and ARDUINO_PIN_1;
	while (1)
	{//	value_set  = ! value_set;

	//	gpio_4b2->gpio_write(value_set << GPIO4B2_0_OFFSET, 4 << GPIO4B2_0_OFFSET);
	//	board_delay_ms(10,1);
		gpio_4b2->gpio_read(&value_read, 1 << GPIO4B2_1_OFFSET);
		value_read = value_read >> GPIO4B2_1_OFFSET;
		//motors will start running at full speed.//////////////////////
		if(value_read==1)//not drunk, car will start.
			{
				pwm_timer_test->pwm_timer_write(0, DEV_PWM_TIMER_MODE_PWM, 100000, init_speed);
				pwm_timer_test->pwm_timer_write(1, DEV_PWM_TIMER_MODE_PWM, 100000, 0);
				pwm_timer_test->pwm_timer_write(2, DEV_PWM_TIMER_MODE_PWM, 100000, init_speed);
				pwm_timer_test->pwm_timer_write(3, DEV_PWM_TIMER_MODE_PWM, 100000, 0);
			}
		if(value_read==0)//drunk
		{
			while(init_speed>=0)
			{
				gpio_4b2->gpio_write(1 << GPIO4B2_0_OFFSET, 1 << GPIO4B2_0_OFFSET); // GPIO4B2_0_OFFSET set high output.
				gpio_4b2->gpio_write(1 << GPIO4B2_2_OFFSET, 1 << GPIO4B2_2_OFFSET); // GPIO4B2_2_OFFSET set high output.
				pwm_timer_test->pwm_timer_write(0, DEV_PWM_TIMER_MODE_PWM, 100000, init_speed);
				pwm_timer_test->pwm_timer_write(1, DEV_PWM_TIMER_MODE_PWM, 100000, 0);
				pwm_timer_test->pwm_timer_write(2, DEV_PWM_TIMER_MODE_PWM, 100000, init_speed);
				pwm_timer_test->pwm_timer_write(3, DEV_PWM_TIMER_MODE_PWM, 100000, 0);
				init_speed=init_speed-5;
				EMBARC_PRINTF("init_speed: %d \r\n", init_speed);
				board_delay_ms(1000,1);
				EMBARC_PRINTF("\x1b[2k\x1b\x45");// CRT control cmd.
				EMBARC_PRINTF("Have had alcoholic drinks! Shouldn't be driving. %d \r\n");
				board_delay_ms(1000,1);
				EMBARC_PRINTF("\x1b[2k\x1b\x45");// CRT control cmd.
			}
			
		}
		gpio_4b2->gpio_write(0 << GPIO4B2_0_OFFSET, 1 << GPIO4B2_0_OFFSET); // GPIO4B2_0_OFFSET set low output.
		gpio_4b2->gpio_write(0 << GPIO4B2_2_OFFSET, 1 << GPIO4B2_2_OFFSET); // GPIO4B2_2_OFFSET set low output.
		EMBARC_PRINTF("PIN1 get: %d \r\n", value_read);
		board_delay_ms(1000,1);
		EMBARC_PRINTF("\x1b[2k\x1b\x45");// CRT control cmd.
	}

	return E_SYS;

}
