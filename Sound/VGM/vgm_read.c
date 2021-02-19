#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"

void VGM_Read_YM2608(VGM vgm,unsigned char *buffer)
{
	int i = vgm.data+52;
	float t;
	t = vgm.totalsample*(1.0f/44100.0f);
	printf("time %.4f s\n",t);

	printf("----------\n");

	int end = vgm.eof;
	if(vgm.gd3 < vgm.eof)
		end = vgm.gd3;

	VGM_Init(&vgm);

	//-------------------
	FILE *file = fopen("test.ngm","wb");
	int delay;

	int cmd,adr,data;

	while(i < end)
	{
		cmd  = buffer[i];
		adr  = buffer[i+1];
		data = buffer[i+2];
		i++;

		//printf("CMD %x %x %x\n",cmd,adr,data);

		switch(cmd)
		{
			//YM2608 Port 0
			case 0x56:

				//SSG
				if( (adr >= 0x00) && (adr <= 0x0D) )
					VGM_SSG(&vgm,file,adr,data);

				//Rhytme(ADPCM-A)
				if( (adr >= 0x10) && (adr <= 0x1D) )
					VGM_ADPCMA(&vgm,file,adr,data);

				//FM Commons
				if( (adr >= 0x20) && (adr <= 0x28) )
					VGM_FM_Commons(&vgm,file,adr,data);

				//FM CH1-CH3
				if( (adr >= 0x30) && (adr <= 0xBF) ) //OP
					VGM_FM_Port0(&vgm,file,adr,data);

				i+=2;
			break;

			//YM2608 Port 1
			case 0x57:
				//ADPCM (ADPCM-B)
				if( (adr >= 0x00) && (adr <= 0x10) )
					VGM_ADPCMB(&vgm,file,adr,data);

				//FM CH4-CH6
				if( (adr >= 0x30) && (adr <= 0xBF) ) //OP
					VGM_FM_Port1(&vgm,file,adr,data);

				i+=2;
			break;

			//YM2610 Port 0
			case 0x58:
				i+=2;
			break;

			//YM2610 Port 1
			case 0x59:
				i+=2;
			break;

			//Delay
			case 0x61:
				delay = buffer[i] + (buffer[i+1]<<8);
				VGM_delay(&vgm,file,delay);

				i+=2;

			break;

			case 0x62: //Delay 1/60 second
				VGM_delay(&vgm,file,735);
			break;

			case 0x63: //Delay 1/50 second
				VGM_delay(&vgm,file,882);
			break;


			//Delay
			case 0x64:
				printf("cmd wait error\n");
				i+=2;
			break;

			//END
			case 0x66:
				printf("cmd end\n");
			break;

			//DATA PCM
			case 0x67:
				i+= VGM_data_pcm(i,buffer);
			break;

			default:
				//printf("cmd  error %d %x\n",i,buffer[i-2]);
				i += VGM_cmd_unk(cmd);
				break;
		}

	}

	VGM_Print_size(&vgm);

	fputc(0x1E,file);
	fclose(file);

}

