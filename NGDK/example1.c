#include <NGDK.h>
#include "data/data_ext.h"

extern const u16 palette_ng[32];

void example1(void)
{

	NG_LoadPal(palette_ng,0);

	NG_BGColor(NG_COLOR(7,7,7)); //Color BG R,G,B 0-15

	//-----------fix--------------
	NG_FixPal(FIXPAL(0)); //Select Pal for Print
	NG_Print("Hello Worlb !",12,0); //Print text , str , Pos X , Pos Y
	NG_Print("CPU:",29,0);
	NG_Printu16(NGDK_VERSION,0,0); //Print variable u16 , Pos X , Pos Y
	NG_Printu16(NG_MAX_SPR,0,1);

	NG_PrintFreeMem(0,5);

	int var = 0;

	while(1)
	{


		var++;

		//NG_PrintCPU(34,0); //Print %CPU , Pos X , Pos Y ;
		//NG_Cycle(32,1); //Print Cycle

		NG_WaitVBlank(); //Wait VBlank
		NG_Printu16(var,10,10);
	}
}




