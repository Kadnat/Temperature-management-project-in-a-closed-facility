#include "common.h" // Send PIC 16f877a configuration
#include "lcd.h"  //call LCD Functons
#include "i2c_soft.h"  // Call I2C Functions
#include"RTC.h"  // Call RTC functions
#include "hal_usart.h"
#include <stdio.h>
#include "ds18b20.h"
#include "SD_PIC.h"
#include "SPI_PIC.h"
#include "AT24C32.h"
#include "PWM.h"
#include "buzzer.h"
#include "led.h"
#include "hal_timer1.h"
#include "hal_timer0.h"
#include "temp_monitoring.h"



uint8_t receive_usart_data;

volatile uint32_t valid_usart_tx;
volatile uint32_t valid_usart_rx;

TemperatureData system_management;

void putch(char c);
void USART_FramingDefaultErrorHandler(void);
void USART_OverrunDefaultErrorHandler(void);
void USART_TxDefaultInterruptHandler(void);
void USART_RxDefaultInterruptHandler(void);
void usart_module_init(void);
void timer1_timer_init(void);


void main(void) {

    uint8_t ret=0;
    volatile uint16_t timer1_counter_val;
    usart_module_init();
    timer1_timer_init();
    set_mode(NORMAL);
    read_address_in_eeprom();

    set_pwm_duty(0);
    start_pwm();
    LCD_Init(0x27); // start LCD function


    //Set_DS1307_RTC_Time(TwentyFourHoursMode,10,8,00);
    //Set_DS1307_RTC_Date(2,4,24,2);
    //read_init_sd_card();
    //single_block_write();
    
    //single_block_read();
    
    //multiple_block_write();
    
    //multiple_block_read();
    read_init_sd_card();
            
    while(1)
    {
        ;
     }

}

void Timer1_DefaultInterruptHandler(void)
{
    static uint16_t cpt_ms_lcd=0, cpt_ms_oled=0, cpt_ms_eeprom=0;
    
    cpt_ms_lcd++;
    cpt_ms_oled++;
    cpt_ms_eeprom++;
    
    if(cpt_ms_lcd >=1000)
    {
        update_system_data(&system_management);

        DisplayTimeToLCD(&system_management);
        DisplayDateOnLCD(&system_management);
        cpt_ms_lcd=0;
    }
    
    if(cpt_ms_oled >=10000)
    {
        print_temperature(system_management.temperature);
        cpt_ms_oled=0;
    }
    
    if(cpt_ms_eeprom >= 60000)
    {
        save_in_eeprom(&system_management);
        cpt_ms_eeprom=0;
        save_address_in_eeprom();


    }
}

void timer1_timer_init(void)
{
    timer1_t timer_obj;
    timer_obj.TMR1_InterruptHandler = Timer1_DefaultInterruptHandler;
    timer_obj.priority = INTERRUPT_LOW_PRIORITY;
    timer_obj.timer1_mode = TIMER1_TIMER_MODE;
    timer_obj.timer1_prescaler_value = TIMER1_PRESCALER_DIV_BY_4;
    //timer_obj.timer1_preload_value = 15536;400 ms
    timer_obj.timer1_preload_value = 65286;//1 ms = (4*Presc*(65536-TMR1))/Fosc
    timer_obj.timer1_reg_rw_mode = TIMER1_RW_16BIT_REGISTER_MODE;
    Timer1_Init(&timer_obj);
}

void putch(char c)
{
    USART_Asynchronous_WriteByte_Blocking(c);
}

void USART_FramingDefaultErrorHandler(void)
{
    uint8_t _data_;
    USART_Asynchronous_ReadByte_NonBlocking(&_data_);
}

void USART_OverrunDefaultErrorHandler(void)
{
    USART_Asynchronous_Restart_RX();
}

void USART_TxDefaultInterruptHandler(void)
{
    valid_usart_tx++;
}

void USART_RxDefaultInterruptHandler(void)
{
    uint8_t ret = 0;
    valid_usart_rx++;
    ret = USART_Asynchronous_ReadByte_NonBlocking(&receive_usart_data);
      
}

void usart_module_init(void)
{
    uint8_t ret = 0;
    usart_t usart_obj;
    
    usart_obj.USART_FramingErrorHandler = USART_FramingDefaultErrorHandler;
    usart_obj.USART_OverrunErrorHandler = USART_OverrunDefaultErrorHandler;
    usart_obj.USART_RxDefaultInterruptHandler = USART_RxDefaultInterruptHandler;
    usart_obj.USART_TxDefaultInterruptHandler = USART_TxDefaultInterruptHandler;
    
    usart_obj.baudrate = 9600;
    usart_obj.baudrate_config = BAUDRATE_ASYNCHRONOUS_MODE_8BIT_HIGH_SPEED;
    
    usart_obj.usart_tx_cfg.usart_enable_tx = USART_ENABLE_ASYNCHRONOUS_MODE_TX;
    usart_obj.usart_tx_cfg.usart_enable_tx_interrupt = USART_ENABLE_ASYNCHRONOUS_MODE_TX_INTERRUPT;
    usart_obj.usart_tx_cfg.usart_enable_9bit_tx = USART_DISABLE_ASYNCHRONOUS_MODE_9BIT_TX;
   
    usart_obj.usart_rx_cfg.usart_enable_rx = USART_ENABLE_ASYNCHRONOUS_MODE_RX;
    usart_obj.usart_rx_cfg.usart_enable_rx_interrupt = USART_ENABLE_ASYNCHRONOUS_MODE_RX_INTERRUPT;
    usart_obj.usart_rx_cfg.usart_enable_9bit_rx = USART_DISABLE_ASYNCHRONOUS_MODE_9BIT_RX;
    
    ret = USART_Asynchronous_Initialize(&usart_obj);
}