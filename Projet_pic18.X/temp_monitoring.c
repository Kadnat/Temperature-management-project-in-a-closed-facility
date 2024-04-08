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


static uint16_t previous_address_eeprom=8;
static uint8_t counter_alarm=0;


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
   tab[9] = 0;
   tab[10] = 0;
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
   save_address_in_eeprom();
}

void save_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
    
    addressH = (previous_address_eeprom & 0xF00)>>8;
    addressL = (previous_address_eeprom & 0xFF);
    
    write_one_byte_in_eeprom(addressH, 1);
    __delay_ms(100);
    write_one_byte_in_eeprom(addressL, 0);
    __delay_ms(100);
    write_one_byte_in_eeprom(counter_alarm, 2);

    //printf("previous add  save %u\r\n", previous_address_eeprom);
}

void read_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
        
    addressH = read_one_byte_in_eeprom(1);
    __delay_ms(100);
    addressL = read_one_byte_in_eeprom(0);
    __delay_ms(100);
    counter_alarm = read_one_byte_in_eeprom(2);     
    
    previous_address_eeprom = (addressH<<8) | addressL;
    
    printf("previous add read %u\r\n", previous_address_eeprom);
    printf("counter alarm %u\r\n", counter_alarm);
    

}

void reset_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
    
    counter_alarm = 0;
    previous_address_eeprom = 0;
    
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
    //reset_address_in_eeprom();
}

// SD MANAGEMENT FUNCTIONS

void extract_one_day_of_data(void)
{
    unsigned char tab2[8]={0};
    
    // Commencer à l'adresse 8
    static uint16_t previous_address_counter = 8;
    
    // Lire 1440 paquets de données
    for(int j=0; j<1440; j++)
    {
        read_one_page_in_eeprom(previous_address_counter,tab2);
        previous_address_counter += 8;
        __delay_ms(10);
        
        // Afficher les données
        printf("{%02x%02x%02x%02x%02x%02x%02x%02x}\r\n", tab2[0], tab2[1], tab2[2], tab2[3], tab2[4], tab2[5], tab2[6], tab2[7]);
    }
    
    // Retourner à l'adresse 8
    previous_address_counter = 8;
}


void SD_control(void)
{
    
}