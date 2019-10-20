#include <NGDK.h>
#include "data/data.h"

const u16 palette_ng[32] = { 0x0f0f,0x0eee,0x0ddd,0x0ccc,0x0bbb,0x0aaa,0x0999,0x0888,0x010f,0x0f00,0x00ff,0x0f0f,0x0f0f,0x0f0f,0x0f0f,0x0000 };

void example1(void);
void example2(void);
void example3(void);
void example4(void);
void example5(void);
void game(void);

void select_example(void)
{
	//Load Palette (FIX 0-15)
	NG_LoadPal(palette_ng,0); //Load palette , PAL : 0
	u16 example = 5;

	if(example == 0)
	{
		//learn use FIX /Printf
		example1();
	}

	if(example == 1)
	{
		//learn use Sprite
		example2();
	}

	if(example == 2)
	{
		//learn use Joypad /Sprite Advanced
		example3();
	}

	if(example == 3)
	{
		//learn use Background
		example4();
	}

	if(example == 4)
	{
		//learn use Sprite anim /fix16
		example5();
	}

	if(example == 5)
	{
		//Demo Metal Slug
		game();
	}
}

//Do not put code on the Main, put only one function call void
int __attribute__((optimize("-O0"), noinline)) main()
{
	select_example();

	return 0;
}

