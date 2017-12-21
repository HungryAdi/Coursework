/*
 * OpenGL demonstration program for UCI ICS Computer Graphics courses
 * sceneModule.cpp,v 2.1 2016/10/05 11:38 pm
 *
 */

#include "sceneModule.h"

#include "my_gl.h"
#include <math.h>

#define PI 3.14159265359

GLfloat angley = 20;    /* in degrees */
GLfloat anglex = 30;   /* in degrees */
GLfloat distanceX = 0.0;
GLfloat distanceY = 0.0;
GLfloat distanceZ = 5.0;
GLdouble frame = 0.0;

static float cubeColors[6][4] =
{
  {0.8, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.0, 1.0},
  {0.0, 0.8, 0.0, 1.0},
  {0.0, 0.0, 0.8, 1.0},
  {0.0, 0.8, 0.8, 1.0},
  {0.8, 0.0, 0.8, 1.0},
};

static float cubeVertexes[6][4][4] =
{
  {
    {-1.0, -1.0, -1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0}},

  {
    {-1.0, -1.0, -1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0}},

  {
    {-1.0, -1.0, -1.0, 1.0},
    {-1.0, 1.0, -1.0, 1.0},
    {1.0, 1.0, -1.0, 1.0},
    {1.0, -1.0, -1.0, 1.0}},

  {
    {1.0, 1.0, 1.0, 1.0},
    {-1.0, 1.0, 1.0, 1.0},
    {-1.0, -1.0, 1.0, 1.0},
    {1.0, -1.0, 1.0, 1.0}}
};

void drawScene(){

  for (int i = 0; i < 6; i++) {
    glBegin(GL_POLYGON);
		glColor3fv(&cubeColors[i][0]);
		glVertex4fv(&cubeVertexes[i][0][0]);
		glVertex4fv(&cubeVertexes[i][1][0]);
		glVertex4fv(&cubeVertexes[i][2][0]);
		glVertex4fv(&cubeVertexes[i][3][0]);
    glEnd();
  }
}

void sceneTransformation(){
    
    glLoadIdentity( );
    
    
    //  glTranslatef(-distanceX, distanceY, -distanceZ);
    
    //  glRotatef( anglex, 1.0, 0.0, 0.0 );
    
    //  glRotatef( angley, 0.0, 1.0, 0.0 );
    
    //    return;
    
    
    
    // The code below should have exactly the same behavior as the three
    //lines above.
    
    GLdouble eyeX, eyeY, eyeZ;
    
    GLdouble atX = 0, atY = 0, atZ = 0;
    
    GLdouble upX = 0, upY = 1, upZ = 0;
    
    
    
    while(anglex < 0)
        
        anglex += 360;
    
    while(anglex >= 360)
        
        anglex -= 360;
    
    if (anglex > 90 && anglex < 270)
        
        upY = -1;
    
    
    
    double radian_x = anglex * (PI / 180), radian_y = angley * (PI / 180);
    
    eyeX = distanceX * cos(radian_y) - sin(radian_y) * sin(radian_x) *
    distanceY - sin(radian_y) * cos(radian_x) * distanceZ;
    
    eyeY = -cos(radian_x) * distanceY + sin(radian_x) * distanceZ;
    
    eyeZ = sin(radian_y) * distanceX - cos(radian_y) * sin(radian_x) *
    distanceY + cos(radian_y) * cos(radian_x) * distanceZ;
    
    gluLookAt(eyeX, eyeY, eyeZ, atX, atY, atZ, upX, upY, upZ);
    
}


void display(){
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  /* Set up transformation */
  sceneTransformation();
  /* Draw the scene into the back buffer */
  glRotated(frame++, 0, 1.0, 0);
  drawScene();
  glTranslatef(3, 0, 0);
  glScaled(0.5, 0.5, 0.5);
  drawScene();
  
    
  /* Swap the front buffer with the back buffer - assumes double buffering */
  glutSwapBuffers();
}