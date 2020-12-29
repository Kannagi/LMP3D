#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int LMP3D_Script_Syntax_Analyse(int *i,char letter,char *word)
{
    if(letter > 32)
    {
        word[*i] = letter;
        *i = *i+1;
    }
    else
    {
        word[*i] = 0x00;
        if(*i > 0)
        {
            *i = 0;
            return 1;
        }
        *i = 0;
    }

    return 0;

}

int LMP3D_Script_Syntax_Find(int *i,char letter,char *word,char *string)
{
    if
    (
    ( (letter >= 'a') && (letter <= 'z') ) ||
    ( (letter >= 'A') && (letter <= 'Z') ) ||
    ( (letter >= '0') && (letter <= '9') ) ||
    (letter == '_') || (letter == '[') || (letter == ']') || (letter == '(') || (letter == ')')
    )
    {
        string[*i] = letter;
        *i = *i+1;
    }
    else
    {
        string[*i] = 0x00;
        if(*i > 0)
        {
            *i = 0;
            if(strcmp(word,string) == 0)
                return 1;
        }
        *i = 0;
    }

    return 0;

}

int LMP3D_Script_Syntax_Find_Extented(int *i,char letter,char *word,char *string)
{
    if(letter > 32)
    {
        string[*i] = letter;
        *i = *i+1;
    }
    else
    {
        string[*i] = 0x00;
        if(*i > 0)
        {
            *i = 0;
            if(strcmp(word,string) == 0)
                return 1;
        }
        *i = 0;
    }

    return 0;

}



int LMP3D_Script_Syntax(char begin,char end,char letter,int *i,int *ignore,char *word)
{
    if( (letter == end) && (*ignore == 1) )
    {
        *ignore = 0;
        word[*i] = 0x00;
        *i =0;
        return 1;
    }

    if(*ignore == 1)
    {
        word[*i] = letter;
        *i = *i+1;
    }

    if(letter == begin)
        *ignore = 1;

    return 0;
}

int LMP3D_Script_Syntax_Ignore(char begin,char end,char letter,int *ignore)
{
    if( (letter == end) && (*ignore == 1) )
    {
        *ignore = 0;
        return 2;
    }

    if( (letter == begin) && (*ignore == 0) )
    {
        *ignore = 1;
        return 1;
    }

    return 0;
}

void LMP3D_Script_Read(char *string,char *string_out,int nbr)
{
    int i,l = 0,n = 0,nstr = strlen(string)+1;

    for(i = 0;i < nstr;i++)
    {
        if(string[i] > 32)
        {
            if(string_out != NULL) string_out[l] = string[i];
            l++;

        }else
        {
            if(l > 0)
            {

                if(string_out != NULL) string_out[l] =  0;
                l = 0;
                if(n == nbr) return;

                n++;
            }
        }
    }

}

int LMP3D_Script_Read_Ext(char *string,char *string_out,int nbr,char no)
{
    int i,l = 0,j = 0,n = strlen(string)+1;

    for(i = 0;i < n;i++)
    {
        if( (string[i] > 32) && (string[i] != no) )
        {
            if(string_out != NULL) string_out[l] = string[i];
            l++;

        }else
        {
            if(l > 0)
            {

                if(string_out != NULL) string_out[l] =  0;
                l = 0;
                if(j == nbr) return 1;

                j++;
            }
        }
    }

    return 0;

}

void LMP3D_Folder_Out(char *filename,char *folder)
{
    int i = 0;
    char string[100];
    folder[0] = 0;
    while(LMP3D_Script_Read_Ext(filename,string,i,'/') != 0)
    {
        if(LMP3D_Script_Read_Ext(filename,NULL,i+1,'/') != 0)
        {
            strcat(folder,string);
            strcat(folder,"/");
        }
        i++;
    }
}

void LMP3D_Script_Fgets(FILE *file,char *string)
{
    int octet = 1,i = 0;

    while(octet != 0)
    {
        octet = fgetc(file);
        string[i] = octet;
        i++;
    }

    string[i] = 0;
}

