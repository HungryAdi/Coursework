

#ifdef WIN32
#include <windows.h>
#endif

#include <GLUT/GLUT.h>
#include <math.h>

//for debugging
#include <iostream>

// Defines maximum stack capacity.
#define STACK_CAP 16
// Defines pi for converting angles.
#define PI 3.14159265359

// Structure for the matrix stack, top specifies current top position on the stack, initially zero (which means one matrix in the stack)
struct matrix_stack
{
    GLdouble m[STACK_CAP][16];
    int top;
};

// Define a macro for retrieving current matrix from top of current stack.
#define current_matrix (current_stack->m[current_stack->top])

// Identity matrix constant.
const GLdouble identity[16] =
{1, 0, 0, 0,
 0, 1, 0, 0,
 0, 0, 1, 0,
 0, 0, 0, 1};

// the model view matrix stack.
struct matrix_stack model_view = {{{0}}, 0};
// the projection matrix stack.
struct matrix_stack projection = {{{0}}, 0};
// The current stack pointer that specifies the matrix mode.
struct matrix_stack *current_stack = &model_view;

//HELPER FUNCTIONS
GLdouble deg_to_rad(GLdouble degrees) {
    return (degrees*PI/180.0);
}

void printMat() {
    std::cout << "[";
    for (int i = 0; i < STACK_CAP; i++) {
        if (i % 4 != 0) {
            std::cout << ",";
        } else if (i % 4 == 0 && i != 0){
            std::cout << "\n";
        }
        std::cout << current_matrix[i];
        
    }
    std::cout << "]\n\n";
    
}

void transpose(GLdouble *matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = i+1; j < 4; ++j) {
            std::swap(matrix[4*i + j], matrix[4*j + i]);
        }
    }
    
}


// Multiplies current matrix with matrix b, put the result in current matrix.
// current = current * b
void matrix_multiply(const GLdouble *b)
{
    GLdouble tempMatrix[16];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)  {
            GLdouble total = 0.0;
            for (int k = 0; k < 4; k++) {
                total = total + current_matrix[k * 4 + j] * b[i * 4 + k];
            }
            tempMatrix[i * 4 + j] = total;
        }
    }
    
    for (int l = 0; l < STACK_CAP; l++) {
        current_matrix[l] = tempMatrix[l];
    }
    
        //transpose(current_matrix);
    
    //std::cout << "Current Matrix\n";
    //printMat();
}

// Calculates cross product of b and c, put the result in a
// a = b x c
void cross_product(GLdouble *ax, GLdouble *ay, GLdouble *az,
    GLdouble bx, GLdouble by, GLdouble bz,
    GLdouble cx, GLdouble cy, GLdouble cz)
{
    *ax = (by*cz - bz*cy);
    *ay = (bz*cx - bx*cz);
    *az = (bx*cy - by*cx);
}

// Normalizes vector (x, y, z).
void normalize(GLdouble *x, GLdouble *y, GLdouble *z)
{
    GLdouble magnitude = sqrt((*x)*(*x) + (*y)*(*y) + (*z)*(*z)); //calculate magnitude of vector (x, y, z)
    //do normalization
    *x = (*x)/magnitude;
    *y = (*y)/magnitude;
    *z = (*z)/magnitude;
}

// Switches matrix mode by changing the current stack pointer.
void I_my_glMatrixMode(GLenum mode)
{
    if (mode == GL_MODELVIEW) {
        current_stack = &model_view;
    } else if (mode == GL_PROJECTION) {
        current_stack = &projection;
    }
}

// Overwrites current matrix with identity matrix.
void I_my_glLoadIdentity(void)
{
    //TEST THIS (current_matrix is 2d 16x16?)
    for (int i = 0; i < STACK_CAP; ++i) {
        current_matrix[i] = identity[i];
    }
}

// Pushes current matrix onto current stack, report error if the stack is already full.
void I_my_glPushMatrix(void)
{
    if (current_stack->top == 15) {
        throw GL_STACK_OVERFLOW;
    } else {
        current_stack->top++;
        for (int i = 0; i < STACK_CAP; ++i) {
            current_stack->m[current_stack->top][i] = current_matrix[i];
        }
    }
}

// Pops current matrix from current stack, report error if the stack has only one matrix left.
void I_my_glPopMatrix(void)
{
    if (current_stack->top == 0) {
        throw GL_STACK_UNDERFLOW;
    } else {
        current_stack->top--;
    }
}

// Overwrites currentmatrix with m.
void I_my_glLoadMatrixf(const GLfloat *m)
{
    for (int i = 0; i < STACK_CAP; ++i) {
        current_matrix[i] = (GLdouble)m[i];
    }
}

void I_my_glLoadMatrixd(const GLdouble *m)
{
    for (int i = 0; i < STACK_CAP; ++i) {
        current_matrix[i] = m[i];
    }
}

void I_my_glTranslated(GLdouble x, GLdouble y, GLdouble z)
{
    
    //Constructs the translation matrix
    GLdouble translate[16];
    //GLdouble t_vector[3] = {x, y, z};
    for (int i = 0; i < STACK_CAP; ++i) {
        if (i == 3) {
            translate[i] = x;
        } else if (i == 7) {
            translate[i] = y;
            //std::cout << "inside" << std::endl;
        } else if (i == 11) {
            translate[i] = z;
        } else {
            translate[i] = identity[i];
        }
    }
    
    //multiplies current_matrix with translation matrix
    transpose(translate);
    matrix_multiply(translate);
    //transpose(current_matrix);
}

void I_my_glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glTranslated((GLdouble)x, (GLdouble)y, (GLdouble)z);
}



// Remember to normalize vector (x, y, z), and to convert angle from degree to radius before calling sin and cos.
void I_my_glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)
{
    //I_my_glPushMatrix();
    normalize(&x, &y, &z);
    angle = deg_to_rad(angle);
    GLdouble c = cos(angle);
    //GLdouble c1 = 1-c;
    GLdouble s = sin(angle);
    
    // rotation matrix organized by row
    GLdouble rotate[16];
    rotate[0] = x*x + (1-x*x)*c;
    rotate[1] = x*y*(1-c) - z*s;
    rotate[2] = x*z*(1-c) + y*s;
    rotate[3] = 0;
    
    rotate[4] = x*y*(1-c)+z*s;
    rotate[5] = y*y + (1-y*y)*c;
    rotate[6] = x*z*(1-c) - x*s;
    rotate[7] = 0;
    
    rotate[8] = x*z*(1-c) - y*s;
    rotate[9] = y*z*(1-c) + x*s;
    rotate[10] = z*z + (1-z*z)*c;
    rotate[11] = 0;
    
    rotate[12] = 0;
    rotate[13] = 0;
    rotate [14] = 0;
    rotate[15] = 1;
    transpose(rotate);
    matrix_multiply(rotate);
}

void I_my_glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glRotated((GLdouble)angle, (GLdouble)x, (GLdouble)y, (GLdouble)z);
}

void I_my_glScaled(GLdouble x, GLdouble y, GLdouble z)
{
    GLdouble scale[16];
  
    // scale matrix organized by row
    scale[0] = x;
    scale[1] = 0;
    scale[2] = 0;
    scale[3] = 0;
    
    scale[4] = 0;
    scale[5] = y;
    scale[6] = 0;
    scale[7] = 0;
    
    scale[8] = 0;
    scale[9] = 0;
    scale[10] = z;
    scale[11] = 0;
    
    scale[12] = 0;
    scale[13] = 0;
    scale[14] = 0;
    scale[15] = 1;
    
    transpose(scale);
    matrix_multiply(scale);
    
}

void I_my_glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    I_my_glScaled((GLdouble)x, (GLdouble)y, (GLdouble)z);
}

// Copies current matrix to m.
void I_my_glGetMatrixf(GLfloat *m)
{
    for (int i = 0; i < STACK_CAP; i++){
        m[i] = (GLfloat)current_matrix[i];
    }
}

void I_my_glGetMatrixd(GLdouble *m)
{
    for (int i = 0; i < STACK_CAP; i++){
        m[i] = current_matrix[i];
    }
    
}

// Remember to normalize vectors.
void I_my_gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, 
    GLdouble centerX, GLdouble centerY, GLdouble centerZ, 
    GLdouble upX, GLdouble upY, GLdouble upZ)
{
    GLdouble f[3], s[3], u[3];
    GLdouble lookMatrix[16];
    
    f[0] = centerX - eyeX;
    f[1] = centerY - eyeY;
    f[2] = centerZ - eyeZ;
    normalize(&f[0], &f[1], &f[2]);
    
    cross_product(&s[0], &s[1], &s[2], f[0], f[1], f[2], upX, upY, upZ);
    normalize(&s[0], &s[1], &s[2]);
    
    cross_product(&u[0], &u[1], &u[2], s[0], s[1], s[2], f[0], f[1], f[2]);
    
    //lookMatrix organized by row
    lookMatrix[0] = s[0];
    lookMatrix[1] = s[1];
    lookMatrix[2] = s[2];
    lookMatrix[3] = 0;
    
    lookMatrix[4] = u[0];
    lookMatrix[5] = u[1];
    lookMatrix[6] = u[2];
    lookMatrix[7] = 0;
    
    lookMatrix[8] = -1.0*(f[0]);
    lookMatrix[9] = -1.0*(f[1]);
    lookMatrix[10] = -1.0*(f[2]);
    lookMatrix[11] = 0;
    
    lookMatrix[12] = 0;
    lookMatrix[13] = 0;
    lookMatrix[14] = 0;
    lookMatrix[15] = 1;
    
    transpose(lookMatrix);
    matrix_multiply(lookMatrix);
    I_my_glTranslated(-eyeX, -eyeY, -eyeZ);
}

void I_my_glFrustum(GLdouble left, GLdouble right, GLdouble bottom,
    GLdouble top, GLdouble zNear, GLdouble zFar)
{
    GLdouble A = (right+left)/(right-left);
    GLdouble B = (top+bottom)/(top-bottom);
    GLdouble C = -1.0*((zFar+zNear)/(zFar-zNear));
    GLdouble D = -1.0*((2.0*zFar*zNear)/(zFar-zNear));
    
    //perspective matrix organized by row
    GLdouble perspective[16];
    perspective[0] = (2.0*zNear)/(right-left);
    perspective[1] = 0;
    perspective[2] = A;
    perspective[3] = 0;
    
    perspective[4] = 0;
    perspective[5] = (2.0*zNear)/(top-bottom);
    perspective[6] = B;
    perspective[7] = 0;
    
    perspective[8] = 0;
    perspective[9] = 0;
    perspective[10] = C;
    perspective[11] = D;
    
    perspective[12] = 0;
    perspective[13] = 0;
    perspective[14] = -1;
    perspective[15] = 0;
    
    transpose(perspective);
    matrix_multiply(perspective);
}

// Based on the inputs, calculate left, right, bottom, top, and call I_my_glFrustum accordingly
// remember to convert fovy from degree to radius before calling tan
void I_my_gluPerspective(GLdouble fovy, GLdouble aspect, 
    GLdouble zNear, GLdouble zFar)
{
    GLdouble xmax, ymax = 0.0;
    fovy = (fovy * PI/180); // should be 360 or 180?
    ymax = zNear * tan(fovy);
    xmax = ymax * aspect;
    
    I_my_glFrustum(-xmax, xmax, -ymax, ymax, zNear, zFar);
}


