/*
 * OGL01Shape3D.cpp: 3D Shapes
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <windows.h>
#endif

#define NUMBER_OF_VECTORS 8
#define NUMBER_OF_AXIS 3
 
/* Global variables */
char title[] = "3D Shapes";
 
/* Initialize OpenGL Graphics */
void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}
 
/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 
   // Render a color-cube consisting of 6 quads with different colors
   glLoadIdentity();                 // Reset the model-view matrix
   glTranslatef(1.5f, 0.0f, -7.0f);  // Move right and into the screen
 
   glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out

      GLfloat v[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

      GLfloat  TransformationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];

      for(int i = 0; i < NUMBER_OF_AXIS; i++){
         for(int j = 0; j < NUMBER_OF_AXIS; j++){
            TransformationMatrix[i][j] = 0.0f;
         }
      }

      //Se asigna los valores para escalar la matriz
      TransformationMatrix[0][0] = TransformationMatrix[1][1] = TransformationMatrix[2][2]  = 0.5f;

      v[0][0] = -1.0f;
      v[1][0] = 1.0f;
      v[2][0] = 1.0f;     

      v[0][1] = 1.0f;
      v[1][1] = 1.0f;
      v[2][1] = 1.0f;

      v[0][2] = 1.0f;
      v[1][2] = -1.0f;
      v[2][2] = 1.0f;

      v[0][3] = -1.0f;
      v[1][3] = -1.0f;
      v[2][3] = 1.0f;

      v[0][4] = -1.0f;
      v[1][4] = 1.0f;
      v[2][4] = -1.0f;

      v[0][5] = 1.0f;
      v[1][5] = 1.0f;
      v[2][5] = -1.0f;

      v[0][6] = 1.0f;
      v[1][6] = -1.0f;
      v[2][6] = -1.0f;

      v[0][7] = -1.0f;
      v[1][7] = -1.0f;
      v[2][7] = -1.0f;

      GLfloat ResultMatrix[3][8];

      //Se realiza la multiplicación de matrices TransformationMatrix * v 
      for(int k = 0; k < NUMBER_OF_VECTORS; k++){
         for(int i = 0; i < NUMBER_OF_AXIS;  i++){
             ResultMatrix[i][k] = 0.0f;
            for(int j = 0; j  < NUMBER_OF_AXIS; j++){
               ResultMatrix[i][k] +=  v[j][k] * TransformationMatrix[i][j];
            }
         }
      }

      //Top face y = 1.0f
      //glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
      glVertex3f( ResultMatrix[0][4], ResultMatrix[1][4], ResultMatrix[2][4]);
      glVertex3f( ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
      glVertex3f( ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
 
      // Bottom face (y = -1.0f)
      //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
      glVertex3f( ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);
      glVertex3f( ResultMatrix[0][7], ResultMatrix[1][7], ResultMatrix[2][7]);
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
 
      // Front face  (z = 1.0f)
      //glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
      glVertex3f( ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
      glVertex3f( ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
 
      // Back face (z = -1.0f)
      //glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
      glVertex3f( ResultMatrix[0][7], ResultMatrix[1][7], ResultMatrix[2][7]);
      glVertex3f( ResultMatrix[0][4], ResultMatrix[1][4], ResultMatrix[2][4]);
      glVertex3f( ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
 
      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
      glVertex3f( ResultMatrix[0][4], ResultMatrix[1][4], ResultMatrix[2][4]);
      glVertex3f( ResultMatrix[0][7], ResultMatrix[1][7], ResultMatrix[2][7]);
      glVertex3f( ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);
 
      // Right face (x = 1.0f)
      //glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
      glVertex3f( ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
   glEnd();  // End of drawing color-cube
 
   // Render a pyramid consists of 4 triangles
   glLoadIdentity();                  // Reset the model-view matrix
   glTranslatef(-1.5f, 0.0f, -6.0f);  // Move left and into the screen
 
   glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
      // Front
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(-1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
 
      // Right
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(1.0f, -1.0f, 1.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
 
      // Back
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glVertex3f(0.0f, 1.0f, 0.0f);
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glVertex3f(1.0f, -1.0f, -1.0f);
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f(-1.0f, -1.0f, -1.0f);
 
      // Left
      glColor3f(1.0f,0.0f,0.0f);       // Red
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glColor3f(0.0f,0.0f,1.0f);       // Blue
      glVertex3f(-1.0f,-1.0f,-1.0f);
      glColor3f(0.0f,1.0f,0.0f);       // Green
      glVertex3f(-1.0f,-1.0f, 1.0f);
   glEnd();   // Done drawing the pyramid
 
   glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}
 
/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;
 
   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);
 
   // Set the aspect ratio of the clipping volume to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();             // Reset
   // Enable perspective projection with fovy, aspect, zNear and zFar
   gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(640, 480);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing loop
   return 0;
}