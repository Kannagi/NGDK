#include <stdio.h>
#include <stdlib.h>

#include "vgm.h"

void VGM_FM_Port0(VGM *vgm,FILE *file,int adr,int data)
{
	int j;

	j = adr-0x30;
	int ch = 0,op1 = 0,op2 = 0,tch,tmp;



	//if( (adr&3) == 0)
	if(vgm->FM1[j] != data)
	{
		vgm->FM1[j] = data;
		tch  = (j&0x03);

		ch = 0x40;
		if(tch == 1)
			ch = 0x60;

		if(tch == 2)
			ch = 0x80;


		if(adr < 0xA0)
		{
			op1 = (j&0x3C)>>2;
			op2 = ((j&0xC0)>>6);

			if(op2 == 1)
				ch += 0x10;

			tmp = op1&3;

			if(tmp == 1)
			{
				op1++;
			}else
			{
				if(tmp == 2)
				{
					op1--;
				}
			}

			tmp = ch+op1;


			//printf("FM %d %x %x",vgm->wait,adr,data);
			//printf(" / %x %x / %x %x\n",tmp,data,op1&3,op2);

			fputc(tmp,file);
			fputc(data,file);
		}else
		{
			int fn2 = 0;
			if(adr == 0xA8) fn2 = 3;
			if(adr == 0xA9) fn2 = 1;
			if(adr == 0xAA) fn2 = 2;

			if(adr == 0xAC) fn2 = 6;
			if(adr == 0xAD) fn2 = 4;
			if(adr == 0xAE) fn2 = 5;

			if(fn2 != 0)
			{
				//printf("FM FTX error \n");
				return;
			}else
			{

				j = adr-0xA0;
				op1 = ((j&0xF0)>>3);
				op2 = ((j&0x04)>>2);

				tmp = ch+op1+op2+0x1C;
				fputc(tmp,file);
				fputc(data,file);

				//vgm->sizet[4]--;
				//vgm->size--;
			}
		}

		vgm->size+=2;
		vgm->sizet[4]+=2;
		vgm->cmde++;

		//printf("FM %d %x %x",vgm->wait,adr,data);
		//printf(" / %x %x\n",tmp,data);
	}
}

void VGM_FM_Port1(VGM *vgm,FILE *file,int adr,int data)
{
	int j;

	if( (adr&3) == 0)
	{
		int ch = 0,op1 = 0,op2 = 0,tmp;

		j = adr-0x30;
		if(vgm->FM1[j] != data)
		{
			vgm->FM1[j] = data;

			ch = 0xA0;

			if(adr < 0xA0)
			{
				op1 = ((j&0x3C)>>2);
				op2 = ((j&0xC0)>>6);

				if(op2 == 1)
					ch += 0x10;

				tmp = op1&3;

				if(tmp == 1)
				{
					op1++;
				}else
				{
					if(tmp == 2)
					{
						op1--;
					}
				}

				tmp = ch+op1;
				fputc(tmp,file);
				fputc(data,file);
			}else
			{
				j = adr-0xA0;
				op1 = ((j&0xF0)>>3);
				op2 = ((j&0x04)>>2);

				tmp = ch+op1+op2+0x1C;
				fputc(tmp,file);
				fputc(data,file);

				//vgm->sizet[4]--;
				//vgm->size--;
			}

			vgm->size+=2;
			vgm->sizet[4]+=2;
			vgm->cmde++;
		}
	}

}

void VGM_FM_Commons(VGM *vgm,FILE *file,int adr,int data)
{

	int tmp = data&0xF;
	int tmp2;

	if( (adr == 0x28) && (tmp <=4) )
	{
		tmp2 = 0;

		if(tmp == 1)
			tmp2 = 1;

		if(tmp == 2)
			tmp2 = 2;

		if(tmp == 4)
			tmp2 = 3;

		tmp2 += (data&0xF0)>>2;
		//fputc(tmp2+0xC0,file);



		tmp2 = 1;

		if(tmp == 1)
			tmp2 = 2;

		if(tmp == 2)
			tmp2 = 5;

		if(tmp == 4)
			tmp2 = 6;

		tmp2 += (data&0xF0);

		fputc(0x3F,file);
		fputc(tmp2,file);

		vgm->size+=2;
		vgm->cmde++;
		vgm->sizet[3]+=2;

		//printf("FM %d %x %x",vgm->wait,adr,data);
		//printf(" / %x %x\n",0x3F,tmp2);

	}else
	{
		static int dataold = -1;

		if( (data != dataold) && (adr == 0x22) )
		{
			dataold = data;
			fputc(0x3E,file);
			fputc(data,file);

			vgm->size+=2;
			vgm->sizet[3]+=2;
			vgm->cmde++;
			//printf("FM %d %x %x\n",vgm->wait,adr,data);
		}


	}


}
