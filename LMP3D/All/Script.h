#ifndef __LMP3D_Script__
#define __LMP3D_Script__

int LMP3D_Script_Syntax_Find(int *i,char lettre,char *mots,char *chaine);
int LMP3D_Script_Syntax_Find_Extented(int *i,char lettre,char *mots);
int LMP3D_Script_Syntax_Analyse(int *i,char lettre,char *mots);
int LMP3D_Script_Syntax(char debut,char fin,char lettre,int *i,int *ignorer,char *mots);
int LMP3D_Script_Syntax_Ignore(char debut,char fin,char lettre,int *ignorer);
void LMP3D_Script_Read(char *chaine,char *ch,int nbr);
int LMP3D_Script_Read_Ext(char *chaine,char *ch,int nbr,char no);
//void LMP3D_Script_Fgets(FILE *fichier,char *chaine);
void LMP3D_Folder_Out(char *nom_du_fichier,char *dossier);

#endif

