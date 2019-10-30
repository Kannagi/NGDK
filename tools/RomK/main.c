#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void createrom8k(int argc, char** argv);
void createrom(int argc, char** argv);
void invert(int argc, char** argv);
void compare(int argc, char** argv);
void divfile(int argc, char** argv);
void divrom(int argc, char** argv);
void divrom2(int argc, char** argv);
void divf3(int argc, char** argv);
void add(int argc, char** argv);
void extract(int argc, char** argv);
void insert(int argc, char** argv);
void fillrom(int argc, char** argv);
void invertng(int argc, char** argv);
void insertbloc(int argc, char** argv);

int main(int argc, char** argv)
{
	if(strcmp(argv[1],"-v") == 0)
		printf("version 1.00\n");
	createrom8k(argc,argv);
	createrom(argc,argv);

	insertbloc(argc,argv);
	insert(argc,argv);
	invert(argc,argv);
	extract(argc,argv);
	compare(argc,argv);

	divfile(argc,argv);
	divrom(argc,argv);
	divrom2(argc,argv);
	divf3(argc,argv);

	add(argc,argv);
	fillrom(argc,argv);
	invertng(argc,argv);

	return 0;
}

//------------------------------------------------------

void extract(int argc, char** argv)
{
	if(argc < 3) return;
	if(strcmp(argv[1],"-extract") != 0) return;

	int byte;
	int n = 1,i,sk = 0;

	if(argc >= 4) n = atoi(argv[3]);
	if(argc == 5) sk = atoi(argv[4]);

	FILE *file,*out;
	file = fopen(argv[2],"rb");
	if(file == NULL) return;

	out = fopen("out.bin","wb");
	if(out == NULL) return;

	fseek ( file , sk , SEEK_SET );

	byte = n;
	for(i = 0;i < byte;i++)
	{
		n = fgetc(file);
		fputc(n,out);
	}

	fclose(file);

	return;

}

void insert(int argc, char** argv)
{
	if(argc < 3) return;
	if(strcmp(argv[1],"-insert") != 0) return;

	int byte = 0;
	int sk = 0;

	if(argc == 5) sk = atoi(argv[4]);

	FILE *file,*in;
	file = fopen(argv[2],"rb+");
	if(file == NULL) return;

	in = fopen(argv[3],"rb");
	if(in == NULL) return;

	fseek ( file , sk , SEEK_SET );

	while(1)
	{
		byte = fgetc(in);
		if(byte == -1) break;
		fputc(byte,file);
	}



	fclose(file);
	fclose(in);

	return;

}

void insertbloc(int argc, char** argv)
{
	if(argc < 3) return;
	if(strcmp(argv[1],"-insertbloc") != 0) return;

	int byte = 0;
	int sk = 0;

	if(argc == 5) sk = atoi(argv[4]);

	FILE *file,*in;
	file = fopen(argv[2],"rb+");
	if(file == NULL) return;

	in = fopen(argv[3],"rb");
	if(in == NULL) return;

	fseek ( file , sk*0x80 , SEEK_SET );

	while(1)
	{
		byte = fgetc(in);
		if(byte == -1) break;
		fputc(byte,file);
	}

	fclose(file);
	fclose(in);

	return;

}

void createrom8k(int argc, char** argv)
{
	if(argc < 3) return;
	if(strcmp(argv[1],"-rom8k") != 0) return;

	int byte;
	int n = 1,i;

	if(argc == 4) n = atoi(argv[3]);

	FILE *file;
	file = fopen(argv[2],"wb");
	if(file == NULL) return;

	byte = 0x8000*n;
	for(i = 0;i < byte;i++)
		fputc(0,file);

	fclose(file);

	return;

}

void createrom(int argc, char** argv)
{
	if(argc < 3) return;
	if(strcmp(argv[1],"-rom") != 0) return;

	int byte;
	int n = 1,i;

	if(argc == 4) n = atoi(argv[3]);

	FILE *file;
	file = fopen(argv[2],"wb");
	if(file == NULL) return;

	byte = n;
	for(i = 0;i < byte;i++)
		fputc(0,file);

	fclose(file);

	return;

}

//------------------------------------------------------

void invert(int argc, char** argv)
{
	if(argc < 3) return;
	if(strcmp(argv[1],"-invert") != 0) return;

	int i,n,tmp;
	char *data;

	FILE *file;
	file = fopen(argv[2],"rb+");
	if(file == NULL) return;

	fseek ( file , 0 , SEEK_END );
	n = ftell (file);
	data = malloc(sizeof(char) * n)+1;
	if(data == NULL) return;
	fseek ( file , 0 , SEEK_SET );

	for(i = 0;i < n;i +=2)
	{
		data[i] = fgetc(file);
		data[i+1] = fgetc(file);
		tmp = data[i];
			data[i] = data[i+1];
		data[i+1] = tmp;
	}

	fseek ( file , 0 , SEEK_SET );
	for(i = 0;i < n;i++)
	{
	fputc(data[i],file);
	}

	fclose(file);
	free(data);

	return;

}


//------------------------------------------------------


void compare(int argc, char** argv)
{
	if(argc < 4) return;
	if(strcmp(argv[1],"-compare") != 0) return;

	int byte,byte2;
	char str[500];

	FILE *file1,*file2,*out;

	file1 = fopen(argv[2],"rb");
	file2 = fopen(argv[3],"rb");


	if(file1 == NULL) return;
	if(file2 == NULL) return;

	sprintf(str,"%s_out.txt",argv[2]);
	out = fopen(str,"wb");

	sprintf(str,"offset %s %s\n\n",argv[2],argv[3]);
	fputs(str,out);

	byte = 0;
	int i = 0,n = 0,fst = 0,old_byte = -1;

	while(byte != -1)
	{

		byte = fgetc(file1);
		byte2 = fgetc(file2);


		if(byte != byte2 && byte != old_byte)
		{
			if(fst == 0) fst = i;
			sprintf(str,"%.2x %.2x %.2x\n",i,byte,byte2);
			fputs(str,out);
			n++;
		}

		old_byte = byte;
		i++;
	}

	sprintf(str,"\nndif :%d \nbegin :%x\n",n,fst);
	fputs(str,out);

	fclose(file1);
	fclose(file2);

	fclose(out);

	return;
}



//------------------------------------------------------
void divrom(int argc, char** argv)
{
	if(argc < 4) return;
	if(strcmp(argv[1],"-divrom") != 0) return;

	FILE *file[8],*infile;
	int n = 1,i;
	int byte = 0;
	char str[200];

	infile = fopen(argv[2],"rb");
	n = atoi (argv[3]);

	for(i = 0;i < n;i++)
	{
		if(argc == 5) sprintf(str,"%s%d.bin",argv[4],i);
		else sprintf(str,"rom%d.bin",i);
		file[i] = fopen(str,"wb");
	}

	while(byte != -1)
	{
		for(i = 0;i < n;i++)
		{
			byte = fgetc(infile);
			if(byte != -1)
			fputc(byte,file[i]);
		}
	}


	for(i = 0;i < n;i++)
		fclose(file[i]);

	fclose(infile);
}

void divrom2(int argc, char** argv)
{
	if(argc < 4) return;
	if(strcmp(argv[1],"-divrom2") != 0) return;

	FILE *file[8],*infile;
	int n = 1,i;
	int byte = 0;
	char str[200];

	infile = fopen(argv[2],"rb");
	n = atoi (argv[3]);

	for(i = 0;i < n;i++)
	{
		if(argc == 5) sprintf(str,"%s%d.bin",argv[4],i);
		else sprintf(str,"rom%d.bin",i);
		file[i] = fopen(str,"wb");
	}

	while(byte != -1)
	{
		for(i = 0;i < n;i++)
		{
			byte = fgetc(infile);
			if(byte != -1)
			fputc(byte,file[i]);

			byte = fgetc(infile);
			if(byte != -1)
			fputc(byte,file[i]);
		}
	}


	for(i = 0;i < n;i++)
		fclose(file[i]);

	fclose(infile);
}

void divfile(int argc, char** argv)
{
	if(argc < 4) return;
	if(strcmp(argv[1],"-div") != 0) return;

	FILE *file[8],*infile;
	int n = 1,i;
	int byte = 0;
	char str[200];

	infile = fopen(argv[2],"rb");
	n = atoi (argv[3]);

	for(i = 0;i < n;i++)
	{
		if(argc == 5) sprintf(str,"%s%d",argv[4],i);
		else sprintf(str,"file%d.bin",i);
		file[i] = fopen(str,"wb");
	}


	fseek (infile,0,SEEK_END);
	int size = ftell (infile)/n;
	fseek (infile,0,SEEK_SET);

	i = 0;
	int ibyte = 0;
	while(byte != -1)
	{
		byte = fgetc(infile);
		if(byte != -1)
			fputc(byte,file[i]);

		ibyte++;
		if(ibyte == size)
		{
			i++;
			ibyte = 0;
		}
	}


	for(i = 0;i < n;i++)
		fclose(file[i]);

	fclose(infile);
}

//for Taito F3
void divf3(int argc, char** argv)
{
	if(argc < 4) return;
	if(strcmp(argv[1],"-divf3") != 0) return;

	FILE *file,*file1,*file2,*file3,*file4;

	if(argc == 3) file = fopen(argv[2],"rb+");
	else		  file = fopen("f3.bin","rb+");
	if(file == NULL) return;

	file1 = fopen("d49-13.20","wb");
	file2 = fopen("d49-14.19","wb");
	file3 = fopen("d49-16.18","wb");
	file4 = fopen("d49-20.17","wb");

	int byte = 0;
	while(byte != -1)
	{
		byte = fgetc(file);
		fputc(byte,file1);

		byte = fgetc(file);
		fputc(byte,file2);

		byte = fgetc(file);
		fputc(byte,file3);

		byte = fgetc(file);
		fputc(byte,file4);
	}

	fclose(file);

	fclose(file1);
	fclose(file2);
	fclose(file3);
	fclose(file4);

	return;
}
//------------------------------------------------------
void add(int argc, char** argv)
{
	if(argc < 4) return;
	if(strcmp(argv[1],"-add") != 0) return;

	int i,n;
	int byte = 0;
	n = atoi (argv[3]);

	FILE *file;
	file = fopen(argv[2],"ab");

	if(file == NULL) return;

	if(argc == 5) byte = atoi (argv[4]);

	for(i = 0;i < n;i++)
		fputc(byte,file);

	fclose(file);

	return;
}

void fillrom(int argc, char** argv)
{
	if(argc < 4) return;
	if(strcmp(argv[1],"-fillrom") != 0) return;

	int i,n;
	int byte = 0;
	n = atoi (argv[3]);

	FILE *file;
	file = fopen(argv[2],"ab");

	if(file == NULL) return;
	fseek (file,0,SEEK_END);
	int min = ftell(file);
	n = n-min;
	printf("%d\n",n);

	if(argc == 5) byte = atoi (argv[4]);

	for(i = 0;i < n;i++)
		fputc(byte,file);

	fclose(file);

	return;
}

void invertng(int argc, char** argv)
{
	if(argc < 3) return;
	if(strcmp(argv[1],"-invertng") != 0) return;

	int byte,i,n,tmp;
	char *data;

	FILE *file;
	file = fopen(argv[2],"rb+");
	if(file == NULL) return;
	byte = fgetc(file);
	if(byte != 0)
	{
		fclose(file);
		return;
	}

	byte = 0;
	fseek(file , 0 , SEEK_END);
	n = ftell(file);
	data = malloc(sizeof(char) * n)+1;
	if(data == NULL) return;
	fseek ( file , 0 , SEEK_SET );

	for(i = 0;i < n;i +=2)
	{
		data[i] = fgetc(file);
		data[i+1] = fgetc(file);
		tmp = data[i];
		data[i] = data[i+1];
		data[i+1] = tmp;
	}

	fseek ( file , 0 , SEEK_SET );
	for(i = 0;i < n;i++)
	{
		fputc(data[i],file);
	}

	fclose(file);
	free(data);

	return;

}
