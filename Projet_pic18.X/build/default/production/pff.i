# 1 "pff.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files/Microchip/MPLABX/v6.10/packs/Microchip/PIC18Fxxxx_DFP/1.4.151/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "pff.c" 2
# 38 "pff.c"
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
# 39 "pff.c" 2
# 1 "./diskio.h" 1
# 16 "./diskio.h"
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
# 40 "pff.c" 2
# 377 "pff.c"
static FATFS *FatFs;






static WORD ld_word (const BYTE* ptr)
{
 WORD rv;

 rv = ptr[1];
 rv = (rv << 8) | ptr[0];
 return rv;
}

static DWORD ld_dword (const BYTE* ptr)
{
 DWORD rv;

 rv = ptr[3];
 rv = rv << 8 | ptr[2];
 rv = rv << 8 | ptr[1];
 rv = rv << 8 | ptr[0];
 return rv;
}
# 411 "pff.c"
static void mem_set (void* dst, int val, int cnt) {
 char *d = (char*)dst;
 while (cnt--) *d++ = (char)val;
}


static int mem_cmp (const void* dst, const void* src, int cnt) {
 const char *d = (const char *)dst, *s = (const char *)src;
 int r = 0;
 while (cnt-- && (r = *d++ - *s++) == 0) ;
 return r;
}







static WORD get_fat (
 WORD clst
)
{
 BYTE buf[4];
 FATFS *fs = FatFs;




 if (clst < 2 || clst >= fs->n_fatent) return 1;

 switch (fs->fs_type) {
# 458 "pff.c"
 case 2 :
  if (disk_readp(buf, fs->fatbase + clst / 256, ((UINT)clst % 256) * 2, 2)) break;
  return ld_word(buf);






 }

 return 1;
}
# 479 "pff.c"
static DWORD clust2sect (
 WORD clst
)
{
 FATFS *fs = FatFs;


 clst -= 2;
 if (clst >= (fs->n_fatent - 2)) return 0;
 return (DWORD)clst * fs->csize + fs->database;
}


static WORD get_clust (
 BYTE* dir
)
{
 FATFS *fs = FatFs;
 WORD clst = 0;


 if (0 || (0 && fs->fs_type == 3)) {
  clst = ld_word(dir+20);
  clst <<= 16;
 }
 clst |= ld_word(dir+26);

 return clst;
}






static FRESULT dir_rewind (
 DIR *dj
)
{
 WORD clst;
 FATFS *fs = FatFs;


 dj->index = 0;
 clst = dj->sclust;
 if (clst == 1 || clst >= fs->n_fatent) {
  return FR_DISK_ERR;
 }
 if (0 && !clst && (0 || fs->fs_type == 3)) {
  clst = (WORD)fs->dirbase;
 }
 dj->clust = clst;
 dj->sect = (0 || clst) ? clust2sect(clst) : fs->dirbase;

 return FR_OK;
}
# 543 "pff.c"
static FRESULT dir_next (
 DIR *dj
)
{
 WORD clst;
 WORD i;
 FATFS *fs = FatFs;


 i = dj->index + 1;
 if (!i || !dj->sect) return FR_NO_FILE;

 if (!(i % 16)) {
  dj->sect++;

  if (dj->clust == 0) {
   if (i >= fs->n_rootdir) return FR_NO_FILE;
  }
  else {
   if (((i / 16) & (fs->csize - 1)) == 0) {
    clst = get_fat(dj->clust);
    if (clst <= 1) return FR_DISK_ERR;
    if (clst >= fs->n_fatent) return FR_NO_FILE;
    dj->clust = clst;
    dj->sect = clust2sect(clst);
   }
  }
 }

 dj->index = i;

 return FR_OK;
}
# 584 "pff.c"
static FRESULT dir_find (
 DIR *dj,
 BYTE *dir
)
{
 FRESULT res;
 BYTE c;


 res = dir_rewind(dj);
 if (res != FR_OK) return res;

 do {
  res = disk_readp(dir, dj->sect, (dj->index % 16) * 32, 32)
   ? FR_DISK_ERR : FR_OK;
  if (res != FR_OK) break;
  c = dir[0];
  if (c == 0) { res = FR_NO_FILE; break; }
  if (!(dir[11] & 0x08) && !mem_cmp(dir, dj->fn, 11)) break;
  res = dir_next(dj);
 } while (res == FR_OK);

 return res;
}
# 651 "pff.c"
static FRESULT create_name (
 DIR *dj,
 const char **path
)
{
 BYTE c, d, ni, si, i, *sfn;
 const char *p;

 static const BYTE cvt[] = {0x80,0x9A,0x45,0x41,0x8E,0x41,0x8F,0x80,0x45,0x45,0x45,0x49,0x49,0x49,0x8E,0x8F, 0x90,0x92,0x92,0x4F,0x99,0x4F,0x55,0x55,0x59,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F, 0x41,0x49,0x4F,0x55,0xA5,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF, 0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF, 0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF, 0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, 0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF, 0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF};



 sfn = dj->fn;
 mem_set(sfn, ' ', 11);
 si = i = 0; ni = 8;
 p = *path;
 for (;;) {
  c = p[si++];
  if (c <= ' ' || c == '/') break;
  if (c == '.' || i >= ni) {
   if (ni != 8 || c != '.') break;
   i = 8; ni = 11;
   continue;
  }

  if (c >= 0x80) c = cvt[c - 0x80];

  if (0 && i < ni - 1) {
   d = p[si++];
   sfn[i++] = c;
   sfn[i++] = d;
  } else {
   if (1 && (((c)>='a')&&((c)<='z'))) c -= 0x20;
   sfn[i++] = c;
  }
 }
 *path = &p[si];

 sfn[11] = (c <= ' ') ? 1 : 0;

 return FR_OK;
}
# 742 "pff.c"
static FRESULT follow_path (
 DIR *dj,
 BYTE *dir,
 const char *path
)
{
 FRESULT res;


 while (*path == ' ') path++;
 if (*path == '/') path++;
 dj->sclust = 0;

 if ((BYTE)*path < ' ') {
  res = dir_rewind(dj);
  dir[0] = 0;

 } else {
  for (;;) {
   res = create_name(dj, &path);
   if (res != FR_OK) break;
   res = dir_find(dj, dir);
   if (res != FR_OK) break;
   if (dj->fn[11]) break;
   if (!(dir[11] & 0x10)) {
    res = FR_NO_FILE; break;
   }
   dj->sclust = get_clust(dir);
  }
 }

 return res;
}
# 783 "pff.c"
static BYTE check_fs (
 BYTE *buf,
 DWORD sect
)
{
 if (disk_readp(buf, sect, 510, 2)) {
  return 3;
 }
 if (ld_word(buf) != 0xAA55) {
  return 2;
 }

 if (!0 && !disk_readp(buf, sect, 54, 2) && ld_word(buf) == 0x4146) {
  return 0;
 }
 if (0 && !disk_readp(buf, sect, 82, 2) && ld_word(buf) == 0x4146) {
  return 0;
 }
 return 1;
}
# 819 "pff.c"
FRESULT pf_mount (
 FATFS *fs
)
{
 BYTE fmt, buf[36];
 DWORD bsect, fsize, tsect, mclst;


 FatFs = 0;

 if (disk_initialize() & 0x01) {
  return FR_NOT_READY;
 }


 bsect = 0;
 fmt = check_fs(buf, bsect);
 if (fmt == 1) {

  if (disk_readp(buf, bsect, 446, 16)) {
   fmt = 3;
  } else {
   if (buf[4]) {
    bsect = ld_dword(&buf[8]);
    fmt = check_fs(buf, bsect);
   }
  }
 }
 if (fmt == 3) return FR_DISK_ERR;
 if (fmt) return FR_NO_FILESYSTEM;


 if (disk_readp(buf, bsect, 13, sizeof (buf))) return FR_DISK_ERR;

 fsize = ld_word(buf+22 -13);
 if (!fsize) fsize = ld_dword(buf+36 -13);

 fsize *= buf[16 -13];
 fs->fatbase = bsect + ld_word(buf+14 -13);
 fs->csize = buf[13 -13];
 fs->n_rootdir = ld_word(buf+17 -13);
 tsect = ld_word(buf+19 -13);
 if (!tsect) tsect = ld_dword(buf+32 -13);
 mclst = (tsect
  - ld_word(buf+14 -13) - fsize - fs->n_rootdir / 16
  ) / fs->csize + 2;
 fs->n_fatent = (WORD)mclst;

 fmt = 0;
 if (0 && mclst < 0xFF7) fmt = 1;
 if (1 && mclst >= 0xFF8 && mclst < 0xFFF7) fmt = 2;
 if (0 && mclst >= 0xFFF7) fmt = 3;
 if (!fmt) return FR_NO_FILESYSTEM;
 fs->fs_type = fmt;

 if (0 || (0 && fmt == 3)) {
  fs->dirbase = ld_dword(buf+(44 -13));
 } else {
  fs->dirbase = fs->fatbase + fsize;
 }
 fs->database = fs->fatbase + fsize + fs->n_rootdir / 16;

 fs->flag = 0;
 FatFs = fs;

 return FR_OK;
}
# 894 "pff.c"
FRESULT pf_open (
 const char *path
)
{
 FRESULT res;
 DIR dj;
 BYTE sp[12], dir[32];
 FATFS *fs = FatFs;


 if (!fs) return FR_NOT_ENABLED;

 fs->flag = 0;
 dj.fn = sp;
 res = follow_path(&dj, dir, path);
 if (res != FR_OK) return res;
 if (!dir[0] || (dir[11] & 0x10)) return FR_NO_FILE;

 fs->org_clust = get_clust(dir);
 fs->fsize = ld_dword(dir+28);
 fs->fptr = 0;
 fs->flag = 0x01;

 return FR_OK;
}
# 928 "pff.c"
FRESULT pf_read (
 void* buff,
 UINT btr,
 UINT* br
)
{
 DRESULT dr;
 WORD clst;
 DWORD sect, remain;
 UINT rcnt;
 BYTE cs, *rbuff = buff;
 FATFS *fs = FatFs;


 *br = 0;
 if (!fs) return FR_NOT_ENABLED;
 if (!(fs->flag & 0x01)) return FR_NOT_OPENED;

 remain = fs->fsize - fs->fptr;
 if (btr > remain) btr = (UINT)remain;

 while (btr) {
  if ((fs->fptr % 512) == 0) {
   cs = (BYTE)(fs->fptr / 512 & (fs->csize - 1));
   if (!cs) {
    if (fs->fptr == 0) {
     clst = fs->org_clust;
    } else {
     clst = get_fat(fs->curr_clust);
    }
    if (clst <= 1) {fs->flag = 0; return FR_DISK_ERR;};
    fs->curr_clust = clst;
   }
   sect = clust2sect(fs->curr_clust);
   if (!sect) {fs->flag = 0; return FR_DISK_ERR;};
   fs->dsect = sect + cs;
  }
  rcnt = 512 - (UINT)fs->fptr % 512;
  if (rcnt > btr) rcnt = btr;
  dr = disk_readp(rbuff, fs->dsect, (UINT)fs->fptr % 512, rcnt);
  if (dr) {fs->flag = 0; return FR_DISK_ERR;};
  fs->fptr += rcnt;
  btr -= rcnt; *br += rcnt;
  if (rbuff) rbuff += rcnt;
 }

 return FR_OK;
}
# 985 "pff.c"
FRESULT pf_write (
 const void* buff,
 UINT btw,
 UINT* bw
)
{
 WORD clst;
 DWORD sect, remain;
 const BYTE *p = buff;
 BYTE cs;
 UINT wcnt;
 FATFS *fs = FatFs;


 *bw = 0;
 if (!fs) return FR_NOT_ENABLED;
 if (!(fs->flag & 0x01)) return FR_NOT_OPENED;

 if (!btw) {
  if ((fs->flag & 0x40) && disk_writep(0, 0)) {fs->flag = 0; return FR_DISK_ERR;};
  fs->flag &= ~0x40;
  return FR_OK;
 } else {
  if (!(fs->flag & 0x40)) {
   fs->fptr &= 0xFFFFFE00;
  }
 }
 remain = fs->fsize - fs->fptr;
 if (btw > remain) btw = (UINT)remain;

 while (btw) {
  if ((UINT)fs->fptr % 512 == 0) {
   cs = (BYTE)(fs->fptr / 512 & (fs->csize - 1));
   if (!cs) {
    if (fs->fptr == 0) {
     clst = fs->org_clust;
    } else {
     clst = get_fat(fs->curr_clust);
    }
    if (clst <= 1) {fs->flag = 0; return FR_DISK_ERR;};
    fs->curr_clust = clst;
   }
   sect = clust2sect(fs->curr_clust);
   if (!sect) {fs->flag = 0; return FR_DISK_ERR;};
   fs->dsect = sect + cs;
   if (disk_writep(0, fs->dsect)) {fs->flag = 0; return FR_DISK_ERR;};
   fs->flag |= 0x40;
  }
  wcnt = 512 - (UINT)fs->fptr % 512;
  if (wcnt > btw) wcnt = btw;
  if (disk_writep(p, wcnt)) {fs->flag = 0; return FR_DISK_ERR;};
  fs->fptr += wcnt; p += wcnt;
  btw -= wcnt; *bw += wcnt;
  if ((UINT)fs->fptr % 512 == 0) {
   if (disk_writep(0, 0)) {fs->flag = 0; return FR_DISK_ERR;};
   fs->flag &= ~0x40;
  }
 }

 return FR_OK;
}
# 1055 "pff.c"
FRESULT pf_lseek (
 DWORD ofs
)
{
 WORD clst;
 DWORD bcs, sect, ifptr;
 FATFS *fs = FatFs;


 if (!fs) return FR_NOT_ENABLED;
 if (!(fs->flag & 0x01)) return FR_NOT_OPENED;

 if (ofs > fs->fsize) ofs = fs->fsize;
 ifptr = fs->fptr;
 fs->fptr = 0;
 if (ofs > 0) {
  bcs = (DWORD)fs->csize * 512;
  if (ifptr > 0 &&
   (ofs - 1) / bcs >= (ifptr - 1) / bcs) {
   fs->fptr = (ifptr - 1) & ~(bcs - 1);
   ofs -= fs->fptr;
   clst = fs->curr_clust;
  } else {
   clst = fs->org_clust;
   fs->curr_clust = clst;
  }
  while (ofs > bcs) {
   clst = get_fat(clst);
   if (clst <= 1 || clst >= fs->n_fatent) {fs->flag = 0; return FR_DISK_ERR;};
   fs->curr_clust = clst;
   fs->fptr += bcs;
   ofs -= bcs;
  }
  fs->fptr += ofs;
  sect = clust2sect(clst);
  if (!sect) {fs->flag = 0; return FR_DISK_ERR;};
  fs->dsect = sect + (fs->fptr / 512 & (fs->csize - 1));
 }

 return FR_OK;
}
