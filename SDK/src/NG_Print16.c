#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"


void NG_Fix_Print_s16(s16 val,u16 x,u16 y)
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

		"btst #0xF,NG_arg3_u16\n	"
		"beq.w NG_Print_s16_neg\n	"
			"move.w  #FIX_ASCII+'-',(%a0)\n	"

			"eor.w #0xFFFF,%d2\n	"
			"addq.w #0x1,%d2\n	"

		"NG_Print_s16_neg:\n	"

		"move.w %d3,%d0\n	"
		"move.w #10000,%d4\n	"
		"digit_convert 0\n	"
		"digit_convertA 0\n	"
		"digit_convertB 0\n	"
		"move.w  %d0,(%a0)\n	"

		"digit_convert1000 1\n	"
		"digit_convert100 2\n	"
		"digit_convert10 3\n	"

		"move.w %d3,%d0\n	"
		"add.w	%d2,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"move.w #0x0320,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"movem.l (%a7)+,%d2-%d4\n	"
		);
}

void NG_Fix_Print_u16(u16 val,u16 x,u16 y)
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

		"move.w %d3,%d0\n	"
		"move.w #10000,%d4\n	"

		"btst #0xF,NG_arg3_u16\n	"
		"beq.w NG_Print_u16_neg\n	"
			"sub.w #30000,%d2\n	"
			"addq.w	#3,%d0\n	"
		"NG_Print_u16_neg:\n	"

		"digit_convert 4\n	"
		"digit_convertA 4\n	"
		"digit_convertB 4\n	"
		"move.w  %d0,(%a0)\n	"

		"digit_convert1000 5\n	"
		"digit_convert100 6\n	"
		"digit_convert10 7\n	"

		NG_NOP
		"move.w %d3,%d0\n	"
		"add.w	%d2,%d0\n	"
		"move.w %d0,(%a0)\n	"

		"movem.l (%a7)+,%d2-%d4\n	"
		);
}

void NG_Fix_Print_h16(u16 val,u16 x,u16 y)
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
		"asr.w	#8,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 10\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"hexa_convert 11\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 12\n	"

		"mov.w	%d2,%d1\n	"
		"hexa_convert 13\n	"


		"movem.l (%a7)+,%d2-%d4\n	"
		);
}

void NG_Fix_Print_h32(u32 val,u16 x,u16 y)
{
	NG_arg1_u16 = x;
	NG_arg2_u16 = y;
	NG_arg3_u32 = val;

	asm (
		"movem.l %d2-%d4,-(%a7)\n	"

		"move.w  #0x20,VRAM_MOD\n	"

		"move.w	NG_arg1_u16,%d1\n	"
		"asl.w	#5,%d1\n	"
		"add.w  #FIXMAP+0x22,%d1\n	"
		"add.w	NG_arg2_u16,%d1\n	"
		"move.w  %d1,VRAM_ADDR\n	"


		"move.w	NG_PAL_ID,%d3\n	"
		"add.w	#FIX_ASCII+'0',%d3\n	"
		"move.w	%d3,%d4\n	"
		"add.w	#'A'-'0'-10,%d4\n	"

		"lea VRAM_RW,%a0\n	"

		"move.w NG_arg3_u32,%d2\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 40\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"hexa_convert 41\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 42\n	"

		"mov.w	%d2,%d1\n	"
		"hexa_convert 43\n	"

		"move.w NG_arg3_u32+2,%d2\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 44\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#8,%d1\n	"
		"hexa_convert 45\n	"

		"mov.w	%d2,%d1\n	"
		"asr.w	#4,%d1\n	"
		"hexa_convert 46\n	"

		"mov.w	%d2,%d1\n	"
		"hexa_convert 47\n	"


		"movem.l (%a7)+,%d2-%d4\n	"
		);
}
