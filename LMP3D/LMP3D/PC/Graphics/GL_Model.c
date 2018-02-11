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
float z = 0;
void LMP3D_Model_Draw(LMP3D_Model *obj)
{
	glLoadIdentity();

	glScalef(obj->scale.x,obj->scale.y,obj->scale.z);
	glTranslatef(obj->position.x,-obj->position.y,-obj->position.z);
	glRotatef(obj->rotate.x*(180.0f/PI),1,0,0);
	glRotatef(obj->rotate.y*(180.0f/PI),0,1,0);
	glRotatef(obj->rotate.z*(180.0f/PI),0,0,1);

z+= 1.01;
	if(obj->type & 0x02)
		glRotatef(-90,1,0,0);


    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//glEnableClientState( GL_NORMAL_ARRAY );

    glVertexPointer(3,GL_FLOAT,0,obj->v);
    glTexCoordPointer(2,GL_FLOAT,0,obj->vt);
    //glNormalPointer(GL_FLOAT,0,obj->vn);

    int i,end,begin;

    if(obj->uv <= 0 || obj->ntexture == 0)
    {
        if(obj->uv == -1)
            glBindTexture(GL_TEXTURE_2D,0);

        if(obj->type & 0x01)
            glDrawElements(GL_TRIANGLES,obj->nf*3,GL_UNSIGNED_SHORT,obj->f);
        else
            glDrawArrays(GL_TRIANGLES,0,obj->nf*3);
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

    //glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );
}

#endif
