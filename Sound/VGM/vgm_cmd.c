#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"


int VGM_cmd_unk(int cmd)
{
	int i = 0;

	if( (cmd >= 0x70) && (cmd <= 0x7F) )
	{

	}

	//YM2610 port 0 address 2A
	if( (cmd >= 0x80) && (cmd <= 0x8F) )
	{
		//printf("cmd inconnu2 %x %x\n",i,cmd);exit(0);


	}

	if( (cmd >= 0x40) && (cmd <= 0x4E) )
		i++;

	if( (cmd >= 0x51) && (cmd <= 0x60) )
		i++;

	if( (cmd >= 0xA0) && (cmd <= 0xBF) )
		i++;

	if( (cmd >= 0xC0) && (cmd <= 0xDF) )
		i+=2;

	if( (cmd >= 0xE0) && (cmd <= 0xFF) )
		i+=3;

	printf("cmd inconnu %x %x\n",i,cmd);

	//exit(0);

	return i;
}
