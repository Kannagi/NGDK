#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"


int VGM_data_pcm(int i,unsigned char *buffer)
{
	int size;

	i+=2;
	size = buffer[i] + (buffer[i+1]<<8)  + (buffer[i+2]<<16)  + (buffer[i+3]<<24);

	printf("PCM data ,type : %x , size : %d\n",buffer[i-1],size);
	int l;
	for(l = 0;l < size;l++)
	{
		//printf("%d\n",buffer[i+4+l]);
	}

	return size+6;
}

