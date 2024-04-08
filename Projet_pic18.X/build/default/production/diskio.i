# 1 "diskio.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files/Microchip/MPLABX/v6.10/packs/Microchip/PIC18Fxxxx_DFP/1.4.151/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "diskio.c" 2




# 1 "./diskio.h" 1
# 12 "./diskio.h"
# 1 "./pff.h" 1
# 24 "./pff.h"
# 1 "./pffconf.h" 1
# 25 "./pff.h" 2
# 36 "./pff.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\musl_xc8.h" 1 3
# 4 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 2 3
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 127 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long uintptr_t;
# 142 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long intptr_t;
# 158 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 188 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 229 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 2 3


typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;
typedef int24_t int_fast24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;
typedef uint24_t uint_fast24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\bits/stdint.h" 1 3
typedef int16_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint16_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 144 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdint.h" 2 3
# 37 "./pff.h" 2
typedef unsigned int UINT;
typedef unsigned char BYTE;
typedef uint16_t WORD;
typedef uint16_t WCHAR;
typedef uint32_t DWORD;
# 61 "./pff.h"
typedef struct {
 BYTE fs_type;
 BYTE flag;
 BYTE csize;
 BYTE pad1;
 WORD n_rootdir;
 WORD n_fatent;
 DWORD fatbase;
 DWORD dirbase;
 DWORD database;
 DWORD fptr;
 DWORD fsize;
 WORD org_clust;
 WORD curr_clust;
 DWORD dsect;
} FATFS;





typedef struct {
 WORD index;
 BYTE* fn;
 WORD sclust;
 WORD clust;
 DWORD sect;
} DIR;





typedef struct {
 DWORD fsize;
 WORD fdate;
 WORD ftime;
 BYTE fattrib;
 char fname[13];
} FILINFO;





typedef enum {
 FR_OK = 0,
 FR_DISK_ERR,
 FR_NOT_READY,
 FR_NO_FILE,
 FR_NOT_OPENED,
 FR_NOT_ENABLED,
 FR_NO_FILESYSTEM
} FRESULT;






FRESULT pf_mount (FATFS* fs);
FRESULT pf_open (const char* path);
FRESULT pf_read (void* buff, UINT btr, UINT* br);
FRESULT pf_write (const void* buff, UINT btw, UINT* bw);
FRESULT pf_lseek (DWORD ofs);
FRESULT pf_opendir (DIR* dj, const char* path);
FRESULT pf_readdir (DIR* dj, FILINFO* fno);
# 13 "./diskio.h" 2



typedef BYTE DSTATUS;



typedef enum {
 RES_OK = 0,
 RES_ERROR,
 RES_NOTRDY,
 RES_PARERR
} DRESULT;





DSTATUS disk_initialize (void);
DRESULT disk_readp (BYTE* buff, DWORD sector, UINT offser, UINT count);
DRESULT disk_writep (BYTE* buff, DWORD sc);
# 6 "diskio.c" 2
# 1 "./memoryCard.h" 1








# 1 "C:\\Program Files\\Microchip\\xc8\\v2.36\\pic\\include\\c99\\stdbool.h" 1 3
# 10 "./memoryCard.h" 2
# 90 "./memoryCard.h"
    typedef union
    {
        struct {
            unsigned is_idle : 1;
            unsigned erase_reset : 1;
            unsigned illegal_cmd_error : 1;
            unsigned crc_error : 1;
            unsigned erase_seq_error : 1;
            unsigned address_error : 1;
            unsigned param_error : 1;
            unsigned valid_header_n : 1;
        };
        uint8_t data;
    } CommandStatus;

        typedef union
    {
        struct {
            unsigned error : 1;
            unsigned cc_error : 1;
            unsigned card_ecc_error : 1;
            unsigned range_error : 1;
            unsigned locked_error : 1;
            unsigned valid_header_n : 3;
        } ErrorToken;
        struct {
            unsigned one : 1;
            unsigned status : 3;
            unsigned zero : 1;
            unsigned dnc : 3;
        } DataToken;

        uint8_t data;
    } RespToken;


    typedef union
    {
        struct {
            unsigned is_locked : 1;
            unsigned wp_erase_skip : 1;
            unsigned unknown_error : 1;
            unsigned cc_error : 1;
            unsigned ecc_fail : 1;
            unsigned wp_violation : 1;
            unsigned erase_param : 1;
            unsigned out_of_range : 1;
            unsigned is_idle : 1;
            unsigned erase_reset : 1;
            unsigned illegal_cmd_error : 1;
            unsigned crc_error : 1;
            unsigned erase_seq_error : 1;
            unsigned address_error : 1;
            unsigned param_error : 1;
            unsigned : 1;

        };
        uint8_t data[2];
        uint16_t packet;
    } CardStatus;

    typedef enum {
        CARD_NO_ERROR = 0, CARD_SPI_TIMEOUT, CARD_CRC_ERROR, CARD_RESPONSE_ERROR,
        CARD_ILLEGAL_CMD, CARD_VOLTAGE_NOT_SUPPORTED, CARD_PATTERN_ERROR,
        CARD_WRITE_IN_PROGRESS, CARD_WRITE_SIZE_ERROR, CARD_NOT_INIT
    } CommandError;

    typedef enum {
        CCS_INVALID = -1, CCS_LOW_CAPACITY, CCS_HIGH_CAPACITY
    } CardCapacityType;

    typedef enum {
        STATUS_CARD_NONE = 0, STATUS_CARD_NOT_INIT, STATUS_CARD_ERROR, STATUS_CARD_READY
    } MemoryCardDriverStatus;


    void memCard_initDriver(void);


    _Bool memCard_initCard(void);


    MemoryCardDriverStatus memCard_getCardStatus(void);


    _Bool memCard_isCardReady(void);


    _Bool memCard_setupFastSPI(void);


    uint16_t memCard_calculateCRC16(uint8_t* data, uint16_t dLen);



    void memCard_attach(void);


    void memCard_detach(void);


    CommandError memCard_configureCard(void);


    uint8_t memCard_sendCMD_R1(uint8_t commandIndex, uint32_t data);


    uint8_t memCard_sendACMD_R1(uint8_t commandIndex, uint32_t data);


    CardCapacityType memCard_getCapacityType(void);


    _Bool memCard_receiveResponse_R1(uint8_t* dst);


    CommandError memCard_readOCR(uint8_t* data);


    CommandError memCard_readCSD(uint8_t* data);


    _Bool memCard_readFromDisk(uint32_t sect, uint16_t offset, uint8_t* data, uint16_t nBytes);



    _Bool memCard_prepareWrite(uint32_t sector);



    _Bool memCard_queueWrite(uint8_t* data, uint16_t dLen);


    CommandError memCard_writeBlock(void);


    CommandError memCard_readBlock(uint32_t sector);


    CommandError memCard_receiveBlockData(uint8_t* data, uint16_t length);


    uint8_t memCard_runCRC7(uint8_t* dataIn, uint8_t len);
# 7 "diskio.c" 2





DSTATUS disk_initialize (void)
{
 if (memCard_initCard())
        return 0x00;

 return 0x01;
}







DRESULT disk_readp (
 BYTE* buff,
 DWORD sector,
 UINT offset,
 UINT count
)
{
 if (!memCard_readFromDisk(sector, offset, buff, count))
    {
        return RES_ERROR;
    }

 return RES_OK;
}







DRESULT disk_writep (
 BYTE* buff,
 DWORD sc
)
{
 DRESULT res = RES_OK;


 if (!buff) {
  if (sc) {


            if (!memCard_prepareWrite(sc))
            {
                res = RES_NOTRDY;
            }

  } else {


            if (memCard_writeBlock() != CARD_NO_ERROR)
            {
                res = RES_ERROR;
            }
  }
 } else {


        if (!memCard_queueWrite(buff, sc))
        {
            res = RES_ERROR;
        }

 }

 return res;
}
