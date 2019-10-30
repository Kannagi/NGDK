
#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"

void NG_SpriteInitHicolor(u16 i,u16 x,u16 y,u16 w,u16 h,u16 tile,u16 flag)
{
	NG_Sprite[i].w = w;
	NG_Sprite[i].h = h;
	NG_Sprite[i].zoom = 0xFFF;
	NG_Sprite[i].tile = tile;
	NG_Sprite[i].flag = flag;

	NG_Sprite[i].x = 8+x;
	NG_Sprite[i].y = 15+y;

	i++;
	NG_Sprite[i].w = w;
	NG_Sprite[i].h = h;
	NG_Sprite[i].zoom = 0xFFF;
	NG_Sprite[i].tile = tile+(w*h);
	NG_Sprite[i].flag = flag+0x0100;

	NG_Sprite[i].x = 8+x;
	NG_Sprite[i].y = 15+y;
}

void __attribute__((noinline)) NG_SpriteUpdateInitHicolor(u16 i)
{
	NG_arg1_u16 = i;
	asm (
		"movem.l %d2-%d6/%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"asl.w   #4,%d0\n	"

		"lea     NG_Sprite,%a0\n	"
		"add.w   %d0,%a0\n	"

		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w  6(%a0),%d4\n	" //h
		"move.w  10(%a0),%d3\n	" //w
		"move.w  12(%a0),%d1\n	" //tile

		"move.w  NG_ID,%d5\n	"
		"move.w  %d5,(%a0)\n	" //id
		"move.w  %d5,%d2\n	"
		"add.w   %d3,%d2\n	"
		"move.w  %d2,0x10(%a0)\n	" //id Hicolor
		"add.w   %d3,%d2\n	"
		"move.w  %d2,NG_ID\n	"

		//SCB3 Y position ,Sticky bit ,Sprite size
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

		"NG_SpriteInit_loopA:\n	"
			"move.w  %d0,(%a2)\n	"
		"dbra.w %d2,NG_SpriteInit_loopA\n	"

		//Hicolor SCB3
		"move.w  #SCB3,%d0\n	"
		"add.w   %d5,%d0\n	"
		"add.w   %d3,%d0\n	"
		"addq.w   #1,%d0\n	"
		"move.w  %d0,(%a1)\n	"

		"move.w  %d3,%d2\n	"
		"NG_SpriteInit_loopAHicolor:\n	"
			"move.w  %d0,(%a2)\n	"
		"dbra.w %d2,NG_SpriteInit_loopAHicolor\n	"
		//−---------

		//SCB1
		"move.w  %d3,%d6\n	"

		"asl.w   #6,%d5\n	"
		"move.w  %d5,%d0\n	"

		"move.w  14(%a0),%d5\n	" //palette/tile/flip/auto

		"move.w %d0,(%a1)\n	"
		"addi.w  #0x40,%d0\n	"
		NG_NOP
		"NG_SpriteInit_loop1:\n	"

			"move.w %d4,%d2\n	"

			"NG_SpriteInit_loop2:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteInit_loop2\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteInit_loop1\n	"


		//SCB1 Hicolor
		"move.w %d6,%d3\n	"
		"addi.w  #0x0100,%d5\n	" //palette/tile/flip/auto

		"NG_SpriteInit_loop1Hicolor:\n	"

			"move.w %d4,%d2\n	"

			"NG_SpriteInit_loop2Hicolor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteInit_loop2Hicolor\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteInit_loop1Hicolor\n	"

		"movem.l (%a7)+,%d2-%d6/%a2\n	"
		);
}

void __attribute__((noinline)) NG_SpriteUpdateTileHicolor(u16 i)
{
	NG_arg1_u16 = i;
	asm (
		"movem.l %d2-%d7/%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"lsl.w   #4,%d0\n	"

		"lea     NG_Sprite,%a0\n	"
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
		"bne NG_SpriteUpdateTile_FLIPH\n	"

		"btst  #1,%d5\n	"
		"bne NG_SpriteUpdateTile_FLIPV\n	"

		//No flip
		"move.w  %d6,%d0\n	"
		"lsl.w   #6,%d0\n	"

		"move.w %d0,(%a1)\n	"
		"addi.w  #0x40,%d0\n	"
		NG_NOP

		"NG_SpriteUpdateTile_loop1:\n	"

			"move.w %d4,%d2\n	"

			"NG_SpriteUpdateTile_loop2:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1\n	"

		//Hicolor
		"move.w  %d7,%d3\n	"
		"add.w  #0x0100,%d5\n	"
		"NG_SpriteUpdateTile_loop1Hicolor:\n	"

			"move.w %d4,%d2\n	"

			"NG_SpriteUpdateTile_loop2Hicolor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2Hicolor\n	"

			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1Hicolor\n	"

		"jmp NG_SpriteUpdateTile_end\n	"

		//--------------------
		//flip H
		"NG_SpriteUpdateTile_FLIPH:\n	"

		"btst  #1,%d5\n	"
		"bne NG_SpriteUpdateTile_FLIPHV\n	"

		"move.w  %d6,%d0\n	"
		"add.w   %d3,%d0\n	"
		"lsl.w   #6,%d0\n	"

		"move.w %d0,(%a1)\n	"
		"subi.w  #0x40,%d0\n	"
		NG_NOP

		"NG_SpriteUpdateTile_loop1H:\n	"

			"move.w %d4,%d2\n	"

			"NG_SpriteUpdateTile_loop2H:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2H\n	"


			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1H\n	"

		//Hicolor
		"move.w  %d6,%d0\n	"
		"add.w   %d7,%d0\n	"
		"add.w   %d7,%d0\n	"
		"addq.w  #1,%d0\n	"
		"lsl.w   #6,%d0\n	"


		"move.w %d0,(%a1)\n	"
		"subi.w  #0x40,%d0\n	"

		"move.w  %d7,%d3\n	"
		"add.w  #0x0100,%d5\n	"

		"NG_SpriteUpdateTile_loop1HHicolor:\n	"

			"move.w %d4,%d2\n	"

			"NG_SpriteUpdateTile_loop2HHicolor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"addq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2HHicolor\n	"


			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1HHicolor\n	"
		"jmp NG_SpriteUpdateTile_end\n	"

		//--------------------
		//flip V
		"NG_SpriteUpdateTile_FLIPV:\n	"

		"move.w  %d6,%d0\n	"
		"lsl.w   #6,%d0\n	"
		"move.w %d0,(%a1)\n	"
		"addi.w  #0x40,%d0\n	"
		"move.w %d4,%d6\n	"
		"addq.w #2,%d6\n	"

		"NG_SpriteUpdateTile_loop1V:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_SpriteUpdateTile_loop2V:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2V\n	"
			"add.w %d6,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1V\n	"

		//Hicolor
		"move.w  %d7,%d3\n	"
		"add.w  #0x0100,%d5\n	"

		"NG_SpriteUpdateTile_loop1VHicolor:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_SpriteUpdateTile_loop2VHicolor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2VHicolor\n	"
			"add.w %d6,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"addi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1VHicolor\n	"

		"jmp NG_SpriteUpdateTile_end\n	"
		//--------------------
		//flip HV
		"NG_SpriteUpdateTile_FLIPHV:\n	"

		"move.w  %d6,%d0\n	"
		"add.w   %d3,%d0\n	"
		"lsl.w   #6,%d0\n	"

		"move.w %d4,%d7\n	"
		"addq.w #2,%d7\n	"

		"move.w %d0,(%a1)\n	"
		"subi.w  #0x40,%d0\n	"

		"NG_SpriteUpdateTile_loop1HV:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_SpriteUpdateTile_loop2HV:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2HV\n	"
			"add.w %d7,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1HV\n	"

		//Hicolor
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

		"NG_SpriteUpdateTile_loop1HVHicolor:\n	"

			"move.w %d4,%d2\n	"
			"add.w %d4,%d1\n	"

			"NG_SpriteUpdateTile_loop2HVHicolor:\n	"
				"move.w  %d1,(%a2)\n	"
				NG_NOP3
				"move.w  %d5,(%a2) \n	"
				"subq.w  #1,%d1\n	"
			"dbra.w %d2,NG_SpriteUpdateTile_loop2HVHicolor\n	"
			"add.w %d7,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_SpriteUpdateTile_loop1HVHicolor\n	"

		"NG_SpriteUpdateTile_end:\n	"
		"movem.l (%a7)+,%d2-%d7/%a2\n	"
		);
}

void __attribute__((noinline)) NG_SpriteUpdateZoomHicolor(u16 i)
{
	NG_arg1_u16 = i;
	asm (

		"move.w  #0x01,VRAM_MOD\n	"

		"lea     NG_Sprite,%a0\n	"
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
		"NG_SpriteUpdateTileW_loop:\n	"
			"move.w  %d0,(%a1)\n	"
		"dbra.w %d1,NG_SpriteUpdateTileW_loop\n	"
		);
}

void __attribute__((noinline)) NG_SpriteUpdateHicolor(u16 i)
{
	NG_arg1_u16 = i;
	asm (
		"movem.l %a2,-(%a7)\n	"

		"move.w  #0x200,VRAM_MOD\n	"

		"lea     NG_Sprite,%a0\n	"
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
