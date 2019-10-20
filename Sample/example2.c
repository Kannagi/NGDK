#include <NGDK.h>
#include "data/data_ext.h"


void example2(void)
{
	NG_ClearFix(); //Clear Fix
	NG_SpriteClearVRAM(); //Clear VRAM Sprite

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//-----------fix--------------
	NG_FixPal(FIXPAL(0)); //Select Pal for Print
	NG_Print("Hello Worlb !",12,0); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	NG_Printu16(NG_MAX_SPR,0,1);

	NG_PrintFreeMem(0,5); //Indicates available memory

	//Sprite
	NG_LoadPal(MarcoH1_pal_data,16);
	NG_SpriteInit(&NG_Sprite[0],2,2,Map_tile_SPR[0],NG_FLAG(16,0,0,0));

	NG_Sprite[0].x += 100;
	NG_Sprite[0].y += 100;

	//----Init Sprite VRAM------------
	NG_SpriteSetID(0x00);
	NG_SpriteUpdateInit(0); //Init ID Sprite 0 + meta sprite + Upload
	NG_SpriteUpdateN(0); // Update position X,Y,MetaSPR , Zoom XY, 0-N

	while(1)
	{
		NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		NG_Cycle(32,1); //Print Cycle

		NG_WaitVBlank(); //Wait VBlank

	}
}
