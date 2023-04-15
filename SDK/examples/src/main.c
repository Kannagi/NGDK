
#include <NGDK.h>
#include "data.h"

const u16 palette_ng[32] = { 0x0f0f, 0x0eee, 0x0ddd, 0x0ccc, 0x0bbb, 0x0aaa, 0x0999, 0x0888, 0x010f, 0x0f00, 0x00ff, 0x0f0f, 0x0f0f, 0x0f0f, 0x0f0f, 0x0000 };
const u8 custom4test_pal_data[32] ={
	0x5f, 0x0f, 0x00, 0x00, 0x01, 0x22, 0x72, 0x22, 
	0x06, 0x77, 0x05, 0x66, 0x24, 0x45, 0x23, 0x34, 
	0x03, 0x44, 0x27, 0x78, 0x07, 0x88, 0x7c, 0xcc, 
	0x2b, 0xbc, 0x5b, 0xcc, 0x09, 0xaa, 0x5b, 0xcb, 
};

u16 buffer_map_A[0x800*2];
u16 buffer_map_B[0x800*2];
u8 buffer_map_C[0x800*1];

void example1(void);
void example2(void);
void example3(void);
void example4(void);
void example5(void);
void example6(void);
void game1(void);
void game2(void);
void game3(void);

void select_example(void)
{
	//Load Palette (FIX 0-15)
	NG_Palette_Load(palette_ng, 0); //Load palette, PAL : 0
	u16 example = 5; //(0-7)

	if(example == 0)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//learn use FIX /Printf
		example1();
	}

	if(example == 1)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//learn use Sprite
		example2();
	}

	if(example == 2)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//learn use Joypad /Sprite Advanced
		example3();
	}

	if(example == 3)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//learn use Sprite anim /fix16
		example4();
	}

	if(example == 4)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//learn use Background
		example5();
	}

	if(example == 5)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_ARRAY);
		//example tile array mode
		example6();
	}

	if(example == 6)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//Demo Metal Slug
		game1();
	}

	if(example == 7)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//Demo Hi-Color sprite
		game2();
	}

	if(example == 8)
	{
		NG_Set_Tile_Mode(NG_TILE_MODE_SIMPLE);
		//Demo shmup
		game3();
	}
}

//Do not put code on the Main, put only one function call void
int __attribute__((optimize("-O0"), noinline)) main()
{
	select_example();

	return 0;
}
