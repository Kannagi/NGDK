#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"


void NG_Prints8(s8 val,u16 x,u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u16 = (u8)val;

	asm (
		"movem.l %d2-%d4,-(%a7)\n	"

		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d1\n	"
		"asl.w	#5,%d1\n	"
		"add.w  #FIXMAP+0x22,%d1\n	"
		"add.w	NG_arg2_u16,%d1\n	"
		"move.w  %d1,VRAM_ADDR\n	"

		"move.w NG_arg3_u16,%d2\n	"

		"move.w	NG_PAL_ID,%d3\n	"
		"add.w	#FIX_ASCII+'0',%d3\n	"

		"lea VRAM_RW,%a0\n	"

		"cmpi #129,%d2\n	"
		"bmi.w NG_Print_s8_neg\n	"
			"move.w  #FIX_ASCII+'-',(%a0)\n	"

			"eor.w #0xFF,%d2\n	"
			"addq.w #0x1,%d2\n	"

		"NG_Print_s8_neg:\n	"


		"move.w #100,%d4\n	"
		"move.w %d3,%d0\n	"
		"digit_convert 111\n	"
		"digit_convert 112\n	"
		"move.w  %d0,(%a0)\n	"

		"digit_convert10 110\n	"

		NG_NOP
		"move.w %d3,%d0\n	"
		"add.w	%d2,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"move.w #0x0320,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"movem.l (%a7)+,%d2-%d4\n	"
		);
}

void NG_Printu8(u8 val,u16 x,u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u16 = val;

	asm (
		"movem.l %d2-%d4,-(%a7)\n	"

		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d1\n	"
		"asl.w	#5,%d1\n	"
		"add.w  #FIXMAP+0x22,%d1\n	"
		"add.w	NG_arg2_u16,%d1\n	"
		"move.w  %d1,VRAM_ADDR\n	"

		"move.w NG_arg3_u16,%d2\n	"

		"move.w	NG_PAL_ID,%d3\n	"
		"add.w	#FIX_ASCII+'0',%d3\n	"

		"lea VRAM_RW,%a0\n	"

		"move.w #100,%d4\n	"
		"move.w %d3,%d0\n	"
		"digit_convert 101\n	"
		"digit_convert 102\n	"
		"move.w  %d0,(%a0)\n	"

		"digit_convert10 100\n	"

		NG_NOP
		"move.w %d3,%d0\n	"
		"add.w	%d2,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"movem.l (%a7)+,%d2-%d4\n	"
		);
}

void NG_Printh8(u8 val,u16 x,u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u16 = val;

	asm (
		"movem.l %d2-%d4,-(%a7)\n	"

		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d1\n	"
		"asl.w	#5,%d1\n	"
		"add.w  #FIXMAP+0x22,%d1\n	"
		"add.w	NG_arg2_u16,%d1\n	"
		"move.w  %d1,VRAM_ADDR\n	"

		"move.w NG_arg3_u16,%d2\n	"

		"move.w	NG_PAL_ID,%d3\n	"
		"add.w	#FIX_ASCII+'0',%d3\n	"
		"move.w	%d3,%d4\n	"
		"add.w	#'A'-'0'-10,%d4\n	"

		"lea VRAM_RW,%a0\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 12\n	"

		"mov.w	%d2,%d1\n	"
		"hexa_convert 13\n	"


		"movem.l (%a7)+,%d2-%d4\n	"
		);
}
