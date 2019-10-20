
#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"


void NG_BGScrollx(const void *data,u16 w,u16 i)
{

	u16 itmp = NG_Sprite[i].x>>4;
	if(itmp > 0) itmp = 4096-itmp;
	itmp += w;

	u16 xtmp = itmp<<2;

	itmp = itmp&0x1F;
	NG_SpriteUpdateMapVertical(&data[xtmp],i,itmp);

}


void NG_BGWidthSet(u16 w)
{
	NG_BG_W = w<<2;
}

u16 NG_BGWidthGet()
{
	return NG_BG_W>>2;
}

void __attribute__((noinline)) NG_SpriteUpdateMapHorizontal(const void *tile,u16 i,u16 offset)
{
	NG_arg1_u16 = i;
	NG_arg1_u32 = tile;
	NG_arg2_u16 = offset;
	asm (
		"movem.l %d0-%d3/%a0-%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"lea     NG_Sprite,%a0\n	"
		"lea     VRAM_RW,%a1\n	"
		"move.l NG_arg1_u32,%a2\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"andi.w  #0x7F,%d0\n	"
		"asl.w   #4,%d0\n	"
		"add.w   %d0,%a0\n	"

		"move.w  10(%a0),%d1\n	" //w
		"subi.w  #1,%d1\n	"

		//ID
		"move.w  (%a0),%d0\n	"
		"asl.w   #6,%d0\n	"
		"lea     VRAM_ADDR,%a0\n	"
		"move.w  #0x40,%d2\n	"

		"move.w   NG_arg2_u16,%d3\n	"
		"asl.w   #1,%d3\n	"
		"add.w   %d3,%d0\n	"


		//SCB1
		"NG_SpriteUpdateMapHorizontal_loop:\n	"
			"move.w  %d0,(%a0)\n	"
			"move.w  (%a2)+,(%a1)\n	"
			"move.w  (%a2)+,(%a1)\n	"
			"add.w   %d2,%d0\n	"
		"dbra.w %d1,NG_SpriteUpdateMapHorizontal_loop\n	"

		"movem.l (%a7)+,%d0-%d3/%a0-%a2\n	"
		);
}


void __attribute__((noinline)) NG_SpriteUpdateMapVertical(const void *tile,u16 i,u16 offset)
{
	NG_arg1_u16 = i;
	NG_arg2_u16 = offset;
	NG_arg1_u32 = tile;
	asm (
		"movem.l %d0-%d1/%a0-%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"lea     NG_Sprite,%a0\n	"
		"lea     VRAM_RW,%a1\n	"
		"move.l  NG_arg1_u32,%a2\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"andi.w  #0x7F,%d0\n	"
		"asl.w   #4,%d0\n	"
		"add.w   %d0,%a0\n	"

		//ID
		"move.w  (%a0),%d0\n	"
		"add.w   NG_arg2_u16,%d0\n	"
		"asl.w   #6,%d0\n	"
		"move.w  %d0,VRAM_ADDR\n	"

		"move.w  6(%a0),%d1\n	" //h
		"subi.w  #1,%d1\n	"

		"move.w  NG_BG_W,%d0\n	"

		//SCB1
		"NG_SpriteUpdateMapVertical_loop:\n	"
			"move.w  (%a2),(%a1)\n	"
			"move.w  2(%a2),(%a1)\n	"
			"add.w   %d0,%a2\n	"
		"dbra.w %d1,NG_SpriteUpdateMapVertical_loop\n	"

		"movem.l (%a7)+,%d0-%d1/%a0-%a2\n	"
		);
}

