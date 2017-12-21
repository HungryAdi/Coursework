#ifndef INPUTMODULE_H
#define INPUTMODULE_H

#include <GLUT/glut.h>

/*---------------external entities from sceneModule-----------------*/
extern GLfloat angley;   
extern GLfloat anglex;  
extern GLfloat distanceX;
extern GLfloat distanceY;
extern GLfloat distanceZ;

/*---------------external entities from main-------------------------*/
extern void cleanup();

void readKeyboard( unsigned char key, int x, int y );
void readSpecialKeys( int key, int x, int y );
void mouseButtHandler( int button, int state, int x, int y );
void mouseMoveHandler( int x, int y );

#endif // INPUTMODULE_H