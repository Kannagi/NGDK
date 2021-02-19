



typedef struct
{
	int tag,eof,version,gd3,totalsample,loopoffset;
	int loopsample,rate,data,YM2608,YM2610;
	int SSGslow,wait,cmde,size,ifm1,ifm2;
	int sizet[8];

	int SSG[0x10],ADPCM_A[0x10],ADPCM_B[0x10],FM1[0x90],FM2[0x90];
}VGM;

void VGM_SSG_Write(VGM *vgm,FILE *file);
int  VGM_Load(void);
void VGM_Read_YM2608(VGM vgm,unsigned char *buffer);

int VGM_cmd_unk(int cmd);
int VGM_data_pcm(int i,unsigned char *buffer);
void VGM_delay(VGM *vgm,FILE *file,int delaynew);
void VGM_Init(VGM *vgm);
void VGM_Print_size(VGM *vgm);

void VGM_SSG(VGM *vgm,FILE *file,int adr,int data);
void VGM_ADPCMA(VGM *vgm,FILE *file,int adr,int data);
void VGM_ADPCMB(VGM *vgm,FILE *file,int adr,int data);
void VGM_FM_Commons(VGM *vgm,FILE *file,int adr,int data);
void VGM_FM_Port0(VGM *vgm,FILE *file,int adr,int data);
void VGM_FM_Port1(VGM *vgm,FILE *file,int adr,int data);
void VGM_FM_Other(VGM *vgm,FILE *file,int adr,int data);
