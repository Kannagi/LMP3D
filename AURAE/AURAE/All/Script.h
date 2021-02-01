#ifndef __AURAE_Script__
#define __AURAE_Script__

int AURAE_Script_Syntax_Find(int *i,char lettre,char *mots,char *chaine);
int AURAE_Script_Syntax_Find_Extented(int *i,char lettre,char *mots);
int AURAE_Script_Syntax_Analyse(int *i,char lettre,char *mots);
int AURAE_Script_Syntax(char debut,char fin,char lettre,int *i,int *ignorer,char *mots);
int AURAE_Script_Syntax_Ignore(char debut,char fin,char lettre,int *ignorer);
void AURAE_Script_Read(char *chaine,char *ch,int nbr);
int AURAE_Script_Read_Ext(char *chaine,char *ch,int nbr,char no);
//void AURAE_Script_Fgets(FILE *fichier,char *chaine);
void AURAE_Folder_Out(char *nom_du_fichier,char *dossier);

#endif

