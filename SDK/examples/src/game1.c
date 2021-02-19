#include <NGDK.h>
#include "data_ext.h"

#define MAP_WIDTH 128

extern const u16 palette_ng[32];
extern u16 buffer_map_A[0x800*2];
extern u16 buffer_map_B[0x800*2];
extern u8 buffer_map_C[0x800*1];

void offset1(void)
{
	NG_Fix_Print_Cycles(33, 1);
	NG_Fix_Print_CPU(34, 0); //Print % CPU, X pos, Y pos
	NG_Sprite_Tiles_Update(2); //Update tile
	NG_Sprite_Tiles_Update(3); //Update tile
	NG_Sprite_Update_N(3);
}

void game1(void )
{
	NG_joypad joypad1 = NG_JOYPAD_INIT; //Init Joypad
	NG_Sprite_Clear_VRAM(); //Clear VRAM Sprite
	NG_Fix_Clear(); //Clear Fix

	NG_Backdrop_Color_Set(NG_COLOR(7, 7, 7)); //Backdrop color R, G, B (0-15)

	//Load Palette (FIX 0-15)
	NG_Palette_Load(palette_ng, 0); //Load palette, PAL : 0

	//MAP

	NG_Palette_Load(tile1_pal_data, 16); //Load palette, PAL : 16
	NG_Palette_Load(tile2_pal_data, 17);
	NG_Palette_Load(tile3_pal_data, 18);
	NG_Palette_Load(tile4_pal_data, 19);
	NG_Palette_Load(tile5_pal_data, 20);
	NG_Palette_Load(tile6A_pal_data, 21);
	NG_Palette_Load(tile6B_pal_data, 22);
	NG_Palette_Load(tile6C_pal_data, 23);
	NG_Palette_Load(tile6D_pal_data, 24);
	NG_Palette_Load(tile6E_pal_data, 25);
	NG_Palette_Load(tile6F_pal_data, 26);
	NG_Palette_Load(tile6G_pal_data, 27);
	NG_Palette_Load(tile6H_pal_data, 28);
	NG_Palette_Load(tile6I_pal_data, 29);
	NG_Palette_Load(tile7_pal_data, 30);
	NG_Palette_Load(tile8_pal_data, 31);
	NG_Palette_Load(tile9_pal_data, 32);
	NG_Palette_Load(tile10_pal_data, 33);
	NG_Palette_Load(tile11_pal_data, 34);

	NG_Palette_Load(MarcoLPal_pal_data, 0x80);
	NG_Palette_Load(MarcoH1_pal_data, 0x81);

	NG_Palette_Load_N(test3_pal_data, 0x82, 1);
	//-----------fix--------------
	NG_Fix_Palette(NG_FIX_PAL(0)); //Select Pal for Print
	NG_Fix_Print_String("Hello Worlb !", 12, 0); //Print text, str, X pos, Y pos
	NG_Fix_Print_String("CPU:", 29, 0);
	NG_Fix_Print_u16(NGDK_VERSION, 0, 0); //Print variable u16, X pos, Y pos
	NG_Fix_Print_u16(NG_MAX_SPR, 0, 1);

	NG_Fix_Print_Free_Mem(0, 5);

	//-----------init sprite--------------
	NG_Sprite_Init(0, 0, 0, 32, 15, 0x0000, 0);  //BG1
	NG_Sprite_Init(1, 0, 0, 32, 15, 0x0000, NG_SPR_FLAGS(0x10, 0, 0, 0)); //BG2

	NG_Sprite_Init(2, 20, 160, 2, 2, MarcoL1_index, NG_SPR_FLAGS(0x80, 0, 0, 0)); //Sprite, w, h, tile, flag(pal, tileMSB, auto-anim, flip)
	NG_Sprite_Init(3, 20, 160, 2, 2, MarcoH1_index, NG_SPR_FLAGS(0x81, 0, 0, 0));

	//----Init Sprite VRAM------------
	NG_Sprite_VRAM_ID_Set(0x00);
	NG_Sprite_VRAM_Init(0); //BG1
	NG_Sprite_VRAM_Init(1);	//BG2
	NG_Sprite_VRAM_Init(2); //Init ID Sprite 2 + meta sprite + Upload
	NG_Sprite_VRAM_Init(3);
	NG_Sprite_Update_N(3); //Update position X, Y, MetaSPR, Zoom XY, 0-N

	//NG_Sprite_VRAM_ID_Get gives real ID, max 380
	NG_Fix_Print_u16(NG_Sprite_VRAM_ID_Get(), 0, 2);
	int i;
	NG_Background_Width_Set(MAP_WIDTH);
	u32 *map1 = (u32*)stage1_A_data;
	u32 *map2 = (u32*)stage1_B_data;

	for(i = 0;i < 20;i++)
		NG_Background_Vertical_Update(0, i, &map1[i]);

	for(i = 0;i < 20;i++)
		NG_Background_Vertical_Update(1, i, &map2[i]);

	NG_Animation anim1;
	NG_Anim_Init(&anim1);

	NG_Animation anim2;
	NG_Anim_Init(&anim2);

	u16 x = 100, y = (16*9)+8;
	u16 mx, my;
	u16 speedx = ftoi2(1.5);
	s16 bgx = 0;
	u16 absx = x;
	u16 ianim = 0;
	NG_Wait_VBlank(); //Wait VBlank
	while(1)
	{
		NG_Joypad1_Read(&joypad1);
		ianim = 0;
		mx = 3;
		my = 8;
		if(NG_Sprites[2].flags&1)
			mx = -4;

		if(joypad1.C == NG_BUTTON_PRESSED)
		{
			NG_Sprites[4].flags &= 0xFFFF-1;
		}

		if(joypad1.D == NG_BUTTON_PRESSED)
		{
			NG_Sprites[4].flags |= 1;
		}

		if(joypad1.LEFT == NG_BUTTON_HOLD)
		{
			x -= speedx;
			absx -= speedx;
			NG_Sprites[2].flags |= 0x1;
			NG_Sprites[3].flags |= 0x1;
			mx = -8;
			my = 12;
			ianim = 1;
		}

		if(joypad1.RIGHT == NG_BUTTON_HOLD)
		{
			x += speedx;
			absx += speedx;
			NG_Sprites[2].flags &= 0xFFFE;
			NG_Sprites[3].flags &= 0xFFFE;
			mx = 5;
			my = 12;
			ianim = 1;
		}

		if(joypad1.A == NG_BUTTON_PRESSED)
		{
			NG_Sprites[2].flags &= 0xFFFF-2;
			NG_Sprites[3].flags &= 0xFFFF-2;
		}

		if(joypad1.B == NG_BUTTON_PRESSED)
		{
			NG_Sprites[2].flags |= 2;
			NG_Sprites[3].flags |= 2;
		}

		if(absx > ftoi2(660) )
			y = (16*11)+8;
		else
			y = (16*9)+8;



		if(x > ftoi2(180) )
		{
			x = ftoi2(180);

			NG_Background_Scroll_X(0, 20, map1);
			NG_Background_Scroll_X(1, 20, map2);

			bgx -= speedx;
		}

		if(x < ftoi2(120)  )
		{
			if(bgx != 0)
				x = ftoi2(120);
			else
			{
				if(x < ftoi2(4))
				{
					x = ftoi2(4);
					absx = ftoi2(4);
				}

			}

			NG_Background_Scroll_X(0, -1, map1);
			NG_Background_Scroll_X(1, -1, map2);
			bgx += speedx;
		}

		u16 tmpx = itoi2(x);

		NG_Sprites[2].x = tmpx;
		NG_Sprites[3].x = tmpx+mx;

		NG_Sprites[2].y = y;
		NG_Sprites[3].y = y-my;

		if(bgx > 0) bgx = 0;
		tmpx = itoi2(bgx);

		NG_Sprites[0].x = tmpx;
		NG_Sprites[1].x = tmpx;

		//Higb
		NG_Anim_Play_Fast(&anim1, 10, 2);
		NG_Sprites[3].tile = MarcoH1_index+(anim1.frame * 8);

		//Low
		if(ianim == 1)
		{
			NG_Anim_Play_Fast(&anim2, 5, 18);
			NG_Sprites[2].tile = MarcoL2_index+(anim2.frame * 4);

		}else
		{
			anim2.frame = 0;
			anim2.frameCounter = 0;
			NG_Sprites[2].tile = MarcoL1_index;
		}

		NG_Wait_VBlank(); //Wait VBlank
		offset1(); //15% max

	}
}

