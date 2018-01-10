#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <GL/glu.h>

#include "LMP3D/LMP3D.h"


void MNS_draw(float l,float h,Vector2 position,float tl,float th,float *vt)
{

    position.x = (position.x-(l/2))/(l/2);
    position.x = position.x + tl/l;


    position.y = ((h/2)-position.y)/(h/2);
    position.y = position.y - tl/h;



	glLoadIdentity();

    glTranslatef(position.x,position.y,0);

    l = tl/l;
    h = th/h;

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    float va[12] = {-l,h,0,l,h,0,-l,-h,0,l,-h,0};

    glVertexPointer(3,GL_FLOAT,0,va);
    glTexCoordPointer(2,GL_FLOAT,0,vt);

	glDrawArrays( GL_TRIANGLE_STRIP, 0,4);

    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}

void MNS_blit(Rect tile,Rectf *t,Vector2 p)
{
    float vt[8];

    if(t != NULL)
    {

        t->w = t->w/tile.w;
        t->h = t->h/tile.h;

        t->x = t->x/tile.w;
        t->y = t->y/tile.h;

        vt[0] = t->x;
        vt[1] = t->y;

        vt[2] = t->x+t->w;
        vt[3] = t->y;

        vt[4] = t->x;
        vt[5] = t->y+t->h;

        vt[6] = t->x+t->w;
        vt[7] = t->y+t->h;

        tile.w = tile.w*t->w;
        tile.h = tile.h*t->h;

    }else
    {
        vt[0] = 0.0;
        vt[1] = 0.0;

        vt[2] = 1.0;
        vt[3] = 0.0;

        vt[4] = 0.0;
        vt[5] = 1.0;

        vt[6] = 1.0;
        vt[7] = 1.0;
    }

    MNS_draw(tile.x,tile.y,p,tile.w,tile.h,vt);
}

void MNS_blit2D(LMP3D_Texture texture,Rectf *vt,Vector2 p)
{
    Rect tile;
    tile.x = 640;
    tile.y = 480;

    tile.w = texture.w;
    tile.h = texture.h;

    glBindTexture(GL_TEXTURE_2D,texture.address);
    MNS_blit(tile,vt,p);
}



void MNS_blit2D256(Rectf *vt,Vector2 p)
{
    Rect tile;
    tile.x = 640;
    tile.y = 480;

    tile.w = 256;
    tile.h = 256;

    //glBindTexture(GL_TEXTURE_2D,texture.id);
    MNS_blit(tile,vt,p);
}

void bitmap_font2(char *texte,int tx,int ty)
{
    int i = 0;
    int x = tx;
	Vector2 p;

	Rectf vf;
	vf.x = 0;
	vf.y = 0;
	vf.w = 16;
	vf.h = 16;

	int letter;

    while(texte[i] != 0)
    {
		p.x = tx;
		p.y = ty;



        if(texte[i] != ' ' && texte[i] != '\n')
        {
        	vf.x = (texte[i]%16)<<4;
			vf.y = (texte[i]/16)<<4;

			vf.w = 16;
			vf.h = 16;

			MNS_blit2D256(&vf,p);
        }


        tx += 16;

        if(texte[i] == '\n')
        {
            tx = x;
            ty += 16;
        }

        i++;
    }
}

#endif
