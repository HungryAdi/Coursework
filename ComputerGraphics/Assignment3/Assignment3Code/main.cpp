
#include <iostream>
#include <time.h>
#include <GLUT/GLUT.h>

#include "inputModule.h"
#include "PLY.h"
#include "geometry.h"

using namespace std;

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

int window;

GLfloat fieldOfView;
GLfloat aspect;
GLfloat nearPlane;
GLfloat farPlane;

int updateFlag;

PLYObject *ply;

Vector4f initial_light_pos = {-100.0, 100.0, 100.0, 0.0};
Vector4f ambient_light = {0.3, 0.3, 0.3, 1.0};
Vector4f light_color = {0.6, 0.6, 0.6, 1.0};
Vector4f black_color = {0.0, 0.0, 0.0, 1.0};

Vector3f light_pos, viewer_pos;


void cleanup(int sig)
{
  // insert cleanup code here (i.e. deleting structures or so)
  if (ply)
    delete(ply);
  exit(0);
}


//##########################################
// OpenGL Display function

void display(void)
{
  int i, j;
  float M[16];
  Matrix4f m;
  
  glutSetWindow(window);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setLightTho();
    glGetFloatv(GL_MODELVIEW_MATRIX, M);
    for (i = 0; i < 3; i++) {
        m[i][3] = 0.0;
        m[3][i] = 0.0;
        for (j = 0; j < 3; j++) {
            m[i][j] = M[i*4+j];
        }
    }
    m[3][3] = 1.0;
    multVector(light_pos, m, initial_light_pos);
    
  setUserView();

  glGetFloatv(GL_MODELVIEW_MATRIX, M);
  for (i = 0; i < 3; i++) {
    m[i][3] = 0.0;
    m[3][i] = 0.0;
    for (j = 0; j < 3; j++) {
      m[i][j] = M[i*4+j];
    }
  }
  m[3][3] = 1.0;
  multVector(viewer_pos, m, current_pos);
    light_pos[3] = 0;
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
	ply->draw();
  
  glutSwapBuffers();
}


//##########################################
// Init display settings

void initDisplay()
{
  fieldOfView = 45.0;
  aspect = (float)IMAGE_WIDTH/IMAGE_HEIGHT;
  nearPlane   = 0.1;
  farPlane    = 50.0;

  /* setup context */
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  gluPerspective( fieldOfView, aspect, nearPlane, farPlane );


  // set basic matrix mode
  glMatrixMode(GL_MODELVIEW);

  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClearIndex(0);
  glClearDepth(1);

  // setup lights
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black_color); // No global ambient light
  glLightfv(GL_LIGHT0, GL_POSITION, initial_light_pos);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

  glEnable(GL_LIGHT0);
}

//##########################################
// Main function

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

	FILE *in;
  char *filename;

  filename = "/Users/HungryAdi/Projects/osx/112/Assignment3/Assignment3/bunny.ply";
  if (!(in = fopen(filename, "r"))) {
	  cerr << "Cannot open input file " << filename << endl;
    exit(1);
  }

  ply = new PLYObject(in);
  ply->resize();
  srand(time(NULL));

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(IMAGE_WIDTH,IMAGE_HEIGHT);
	
  window = glutCreateWindow("Assignment 3");

  glutDisplayFunc(display);
  glutKeyboardFunc(readKeyboard);
  glutMouseFunc(mouseButtHandler);
  glutMotionFunc(mouseMoveHandler);
  glutPassiveMotionFunc(mouseMoveHandler);
  glutIdleFunc(NULL);

  initDisplay();

  glutMainLoop();

  return 0;             /* ANSI C requires main to return int. */
}
