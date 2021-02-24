#include <NGDK.h>
#include "data/data_ext.h"

extern const u16 palette_ng[32];
void NG_BCD_Add(u32 *val,u32 add);
void NG_BCD_Sub(u32 *val,u32 add);

void example1(void)
{

	NG_LoadPal(palette_ng,0);

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//-----------fix--------------
	NG_FixPal(NG_FIX_PAL(0)); //Select Pal for Print
	//NG_Print("Neo Megalopolis !",10,14); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	NG_Printu16(NG_MAX_SPR,0,1);
	//NG_Printu16(0b11,0,1);

	NG_PrintFreeMem(0,5);


	u32 score = 0x00;

	NG_BCD_Add(&score,0x10);
	NG_BCD_Add(&score,0x80);

	NG_Fix_Print_h32(score,1,10);

	u8 var = 0;

	NG_Set_YM2610(4);

	NG_WaitVBlank();
	while(1)
	{
		NG_Printh8(NG_Get_YM2610(),1,8);
		//NG_Printu8(var,1,9);
		var++;

		//NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		//NG_Cycle(32,1); //Print Cycle

		NG_WaitVBlank(); //Wait VBlank
		//NG_Printu16(var,10,10);
	}
}




