#include <windows.h>
#include <GL/glut.h>

void initGL() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Color de fondo
}

void display() {
   glClear(GL_COLOR_BUFFER_BIT);    // Clear the color buffer
   glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
   glLoadIdentity();                // Reset the model-view matrix

   glTranslatef(0.0f, 0.0f, 0.0f);  //Origen
   glBegin(GL_QUADS);               // Vertices
      glColor3f(1.0f, 0.0f, 0.0f);  // Color rojo
      glVertex2f(-0.3f, -0.3f);
      glVertex2f( 0.3f, -0.3f);
      glVertex2f( 0.3f,  0.3f);
      glVertex2f(-0.3f,  0.3f);
   glEnd();

   glTranslatef(0.5f, -0.7f, 0.0f); // Translada a la derecha y abajo
   glBegin(GL_QUADS);
      glColor3f(0.0f, 1.0f, 0.0f);  //Verde
      glVertex2f(-0.3f, -0.3f);
      glVertex2f( 0.3f, -0.3f);
      glVertex2f( 0.3f,  0.3f);
      glVertex2f(-0.3f,  0.3f);
   glEnd();

   glFlush();   // Rende
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
   if (height == 0) height = 1;                // To prevent divide by 0
   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   // Set the viewport to cover the new window
   glViewport(0, 0, width, height);

   // Set the aspect ratio of the clipping area to match the viewport
   glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
   glLoadIdentity();
   if (width >= height) {
     // aspect >= 1, set the height from -1 to 1, with larger width
      gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
   } else {
      // aspect < 1, set the width to -1 to 1, with larger height
     gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
   }
}

int main(int argc, char** argv) {
   glutInit(&argc, argv);          // Initialize GLUT
   glutInitWindowSize(640, 480);   // Tamaño de ventana
   glutInitWindowPosition(50, 50); // Posicion Inicial
   glutCreateWindow("Transformaciones");  // Crea ventana
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   initGL();
   glutMainLoop();
   return 0;
}


//ref : https://www.ntu.edu.sg/home/ehchua/programming/opengl/CG_Introduction.html
