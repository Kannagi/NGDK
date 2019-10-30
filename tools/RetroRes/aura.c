#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aura.h"

void AURA_Init(AURA *aura)
{
	aura->text[0] = 0;
	aura->str[0] = 0;

	aura->i = 0;
	aura->l = 0;
	aura->com = 0;
	aura->rinst = 0;
	aura->rinstw = 0;
	aura->ext = 0;
	aura->preproc = 0;
	aura->point = 0;

	aura->n = 0;
	aura->nvar = 0;
	aura->opmax = 0;

	int i;
	for(i = 0;i<4;i++)
		aura->bloc[i] = 0;
}
int AURA_Analyse_Number(AURA *aura)
{
	int i =0;

	while(aura->str[i] != 0)
	{
		if(aura->str[i] < '0')
			return 0;

		if(aura->str[i] > '9')
			return 0;

		i++;
	}

	return 1;
}

void AURA_Analyse_R(char letter,AURA *aura)
{
	if(letter > 32) aura->rinst++;
	else if(aura->rinst > 0) aura->rinst++;

	if(letter == '\n')
		aura->rinst = 0;

}

void AURA_Analyse_Com(char letter,AURA *aura)
{
	static int com = 0;
	if(aura->com == 0)
	{
		if(letter == '/')
		{
			if(com <= 1)
				com++;
		}else
		{
			if(com >= 2)
				aura->com = 3;
			else
				com = 0;


		}

	}

	if(letter == '\n')
	{
		if(com >= 2)
			aura->com = 0;
		com = 0;
	}
}


void AURA_Analyse_Text(char letter,AURA *aura)
{
	if( aura->com == 0 && letter == '"')
    {
        aura->com = 1;
        return;
    }

	if( aura->com == 1 && letter == '"')
	{
		aura->com = 2;
		return;
	}


    if( aura->com == 0 && letter == '\'')
    {
        aura->com = 1;
        return;
    }
	if( aura->com == 1 && letter == '\'')
	{
		aura->com = 2;
		return;
	}

	if(aura->com == 1 && aura->l < 199)
	{
		aura->text[aura->l] = letter;
		aura->l++;
		aura->text[aura->l] = 0;
	}


	if( aura->com == 2)
	{
		aura->com = 0;
		aura->l = 0;
	}

}
int AURA_Analyse_Word(char letter,AURA *aura)
{

	if(aura->com > 0)
	{
		if(aura->i > 0)
        {
            aura->str[aura->i] = 0;
            aura->i = 0;
            aura->rinstw++;
            return 1;
        }
        aura->i = 0;
		return 0;
	}

	if( (letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || (letter >= '0' && letter <= '9') || (letter == '_') )
    {
        aura->str[aura->i] = letter;
        aura->i++;
        return 0;
    }else
    {
        if(aura->i > 0)
        {
        	if(letter == '\n')
				aura->rinstw = -1;
            aura->str[aura->i] = 0;
            aura->i = 0;
            aura->rinstw++;
            return 1;
        }

        if(letter == '\n')
			aura->rinstw = 0;
        aura->i = 0;
    }

    return letter;
}
