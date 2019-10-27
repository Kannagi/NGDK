
#include "NGDK.h"
#include "asm.h"

#include "NG_arg.h"

void NG_Cycle(u16 x,u16 y)
{
	u16 cpu= RW_REGISTER_U16(0x100002)*7;


	NG_Printu16(20034-cpu,x,y);
	NG_PrintFix(0x300+'0',x+5,y);
}

/*
u16 NG_CPU()
{
	u16 cpu= RW_REGISTER_U16(0x100002);


	return cpu;
	u16 percent = 100;
	if(cpu >= 1431)
	{
		cpu -= 1431;
		percent -= 50;
	}

	if(cpu >= 572)
	{
		cpu -= 572;
		percent -= 20;
	}

	if(cpu >= 572)
	{
		cpu -= 572;
		percent -= 20;
	}

	if(cpu >= 286)
	{
		cpu -= 286;
		percent -= 10;
	}

	if(cpu >= 143)
	{
		cpu -= 143;
		percent -= 5;
	}

	if(cpu >= 28)
	{
		cpu -= 28;
		percent -= 1;
	}

	if(cpu >= 28)
	{
		cpu -= 28;
		percent -= 1;
	}

	if(cpu >= 28)
	{
		cpu -= 28;
		percent -= 1;
	}

	if(cpu >= 28)
	{
		cpu -= 28;
		percent -= 1;
	}
	return percent;
}
*/
/*
joypad_event macro
	move.b \3,d0
	andi.b #\1,d0
	cmpi.b #\1,d0

	if_ne
		clr.b MEM_STDCTRL+\2
	endi


	if_eq
		cmpi.w #$00,MEM_STDCTRL+\2
		if_eq
			move.b #2,MEM_STDCTRL+\2
		else_
			move.b #1,MEM_STDCTRL+\2
		endie
	endi
endm
*/
static inline void Joypad(u16 joypad,u16 *event,u16 test)
{
	if(joypad&test)
	{
		if(*event == 1)
			*event = 2;

		if(*event == 0)
			*event = 1;
	}else
	{
		if(*event == 3)
			*event = 0;

		if(*event == 2)
			*event= 3;
	}
}

u16 NG_Joypad1(NG_Joypads *pad)
{
	u16 joypad = RW_REGISTER_U16(0x100004);
	joypad = joypad>>8;
	joypad ^= 0xFF;

	Joypad(joypad,&pad->UP,0x01);
	Joypad(joypad,&pad->DOWN,0x02);
	Joypad(joypad,&pad->LEFT,0x04);
	Joypad(joypad,&pad->RIGHT,0x08);


	Joypad(joypad,&pad->A,0x10);
	Joypad(joypad,&pad->B,0x20);
	Joypad(joypad,&pad->C,0x40);
	Joypad(joypad,&pad->D,0x80);

	return joypad;
}

u16 NG_Joypad2(NG_Joypads *pad)
{
	u16 joypad = RW_REGISTER_U16(0x100004);
	joypad = joypad&0xFF;
	joypad ^= 0xFF;

	Joypad(joypad,&pad->UP,0x01);
	Joypad(joypad,&pad->DOWN,0x02);
	Joypad(joypad,&pad->LEFT,0x04);
	Joypad(joypad,&pad->RIGHT,0x08);


	Joypad(joypad,&pad->A,0x10);
	Joypad(joypad,&pad->B,0x20);
	Joypad(joypad,&pad->C,0x40);
	Joypad(joypad,&pad->D,0x80);

	return joypad;
}




void NG_ClearFix()
{
	asm (
		"movem.l %d0-%d2/%a0,-(%a7)\n	"

		"move.w  #0x1,VRAM_MOD\n	"
		"move.w  #0x00FF,%d1\n	"
		"move.w  #FIXMAP+0x22,%d2\n	"
		"lea VRAM_RW,%a0\n	"

		"move.l  #37,%d0\n	" //0x4E0
		"NG_Clear_Fix_loop:\n	"
			"move.w  %d2,VRAM_ADDR\n	"

			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"

			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"

			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"

			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"move.w  %d1,(%a0)\n	"
			"add.w   #0x20,%d2\n	"
		"dbra.w %d0,NG_Clear_Fix_loop\n	"

		"movem.l (%a7)+,%d0-%d2/%a0\n	"

		);
}

void NG_LoadPal(u16 *palette,u16 address)
{
	NG_arg1 = palette;
	NG_arg1_u16 = address;
	asm (
		"movem.l %d0/%a0-%a1,-(%a7)\n	"

		"lea NG_arg1,%a1\n	"
		"move.l (%a1),%a0\n	"

		"move.w	 NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"

		"lea     NG_PALETTE,%a1\n	"
		"add.w	 %d0,%a1\n	"

		"move.l  (%a0)+,(%a1)+\n	"
		"move.l  (%a0)+,(%a1)+\n	"
		"move.l  (%a0)+,(%a1)+\n	"
		"move.l  (%a0)+,(%a1)+\n	"

		"move.l  (%a0)+,(%a1)+\n	"
		"move.l  (%a0)+,(%a1)+\n	"
		"move.l  (%a0)+,(%a1)+\n	"
		"move.l  (%a0)+,(%a1)+\n	"

		"movem.l (%a7)+,%d0/%a0-%a1\n	"
		);
}

void NG_LoadPalN(u16 *palette,u16 address,u16 num)
{
	NG_arg1 = palette;
	NG_arg1_u16 = address;
	NG_arg2_u16 = num;
	asm (
		"movem.l %d0/%a0-%a1,-(%a7)\n	"

		"lea NG_arg1,%a1\n	"
		"move.l (%a1),%a0\n	"

		"move.w	 NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"

		"lea     NG_PALETTE,%a1\n	"
		"add.w	 %d0,%a1\n	"

		"move.w  NG_arg2_u16,%d0\n	"
		"NG_Load_Pal_loop:\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"

			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
		"dbra.w %d0,NG_Load_Pal_loop\n	"

		"movem.l (%a7)+,%d0/%a0-%a1\n	"
		);
}

void NG_BGColor(u16 color)
{
	NG_arg1_u16 = color;
	asm (
		"movem.l %d0/%a0,-(%a7)\n	"

		"lea     NG_PALETTE+0x1FFE,%a0\n	"
		"move.w  NG_arg1_u16,(%a0)\n	"

		"movem.l (%a7)+,%d0/%a0\n	"
		);
}
