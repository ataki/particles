#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Question 1: In a GLUT program, how is control passed
// back to the programmer?  How is this set up during
// initialization?

int win_width = 512;
int win_height = 512;
int draw_mode=1;

void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();

  switch(draw_mode){
      case 1:// Draw a point
          glPointSize(8.);
          glColor3f(1.f,0.f,0.f);
          glBegin(GL_POINTS);
          glVertex3f(0.4f,0.4f,0.f);
          glEnd();
          break;

      case 2:// Draw a line
          glLineWidth(4.);
          glColor3f(0.f,1.f,0.f);
          glBegin(GL_LINES);
          glVertex3f(0.3f,0.3f,0.f);
          glVertex3f(0.6f,0.6f,0.f);
          glEnd();
          break;

      case 3:// Draw a triangle
          glColor3f(0.f,0.f,1.f);
          glBegin(GL_TRIANGLES);
          glVertex3f(0.2f,0.1f,0.f);
          glVertex3f(0.4f,0.1f,0.f);
          glVertex3f(0.3f,0.2f,0.f);
          glEnd();
          break;

      case 4:// Draw a quad
          glColor3f(1.f,1.f,0.f);
          glBegin(GL_QUADS);
          glVertex3f(0.6f,0.7f,0.f);
          glVertex3f(0.8f,0.7f,0.f);
          glVertex3f(0.8f,0.9f,0.f);
          glVertex3f(0.6f,0.9f,0.f);
          glEnd();
          break;

      case 5:// Draw a polygon
          glColor3f(1.f,0.f,1.f);
          glBegin(GL_POLYGON);
          glVertex3f(0.6f,0.7f,0.f);
          glVertex3f(0.8f,0.9f,0.f);
          glVertex3f(0.9f,0.7f,0.f);
          glVertex3f(0.8f,0.8f,0.f);
          glVertex3f(0.6f,0.9f,0.f);
          glEnd();
          break;

	  case 6://triangle strip
          glLoadIdentity();
          glBegin(GL_TRIANGLE_STRIP);

          glColor3f(1.f,0.f,0.f);
          glVertex2f(0.2f,0.2f);

          glColor3f(0.f,1.f,0.f);
          glVertex2f(0.2f,0.8f);

          glColor3f(0.f,0.f,1.f);
          glVertex2f(0.5f,0.2f);

          glColor3f(1.f,1.f,0.f);
          glVertex2f(0.5f,0.8f);

          glColor3f(0.f,1.f,1.f);
          glVertex2f(0.8f,0.2f);

          glColor3f(1.f,0.f,1.f);
          glVertex2f(0.8f,0.8f);

          glEnd();
          break;

      case 7://use transforms

          glLineWidth(4.);
          glLoadIdentity();

          //body
          glPushMatrix(); //push identity
          glTranslatef(0.5f,0.75f,0.f);
          glBegin(GL_LINES);
          glColor3f(1.f,1.f,1.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          //right arm
          glPushMatrix(); //push body transform
          glRotatef(35.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(1.f,0.f,0.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          //right forearm
          glPushMatrix(); //push right arm transform
          glTranslatef(0.f,-0.25f,0.f);
          glRotatef(-15.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(0.f,1.f,0.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          glPopMatrix(); //load right arm transform
          glPopMatrix(); //load body transform

          //left arm
          glPushMatrix(); //push body transform
          glRotatef(-35.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(1.f,0.f,0.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          //left forearm
          glPushMatrix(); //push left arm transform
          glTranslatef(0.f,-0.25f,0.f);
          glRotatef(15.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(0.f,1.f,0.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          glPopMatrix(); //load left arm transform
          glPopMatrix(); //load body transform

          //right leg
          glPushMatrix(); //push body transform
          glTranslatef(0.f,-0.25f,0.f);
          glRotatef(25.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(0.f,0.f,1.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          //right lower leg
          glPushMatrix(); //push right leg transform
          glTranslatef(0.f,-0.25f,0.f);
          glRotatef(-15.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(1.f,1.f,0.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          glPopMatrix(); //load right leg transform
          glPopMatrix(); //load body transform

          //left leg
          glPushMatrix(); //push body transform
          glTranslatef(0.f,-0.25f,0.f);
          glRotatef(-25.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(0.f,0.f,1.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          //left lower leg
          glPushMatrix(); //push left leg transform
          glTranslatef(0.f,-0.25f,0.f);
          glRotatef(15.f,0.f,0.f,1.f);
          glBegin(GL_LINES);
          glColor3f(1.f,1.f,0.f);
          glVertex3f(0.0f,0.0f,0.f);
          glVertex3f(0.0f,-0.25f,0.f);
          glEnd();

          glPopMatrix(); //load left leg transform
          glPopMatrix(); //load body transform

          glBegin(GL_TRIANGLES);
          glColor3f(1.f,1.f,1.f);
          glVertex3f(-0.1f,0.f,0.f);
          glVertex3f(0.1f,0.f,0.f);
          glVertex3f(0.f,0.15f,0.f);
          glEnd();

          break;
  }

  glutSwapBuffers();
}

void reshape( int w, int h )
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();

  // Question 3: What do the calls to glOrtho()
  // and glViewport() accomplish?
  glOrtho( 0., 1., 0., 1., -1., 1. );
  glViewport( 0, 0, w, h );

  win_width = w;
  win_height = h;

  glutPostRedisplay();
}

void keyboard( unsigned char key, int x, int y ) {
  switch(key) {
      case '1':// Press number keys to switch the draw mode
      case '2':
      case '3':
      case '4':
      case '5':
	  case '6':
      case '7':
          draw_mode=key-'0';
          glutPostRedisplay();
          break;
      case 27: // Escape key
          exit(0);
          break;
  }
}

int main (int argc, char *argv[]) {

  glutInit( &argc, argv );
  // Question 2: What does the parameter to glutInitDisplayMode()
  // specify?
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize( win_width, win_height );

  glutCreateWindow( "Intro Graphics Assignment 1" );

  glutDisplayFunc( display );
  glutReshapeFunc( reshape );
  glutKeyboardFunc( keyboard );

  glutMainLoop();
}
