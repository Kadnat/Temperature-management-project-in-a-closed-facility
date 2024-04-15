#include "common.h"
#include "lcd.h"  
#include "i2c_soft.h" 
#include"RTC.h"  
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
#include "heater.h"

#define MAX_BUFFER_SIZE 100
#define NUM_COMMANDS 3




char rx_buffer[MAX_BUFFER_SIZE];
int rx_buffer_index = 0;
char* commands[NUM_COMMANDS] = {"HISTORY:", "ALARMS", "COMMAND:"};

uint8_t receive_usart_data;

volatile uint32_t valid_usart_tx;
volatile uint32_t valid_usart_rx;

SystemData system_management;

void putch(char c);
void USART_FramingDefaultErrorHandler(void);
void USART_OverrunDefaultErrorHandler(void);
void USART_TxDefaultInterruptHandler(void);
void USART_RxDefaultInterruptHandler(void);
void usart_module_init(void);
void timer1_timer_init(void);
void execute_rx_command(int command_index);

void main(void) {
    OSCILLATOR = 0x70;
    PLL = 0x40;

    uint8_t ret=0;
    volatile uint16_t timer1_counter_val;
    
    usart_module_init();
    timer1_timer_init();
    
    read_eep_address_in_eeprom();
    read_sd_address_in_eeprom();

    start_pwm();
    LCD_Init(0x27); // start LCD function

            
    while(1)
    {
        ;
     }

}

void Timer1_DefaultInterruptHandler(void)
{
    static uint16_t cpt_ms_lcd=0, cpt_ms_oled=0, cpt_ms_buzzer=0;
    static uint32_t cpt_ms_sd=0;
    static uint8_t cpt_ms_temp_management=0;
    
    cpt_ms_lcd++;
    cpt_ms_oled++;
    cpt_ms_sd++;
    cpt_ms_temp_management++;
    
    if(cpt_ms_temp_management >= 100)
    {
        cpt_ms_temp_management=0;
        update_system_data(&system_management);
        temp_management(&system_management);
    }
    if(cpt_ms_lcd >=500)
    {
        DisplayTimeToLCD(&system_management);
        DisplayDateOnLCD(&system_management);
        cpt_ms_lcd=0; 
    }
    
    if(activate_buzzer == 1)
    {
        cpt_ms_buzzer++;
        buzzer(activate_buzzer);
        if(cpt_ms_buzzer >= 2000)
        {
            activate_buzzer = 0;
            cpt_ms_buzzer = 0;
            buzzer(activate_buzzer);
            
        }
    }
    
    if(cpt_ms_oled >=3000)
    {
        print_temperature(&system_management);
        log_system(&system_management);
        cpt_ms_oled=0;
    }
    
    if(cpt_ms_sd >= 30000)
    {
        update_SD_tab(&system_management);
        cpt_ms_sd=0;
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
    //timer_obj.timer1_preload_value = 65286;//1 ms = (4*Presc*(65536-TMR1))/Fosc
    timer_obj.timer1_preload_value = 63536;//1 ms = (4*Presc*(65536-TMR1))/Fosc
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

    // Ajouter le caractère reçu au buffer
    if (rx_buffer_index < MAX_BUFFER_SIZE - 1) {
        rx_buffer[rx_buffer_index] = receive_usart_data;
        rx_buffer_index++;
    }

    // Vérifier si le buffer contient un caractère de fin de ligne
    if (strchr(rx_buffer, '\n') != NULL) {
        // Vérifier si le buffer correspond à une commande
        for (int i = 0; i < NUM_COMMANDS; i++) {
            if (strncmp(rx_buffer, commands[i], strlen(commands[i])) == 0) {
                // La commande correspond, exécuter l'action correspondante
                printf("command receive\r\n");
                execute_rx_command(i);
                break;
            }
        }

        // Vider le buffer après avoir traité la commande
        memset(rx_buffer, 0, sizeof(rx_buffer));
        rx_buffer_index = 0;
    }
}

void execute_rx_command(int command_index) {
    // Exécuter l'action correspondante à la commande
    switch (command_index) {
        case 0:
            {
                // Extraire le nombre après "HISTORY:"
                int days = atoi(rx_buffer + strlen(commands[0]));
                extract_data_for_days(days);
            }
            
            break;
        case 1:
            // Exécuter l'action pour "ALARMS"
            extract_all_alarms();
            break;
            
        case 2:
            // Exécuter l'action pour "COMMAND"
        {
            float temperature = atof(rx_buffer + strlen(commands[2]));
            system_management.command_decimal = (uint8_t)temperature;
            system_management.command_fraction = (temperature - (float)system_management.command_decimal) * 100.0;
        }       
            break;
        default:
            break;
    }
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