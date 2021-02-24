
#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"

void NG_Background_Width_Set(u16 width)
{
	NG_BG_W = width << 2;
}

u16 NG_Background_Width_Get()
{
	return NG_BG_W >> 2;
}

void __attribute__((noinline)) NG_Background_Horizontal_Update(u16 id, u16 row, void *data)
{
	NG_arg1_u16 = id;
	NG_arg1_u32 = (u32)data;
	NG_arg2_u16 = row;
	asm (
		"movem.l %a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"lea     NG_Sprites,%a0\n	"
		"lea     VRAM_RW,%a1\n	"
		"move.l  NG_arg1_u32,%a2\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"asl.w   #4,%d0\n	"
		"add.w   %d0,%a0\n	"

		//ID
		"move.w  (%a0),%d0\n	"
		"asl.w   #6,%d0\n	"
		"lea     VRAM_ADDR,%a0\n	"

		"move.w  NG_arg2_u16,%d1\n	"
		"asl.w   #1,%d1\n	"
		"add.w   %d1,%d0\n	"

		"move.w  10(%a0),%d1\n	" //w
		"subi.w  #1,%d1\n	"

		//SCB1
		"NG_Background_Horizontal_Update_Loop:\n	"
			"move.w  %d0,(%a0)\n	"
			"addi.w  #0x40,%d0\n	"
			NG_NOP
			NG_NOP
			"move.w  (%a2)+,(%a1)\n	"
			NG_NOP3
			"move.w  (%a2)+,(%a1)\n	"
			NG_NOP
			NG_NOP
		"dbra.w %d1,NG_Background_Horizontal_Update_Loop\n	"

		"movem.l (%a7)+,%a2\n	"
		);
}

void __attribute__((noinline)) NG_Background_Vertical_Update(u16 id, u16 col, void *data)
{
	NG_arg1_u16 = id;
	NG_arg2_u16 = col;
	NG_arg1_u32 = (u32)data;
	asm (
		"movem.l %a2,-(%a7)\n	"		//save a2 to a7

		"move.w  #0x01,VRAM_MOD\n	"	//set VRAM increment to 0x01

		"lea     NG_Sprites,%a0\n	"	//a0 = NG_Sprites
		"lea     VRAM_RW,%a1\n	"		//a1 = VRAM_RW

		"move.w  NG_arg1_u16,%d0\n	"	//d0 = i (NG_arg1_u16)
		"andi.w  #0x7F,%d0\n	"		//d0 = d0 & 0x7f (cap d0 to 0x7f)
		"asl.w   #4,%d0\n	"			//d0 = d0 << 4
		"add.w   %d0,%a0\n	"			//a0 = a0 + d0 (updates pointer to sprite)

		//ID
		"move.w  (%a0),%d0\n	"		//d0 = VRAM sprite ID
		"add.w   NG_arg2_u16,%d0\n	"	//d0 = d0 + col (NG_arg2_u16)
		"asl.w   #6,%d0\n	"			//d0 = d0 << 6
		"move.w  %d0,VRAM_ADDR\n	"	//VRAM_ADDR = d0

		//height
		"move.w  6(%a0),%d1\n	"		//d1 = height (a0+6)
		"subq.w  #1,%d1\n	"			//d1 = d1 - 1

		"move.w  NG_BG_W,%d0\n	"		//d0 = map width
		"move.l  NG_arg1_u32,%a0\n	"	//a0 = map tile pointer (NG_arg1_u32)

		//SCB1
		"NG_Background_Vertical_Update_Loop:\n	"
			"move.w  (%a0),(%a1)\n	"	//write tile index (a0) to VRAM_RW (a1)
			NG_NOP3
			"move.w  2(%a0),(%a1)\n	"	//write tile attributes (a0+2) to VRAM_RW (a1)
			"add.w   %d0,%a0\n	"		//a0 = a0 + d0 (update map tile pointer to next line)
		"dbra.w %d1,NG_Background_Vertical_Update_Loop\n	"	//decrement d1 and branch if d1 > 0

		"movem.l (%a7)+,%a2\n	"		//restore a2 to a7
		);
}

void NG_Background_Scroll_X(u16 id, u16 offset, void *data)
{
	u16 col = NG_Sprites[id].x >> 4;	//let's divide by 16 to get 16px tile row
	if(col > 0) col = 4096 - col;		//reverse it if needed
	col += offset;						//add offset (to process left or right border for example)

	u16 xtmp = col << 2;				//multiply by 4 because map data uses 2 words per tile

	col = col & 0x1F;					//cap column index to 0x1f
	u8 *scroll = data;
	NG_Background_Vertical_Update(id, col, &scroll[xtmp]);
}
