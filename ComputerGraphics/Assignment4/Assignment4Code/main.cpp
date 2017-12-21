#include "sceneModule.h"
#include "inputModule.h"

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

int window;

GLfloat fieldOfView;
GLfloat aspect;
GLfloat nearPlane;
GLfloat farPlane;

GLfloat initial_light_pos[4] = {-100.0, 100.0, 100.0, 0.0};
GLfloat ambient_light[4] = {0.3, 0.3, 0.3, 1.0};
GLfloat light_color[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat black_color[4] = {0.0, 0.0, 0.0, 1.0};

const int checkImageWidth = 64;
const int checkImageHeight = 64;

// name of the texture you will generate (actually it's an ID)
GLuint textureName;

// the array for actually storing texture
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

//##########################################
// Init display settings

void initDisplay( ){
  /* Perspective projection parameters */
	
  fieldOfView = 45.0;
  aspect = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
  nearPlane   = 0.1;
  farPlane    = 50.0;

  /* setup context */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( fieldOfView, aspect, nearPlane, farPlane );

  glEnable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE );

  glClearColor( 0.2, 0.2, 0.2, 1.0 );
  glClearDepth( 1 );

  glMatrixMode( GL_MODELVIEW );

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black_color); // No global ambient light
  glLightfv(GL_LIGHT0, GL_POSITION, initial_light_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
}

// create a checkboard texture pattern. 
// if you would like, you may change this function to make your own texture pattern.
void makeCheckImage(void) {
  for (int i=0; i<checkImageHeight; i++) {
    for (int j=0; j<checkImageWidth; j++) {
      int c = (((i & 0x8) ^ (j & 0x8)) == 0) * 255;
      checkImage[i][j][0] = (GLubyte)c;
      checkImage[i][j][1] = (GLubyte)c;
      checkImage[i][j][2] = (GLubyte)c;
      checkImage[i][j][3] = (GLubyte)255;
    }
  }
}

void initTexture() 
{
  makeCheckImage();

  // TODO: 1. generate texture using glGenTextures
  glGenTextures(1, &textureName);

  // TODO: 2. bind the texture, using glBindTexture
  glBindTexture(GL_TEXTURE_2D, textureName);

  // TODO: 3. specify texture parameters, using glTexParameteri
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // TODO: 4. specify image data to the texture, using glTexImage2D
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,checkImage);
}


//##########################################
// Main function

int main( int argc, char **argv ){

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |
                       GLUT_DEPTH | GLUT_MULTISAMPLE );

  glutInitWindowSize( IMAGE_WIDTH,IMAGE_HEIGHT );

  glutInitWindowPosition(100,120);
  window = glutCreateWindow("Assignment 4");

  /* Register the appropriate callback functions with GLUT */
  glutDisplayFunc( display );
  glutKeyboardFunc( readKeyboard );
  glutMouseFunc( mouseButtHandler );
  glutMotionFunc( mouseMoveHandler );
  glutPassiveMotionFunc( mouseMoveHandler );

  initDisplay( );
  initTexture();
  glutMainLoop( );

  return( 0 );
}
