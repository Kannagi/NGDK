#include <NGDK.h>
#include <data_ext.h>

extern u8 custom4test_pal_data[];

void example6(void)
{
	NG_Sprite_Clear_VRAM();
	NG_Fix_Clear();

	NG_Backdrop_Color_Set(NG_COLOR(7, 7, 7));

	NG_Fix_Palette(NG_FIX_PAL(0));
	NG_Fix_Print_String("Test Indexed Tile !", 3, 0);

	NG_Palette_Load(colors_pal_data, 16);
	NG_Palette_Load(custom4test_pal_data, 17);

	u16 tile_index[4];

	//----Init Sprite VRAM------------
	NG_Sprite_VRAM_ID_Set(0x00);
	
	tile_index[0] = colors_index+0;
	tile_index[1] = NG_SPR_FLAGS(16, 0, 0, 0);
	tile_index[2] = colors_index+1;
	tile_index[3] = NG_SPR_FLAGS(17, 0, 0, 0);
	NG_tile_array_ptr = tile_index;
	NG_Sprite_Init(0, 20, 20, 1, 2, NG_TILE_NULL, NG_TILE_NULL);
	NG_Sprite_VRAM_Init(0);

	tile_index[0] = colors_index+3;
	tile_index[1] = NG_SPR_FLAGS(17, 0, 0, 0);
	tile_index[2] = colors_index+2;
	tile_index[3] = NG_SPR_FLAGS(16, 0, 0, 0);
	NG_tile_array_ptr = tile_index;
	NG_Sprite_Init(1, 100, 20, 1, 2, NG_TILE_NULL, NG_TILE_NULL);
	NG_Sprite_VRAM_Init(1);

	tile_index[0] = colors_index+0;
	tile_index[1] = NG_SPR_FLAGS(16, 0, 0, 0);
	tile_index[2] = colors_index+1;
	tile_index[3] = NG_SPR_FLAGS(16, 0, 0, 0);
	NG_tile_array_ptr = tile_index;
	NG_Sprite_Init(2, 20, 20+100, 2, 1, NG_TILE_NULL, NG_TILE_NULL);
	NG_Sprite_VRAM_Init(2);
	
	NG_Sprite_Update_N(2);

	u8 inc_tile = 0;
	while(1)
	{
		if(inc_tile > 5)
		{
			inc_tile = 0;
		}

		tile_index[0] = colors_index+inc_tile;
		tile_index[1] = NG_SPR_FLAGS(16, 0, 0, 0);
		tile_index[2] = colors_index+(5-inc_tile);
		tile_index[3] = NG_SPR_FLAGS(16, 0, 0, 0);
		NG_Sprite_Tiles_Update(2);

		inc_tile++;

		NG_Wait_VBlank();
	}
}