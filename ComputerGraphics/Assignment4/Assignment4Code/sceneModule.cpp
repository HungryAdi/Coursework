#include "sceneModule.h"

GLfloat angley = 20;    /* in degrees */
GLfloat anglex = 30;   /* in degrees */
GLfloat distanceX = 0.0;
GLfloat distanceY = 0.0;
GLfloat distanceZ = 5.0;

GLfloat ambient[4] = {0.2, 0.2, 0.2, 1.0};
GLfloat diffuse[4] = {0.7, 0.7, 0.7, 1.0};
GLfloat specular[4] = {0.5, 0.5, 0.5, 1.0};
GLfloat shininess[1] = {5.0};

extern GLuint textureName;

void drawScene(){
    // lighting
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glColor3fv(diffuse);

    // TODO 5. enable texture 2D using glEnable
    glEnable(GL_TEXTURE_2D);

    // TODO 6. set texture environment parameters using glTexEnvf
    // recommended parameters are 
    //    target: GL_TEXTURE_ENV
    //    pname : GL_TEXTURE_ENV_MODE
    //    param : GL_MODULATE 
    // after you finish your project, try replace GL_MODULATE with GL_REPLACE
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // TODO 7. create a quad using gluNewQuadric
    GLUquadric* quad = gluNewQuadric();

    // TODO 8. enable quadric texture generation using gluQuadricTexture
    gluQuadricTexture(quad, true);
    
    // TODO 9. draw a sphere using gluSphere
    //    radius = 1
    //    slices = 20
    //    stacks = 20
    GLdouble radius = 1;
    GLint slices = 20;
    GLint stacks = 20;
    gluSphere(quad, radius, slices, stacks);

    // TODO 10. disable texture using GLDisable
    glDisable(GL_TEXTURE_2D);
}

void sceneTransformation(){
  glLoadIdentity( );

  glTranslatef(-distanceX, distanceY, -distanceZ);
  glRotatef( anglex, 1.0, 0.0, 0.0 );
  glRotatef( angley, 0.0, 1.0, 0.0 );
}

void display(){
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  /* Set up transformation */
  sceneTransformation();
  /* Draw the scene into the back buffer */
  drawScene();
  /* Swap the front buffer with the back buffer - assumes double buffering */
  glutSwapBuffers();
}