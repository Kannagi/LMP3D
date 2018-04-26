#include <stdio.h>
#include <stdlib.h>

#ifdef PC

#include <GL/gl.h>
#include <GL/glu.h>

#include "LMP3D/LMP3D.h"

void LookAt(float eyex, float eyey, float eyez,
		float centerx, float centery, float centerz,
		float upx, float upy, float upz )
{
   float m[16];
   float x[3], y[3], z[3];
   float mag;



   /* Make rotation matrix */

   // Z = Eye - Center;
   z[0] = eyex - centerx;
   z[1] = eyey - centery;
   z[2] = eyez - centerz;

	//Z normalize
   mag = ( z[0]*z[0] + z[1]*z[1] + z[2]*z[2] );
   mag = sqrt(mag);
   if (mag)
   {  /* mpichler, 19950515 */
      z[0] /= mag;
      z[1] /= mag;
      z[2] /= mag;
   }

   /*  Y = Up;*/
   y[0] = upx;
   y[1] = upy;
   y[2] = upz;

   /* X vector = Y cross Z */
   x[0] =  y[1]*z[2] - y[2]*z[1];
   x[1] = -y[0]*z[2] + y[2]*z[0];
   x[2] =  y[0]*z[1] - y[1]*z[0];

   /* Recompute Y = Z cross X */
   y[0] =  z[1]*x[2] - z[2]*x[1];
   y[1] = -z[0]*x[2] + z[2]*x[0];
   y[2] =  z[0]*x[1] - z[1]*x[0];

   /* mpichler, 19950515 */
   /* cross product gives area of parallelogram, which is < 1.0 for
    * non-perpendicular unit-length vectors; so normalize x, y here
    */

   mag = ( x[0]*x[0] + x[1]*x[1] + x[2]*x[2] );
   mag = sqrt(mag);
   if (mag) {
      x[0] /= mag;
      x[1] /= mag;
      x[2] /= mag;
   }

   mag = ( y[0]*y[0] + y[1]*y[1] + y[2]*y[2] );
   mag = sqrt(mag);
   if (mag) {
      y[0] /= mag;
      y[1] /= mag;
      y[2] /= mag;
   }

	float matrix[16];
	matrix[(0<<2)+0] = x[0];
	matrix[(0<<2)+1] = x[1];
	matrix[(0<<2)+2] = x[2];
	matrix[(0<<2)+3] = 0;

	matrix[(1<<2)+0] = y[0];
	matrix[(1<<2)+1] = y[1];
	matrix[(1<<2)+2] = y[2];
	matrix[(1<<2)+3] = 0;

	matrix[(2<<2)+0] = z[0];
	matrix[(2<<2)+1] = z[1];
	matrix[(2<<2)+2] = z[2];
	matrix[(2<<2)+3] = 0;

	matrix[(3<<2)+0] = -eyex;
	matrix[(3<<2)+1] = -eyey;
	matrix[(3<<2)+2] = -eyez;
	matrix[(3<<2)+3] = 1.0f;





   glLoadMatrixf(matrix);



}

void LMP3D_Model_Draw(LMP3D_Model *model)
{
	glLoadIdentity();

	glScalef(model->scale.x,model->scale.y,model->scale.z);
	glTranslatef(model->position.x,-model->position.y,-model->position.z);
	glRotatef(model->rotate.x*(180.0f/PI),1,0,0);
	glRotatef(model->rotate.y*(180.0f/PI),0,1,0);
	glRotatef(model->rotate.z*(180.0f/PI),0,0,1);


	glRotatef(-90,1,0,0);

	int i,end,begin,sizeindex = GL_UNSIGNED_SHORT;

	if(model->flag & LMP3D_MODEL_INDEX_U32) sizeindex = GL_UNSIGNED_INT;

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer(3,GL_FLOAT,0,model->v);

	if(model->flag & LMP3D_MODEL_TEXTCOORD)
	{
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2,GL_FLOAT,0,model->vt);
	}


    if(model->flag & LMP3D_MODEL_NORMAL)
	{
		glEnableClientState( GL_NORMAL_ARRAY );
		glNormalPointer(GL_FLOAT,0,model->vn);
	}

    if((model->flag & LMP3D_MODEL_TEXTUREDISABLE) || model->ntexture == 0)
    {

        if(model->flag & LMP3D_MODEL_TEXTUREDISABLE)
            glBindTexture(GL_TEXTURE_2D,0);

        if(model->flag & LMP3D_MODEL_INDEX)
            glDrawElements(GL_TRIANGLES,model->nf*3,sizeindex,model->index);
        else
            glDrawArrays(GL_TRIANGLES,0,model->nf*3);
    }
    else
    for(i = 0;i < model->ntexture;i++)
    {
        glBindTexture(GL_TEXTURE_2D,model->texture[i]->address);

        begin = model->texture_begin[i];

        if(i+1 < model->ntexture)
            end = model->texture_begin[i+1];
        else
            end = model->nf*3;

        end -= begin;

        if(model->flag & LMP3D_MODEL_INDEX)
            glDrawElements(GL_TRIANGLES,end,sizeindex,model->index + begin);
        else
            glDrawArrays( GL_TRIANGLES, begin,end);

    }


	if(model->flag & LMP3D_MODEL_NORMAL)
		glDisableClientState( GL_NORMAL_ARRAY );


    if(model->flag & LMP3D_MODEL_TEXTCOORD)
		glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}

#endif
