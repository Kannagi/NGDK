
#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"

void NG_Sprite_Init(u16 id, u16 x, u16 y, u16 width, u16 height, u16 tile, u16 flags)
{
	NG_Sprites[id].width = width;
	NG_Sprites[id].height = height;
	NG_Sprites[id].zoom = 0xFFF;
	NG_Sprites[id].tile = tile;
	NG_Sprites[id].flags = flags;

	NG_Sprites[id].x = 8 + x;
	NG_Sprites[id].y = 15 + y;
}

void NG_Sprite_VRAM_ID_Set(u16 id)
{
	NG_VRAM_sprite_id = id+1;
}

u16 NG_Sprite_VRAM_ID_Get()
{
	return NG_VRAM_sprite_id-1;
}

void __attribute__((noinline)) NG_Sprite_VRAM_Init(u16 id)
{
	NG_arg1_u16 = id;
	asm (
		"movem.l %d2-%d5/%a2,-(%a7)\n	"

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
		"move.w  %d5,(%a0)\n	" //VRAM sprite id
		"move.w  %d5,%d2\n	"
		"add.w   %d3,%d2\n	"
		"move.w  %d2,NG_VRAM_sprite_id\n	"

		//SCB3 Y position, sticky bit, sprite size
		"move.w  #SCB3,%d0\n	"
		"add.w   %d5,%d0\n	"
		"move.w  %d0,(%a1)\n	"

		"move.w  4(%a0),%d0\n	"
		"eor.w   #0xFFFF,%d0\n	"
		"asl.w   #7,%d0\n	"
		"or.w    %d4,%d0\n	"
		"ori.w   #0x40,%d0 \n	"

		"subi.w  #1,%d3\n	"
		"subi.w  #1,%d4\n	"
		"move.w  %d3,%d2\n	"

		"NG_Sprite_Init_loop_A:\n	"
			"move.w  %d0,(%a2)\n	"
		"dbra.w %d2,NG_Sprite_Init_loop_A\n	"

		//SCB1
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

		"movem.l (%a7)+,%d2-%d5/%a2\n	"
		);
}

void __attribute__((noinline)) NG_Sprite_Tiles_Update(u16 id)
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

		"move.w  (%a0),%d6\n	"//id
		"move.w  6(%a0),%d4\n	" //h
		"move.w  10(%a0),%d3\n	" //w
		"move.w  12(%a0),%d1\n	" //tile
		"move.w  14(%a0),%d5\n	" //palette/tile/flip/auto

		"subi.w  #1,%d3\n	"
		"subi.w  #1,%d4\n	"

		//SCB1
		"btst  #0,%d5\n	"
		"bne NG_Sprite_Tiles_Update_FLIP_H\n	"

		"btst  #1,%d5\n	"
		"bne NG_Sprite_Tiles_Update_FLIP_V\n	"

		//No flip
		"move.w  %d6,%d0\n	"
		"asl.w   #6,%d0\n	"

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

		"jmp NG_Sprite_Tiles_Update_end\n	"

		//--------------------
		//flip H
		"NG_Sprite_Tiles_Update_FLIP_H:\n	"

		"btst  #1,%d5\n	"
		"bne NG_Sprite_Tiles_Update_FLIP_HV\n	"

		"move.w  %d6,%d0\n	"
		"add.w   %d3,%d0\n	"
		"asl.w   #6,%d0\n	"

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
		"jmp NG_Sprite_Tiles_Update_end\n	"

		//--------------------
		//flip V
		"NG_Sprite_Tiles_Update_FLIP_V:\n	"

		"move.w  %d6,%d0\n	"
		"asl.w   #6,%d0\n	"
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

		"jmp NG_Sprite_Tiles_Update_end\n	"
		//--------------------
		//flip HV
		"NG_Sprite_Tiles_Update_FLIP_HV:\n	"

		"move.w  %d6,%d0\n	"
		"add.w   %d3,%d0\n	"
		"asl.w   #6,%d0\n	"

		"move.w %d4,%d6\n	"
		"addq.w #2,%d6\n	"

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
			"add.w %d6,%d1\n	"
			"move.w %d0,(%a1)\n	"
			"subi.w  #0x40,%d0\n	"

		"dbra.w %d3,NG_Sprite_Tiles_Update_loop1HV\n	"

		"NG_Sprite_Tiles_Update_end:\n	"
		"movem.l (%a7)+,%d2-%d6/%a2\n	"
		);
}

void __attribute__((noinline)) NG_Sprite_Zoom_Update(u16 id)
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
		"NG_Sprite_Tiles_UpdateW_loop:\n	"
			"move.w  %d0,(%a1)\n	"
		"dbra.w %d1,NG_Sprite_Tiles_UpdateW_loop\n	"
		);
}

void __attribute__((noinline)) NG_Sprite_Update(u16 id)
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

		//Sprite ID
		"move.w  (%a0),%d0\n	"
		"add.w   #SCB2,%d0\n	"
		"move.w  %d0,(%a1)\n	"

		//Zoom
		"move.w  2(%a0),(%a2)\n	"

		//Y position, sprite size
		"move.w 4(%a0),%d0\n	"
		"eor.w  #0xFFFF,%d0\n	"
		"asl.w  #7,%d0\n	"
		"or.w   6(%a0),%d0\n	"
		"move.w %d0,(%a2)\n	"

		//X Position
		"move.w  8(%a0),%d0\n	"
		"asl.w   #7,%d0\n	"
		"move.w  %d0,(%a2)\n	"

		"movem.l (%a7)+,%a2\n	"
		);
}

void __attribute__((noinline)) NG_Sprite_Update_N(u16 n)
{
	NG_arg1_u16 = n;
	asm (
		"movem.l %d2-%d3/%a2,-(%a7)\n	"	//4*n+12c=24c / Save registers d2,d3,a2 to a7,a6,a5

		"move.w  #0x200,VRAM_MOD\n	" 		//20c / Set VRAM increment to 0x200

		"lea     NG_Sprites,%a0\n	" 		//12c / a0 = NG_Sprites address
		"lea     VRAM_ADDR,%a1\n	" 		//12c / a1 = VRAM_ADDR (VRAM address register)
		"lea     VRAM_RW,%a2\n	" 			//12c / a2 = VRAM_RW (VRAM data R/W register)

		"move.w  #SCB2,%d2\n	" 			//12c / d2 = SCB2 address (0x8000)
		"move.w  #0x10,%d3\n	" 			//12c / d3 = 16 (NG_Sprite struct size, 16 bytes)

		"move.w  NG_arg1_u16,%d0\n	" 		//12c / d0 = n (NG_arg1_u16)
		"and.w   #0x7F,%d0\n	"			//8c / Cap d0 to x07f

		//160*n+136c
		"NG_Sprite_update_loop:\n	"

			//Sprite ID
			"move.w  (%a0),%d1\n	"		//8c / d1 = sprite VRAM ID
			"add.w   %d2,%d1\n	"			//4c / d1 = d1 + d2 (SCB2 address)
			"move.w  %d1,(%a1)\n	"		//8c / Write sprite VRAM address (d1) to VRAM_ADDR (a1)

			//Zoom
			"move.w  2(%a0),(%a2)\n	"		//16c / Write zoom (a0 + 2) to VRAM_RW (a2)
											//VRAM address is automatically incremented by 0x200

			//Y position , sprite size
			"move.w 4(%a0),%d1\n	"		//12c / d1 = Y position (a0 + 4)
			"eor.w  #0xFFFF,%d1\n	"		//8c / d1 = d1 ^ 0xffff
			"asl.w  #7,%d1\n	"			//2*n+6c=20c / d1 = d1 << 7
			"or.w   6(%a0),%d1\n	"		//12c / d1 = d1 | height (a0 + 6)
			"move.w %d1,(%a2)\n	"			//8c / Write y position (d1) to VRAM_RW (a2)
											//VRAM address is automatically incremented by 0x200

			//X Position
			"move.w  8(%a0),%d1\n	" 		//12c / d1 = X position (a0 + 8)
			"asl.w   #7,%d1\n	"			//2*n+6c=20c / d1 = d1 << 7
			"move.w  %d1,(%a2)\n	"		//8c / Write X position (d1) to VRAM_RW (a2)
											//VRAM address is automatically incremented by 0x200

			"add.w  %d3,%a0\n	"			//12c / a0 = a0 + 16 (NG_Sprite struct size, 16 bytes)

		"dbra.w %d0,NG_Sprite_update_loop\n	"	//12c / loop if needed


		"movem.l (%a7)+,%d2-%d3/%a2\n	"	//4*n+12c=24c / restore d2,d3,a2 from a7,a6,a5
		);
}
