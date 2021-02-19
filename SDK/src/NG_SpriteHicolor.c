
#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"

void NG_Sprite_HiColor_Init(u16 id, u16 x, u16 y, u16 width, u16 height, u16 tile, u16 flags)
{
	NG_Sprites[id].width = width;
	NG_Sprites[id].height = height;
	NG_Sprites[id].zoom = 0xFFF;
	NG_Sprites[id].tile = tile;
	NG_Sprites[id].flags = flags;

	NG_Sprites[id].x = 8 + x;
	NG_Sprites[id].y = 15 + y;

	id++;
	NG_Sprites[id].width = width;
	NG_Sprites[id].height = height;
	NG_Sprites[id].zoom = 0xFFF;
	NG_Sprites[id].tile = tile + (width * height);
	NG_Sprites[id].flags = flags + 0x0100;

	NG_Sprites[id].x = 8 + x;
	NG_Sprites[id].y = 15 + y;
}

void __attribute__((noinline)) NG_Sprite_HiColor_VRAM_Init(u16 id)
{
	NG_arg1_u16 = id;
	asm (
		"movem.l %d2-%d6/%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"asl.w   #4,%d0\n	"

		"lea     NG_Sprites,%a0\n	"
		"add.w   %d0,%a0\n	"

		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w  6(%a0),%d4\n	" //h
		"move.w  10(%a0),%d3\n	" //w
		"move.w  12(%a0),%d1\n	" //tile

		"move.w  NG_VRAM_sprite_id,%d5\n	"
		"move.w  %d5,(%a0)\n	" //id
		"move.w  %d5,%d2\n	"
		"add.w   %d3,%d2\n	"
		"move.w  %d2,0x10(%a0)\n	" //id HiColor
		"add.w   %d3,%d2\n	"
		"move.w  %d2,NG_VRAM_sprite_id\n	"

		//SCB3 Y position, Sticky bit, Sprite size
		"move.w  #SCB3,%d0\n	"
		"add.w   %d5,%d0\n	"
		"move.w  %d0,(%a1)\n	"

		"move.w  4(%a0),%d0\n	" //y
		"eor.w   #0xFFFF,%d0\n	"
		"asl.w   #7,%d0\n	"
		"or.w    %d4,%d0\n	"
		"ori.w   #0x40,%d0 \n	"

		"subi.w  #1,%d3\n	"
		"subi.w  #1,%d4\n	"
		"move.w  %d3,%d2\n	"

		"NG_Sprite_Init_Loop_A:\n	"
			"move.w  %d0,(%a2)\n	"
		"dbra.w %d2,NG_Sprite_Init_Loop_A\n	"

		//HiColor SCB3
		"move.w  #SCB3,%d0\n	"
		"add.w   %d5,%d0\n	"
		"add.w   %d3,%d0\n	"
		"addq.w   #1,%d0\n	"
		"move.w  %d0,(%a1)\n	"

		"move.w  %d3,%d2\n	"
		"NG_Sprite_Init_Loop_A_HiColor:\n	"
			"move.w  %d0,(%a2)\n	"
		"dbra.w %d2,NG_Sprite_Init_Loop_A_HiColor\n	"
		//−---------

		//SCB1
		"move.w  %d3,%d6\n	"

		"asl.w   #6,%d5\n	"
		"move.w  %d5,%d0\n	"

		"move.w  14(%a0),%d5\n	" //palette/tile/flip/auto

		"move.w %d0,(%a1)\n	"
		"addi.w  #0x40,%d0\n	"
		NG_NOP
		"NG_Sprite_Init_loop1:\n	"

			"move.w %d4,%d2\n	"

			"NG_Sprite_Init_loop2:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Init_loop2\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Init_loop1\n	"


		//SCB1 HiColor
		"move.w %d6,%d3\n	"
		"addi.w  #0x0100,%d5\n	" //palette/tile/flip/auto

		"NG_Sprite_Init_loop1HiColor:\n	"

			"move.w %d4,%d2\n	"

			"NG_Sprite_Init_loop2HiColor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Init_loop2HiColor\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Init_loop1HiColor\n	"

		"movem.l (%a7)+,%d2-%d6/%a2\n	"
		);
}

void __attribute__((noinline)) NG_Sprite_HiColor_Tiles_Update(u16 id)
{
	NG_arg1_u16 = id;
	asm (
		"movem.l %d2-%d7/%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"lsl.w   #4,%d0\n	"

		"lea     NG_Sprites,%a0\n	"
		"add.w   %d0,%a0\n	"

		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w  (%a0),%d6\n	"//id
		"move.w  6(%a0),%d4\n	" //h
		"move.w  10(%a0),%d3\n	" //w
		"move.w  12(%a0),%d1\n	" //tile
		"move.w  14(%a0),%d5\n	" //palette/tile/flip/auto

		"subi.w  #1,%d3\n	"
		"subi.w  #1,%d4\n	"

		"move.w  %d3,%d7\n	"

		//SCB1
		"btst  #0,%d5\n	"
		"bne NG_Sprite_Tiles_Update_FLIP_H\n	"

		"btst  #1,%d5\n	"
		"bne NG_Sprite_Tiles_Update_FLIP_V\n	"

		//No flip
		"move.w  %d6,%d0\n	"
		"lsl.w   #6,%d0\n	"

		"move.w %d0,(%a1)\n	"
		"addi.w  #0x40,%d0\n	"
		NG_NOP

		"NG_Sprite_Tiles_Update_loop1:\n	"

			"move.w %d4,%d2\n	"

			"NG_Sprite_Tiles_Update_loop2:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1\n	"

		//HiColor
		"move.w  %d7,%d3\n	"
		"add.w  #0x0100,%d5\n	"
		"NG_Sprite_Tiles_Update_loop1HiColor:\n	"

			"move.w %d4,%d2\n	"

			"NG_Sprite_Tiles_Update_loop2HiColor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2HiColor\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1HiColor\n	"

		"jmp NG_Sprite_Tiles_Update_end\n	"

		//--------------------
		//flip H
		"NG_Sprite_Tiles_Update_FLIP_H:\n	"

		"btst  #1,%d5\n	"
		"bne NG_Sprite_Tiles_Update_FLIP_HV\n	"

		"move.w  %d6,%d0\n	"
		"add.w   %d3,%d0\n	"
		"lsl.w   #6,%d0\n	"

		"move.w %d0,(%a1)\n	"
		"subi.w  #0x40,%d0\n	"
		NG_NOP

		"NG_Sprite_Tiles_Update_loop1H:\n	"

			"move.w %d4,%d2\n	"

			"NG_Sprite_Tiles_Update_loop2H:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2H\n	"


			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1H\n	"

		//HiColor
		"move.w  %d6,%d0\n	"
		"add.w   %d7,%d0\n	"
		"add.w   %d7,%d0\n	"
		"addq.w  #1,%d0\n	"
		"lsl.w   #6,%d0\n	"


		"move.w %d0,(%a1)\n	"
		"subi.w  #0x40,%d0\n	"

		"move.w  %d7,%d3\n	"
		"add.w  #0x0100,%d5\n	"

		"NG_Sprite_Tiles_Update_loop1HHiColor:\n	"

			"move.w %d4,%d2\n	"

			"NG_Sprite_Tiles_Update_loop2HHiColor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2HHiColor\n	"


			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1HHiColor\n	"
		"jmp NG_Sprite_Tiles_Update_end\n	"

		//--------------------
		//flip V
		"NG_Sprite_Tiles_Update_FLIP_V:\n	"

		"move.w  %d6,%d0\n	"
		"lsl.w   #6,%d0\n	"
		"move.w %d0,(%a1)\n	"
		"addi.w  #0x40,%d0\n	"
		"move.w %d4,%d6\n	"
		"addq.w #2,%d6\n	"

		"NG_Sprite_Tiles_Update_loop1V:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_Sprite_Tiles_Update_loop2V:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2V\n	"
			"add.w %d6,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1V\n	"

		//HiColor
		"move.w  %d7,%d3\n	"
		"add.w  #0x0100,%d5\n	"

		"NG_Sprite_Tiles_Update_loop1VHiColor:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_Sprite_Tiles_Update_loop2VHiColor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2VHiColor\n	"
			"add.w %d6,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1VHiColor\n	"

		"jmp NG_Sprite_Tiles_Update_end\n	"
		//--------------------
		//flip HV
		"NG_Sprite_Tiles_Update_FLIP_HV:\n	"

		"move.w  %d6,%d0\n	"
		"add.w   %d3,%d0\n	"
		"lsl.w   #6,%d0\n	"

		"move.w %d4,%d7\n	"
		"addq.w #2,%d7\n	"

		"move.w %d0,(%a1)\n	"
		"subi.w  #0x40,%d0\n	"

		"NG_Sprite_Tiles_Update_loop1HV:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_Sprite_Tiles_Update_loop2HV:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2HV\n	"
			"add.w %d7,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1HV\n	"

		//HiColor
		"move.w  10(%a0),%d3\n	" //w
		"subi.w  #1,%d3\n	"

		"move.w  %d6,%d0\n	"
		"add.w   %d3,%d0\n	"
		"add.w   %d3,%d0\n	"
		"addq.w  #1,%d0\n	"
		"lsl.w   #6,%d0\n	"

		"move.w %d0,(%a1)\n	"
		"subi.w #0x40,%d0\n	"
		"add.w  #0x0100,%d5\n	"

		"NG_Sprite_Tiles_Update_loop1HVHiColor:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_Sprite_Tiles_Update_loop2HVHiColor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_Sprite_Tiles_Update_loop2HVHiColor\n	"
			"add.w %d7,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1HVHiColor\n	"

		"NG_Sprite_Tiles_Update_end:\n	"
		"movem.l (%a7)+,%d2-%d7/%a2\n	"
		);
}

void __attribute__((noinline)) NG_Sprite_HiColor_Zoom_Update(u16 id)
{
	NG_arg1_u16 = id;
	asm (

		"move.w  #0x01,VRAM_MOD\n	"

		"lea     NG_Sprites,%a0\n	"
		"lea     VRAM_RW,%a1\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"andi.w  #0x7F,%d0\n	"
		"asl.w   #4,%d0\n	"
		"add.w   %d0,%a0\n	"

		//ID
		"move.w  (%a0),%d0\n	"
		"add.w   #SCB2,%d0\n	"
		"move.w  %d0,VRAM_ADDR\n	"

		//Zoom
		"move.w  2(%a0),%d0\n	"

		"move.w  10(%a0),%d1\n	" //w
		"subi.w  #1,%d1\n	"

		//SCB2
		"add.w  %d1,%d1\n	"
		"NG_Sprite_Tiles_UpdateW_loop:\n	"
			"move.w  %d0,(%a1)\n	"
		"dbra.w %d1,NG_Sprite_Tiles_UpdateW_loop\n	"
		);
}

void __attribute__((noinline)) NG_Sprite_HiColor_Update(u16 id)
{
	NG_arg1_u16 = id;
	asm (
		"movem.l %a2,-(%a7)\n	"

		"move.w  #0x200,VRAM_MOD\n	"

		"lea     NG_Sprites,%a0\n	"
		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"andi.w  #0x7F,%d0\n	"
		"asl.w   #4,%d0\n	"
		"add.w   %d0,%a0\n	"

		//ID
		"move.w  (%a0),%d0\n	"
		"add.w   #SCB2,%d0\n	"
		"move.w  %d0,(%a1)\n	"

		//Zoom
		"move.w  2(%a0),(%a2)\n	"

		//Y position , size sprite
		"move.w 4(%a0),%d0\n	"
		"eor.w  #0xFFFF,%d0\n	"
		"asl.w  #7,%d0\n	"
		"or.w   6(%a0),%d0\n	"
		"move.w %d0,(%a2)\n	"
		"move.w %d0,%d1\n	"

		//X Position
		"move.w  8(%a0),%d0\n	"
		"asl.w   #7,%d0\n	"
		"move.w  %d0,(%a2)\n	"

		//----------------
		//ID
		"move.w  0x10(%a0),%d0\n	"
		"add.w   #SCB2,%d0\n	"
		"move.w  %d0,(%a1)\n	"

		//Zoom
		"move.w  2(%a0),(%a2)\n	"

		//Y position , size sprite
		"move.w %d1,%d0\n	"
		"move.w %d0,(%a2)\n	"

		//X Position
		"move.w  8(%a0),%d0\n	"
		"asl.w   #7,%d0\n	"
		"move.w  %d0,(%a2)\n	"

		"movem.l (%a7)+,%a2\n	"
		);
}
