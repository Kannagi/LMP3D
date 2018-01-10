#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <GL/glu.h>

#include "LMP3D/LMP3D.h"



void LMP3D_Model_Draw(LMP3D_Model *obj)
{
	glLoadIdentity();
	//glScalef(obj->scale.x,-obj->scale.y,-obj->scale.z);
	glTranslatef(obj->position.x,-obj->position.y,-obj->position.z);
	glRotatef(obj->rotate.z*(180.0f/PI),0,1,0);


	if(obj->type & 0x02)
		glRotatef(-90,1,0,0);


    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glVertexPointer(3,GL_FLOAT,0,obj->v);
    glTexCoordPointer(2,GL_FLOAT,0,obj->vt);
    glNormalPointer(GL_FLOAT,0,obj->vn);

    int i,end,begin;

    if(obj->uv <= 0 || obj->ntexture == 0)
    {
        if(obj->uv == -1)
            glBindTexture(GL_TEXTURE_2D,0);

        if(obj->type & 0x01)
            glDrawElements(GL_TRIANGLES,obj->nf*3,GL_UNSIGNED_SHORT,obj->f);
        else
            glDrawArrays( GL_TRIANGLES,0,obj->nf*3);
    }
    else
    for(i = 0;i < obj->ntexture;i++)
    {
        glBindTexture(GL_TEXTURE_2D,obj->texture[i]->address);

        begin = obj->texture_begin[i];

        if(i+1 < obj->ntexture)
            end = obj->texture_begin[i+1];
        else
            end = obj->nf*3;

        end -= begin;

        if(obj->type & 0x01)
            glDrawElements(GL_TRIANGLES,end,GL_UNSIGNED_SHORT,obj->f + begin);
        else
            glDrawArrays( GL_TRIANGLES, begin,end);

    }

    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}

#endif
