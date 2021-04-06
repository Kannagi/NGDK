#ifndef __RETRO__
#define __RETRO__

void bloc_palette(SDL_Surface *image,int blocx,int blocy,int blocw,unsigned char *pixel,unsigned char *palette,int *tiles);
int load_palette_ext(unsigned char *palette,char *addresspal);
int load_palette(SDL_Surface *image,unsigned char *palette,int noalpha);
int load_palette_sp(SDL_Surface *image,unsigned char *palette,int noalpha);
void output_filename(char *address,char *str);

int nes_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode);
int nes_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type);

int sms_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode);
int sms_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type);

int pce_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode);
int pce_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type);

int md_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode);
int md_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type);

int snes_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode);
int snes_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type);

int ng_write_pal(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int mode);
int ng_write_rom(FILE *file,SDL_Surface *image,unsigned char *palette,int ncolor,int type);

void retro_convert(SDL_Surface *image,char *address,char *addresspal,int *option,int num);
void tilemap(SDL_Surface *imagein,char *address);

#endif



