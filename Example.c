/*
 * OGL01Shape3D.cpp: 3D Shapes
 */

/*

To compile from console in mac:
gcc -o Example.o Example.c  -L/System/Library/Frameworks -framework GLUT -framework OpenGL -w

*/
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <windows.h>
#endif

#include <math.h>

#define NUMBER_OF_VECTORS 14
#define NUMBER_OF_AXIS 4

#define PI 3.14159265
 
/* Global variables */
char title[] = "3D Shapes";

void initMatrix(GLfloat matrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS]){
   for(int i = 0; i < NUMBER_OF_AXIS; i++){
      for(int j = 0; j < NUMBER_OF_AXIS; j++){
         matrix[i][j] = 0.0f;

         if(i == j){
            matrix[i][j] = 1.0f;
         }
      }
   }
}

void transformMatrix(GLfloat transformationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS], GLfloat vectors[NUMBER_OF_AXIS][NUMBER_OF_VECTORS], GLfloat result[NUMBER_OF_AXIS][NUMBER_OF_VECTORS]){
    for(int k = 0; k < NUMBER_OF_VECTORS; k++){
         for(int i = 0; i < NUMBER_OF_AXIS;  i++){
             result[i][k] = 0.0f;
            for(int j = 0; j  < NUMBER_OF_AXIS; j++){
               result[i][k] +=  vectors[j][k] * transformationMatrix[i][j];
            }
         }
      }
}

void multMatrix(GLfloat matrixA[NUMBER_OF_AXIS][NUMBER_OF_AXIS], GLfloat matrixB[NUMBER_OF_AXIS][NUMBER_OF_AXIS], GLfloat result[NUMBER_OF_AXIS][NUMBER_OF_AXIS]){

   for(int k = 0; k < NUMBER_OF_AXIS; k++){

      for(int i = 0; i < NUMBER_OF_AXIS; i++){

         result[i][k]  = 0.0f;

         for(int j = 0; j < NUMBER_OF_AXIS; j++){
            result[i][k] += matrixA[j][k] * matrixB[i][j];
            //result[i][k] += 1;
         }
      }
   }
   
}

void rotateMatrix(GLfloat matrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS], GLfloat ResultMatrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS], double angleX, double angleY, double angleZ){
   
   GLfloat RotationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];
   GLfloat matrixAux[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

   double  angleRadX = angleX * (PI / 180); //Converting degrees into radians

   //Rotating in X axis

   initMatrix(RotationMatrix);

   RotationMatrix[1][1] = cos(angleRadX);
   RotationMatrix[1][2] = -sin(angleRadX);
   RotationMatrix[2][1] = sin(angleRadX);
   RotationMatrix[2][2] = cos(angleRadX);

   transformMatrix(RotationMatrix, matrix, ResultMatrix);

   //Rotating in Y axis
   double  angleRadY = angleY * (PI / 180); //Converting degrees into radians

   initMatrix(RotationMatrix);

   RotationMatrix[0][0] = cos(angleRadY);
   RotationMatrix[0][2] = sin(angleRadY);
   RotationMatrix[2][0] = -sin(angleRadY);
   RotationMatrix[2][2] = cos(angleRadY);

   transformMatrix(RotationMatrix, ResultMatrix, matrixAux);

   //Rotating in Z axis
   double  angleRadZ = angleZ * (PI / 180); //Converting degrees into radians

   initMatrix(RotationMatrix);

   RotationMatrix[0][0] = cos(angleRadZ);
   RotationMatrix[0][1] = -sin(angleRadZ);
   RotationMatrix[1][0] = sin(angleRadZ);
   RotationMatrix[1][1] = cos(angleRadZ);

   transformMatrix(RotationMatrix, matrixAux, ResultMatrix);

}
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
 
   
      // Top face (y = 1.0f)
      // Define vertices in counter-clockwise (CCW) order with normal pointing out

      GLfloat v[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

      GLfloat  TransformationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];
      GLfloat  TranslationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];
      GLfloat  TranslationMatrixToOrigin[NUMBER_OF_AXIS][NUMBER_OF_AXIS];
      GLfloat  ScalingMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];

      initMatrix(TransformationMatrix);
      initMatrix(TranslationMatrix);
      initMatrix(TranslationMatrixToOrigin);
      initMatrix(ScalingMatrix);

      //Se inicializa la matriz de transformación
     /* for(int i = 0; i < NUMBER_OF_AXIS; i++){
         for(int j = 0; j < NUMBER_OF_AXIS; j++){
            TransformationMatrix[i][j] = 0.0f;
         }
      }
*/
      //Se asignan los valores de escalación
      ScalingMatrix[0][0] = -1;
      ScalingMatrix[1][1] = 1.0;
      ScalingMatrix[2][2] = 1.0;

      //Se agregan los valores de traslación de la matriz

      //Se asigna los valores para escalar la matriz
      //TransformationMatrix[0][0] = TransformationMatrix[1][1] = TransformationMatrix[2][2]  = 0.5f;

      //Se agregan los valores de traslación de la matriz
      TranslationMatrix[0][3] = 0;
      TranslationMatrix[1][3] = 0;
      TranslationMatrix[2][3] = 0;

      v[0][0] = -1.0f;
      v[1][0] = 3.0f;
      v[2][0] = 1.0f;     
      v[3][0] = 1.0f;     

      v[0][1] = 1.0f;
      v[1][1] = 3.0f;
      v[2][1] = 1.0f;
      v[3][1] = 1.0f;

      v[0][2] = 1.0f;
      v[1][2] = -1.0f;
      v[2][2] = 1.0f;
      v[3][2] = 1.0f;

      v[0][3] = -1.0f;
      v[1][3] = -1.0f;
      v[2][3] = 1.0f;
      v[3][3] = 1.0f;

      v[0][4] = -1.0f;
      v[1][4] = 3.0f;
      v[2][4] = -1.0f;
      v[3][4] = 1.0f;

      v[0][5] = 1.0f;
      v[1][5] = 3.0f;
      v[2][5] = -1.0f;
      v[3][5] = 1.0f;

      v[0][6] = 1.0f;
      v[1][6] = -1.0f;
      v[2][6] = -1.0f;
      v[3][6] = 1.0f;

      v[0][7] = -1.0f;
      v[1][7] = -1.0f;
      v[2][7] = -1.0f;
      v[3][7] = 1.0f;

      v[0][8] = 1.3f;
      v[1][8] = -1.0f;
      v[2][8] = 1.0f;
      v[3][8] = 1.0f;

      v[0][9] = 1.3f;
      v[1][9] = -1.0f;
      v[2][9] = -1.0f;
      v[3][9] = 1.0f;

      v[0][10] = 1.3f;
      v[1][10] = 1.0f;
      v[2][10] = -1.0f;
      v[3][10] = 1.0f;

      v[0][11] = 1.3f;
      v[1][11] = 1.0f;
      v[2][11] = 1.0f;
      v[3][11] = 1.0f;

      v[0][12] = 1.0f;
      v[1][12] = 1.0f;
      v[2][12] = -1.0f;
      v[3][12] = 1.0f;

      v[0][13] = 1.0f;
      v[1][13] = 1.0f;
      v[2][13] = 1.0f;
      v[3][13] = 1.0f;
/*
      //High Ceiling
      v[0][14] = 1.2f;
      v[1][14] = 2.2f;
      v[2][14] = 1.2f;     
      v[3][14] = 1.0f;     

      v[0][15] = -1.2f;
      v[1][15] = 2.2f;
      v[2][15] = 1.2f;
      v[3][15] = 1.0f;

      v[0][16] = 1.2f;
      v[1][16] = 2.2f;
      v[2][16] = 1.2f;
      v[3][16] = 1.0f;

      v[0][17] = -1.2f;
      v[1][17] = 2.6f;
      v[2][17] = 1.2f;
      v[3][17] = 1.0f;

      v[0][18] = -1.0f;
      v[1][18] = 2.2f;
      v[2][18] = -1.2f;
      v[3][18] = 1.0f;

      v[0][19] = 1.0f;
      v[1][19] = 2.2f;
      v[2][19] = -1.2f;
      v[3][19] = 1.0f;

      v[0][20] = 1.0f;
      v[1][20] = 2.6f;
      v[2][20] = -1.2f;
      v[3][20] = 1.0f;

      v[0][21] = -1.0f;
      v[1][21] = -2.6f;
      v[2][21] = -1.2f;
      v[3][21] = 1.0f;


*/
      GLfloat ResultMatrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

      TranslationMatrixToOrigin[0][3] =  -v[0][0];
      TranslationMatrixToOrigin[1][3] =  -v[1][0];
      TranslationMatrixToOrigin[2][3] =  -v[2][0];


      multMatrix(ScalingMatrix, TranslationMatrixToOrigin, ResultMatrix);
      multMatrix(TranslationMatrix, ResultMatrix, TransformationMatrix);

      //Se realiza la multiplicación de matrices TransformationMatrix * v 
/*      for(int k = 0; k < NUMBER_OF_VECTORS; k++){
         for(int i = 0; i < NUMBER_OF_AXIS;  i++){
             ResultMatrix[i][k] = 0.0f;
            for(int j = 0; j  < NUMBER_OF_AXIS; j++){
               ResultMatrix[i][k] +=  v[j][k] * TransformationMatrix[i][j];
            }
         }
      }*/
      GLfloat ResultMatrixAux[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

      //Transforms the matrix aplying translation and scaling
      transformMatrix(TransformationMatrix, v, ResultMatrixAux);

      //Rotates the matrix, parameters in degrees x,y,z
      rotateMatrix(ResultMatrixAux,ResultMatrix, 0,0,0);
      

      glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      //Top face 
      glColor3f(0.0f, 1.0f, 0.0f);     // Green
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
      glVertex3f( ResultMatrix[0][4], ResultMatrix[1][4], ResultMatrix[2][4]);
      glVertex3f( ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
      glVertex3f( ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
 
      // Bottom face 
      glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
      glVertex3f( ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);
      glVertex3f( ResultMatrix[0][7], ResultMatrix[1][7], ResultMatrix[2][7]);
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
 
      // Front face  (z = 1.0f)
      glColor3f(1.0f, 0.0f, 0.0f);     // Red
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
      glVertex3f( ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
      glVertex3f( ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
 
      // Back face (z = -1.0f)
      glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
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
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
      glVertex3f( ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
   glEnd();  // End of drawing color-cube

   //Lobby Entrance
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      //Top face 
      //glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);
      glVertex3f( ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
      glVertex3f( ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
      glVertex3f( ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);
 
      // Bottom face 
      //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
      glVertex3f( ResultMatrix[0][8], ResultMatrix[1][8], ResultMatrix[2][8]);
      glVertex3f( ResultMatrix[0][9], ResultMatrix[1][9], ResultMatrix[2][9]);
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
 
      // Front face  (z = 1.0f)
      //glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
      glVertex3f( ResultMatrix[0][8], ResultMatrix[1][8], ResultMatrix[2][8]);
      glVertex3f( ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
 
      // Back face (z = -1.0f)
      //glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
      glVertex3f( ResultMatrix[0][9], ResultMatrix[1][9], ResultMatrix[2][9]);
      glVertex3f( ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);
      glVertex3f( ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
 
      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
      glVertex3f( ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
      glVertex3f( ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
      glVertex3f( ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);
 
      // Right face (x = 1.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][8], ResultMatrix[1][8], ResultMatrix[2][8]);
      glVertex3f( ResultMatrix[0][9], ResultMatrix[1][9], ResultMatrix[2][9]);
      glVertex3f( ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);
      glVertex3f( ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
   glEnd();  // End of drawing color-cube
   
   /*
   //Roof
   glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
      //Top face 
      //glColor3f(0.0f, 1.0f, 0.0f);     // Green
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][16], ResultMatrix[1][16], ResultMatrix[2][16]);
      glVertex3f( ResultMatrix[0][20], ResultMatrix[1][20], ResultMatrix[2][20]);
      glVertex3f( ResultMatrix[0][21], ResultMatrix[1][21], ResultMatrix[2][21]);
      glVertex3f( ResultMatrix[0][17], ResultMatrix[1][17], ResultMatrix[2][17]);
 
      // Bottom face 
      //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][14], ResultMatrix[1][14], ResultMatrix[2][14]);
      glVertex3f( ResultMatrix[0][15], ResultMatrix[1][15], ResultMatrix[2][15]);
      glVertex3f( ResultMatrix[0][19], ResultMatrix[1][19], ResultMatrix[2][19]);
      glVertex3f( ResultMatrix[0][18], ResultMatrix[1][18], ResultMatrix[2][18]);
 
      // Front face  (z = 1.0f)
      //glColor3f(1.0f, 0.0f, 0.0f);     // Red
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][14], ResultMatrix[1][14], ResultMatrix[2][14]);
      glVertex3f( ResultMatrix[0][15], ResultMatrix[1][15], ResultMatrix[2][15]);
      glVertex3f( ResultMatrix[0][16], ResultMatrix[1][16], ResultMatrix[2][16]);
      glVertex3f( ResultMatrix[0][17], ResultMatrix[1][17], ResultMatrix[2][17]);
 
      // Back face (z = -1.0f)
      //glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][18], ResultMatrix[1][18], ResultMatrix[2][18]);
      glVertex3f( ResultMatrix[0][19], ResultMatrix[1][19], ResultMatrix[2][19]);
      glVertex3f( ResultMatrix[0][20], ResultMatrix[1][20], ResultMatrix[2][20]);
      glVertex3f( ResultMatrix[0][21], ResultMatrix[1][21], ResultMatrix[2][21]);
 
      // Left face (x = -1.0f)
      glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][14], ResultMatrix[1][14], ResultMatrix[2][14]);
      glVertex3f( ResultMatrix[0][18], ResultMatrix[1][18], ResultMatrix[2][18]);
      glVertex3f( ResultMatrix[0][21], ResultMatrix[1][21], ResultMatrix[2][21]);
      glVertex3f( ResultMatrix[0][17], ResultMatrix[1][17], ResultMatrix[2][17]);
 
      // Right face (x = 1.0f)
      glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
      //glColor3f(0.0f, 0.0f, 1.0f);     // Blue
      glVertex3f( ResultMatrix[0][15], ResultMatrix[1][15], ResultMatrix[2][15]);
      glVertex3f( ResultMatrix[0][19], ResultMatrix[1][19], ResultMatrix[2][19]);
      glVertex3f( ResultMatrix[0][20], ResultMatrix[1][20], ResultMatrix[2][20]);
      glVertex3f( ResultMatrix[0][16], ResultMatrix[1][16], ResultMatrix[2][16]);
   glEnd();  // End of drawing color-cube
*/
   
 /*  // Render a pyramid consists of 4 triangles
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
   glEnd();   // Done drawing the pyramid*/
 
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
   glTranslatef(0.0f, 0.0f, -10.0f);
   glRotatef(30, 0,1,0);
}
 
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(1000, 1000);   // Set the window's initial width & height
   glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
   glutCreateWindow(title);          // Create window with the given title
   glutDisplayFunc(display);       // Register callback handler for window re-paint event
   glutReshapeFunc(reshape);       // Register callback handler for window re-size event
   initGL();                       // Our own OpenGL initialization
   glutMainLoop();                 // Enter the infinite event-processing 

   return 0;
}