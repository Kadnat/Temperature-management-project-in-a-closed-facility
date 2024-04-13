/*
 * File:   temp_monitoring.c
 * Author: Nathanael
 *
 * Created on 1 avril 2024, 11:31
 */

#include "temp_monitoring.h"
#include "ds18b20.h"
#include "RTC.h"
#include "AT24C32.h"
#include "SD_PIC.h"
#include "PWM.h"
#include "led.h"
#include "heater.h"
#include "buzzer.h"


static uint16_t previous_address_eeprom=8;
static uint8_t counter_alarm=0;
static unsigned long sector_address = 0;
uint8_t activate_buzzer = 0;

void update_system_data(SystemData* pSystem_data)
{
   float temp = OneWireTemp();
   Date date = Get_DS1307_RTC_Date();
   Time time = Get_DS1307_RTC_Time();
    
    pSystem_data->year = date.year;
    pSystem_data->month = date.month;
    pSystem_data->day = date.day;
    
    pSystem_data->hour = time.hour;
    pSystem_data->minute = time.minute;
    pSystem_data->second = time.second;
    
    pSystem_data->temperature = temp;
    // Convertir la température en deux valeurs entières
    uint8_t temp_int = (uint8_t)temp;
    uint8_t temp_frac = (temp - (float)temp_int) * 100.0;
    
    pSystem_data->temp_decimal = temp_int;
    pSystem_data->temp_fraction = temp_frac;

    // Mettre à jour command_decimal et command_fraction
    //pSystem_data->command_decimal = command_decimal;
    //pSystem_data->command_fraction = command_fraction;
}

// EEPROM MAGAGEMENTS FUNCTIONS

void save_in_eeprom(SystemData* pSystem_data)
{
   unsigned char tab[16]={0};

   tab[0] = pSystem_data->year;
   tab[1] = pSystem_data->month;
   tab[2] = pSystem_data->day;
   tab[3] = pSystem_data->hour;
   tab[4] = pSystem_data->minute;
   tab[5] = pSystem_data->second;
   tab[6] = pSystem_data->temp_decimal;
   tab[7] = pSystem_data->temp_fraction;
   tab[8] = pSystem_data->error_type;
   tab[9] = pSystem_data->command_decimal;
   tab[10] = pSystem_data->command_fraction;
   tab[11] = 0;
   tab[12] = 0;//command_decimal; // 
   tab[13] = 0;//command_fraction; // 
   tab[14] = 0;//address upper byte; // 
   tab[15] = 0;//address lower byte; // 
  
    printf("Tableau avant écriture dans l'EEPROM:\n");
    for (int i = 0; i < 16; i++) {
        printf("%02x ", tab[i]);
    }
    printf("\n");
   
   // Écrire les données dans l'EEPROM
    
   write_one_page_in_eeprom(tab, previous_address_eeprom);
   __delay_ms(100);
   write_one_page_in_eeprom(&tab[8], previous_address_eeprom+8);
   __delay_ms(100);
   
   // Mettre à jour l'adresse pour la prochaine écriture
   previous_address_eeprom += 16;
   counter_alarm++;
   __delay_ms(100);
   save_eep_address_in_eeprom();
   
   if(counter_alarm == 254)
   {
       reset_eep_address_in_eeprom();
   }
}

void save_eep_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
    
    addressH = (previous_address_eeprom & 0xF00)>>8;
    addressL = (previous_address_eeprom & 0xFF);
    
    write_one_byte_in_eeprom(addressH, 1);
    __delay_ms(10);
    write_one_byte_in_eeprom(addressL, 0);
    __delay_ms(10);
    write_one_byte_in_eeprom(counter_alarm, 2);
     __delay_ms(10);

    //printf("previous add  save %u\r\n", previous_address_eeprom);
}

void read_eep_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
        
    addressH = read_one_byte_in_eeprom(1);
    __delay_ms(100);
    addressL = read_one_byte_in_eeprom(0);
    __delay_ms(100);
    counter_alarm = read_one_byte_in_eeprom(2);     
    
    previous_address_eeprom = (addressH<<8) | addressL;
    
    //printf("previous add read %u\r\n", previous_address_eeprom);
    //printf("counter alarm %u\r\n", counter_alarm);
    

}

void reset_eep_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
    
    counter_alarm = 0;
    previous_address_eeprom = 8;
    
    addressH = (previous_address_eeprom & 0xF00)>>8;
    addressL = (previous_address_eeprom & 0xFF);
    
    write_one_byte_in_eeprom(addressH, 1);
    __delay_ms(100);
    write_one_byte_in_eeprom(addressL, 0);
    __delay_ms(100);
    write_one_byte_in_eeprom(counter_alarm, 2);
}

void extract_all_alarms(void)
{
    unsigned char tab2[16]={0};
    
    // Commencer à l'adresse 8
    uint16_t previous_address_counter = 8;
    
    
    
    for(int j=0; j<counter_alarm; j++)
    {
        read_one_page_in_eeprom(previous_address_counter,tab2);
        previous_address_counter += 8;
        __delay_ms(10);
        read_one_page_in_eeprom(previous_address_counter,&tab2[8]);
        previous_address_counter += 8;
        __delay_ms(10);
        
        // Afficher les données penser à envoyer par interruption haute priorité
        printf("{%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x}\r\n", tab2[0], tab2[1], tab2[2], tab2[3], tab2[4], tab2[5], tab2[6], tab2[7], tab2[8], tab2[9], tab2[10], tab2[11], tab2[12], tab2[13], tab2[14], tab2[15]);
    
    }
    reset_eep_address_in_eeprom();
}

// SD MANAGEMENT FUNCTIONS


void update_SD_tab(SystemData* pSystem_data)
{
    static int counter = 0; // Compteur pour suivre la position actuelle dans le tableau
    unsigned char tab[16] = {0};

    tab[0] = pSystem_data->year;
    tab[1] = pSystem_data->month;
    tab[2] = pSystem_data->day;
    tab[3] = pSystem_data->hour;
    tab[4] = pSystem_data->minute;
    tab[5] = pSystem_data->second;
    tab[6] = pSystem_data->temp_decimal;
    tab[7] = pSystem_data->temp_fraction;
    tab[8] = pSystem_data->error_type;
    tab[9] = pSystem_data->command_decimal;
    tab[10] = pSystem_data->command_fraction;
    tab[11] = 0;
    tab[12] = 0; // command_decimal
    tab[13] = 0; // command_fraction
    tab[14] = 0; // address upper byte
    tab[15] = 0; // address lower byte

    // Assurez-vous que le tableau SDwriteBuffer est accessible
    memcpy(&SDwriteBuffer[counter], tab, 16); // Copie les données du tableau 'tab' dans 'SDwriteBuffer' à la position actuelle du compteur

    counter += 16; // Augmente le compteur de 16 pour le prochain appel de la fonction

    // Réinitialise le compteur si nous avons atteint la fin du tableau SDwriteBuffer
    if (counter >= 512) {
        single_block_write(sector_address);
        sector_address++;
        save_sd_address_in_eeprom();
        counter = 0;
    }
    
    if(sector_address == 262144) // number sectors = 128*1024*1024/512  (128 Mo SDCard) 
    {
        reset_sd_address_in_eeprom();
    }
}


void save_sd_address_in_eeprom(void)
{      
    for(int i = 0; i < 4; i++) {
        write_one_byte_in_eeprom((sector_address >> (i * 8)) & 0xFF, 3 + i);
        __delay_ms(10);
    }
}

void read_sd_address_in_eeprom(void)
{   
    for(int i = 0; i < 4; i++) {
        sector_address |= ((unsigned long)read_one_byte_in_eeprom(6 - i)) << ((3 - i) * 8);
        __delay_ms(10);
    }
}

void reset_sd_address_in_eeprom(void)
{
    for(int i = 0; i < 4; i++) {
        write_one_byte_in_eeprom(0, 3 + i);
        __delay_ms(10);
    }
}

void extract_data_for_days(int number_days)
{
    unsigned long i;
    unsigned short numWrites = number_days * 25;//90; // Nombre de secteurs à lire pour 'days' jours
    long firstBlock = sector_address - numWrites; // Calculer le premier bloc à lire

    initSD();
    // Si le nombre de jours demandé est supérieur au nombre de jours de données disponibles,
    // nous commençons à lire à partir du premier bloc de données
    if (firstBlock < 0) {
        firstBlock = 0;
    }

    for(i = 0; i < sizeof(SDreadBuffer); i++)
    {
        // Clear buffer to prove we are reading from the card
        SDreadBuffer[i] = 0;
    }
    
    // Start multiple block read (MBR) from the calculated block address
    SD_MBR_Start(firstBlock);
    
    printf("Reading sectors ");
    printf("%d-%d\r\n", firstBlock, firstBlock + numWrites - 1);
    
    for(i = 0; i < numWrites; i++)
    {
        // Read the sector and store it in SDreadBuffer
        SD_MBR_Receive(SDreadBuffer);
        
        // Afficher les données
        for(int j = 0; j<32;j++)
        {
            printf("{%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x}\r\n", SDreadBuffer[0+(16*j)], SDreadBuffer[1+(16*j)], SDreadBuffer[2+(16*j)], SDreadBuffer[3+(16*j)], SDreadBuffer[4+(16*j)], SDreadBuffer[5+(16*j)], SDreadBuffer[6+(16*j)], SDreadBuffer[7+(16*j)], SDreadBuffer[8+(16*j)], SDreadBuffer[9+(16*j)], SDreadBuffer[10+(16*j)], SDreadBuffer[11+(16*j)], SDreadBuffer[12+(16*j)], SDreadBuffer[13+(16*j)], SDreadBuffer[14+(16*j)], SDreadBuffer[15+(16*j)]);
        }
        /*
        if((i > 0) && (i % 250 == 0)){
            printf(".");
        }
         */
    }
    SD_MBR_Stop();
    
    sd_stop(); // Stop SPI and deselect SD card
    
    printf("Sec %d-%d\r\n", (int)firstBlock, (int)(firstBlock + numWrites - 1));
}

// TEMP MANAGEMENT FUNCTIONS

 void temp_management(SystemData* pSystem_data)
{
    float command_temp = 23.0;
    static uint8_t already_save = 0;
    
    command_temp = pSystem_data->command_decimal + (float)pSystem_data->command_fraction/100;
    //printf("temp %f",command_temp);
    
    if(pSystem_data->temperature >= command_temp + 1.0)
    {
        pSystem_data->error_type = TOO_HOT;
        set_pwm_duty(100);
        heater_set_mode(OFF);
        if(already_save == 0)
        {
            save_in_eeprom(pSystem_data);
            already_save = 1;
            activate_buzzer = 1;
        }
    }
    else if(pSystem_data->temperature <= command_temp - 1.0)
    {
        pSystem_data->error_type = TOO_COLD;
        heater_set_mode(ON);
        set_pwm_duty(0);
        if(already_save == 0)
        {
            save_in_eeprom(pSystem_data);
            already_save = 1;
        }
    }
    else
    {
        pSystem_data->error_type = NO_ERROR;
        heater_set_mode(OFF);
        set_pwm_duty(0);
        already_save = 0;
        activate_buzzer = 0;
    }
    led_set_mode(pSystem_data);

        
}
 
 void log_system(SystemData* pSystem_data)
{
   unsigned char tab[16]={0};

   tab[0] = pSystem_data->year;
   tab[1] = pSystem_data->month;
   tab[2] = pSystem_data->day;
   tab[3] = pSystem_data->hour;
   tab[4] = pSystem_data->minute;
   tab[5] = pSystem_data->second;
   tab[6] = pSystem_data->temp_decimal;
   tab[7] = pSystem_data->temp_fraction;
   tab[8] = pSystem_data->error_type;
   tab[9] = pSystem_data->command_decimal;
   tab[10] = pSystem_data->command_fraction;
   tab[11] = 0;
   tab[12] = 0;//command_decimal; // 
   tab[13] = 0;//command_fraction; // 
   tab[14] = 0;//address upper byte; // 
   tab[15] = 0;//address lower byte; // 
  
   printf("{%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x}\r\n", tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6], tab[7], tab[8], tab[9], tab[10], tab[11], tab[12], tab[13], tab[14], tab[15]);
    
}
