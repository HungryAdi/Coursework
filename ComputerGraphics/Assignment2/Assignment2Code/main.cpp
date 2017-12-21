/*
 * OpenGL demonstration program for UCI ICS Computer Graphics courses
 * main.cpp,v 2.2 2016/10/05 11:38 pm
 *
 */

#include "sceneModule.h"
#include "inputModule.h"

#include "my_gl.h"

#define IMAGE_WIDTH  512
#define IMAGE_HEIGHT 512

int window;

GLfloat fieldOfView;
GLfloat aspect;
GLfloat nearPlane;
GLfloat farPlane;

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
    
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClearDepth( 1 );
    
    glMatrixMode( GL_MODELVIEW );
}

//##########################################
// Main function

int main( int argc, char **argv ){
    
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |
                        GLUT_DEPTH | GLUT_MULTISAMPLE );
    
    glutInitWindowSize( IMAGE_WIDTH,IMAGE_HEIGHT );
    
    glutInitWindowPosition(100,120);
    window = glutCreateWindow("Assignment 1");
    
    /* Register the appropriate callback functions with GLUT */
    glutDisplayFunc( display );
    glutKeyboardFunc( readKeyboard );
    glutMouseFunc( mouseButtHandler );
    glutMotionFunc( mouseMoveHandler );
    glutPassiveMotionFunc( mouseMoveHandler );
    
    glutIdleFunc(display);
    
    initDisplay( );
    glutMainLoop( );
    
    return( 0 );
}
