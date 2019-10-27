
#include "NGDK.h"
#include "asm.h"

#include "NG_arge.h"


void __attribute__((noinline)) NG_BulletPosition(u16 pile,u16 offset,u16 n)
{
	if(pile == 0)
		NG_arg1_u32 = (u32)NG_Bullet_Pile1;
	else
		NG_arg1_u32 = (u32)NG_Bullet_Pile2;

	NG_arg1_u16 = offset;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d7,-(%a7)\n	"

		"lea    NG_Bullet,%a0\n	"
		"move.l NG_arg1_u32,%a1\n	"

		"move.w NG_arg1_u16,%d7\n	"
		"asl.w  #3,%d7\n	"
		"move.w %d7,%d0\n	"
		"asl.w  #4,%d0\n	"
		"add.w %d0,%a0\n	"


		"move.w #0x10,%d3\n	"
		"move.w #0x1,%d0\n	"
		"move.w #0xFFFF,%d4\n	"
		"move.w #0xA000,%d5\n	"
		"move.w #0x8000,%d6\n	"

		//----------------------------------
		"move.w NG_arg2_u16,%d2\n	"
		"addq.w #1,%d2\n	"
		"asl.w  #3,%d2\n	"
		"subi.w #1,%d2\n	"

		"NG_BulletPosition_loop:\n	"
			//Y position
			"move.w (%a0),%d1\n	"

			"btst  #15,%d1\n	"
			"bne NG_BulletPosition_off\n	"

			"add.w  10(%a0),%d1\n	"
			"move.w %d1,(%a0)\n	"
			"eor.w  %d4,%d1\n	"
			"andi.w #0xFF80,%d1\n	"
			"add.w  %d0,%d1\n	"
			"move.w %d1,12(%a0)\n	"

			//X Position
			"move.w  2(%a0),%d1\n	"

			"cmp.w   %d5,%d1\n	"
			"bcc NG_BulletPosition_off\n	"

			"add.w   8(%a0),%d1\n	"
			"move.w  %d1,2(%a0)\n	"
			"andi.w  #0xFF80,%d1\n	"
			"move.w  %d1,14(%a0)\n	"

			"NG_BulletPosition_off_ret:\n	"

			"add.w  #1,%d7\n	"
			"add.w  %d3,%a0\n	"
		"dbra.w %d2,NG_BulletPosition_loop\n	"
		"bra.w NG_BulletPosition_end\n	"

		//------
		"NG_BulletPosition_off:\n	"
		"move.w %d6,12(%a0)\n	"
		"move.w %d5,14(%a0)\n	"
		"clr.w %d1\n	"
		"move.w %d1,8(%a0)\n	"
		"move.w %d1,10(%a0)\n	"

		"move.w %d7,(%a1)+\n	"
		"bra.w NG_BulletPosition_off_ret\n	"
		//----------------------------------
		"NG_BulletPosition_end:\n	"

		"move.w  %d7,2(%a1)\n	"

		"movem.l (%a7)+,%d2-%d7\n	"
		);
}

void __attribute__((noinline)) NG_BulletUpdatePosition(u16 id,u16 n)
{
	NG_arg1_u16 = id;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d3/%a2,-(%a7)\n	"

		"move.w  #0x200,VRAM_MOD\n	"

		"lea     NG_Bullet+12,%a0\n	"
		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w #SCB3+1,%d2\n	"
		"add.w  NG_arg1_u16,%d2\n	"

		"move.w  #0x10-2,%d3\n	"
		"move.w #0x1,%d0\n	"

		//----------------------------------
		"move.w NG_arg2_u16,%d1\n	"

		//SCB3
		"NG_BulletUpdatePosition_loop:\n	"
			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"

			"move.w %d2,(%a1)\n	"
			"move.w (%a0)+,(%a2)\n	"
			"move.w (%a0),(%a2)\n	"
			"add.w  %d0,%d2\n	"
			"add.w  %d3,%a0\n	"
		"dbra.w %d1,NG_BulletUpdatePosition_loop\n	"


		//----------------------------------
		"movem.l (%a7)+,%d2-%d3/%a2\n	"
		);
}


void __attribute__((noinline)) NG_BulletUpdate(u16 id,u16 n)
{
	NG_arg1_u16 = id;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d5/%a2,-(%a7)\n	"

		"move.w  #0x200,VRAM_MOD\n	"

		"lea     NG_Bullet,%a0\n	"
		"lea     VRAM_ADDR,%a1\n	"
		"lea     VRAM_RW,%a2\n	"

		"move.w #SCB3+1,%d2\n	"
		"add.w  NG_arg1_u16,%d2\n	"

		"move.w #0x10,%d3\n	"
		"move.w #0x1,%d0\n	"
		"move.w #0xFFFF,%d4\n	"

		//----------------------------------
		"move.w NG_arg2_u16,%d5\n	"
		"addq.w #1,%d5\n	"
		"asl.w  #3,%d5\n	"
		"subi.w #1,%d1\n	"

		//SCB3
		"NG_BulletUpdate_loop:\n	"
			"move.w %d2,(%a1)\n	"

			//Y position
			"move.w (%a0),%d1\n	"
			"eor.w  %d4,%d1\n	"
			"asl.w  #7,%d1\n	"
			"add.w  %d0,%d1\n	"
			"move.w %d1,(%a2)\n	"

			//X Position
			"move.w  2(%a0),%d1\n	"
			"asl.w   #7,%d1\n	"
			"move.w  %d1,(%a2)\n	"

			"add.w  %d3,%a0\n	"
			"add.w  %d0,%d2\n	"


		"dbra.w %d5,NG_BulletUpdate_loop\n	"


		//----------------------------------
		"movem.l (%a7)+,%d2-%d5/%a2\n	"
		);
}



void __attribute__((noinline)) NG_BulletUpdateTile(u16 id,u16 n)
{
	NG_arg1_u16 = id;
	NG_arg2_u16 = n;
	asm (
		"movem.l %d2-%d3/%a2,-(%a7)\n	"

		"move.w  #0x01,VRAM_MOD\n	"

		"lea     NG_Bullet+4,%a2\n	"
		"lea     VRAM_ADDR,%a0\n	"
		"lea     VRAM_RW,%a1\n	"

		"move.w  NG_arg1_u16,%d0\n	"
		"addq.w #1,%d0\n	"
		"asl.w  #6,%d0\n	"

		"move.w  #12,%d3\n	"

		"move.w NG_arg2_u16,%d1\n	"
		"addq.w #1,%d1\n	"
		"asl.w  #3,%d1\n	"
		"subi.w #1,%d1\n	"

		//SCB1
		"NG_BulletUpdateTile_loop:\n	"
			"move.w  %d0,(%a0)\n	" //8 cycles
			"addi.w  #0x40,%d0\n	" //8 cycles

			"move.w  (%a2)+,%d2\n	" //8 cycles
			"move.w  %d2,(%a1)\n	" //8 cycles
			NG_NOP //4 cycles

			"move.w  (%a2)+,%d2\n	"  //8 cycles
			"move.w  %d2,(%a1)\n	" //8 cycles

			"add.w  %d3,%a2\n	"  //4 cycles
		"dbra.w %d1,NG_BulletUpdateTile_loop\n	"

		//----------------------------------
		"movem.l (%a7)+,%d2-%d3/%a2\n	"
		);
}

