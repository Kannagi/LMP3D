#ifndef __LMP3D_Load__
#define __LMP3D_Load__



LMP3D_Texture* LMP3D_Load_Texture(const char *adresse);
void LMP3D_Load_Texture_Array(LMP3D_Model *obj,char *dossier);

//Load Image Format
LMP3D_Texture *LMP3D_Load_png(const char *adresse);


//Load Model Format
LMP3D_Model *LMP3D_Load_Model(char *nom_du_fichier);
LMP3D_Model *LMP3D_Load_Model_smd(char *nom_du_fichier);
LMP3D_Model *LMP3D_Load_Model_obj(char *nom_du_fichier);
LMP3D_Model *LMP3D_Load_Model_bcm(char *nom_du_fichier);

LMP3D_Anim3D *LMP3D_Load_Anim3D(char *nom_du_fichier);


void LMP3D_SMD_Create_Skeleton(LMP3D_Anim3D *anim,int n,short *bones);

#endif

