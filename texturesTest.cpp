/*
 * Texture Mapping Reading from PPM File
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//#include <GLUT/glut.h>


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <math.h>


using namespace std;

/*  Create checkerboard texture  */
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
int width,height,colours;

#define NUMBER_OF_VECTORS 24 //Number of vertex in our model
#define NUMBER_OF_AXIS 4  //Dimension(3D) + 1

#define ROTATION_VEL 10   //Constant that defines the velocity of rotation
#define TRANSLATION_VEL 1 //Constant that defines the velocity of translation
#define SCALE_VEL 0.1   //Constant that defines the velocity of scaling

#define PI 3.14159265   //Constant that defines the value of pi

/* Global variables */
char title[] = "3D Shapes";

GLfloat initialx, initialy, newx, newY;

float r, g, b, x, y;
bool check = true;


GLfloat translateModel[3] = { 0,0,0 };  //Matrix that contains the value of the transformations for the translation
double rotateModel[3] = { 0,0,0 };    //Matrix that contains the value of the transformations for the rotation
GLfloat scaleModel[3] = { 0,0,0 };    //Matrix that contains the value of the transformations for the scaling

bool ctrlKeyPressed = false;

GLfloat v[NUMBER_OF_AXIS][NUMBER_OF_VECTORS] = {
  //  0      1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20    21    22    23    
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

void renderFigure(GLfloat ResultMatrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS]) {

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

void makeCheckImage(void)
{
   int i, j, c;
    char keyword[2];
    string comment;
    //Generating Checker Pattern
   /*for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }*/

   //Reading texture file
    ifstream inFile;

    inFile.open("./Raul.ppm");
    if (!inFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }

    inFile >> keyword;
    cout << keyword << " ";
    inFile >> comment;
    cout << comment << " ";
    inFile >> comment;
    cout << comment << " ";
    inFile >> comment;
    cout << comment << " ";
    inFile >> comment;
    cout << comment << " ";

    inFile >> width;
    cout << width;
    inFile >> height;
    cout << height << "\n";
    inFile >> colours;
    cout << colours;
    cout << endl;
    for (i = 0; i < height; i++) {
      for (j = 0; j < width; j++) {
         inFile >> c;
         checkImage[height-i][j][0] = (GLubyte) c;
         //cout << c << " ";
         inFile >> c;
         checkImage[height-i][j][1] = (GLubyte) c;
         //cout << c << " ";
         inFile >> c;
         checkImage[height-i][j][2] = (GLubyte) c;
         //cout << c << " ";
         checkImage[height-i][j][3] = (GLubyte) 255;
         //cout << "255n";
      }
      //cout << endl;
   }
   inFile.close();
}

void init(void)
{
   glEnable(GL_DEPTH_TEST);

   makeCheckImage();
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                checkImage);
   //gluBuild2DMipmaps(GL_TEXTURE_2D,1,checkImageWidth,checkImageHeight,GL_RGBA,GL_UNSIGNED_BYTE,checkImage);

}

float angle = 0.0f;

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   gluLookAt(	0.0f, 0.0f,  5.0f,
				0.0f, 0.0f,  0.0f,
				0.0f, 1.0f,  0.0f);


  //Initializing matrix
  initMatrix(TransformationMatrix);
  initMatrix(TranslationMatrix);
  initMatrix(TranslationMatrixToOrigin);
  initMatrix(ScalingMatrix);


  //Scaling factors for the transformation matrix
  ScalingMatrix[0][0] = -1 + scaleModel[0];   //SCALE ON X
  ScalingMatrix[1][1] = -1 + scaleModel[1]; //SCALE ON Y
  ScalingMatrix[2][2] = -1 + scaleModel[2]; //SCALE ON Z

  //Values are added to the transf matrix

  //Translation units for the transformation matrix
  //TranslationMatrix[0][3] = 0 + translationXAxis; //TRANSLATION ON X
  //TranslationMatrix[1][3] = 0 + translationYAxis; //TRANSLATION ON Y
  //TranslationMatrix[2][3] = 0 + translationZAxis; //TRANSLATION ON Z


  GLfloat ResultMatrix[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

  //Translation to origin is made before scaling is applied
  TranslationMatrixToOrigin[0][3] = -v[0][0];
  TranslationMatrixToOrigin[1][3] = -v[1][0];
  TranslationMatrixToOrigin[2][3] = -v[2][0];

  GLfloat TranslationAux[NUMBER_OF_AXIS][NUMBER_OF_AXIS];

  multMatrix(ScalingMatrix, TranslationMatrixToOrigin, TranslationAux);

  //Translation units for the transformation matrix
  TranslationMatrix[0][3] = 0 + translateModel[0];  //TRANSLATION ON X
  TranslationMatrix[1][3] = 0 + translateModel[1];  //TRANSLATION ON Y
  TranslationMatrix[2][3] = 0 + translateModel[2];  //TRANSLATION ON Z

  multMatrix(TranslationMatrix, TranslationAux, TransformationMatrix);

  GLfloat ResultMatrixAux[NUMBER_OF_AXIS][NUMBER_OF_VECTORS];

  //Transforms the matrix aplying translation and scaling
  transformMatrix(TransformationMatrix, v, ResultMatrixAux);

  //Rotates the matrix, parameters in degrees x,y,z
  rotateMatrix(ResultMatrixAux, ResultMatrix, 180 + rotateModel[0], 40 + rotateModel[1], 0 + rotateModel[2]);

  


   glEnable(GL_TEXTURE_2D);

   //glRotatef(angle, 0.0f, 1.0f, 0.0f);

   /*glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
       glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
       glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
       glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

       glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
       glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
       glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
       glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
   glEnd();*/

   glutSolidTeapot(1);
   renderFigure(ResultMatrix);
   //glutSolidTorus(1,2,16,16);

   glDisable(GL_TEXTURE_2D);

    angle+=0.05f;

	glutSwapBuffers();
}

void reshape(int w, int h)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init(); 
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(display);
   glutMainLoop();
   return 0;
}
  