/*
Compuer Graphics
Challenge 2 Transformation
Team 8
Alejandro Miguel A01272385
Adrian Contreras A01039138
Arturo Rojas  	 A01039185

Code is based from the following original source:
3D Graphics with OpenGL
https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html

To compile from console in mac:
gcc -o Example.o Example.c  -L/System/Library/Frameworks -framework GLUT -framework OpenGL -w
*/

#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <math.h>

#define NUMBER_OF_VECTORS 24 //Number of vertex in our model
#define NUMBER_OF_AXIS 4 //Dimension(3D) + 1

#define PI 3.14159265

 /* Global variables */
char title[] = "3D Shapes";

GLfloat translateModel[3] = {0,0,0};
GLfloat rotateModel[3] = {0,0,0};
GLfloat scaleModel[3] = {0,0,0};

bool keyStates[256] = {false};

GLfloat v[NUMBER_OF_AXIS][NUMBER_OF_VECTORS] = {
	//	0      1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    
		{0.0, 2.0,  2.0,  0.0,  0.0,  2.0,  2.0,  0.0,  2.5,  2.5,  2.0,  2.5,  2.5,  2.0,  2.0,  2.0,  -0.2, 2.2,  2.2,  -0.2, -0.2, 2.2,  2.2,  -0.2},
		{0.0, 0.0,  0.0,  0.0,  5.0,  5.0,  5.0,  5.0,  0.0,  0.0,  2.0,  2.0,  2.0,  2.0,  3.0,  3.0,  4.5,  4.5,  4.5,  4.5,  4.7,  4.7,  4.7,  4.7},
		{0.0, 0.0,  -2.0, -2.0, 0.0,  0.0,  -2.0, -2.0, 0.0,  -2.0, 0.0,  0.0,  -2.0, -2.0, 0.0,  -2.0, 0.2,  0.2,  -2.2, -2.2, 0.2,  0.2,  -2.2, -2.2},
		{1.0, 1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0}
	};

GLfloat  TransformationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];
GLfloat  TranslationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];
GLfloat  TranslationMatrixToOrigin[NUMBER_OF_AXIS][NUMBER_OF_AXIS];
GLfloat  ScalingMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS];

//Initial transformation matrix with default values
void initMatrix(GLfloat matrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS]) {
	for (int i = 0; i < NUMBER_OF_AXIS; i++) {
		for (int j = 0; j < NUMBER_OF_AXIS; j++) {
			matrix[i][j] = 0.0f;

			if (i == j) {
				matrix[i][j] = 1.0f;
			}
		}
	}
}

void transformMatrix(GLfloat transformationMatrix[NUMBER_OF_AXIS][NUMBER_OF_AXIS], GLfloat vectors[NUMBER_OF_AXIS][NUMBER_OF_VECTORS], GLfloat result[NUMBER_OF_AXIS][NUMBER_OF_VECTORS]) {
	for (int k = 0; k < NUMBER_OF_VECTORS; k++) {
		for (int i = 0; i < NUMBER_OF_AXIS; i++) {
			result[i][k] = 0.0f;
			for (int j = 0; j < NUMBER_OF_AXIS; j++) {
				result[i][k] += vectors[j][k] * transformationMatrix[i][j];
			}
		}
	}
}

void multMatrix(GLfloat matrixA[NUMBER_OF_AXIS][NUMBER_OF_AXIS], GLfloat matrixB[NUMBER_OF_AXIS][NUMBER_OF_AXIS], GLfloat result[NUMBER_OF_AXIS][NUMBER_OF_AXIS]) {
	for (int k = 0; k < NUMBER_OF_AXIS; k++) {
		for (int i = 0; i < NUMBER_OF_AXIS; i++) {
			result[i][k] = 0.0f;
			for (int j = 0; j < NUMBER_OF_AXIS; j++) {
				result[i][k] += matrixA[j][k] * matrixB[i][j];
			}
		}
	}
}

void rotateMatrix(GLfloat matrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS], GLfloat ResultMatrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS], double angleX, double angleY, double angleZ) {

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

void renderFigure(GLfloat ResultMatrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS]){

	glBegin(GL_QUADS);
	//#############
	//MAIN BUILDING
	//#############
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
	glVertex3f(ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
	glVertex3f(ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
	glVertex3f(ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);

	// Bottom face 
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(ResultMatrix[0][4], ResultMatrix[1][4], ResultMatrix[2][4]);
	glVertex3f(ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
	glVertex3f(ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
	glVertex3f(ResultMatrix[0][7], ResultMatrix[1][7], ResultMatrix[2][7]);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
	glVertex3f(ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
	glVertex3f(ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
	glVertex3f(ResultMatrix[0][4], ResultMatrix[1][4], ResultMatrix[2][4]);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);
	glVertex3f(ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
	glVertex3f(ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
	glVertex3f(ResultMatrix[0][7], ResultMatrix[1][7], ResultMatrix[2][7]);

	// Left face (x = -1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(ResultMatrix[0][0], ResultMatrix[1][0], ResultMatrix[2][0]);
	glVertex3f(ResultMatrix[0][3], ResultMatrix[1][3], ResultMatrix[2][3]);
	glVertex3f(ResultMatrix[0][7], ResultMatrix[1][7], ResultMatrix[2][7]);
	glVertex3f(ResultMatrix[0][4], ResultMatrix[1][4], ResultMatrix[2][4]);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
	glVertex3f(ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
	glVertex3f(ResultMatrix[0][6], ResultMatrix[1][6], ResultMatrix[2][6]);
	glVertex3f(ResultMatrix[0][5], ResultMatrix[1][5], ResultMatrix[2][5]);
	glEnd();  // End of MAIN BUILDING

	//####################
	//LOBBY ENTRANCE
	//####################
	glBegin(GL_QUADS);                
	//Top face 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);
	glVertex3f(ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
	glVertex3f(ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
	glVertex3f(ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);

	// Bottom face 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
	glVertex3f(ResultMatrix[0][8], ResultMatrix[1][8], ResultMatrix[2][8]);
	glVertex3f(ResultMatrix[0][9], ResultMatrix[1][9], ResultMatrix[2][9]);
	glVertex3f(ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);

	// Front face 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
	glVertex3f(ResultMatrix[0][8], ResultMatrix[1][8], ResultMatrix[2][8]);
	glVertex3f(ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
	glVertex3f(ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);

	// Back face
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
	glVertex3f(ResultMatrix[0][9], ResultMatrix[1][9], ResultMatrix[2][9]);
	glVertex3f(ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
	glVertex3f(ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);

	// Left face 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][1], ResultMatrix[1][1], ResultMatrix[2][1]);
	glVertex3f(ResultMatrix[0][2], ResultMatrix[1][2], ResultMatrix[2][2]);
	glVertex3f(ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);
	glVertex3f(ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);

	// Right face 
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][8], ResultMatrix[1][8], ResultMatrix[2][8]);
	glVertex3f(ResultMatrix[0][9], ResultMatrix[1][9], ResultMatrix[2][9]);
	glVertex3f(ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
	glVertex3f(ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
	glEnd();  // End of LOBBY ENTRANCE

	//############
	//TERRACE
	//############
	glBegin(GL_QUADS);                
	//Top face 
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(ResultMatrix[0][20], ResultMatrix[1][20], ResultMatrix[2][20]);
	glVertex3f(ResultMatrix[0][21], ResultMatrix[1][21], ResultMatrix[2][21]);
	glVertex3f(ResultMatrix[0][22], ResultMatrix[1][22], ResultMatrix[2][22]);
	glVertex3f(ResultMatrix[0][23], ResultMatrix[1][23], ResultMatrix[2][23]);

	// Bottom face 
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(ResultMatrix[0][16], ResultMatrix[1][16], ResultMatrix[2][16]);
	glVertex3f(ResultMatrix[0][17], ResultMatrix[1][17], ResultMatrix[2][17]);
	glVertex3f(ResultMatrix[0][18], ResultMatrix[1][18], ResultMatrix[2][18]);
	glVertex3f(ResultMatrix[0][19], ResultMatrix[1][19], ResultMatrix[2][19]);

	// Front face 
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(ResultMatrix[0][16], ResultMatrix[1][16], ResultMatrix[2][16]);
	glVertex3f(ResultMatrix[0][17], ResultMatrix[1][17], ResultMatrix[2][17]);
	glVertex3f(ResultMatrix[0][21], ResultMatrix[1][21], ResultMatrix[2][21]);
	glVertex3f(ResultMatrix[0][20], ResultMatrix[1][20], ResultMatrix[2][20]);

	// Back face
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(ResultMatrix[0][19], ResultMatrix[1][19], ResultMatrix[2][19]);
	glVertex3f(ResultMatrix[0][18], ResultMatrix[1][18], ResultMatrix[2][18]);
	glVertex3f(ResultMatrix[0][22], ResultMatrix[1][22], ResultMatrix[2][22]);
	glVertex3f(ResultMatrix[0][23], ResultMatrix[1][23], ResultMatrix[2][23]);

	// Left face
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(ResultMatrix[0][16], ResultMatrix[1][16], ResultMatrix[2][16]);
	glVertex3f(ResultMatrix[0][19], ResultMatrix[1][19], ResultMatrix[2][19]);
	glVertex3f(ResultMatrix[0][23], ResultMatrix[1][23], ResultMatrix[2][23]);
	glVertex3f(ResultMatrix[0][20], ResultMatrix[1][20], ResultMatrix[2][20]);

	// Right face
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(ResultMatrix[0][17], ResultMatrix[1][17], ResultMatrix[2][17]);
	glVertex3f(ResultMatrix[0][18], ResultMatrix[1][18], ResultMatrix[2][18]);
	glVertex3f(ResultMatrix[0][22], ResultMatrix[1][22], ResultMatrix[2][22]);
	glVertex3f(ResultMatrix[0][21], ResultMatrix[1][21], ResultMatrix[2][21]);
	glEnd();  // End of TERRACE

	//##########
	//LOBBY ROOF
	//##########
	glBegin(GL_QUADS);              
	   //Top face
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][14], ResultMatrix[1][14], ResultMatrix[2][14]);
	glVertex3f(ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
	glVertex3f(ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
	glVertex3f(ResultMatrix[0][15], ResultMatrix[1][15], ResultMatrix[2][15]);

	// Bottom face 
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);
	glVertex3f(ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
	glVertex3f(ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
	glVertex3f(ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);

	// Front face 
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);
	glVertex3f(ResultMatrix[0][11], ResultMatrix[1][11], ResultMatrix[2][11]);
	glVertex3f(ResultMatrix[0][14], ResultMatrix[1][14], ResultMatrix[2][14]);

	// Back face 
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);
	glVertex3f(ResultMatrix[0][12], ResultMatrix[1][12], ResultMatrix[2][12]);
	glVertex3f(ResultMatrix[0][14], ResultMatrix[1][14], ResultMatrix[2][14]);

	// Left face
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(ResultMatrix[0][10], ResultMatrix[1][10], ResultMatrix[2][10]);
	glVertex3f(ResultMatrix[0][13], ResultMatrix[1][13], ResultMatrix[2][13]);
	glVertex3f(ResultMatrix[0][15], ResultMatrix[1][15], ResultMatrix[2][15]);
	glVertex3f(ResultMatrix[0][14], ResultMatrix[1][14], ResultMatrix[2][14]);

	glEnd();  // End of LOBBY ROOF

	glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
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

	//Vector matrix representing the vertexes of the polygons, defined in counter-clockwise position. 
	

	//Initializing matrix
	initMatrix(TransformationMatrix);
	initMatrix(TranslationMatrix);
	initMatrix(TranslationMatrixToOrigin);
	initMatrix(ScalingMatrix);


	//Scaling factors for the transformation matrix
	ScalingMatrix[0][0] = -1;   //SCALE ON X
	ScalingMatrix[1][1] = -1;	//SCALE ON Y
	ScalingMatrix[2][2] = -1;	//SCALE ON Z

	//Values are added to the transf matrix

	//Translation units for the transformation matrix
	//TranslationMatrix[0][3] = 0 + translationXAxis;	//TRANSLATION ON X
	//TranslationMatrix[1][3] = 0 + translationYAxis;	//TRANSLATION ON Y
	//TranslationMatrix[2][3] = 0 + translationZAxis;	//TRANSLATION ON Z


	GLfloat ResultMatrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

	//Translation to origin is made before scaling is applied
	TranslationMatrixToOrigin[0][3] = -v[0][0];
	TranslationMatrixToOrigin[1][3] = -v[1][0];
	TranslationMatrixToOrigin[2][3] = -v[2][0];

	GLfloat TranslationAux[NUMBER_OF_AXIS][NUMBER_OF_AXIS];

	multMatrix(ScalingMatrix, TranslationMatrixToOrigin, TranslationAux);

	//Translation units for the transformation matrix
	TranslationMatrix[0][3] = 0 + translateModel[0];	//TRANSLATION ON X
	TranslationMatrix[1][3] = 0 + translateModel[1];	//TRANSLATION ON Y
	TranslationMatrix[2][3] = 0 + translateModel[2];	//TRANSLATION ON Z

	multMatrix(TranslationMatrix, TranslationAux, TransformationMatrix);

	GLfloat ResultMatrixAux[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

	//Transforms the matrix aplying translation and scaling
	transformMatrix(TransformationMatrix, v, ResultMatrixAux);

	//Rotates the matrix, parameters in degrees x,y,z
	rotateMatrix(ResultMatrixAux, ResultMatrix, 180, 40, 0);


	renderFigure(ResultMatrixAux);
	
	
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
	glRotatef(10, 0, 1, 0);
}

void keySpecial(int key, int x, int  y){
	switch(key){
		case GLUT_KEY_LEFT:
			printf("Tecla izquierda\n");
			translateModel[0] += 1;
			glutPostRedisplay();
			display();
			break;

		case GLUT_KEY_RIGHT:
			printf("Tecla derecha\n");
			translateModel[0] -= 1;
			glutPostRedisplay();
			break;

		case GLUT_KEY_UP:
			printf("Tecla arriba\n");
			translateModel[1] -= 1;
			glutPostRedisplay();
			break;

		case GLUT_KEY_DOWN:
			printf("Tecla abajo\n");
			translateModel[1] += 1;
			glutPostRedisplay();
			break;	
	}
}

void keyDown (unsigned char key, int x, int y) {  
	keyStates[key] = true; // Set the state of the current key to pressed  
}  

void keyUp (unsigned char key, int x, int y) {  
	keyStates[key] = false; // Set the state of the current key to pressed  
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
	glutSpecialFunc(keySpecial);
	initGL();                       // Our own OpenGL initialization
	glutMainLoop();                 // Enter the infinite event-processing 

	return 0;
}