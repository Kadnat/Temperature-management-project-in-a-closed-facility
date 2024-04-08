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


void update_system_data(TemperatureData* pSystem_data)
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


void save_in_eeprom(TemperatureData* pSystem_data)
{
   unsigned char tab[8]={0};
   float temp = OneWireTemp();
   Date date = Get_DS1307_RTC_Date();
   Time time = Get_DS1307_RTC_Time();


   // Convertir la température en deux valeurs entières
   uint8_t temp_int = (uint8_t)temp;
   uint8_t temp_frac = (temp - (float)temp_int) * 100.0;

   // Remplir le tableau avec les données à enregistrer
   tab[0] = date.day;
   tab[1] = time.hour;
   tab[2] = time.minute;
   tab[3] = time.second;
   tab[4] = temp_int;
   tab[5] = temp_frac;
   tab[6] = 0;//command_decimal; // 
   tab[7] = 0;//command_fraction; // 
  
   // Écrire les données dans l'EEPROM
   write_one_page_in_eeprom(tab, previous_address_eeprom);

   // Mettre à jour l'adresse pour la prochaine écriture
   previous_address_eeprom += 8;
}

void save_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
    
    addressH = (previous_address_eeprom & 0xF00)>>8;
    addressL = (previous_address_eeprom & 0xFF);
    
    write_one_byte_in_eeprom(addressH, 1);
    __delay_ms(100);
    write_one_byte_in_eeprom(addressL, 0);

    //printf("previous add  save %u\r\n", previous_address_eeprom);
}

void read_address_in_eeprom(void)
{   
    unsigned char addressH=0, addressL=0;
        
    addressH = read_one_byte_in_eeprom(1);
    __delay_ms(100);
    addressL = read_one_byte_in_eeprom(0);    
    
    previous_address_eeprom = (addressH<<8) | addressL;
    
    printf("previous add read %u\r\n", previous_address_eeprom);
    

}

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