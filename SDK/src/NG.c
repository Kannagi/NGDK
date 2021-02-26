#include "NGDK.h"
#include "asm.h"

#include "NG_arg.h"

void NG_Anim_Init(NG_Animation *anim)
{
	anim->frameCounter = 0;
	anim->frame = 0;
	anim->status = 0;
	anim->current = 0;
	anim->previous = 0;
}

void NG_Anim_Play_Fast(NG_Animation *anim, u16 frameWait, u16 frameMax)
{
	if(anim->current != anim->previous)
	{
		anim->frameCounter = 0;
		anim->frame = 0;
		anim->previous = anim->current;
	}

	anim->frameCounter++;
	if(anim->frameCounter >= frameWait)
	{
		anim->frameCounter = 0;
		anim->frame++;
	}

	if(anim->frame >= frameMax)
	{
		anim->frame = 0;
		anim->frameCounter = 0;
	}

}

u16 NG_Anim_Play(NG_Animation *anim, u16 frameWait, u16 frameMax)
{
	if(anim->current != anim->previous)
	{
		anim->frameCounter = 0;
		anim->frame = 0;
		anim->status = NG_ANIM_RUNNING; //or NG_ANIM_UPDATED ?
		anim->previous = anim->current;
		return NG_ANIM_UPDATED;
	}

	anim->frameCounter++;
	if(anim->frameCounter >= frameWait)
	{
		anim->frameCounter = 0;
		anim->frame++;
	}

	anim->status = NG_ANIM_RUNNING;
	if(anim->frame >= frameMax)
	{
		anim->frame = 0;
		anim->frameCounter = 0;
		anim->status = NG_ANIM_DONE;
		return NG_ANIM_DONE;
	}

	return NG_ANIM_RUNNING;
}

u8 addrssbss;
void NG_Fix_Print_Free_Mem(u16 x, u16 y)
{
	u16 var;
	u16 free1 = (u32)&var;
	u16 free2 = (u32)&addrssbss;
	u16 free = free1 - free2;
	NG_Fix_Print_u16(free,x,y);
	NG_Fix_Print_Tile(0x300+'d',x+5,y);
	//NG_Printh16(&addrssbss,x,y+1);
}

void NG_Fix_Print_Cycles(u16 x, u16 y)
{
	u16 cpu = (RW_REGISTER_U16(0x100002));

	NG_Fix_Print_u16(cpu,x,y);
}

static inline void joypad_test(u16 joypad, u16 *event, u16 test)
{
	if(joypad & test)
	{
		if(*event == NG_BUTTON_PRESSED)
			*event = NG_BUTTON_HOLD;

		if(*event == NG_BUTTON_NOT_PRESSED)
			*event = NG_BUTTON_PRESSED;
	}else
	{
		if(*event == NG_BUTTON_RELEASED)
			*event = NG_BUTTON_NOT_PRESSED;

		if(*event == NG_BUTTON_HOLD)
			*event= NG_BUTTON_RELEASED;

		if(*event == NG_BUTTON_PRESSED)
			*event = NG_BUTTON_NOT_PRESSED;
	}
}

u16 NG_Joypad1_Read(NG_joypad *pad)
{
	u16 joypad = RW_REGISTER_U16(0x100004);
	joypad = joypad>>8;
	joypad ^= 0xFF;

	pad->state = joypad;

	joypad_test(joypad, &pad->UP, 	NG_BUTTON_UP);
	joypad_test(joypad, &pad->DOWN, NG_BUTTON_DOWN);
	joypad_test(joypad, &pad->LEFT, NG_BUTTON_LEFT);
	joypad_test(joypad, &pad->RIGHT,NG_BUTTON_RIGHT);

	joypad_test(joypad, &pad->A, 	NG_BUTTON_A);
	joypad_test(joypad, &pad->B, 	NG_BUTTON_B);
	joypad_test(joypad, &pad->C, 	NG_BUTTON_C);
	joypad_test(joypad,	&pad->D, 	NG_BUTTON_D);

	return joypad;
}

u16 NG_Joypad2_Read(NG_joypad *pad)
{
	u16 joypad = RW_REGISTER_U16(0x100004);
	joypad = joypad&0xFF;
	joypad ^= 0xFF;

	pad->state = joypad;

	joypad_test(joypad, &pad->UP, NG_BUTTON_UP);
	joypad_test(joypad, &pad->DOWN, NG_BUTTON_DOWN);
	joypad_test(joypad, &pad->LEFT, NG_BUTTON_LEFT);
	joypad_test(joypad, &pad->RIGHT, NG_BUTTON_RIGHT);

	joypad_test(joypad, &pad->A, NG_BUTTON_A);
	joypad_test(joypad, &pad->B, NG_BUTTON_B);
	joypad_test(joypad, &pad->C, NG_BUTTON_C);
	joypad_test(joypad, &pad->D, NG_BUTTON_D);

	return joypad;
}

void NG_Fix_Clear()
{
	asm (
		"jsr BIOSF_CLEARFIX\n	"
		);
}

void NG_Palette_Load(const void *palette, u16 index)
{
	NG_arg1_u32 = (u32)palette;
	NG_arg1_u16 = index;
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

void NG_Palette_Load_N(const void *palette, u16 index, u16 n)
{
	NG_arg1_u32 = (u32)palette;
	NG_arg1_u16 = index;
	NG_arg2_u16 = n;
	asm (
		"move.l NG_arg1_u32,%a0\n	"

		"move.w	 NG_arg1_u16,%d0\n	"
		"asl.w	#5,%d0\n	"

		"lea     NG_PALETTE,%a1\n	"
		"add.w	 %d0,%a1\n	"

		"move.w  NG_arg2_u16,%d0\n	"
		"NG_Load_Pal_Loop:\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"

			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
			"move.l  (%a0)+,(%a1)+\n	"
		"dbra.w %d0,NG_Load_Pal_Loop\n	"
		);
}

void NG_Backdrop_Color_Set(u16 color)
{
	NG_arg1_u16 = color;
	asm (
		"lea     NG_PALETTE+0x1FFE,%a0\n	"
		"move.w  NG_arg1_u16,(%a0)\n	"
		);
}


void NG_Sprite_Clear_VRAM()
{
	asm (
		"jsr BIOSF_CLEARSPR\n	"
		);
}

void NG_Set_YM2610(u8 command)
{
	NG_arg1_u8 = command;
	asm (
		"move.b  NG_arg1_u8,%d0\n	"
		"move.b  %d0,REG_SOUND \n	"
		);
}

u8 NG_Get_YM2610()
{
	asm (
		"move.b  REG_SOUND,%d0 \n	"
		"rts\n	"
		);


		return 0;
}
