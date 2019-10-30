#ifndef _AURA_
#define _AURA_

typedef struct
{
	char text[200];
    char str[200];
    int i,l,com,rinst,rinstw,ext,preproc,point;
    int bloc[4];
    int n,opmax,nvar;

}AURA;

int AURA_Analyse_Word(char letter,AURA *aura);
void AURA_Analyse_Text(char letter,AURA *aura);
void AURA_Analyse_R(char letter,AURA *aura);
int AURA_Analyse_Number(AURA *aura);
void AURA_Init(AURA *aura);
#endif
