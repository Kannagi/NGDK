
#include "NGDK.h"
#include "asm.h"

#include "NG_arg.h"

void NG_SetYM2612(u8 command)
{
	NG_arg1_u8 = command;
	asm (
		"move.b  NG_arg1_u8,%d0\n	"
		"move.b  %d0,REG_SOUND \n	"
		);
}

void NG_Anim_Play_Fast(NG_Anim *anim,u16 cadence,u16 n)
{
    if(anim->act != anim->old)
    {
        anim->itmp = 0;
        anim->i = 0;
        anim->old = anim->act;
    }

    anim->itmp++;
    if(anim->itmp >= cadence)
    {
        anim->itmp = 0;
        anim->i++;
    }

    if(anim->i >= n)
    {
        anim->i = 0;
        anim->itmp = 0;
    }

}

u16 NG_Anim_Play(NG_Anim *anim,u16 cadence,u16 n)
{
    if(anim->act != anim->old)
    {
        anim->itmp = 0;
        anim->i = 0;
        anim->end = 0;
        anim->old = anim->act;
        return 2;
    }

    anim->itmp++;
    if(anim->itmp >= cadence)
    {
        anim->itmp = 0;
        anim->i++;
    }

    anim->end = 0;
    if(anim->i >= n)
    {
        anim->i = 0;
        anim->itmp = 0;
        anim->end = 1;
        return 1;
    }

    return 0;
}

void NG_Anim_Init(NG_Anim *anim)
{
    anim->itmp = 0;
    anim->i = 0;
    anim->end = 0;
    anim->act = 0;
    anim->old = 0;
}


u8 addrssbss;
void NG_PrintFreeMem(u16 x,u16 y)
{
	u16 var;
	u16 free1 = (u32)&var;
	u16 free2 = (u32)&addrssbss;
	u16 free = free1 - free2;
	NG_Printu16(free,x,y);
	NG_PrintFix(0x300+'d',x+5,y);
	//NG_Printh16(&addrssbss,x,y+1);
}

void NG_Cycle(u16 x,u16 y)
{
	u16 cpu = (RW_REGISTER_U16(0x100002));

	NG_Printu16(cpu,x,y);
}

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
		"jsr BIOSF_CLEARFIX\n	"
		);
}

void NG_LoadPal(const void *palette,u16 address)
{
	NG_arg1_u32 = (u32)palette;
	NG_arg1_u16 = address;
	asm (
		"move.l NG_arg1_u32,%a0\n	"

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
		);
}

void NG_LoadPalN(const void *palette,u16 address,u16 num)
{
	NG_arg1_u32 = (u32)palette;
	NG_arg1_u16 = address;
	NG_arg2_u16 = num;
	asm (
		"move.l NG_arg1_u32,%a0\n	"

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
		);
}

void NG_BGColor(u16 color)
{
	NG_arg1_u16 = color;
	asm (
		"lea     NG_PALETTE+0x1FFE,%a0\n	"
		"move.w  NG_arg1_u16,(%a0)\n	"
		);
}


void __attribute__((noinline)) NG_SpriteClearVRAM()
{
	asm (
		"jsr BIOSF_CLEARSPR\n	"
		);
}
