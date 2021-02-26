#include <NGDK.h>
#include "data/data_ext.h"

extern const u16 palette_ng[32];
void NG_BCD_Add(u32 *val,u32 add);
void NG_BCD_Sub(u32 *val,u32 add);
void NG_Fix_Print_TileN(const u16 *id,u16 x,u16 y,u16 n);
void NG_Fix_Print_TileN_PalS(const u16 *id,u16 x,u16 y,u16 n);
const u16 fixtile[32] = { 0x0300,0x0301,0x0302,0x0303,0x0bbb,0x0aaa,0x0999,0x0888,0x010f,0x0f00,0x00ff,0x0f0f,0x0f0f,0x0f0f,0x0f0f,0x0000 };

void example1(void)
{

	NG_LoadPal(palette_ng,0);

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//-----------fix--------------
	NG_FixPal(NG_FIX_PAL(0)); //Select Pal for Print
	NG_Print("Neo Megalopolis !",10,14); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	NG_Printu16(NG_MAX_SPR,0,1);

	NG_PrintFreeMem(0,5);


	u32 score = 0x100;

	NG_BCD_Add(&score,0x10);
	NG_BCD_Sub(&score,0x80);

	NG_Fix_Print_h32(score,1,10);

	u8 var = 0;

	NG_Set_YM2610(4);

	NG_Fix_Print_TileN(fixtile,1,20,4);


	NG_Palette_Load(MarcoH1_pal_data, 16);
	NG_Sprite_Init(0, 0, 0, 2, 2, MarcoH1_index, NG_SPR_FLAGS(16, 0, 0, 0));


	//----Init Sprite VRAM------------
	NG_Sprite_VRAM_ID_Set(0x00);
	NG_Sprite_VRAM_Init(0); //Init ID Sprite 0 + meta sprite + Upload


	NG_WaitVBlank();
	while(1)
	{
		NG_Printh8(NG_Get_YM2610(),1,8);
		NG_Printu8(var,1,9);
		var++;

		//NG_Sprites[0].x += 1;

		NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		NG_Cycle(33,1); //Print Cycle

		NG_Sprite_Update_N(0); //Update position X, Y, MetaSPR, Zoom XY, 0-N

		NG_WaitVBlank(); //Wait VBlank
		//NG_Printu16(var,10,10);
	}
}




