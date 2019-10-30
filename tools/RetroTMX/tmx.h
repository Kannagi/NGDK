


typedef struct
{
	int width,height,encoding;
	char name[40];
	unsigned int *data;

}LAYER;


typedef struct
{
	int firstgid;
	int tilewidth,tileheight,tilecount,columns;
	int width,height,trans;
	char name[40];
	char source[50];
}TILESET;

typedef struct
{
	float version,tiledversion;
	int orientation,renderorder,width,height;
	int tilewidth,tileheight,infinite,nextobjectid;
}MAP;


typedef struct
{
	float xmlversion;
	int encoding;
	int nmap,ntileset;
	MAP map;
	TILESET tileset[4096];
	LAYER layer[16];
}TMX;

void Load_TMX(TMX *tmx,char *address);
