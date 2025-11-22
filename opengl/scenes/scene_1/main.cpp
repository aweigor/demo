#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define r {0xff, 0x00, 0x00}
#define y {0xff, 0xff, 0x00}
#define m {0xff, 0, 0xff}
GLubyte texture[][3] = {
	r, y,
	y, r,
};

static GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat gray[] = { 0.5, 0.5, 0.5, 1.0 };
static GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
static GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
static GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };
static GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
static GLfloat magenta[] = { 1.0, 0.0, 1.0, 1.0 };
static GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
static GLfloat darkcyan[] = { 0.0, 0.4, 0.4, 1.0 };
static GLfloat gold[] = { 255.0 / 255.0, 215.0 / 255.0, 0 / 255.0 };
static GLfloat silver[] = { 204.0 / 255.0, 204.0 / 255.0, 204.0 / 255.0 };
static GLfloat tranparent[] = { 0.5, 0.5, 0.5, 0.3 };

GLfloat lpos[] = { 1.0, 2.0, 1.0, 0.0 };

static double ligtState = 0.0;

static void drawBox(GLfloat size, GLenum type)
{

  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
	static GLfloat t[6][8] =
  {
    { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 },
    { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 },
    { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 },
    { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 },
    { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 },
    { 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 },
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(type);
    glNormal3fv(&n[i][0]);
    glTexCoord2f(t[i][0], t[i][1]); glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2f(t[i][2], t[i][3]); glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2f(t[i][4], t[i][5]); glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2f(t[i][6], t[i][7]); glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void APIENTRY texturedSolidCube(GLdouble size)
{
  drawBox(size, GL_QUADS);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  ligtState += 0.01;
  GLfloat lpos_new[] = { lpos[0] * (float)cos(ligtState), lpos[1] * (float)cos(ligtState), lpos[2] * (float)sin(ligtState), 0.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lpos_new);

  glMaterialfv(GL_FRONT, GL_EMISSION, white);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);
  glPushMatrix();
  glTranslatef(lpos_new[0], lpos_new[1], lpos_new[2]);
  glutSolidSphere(0.05, 10, 8);
  glPopMatrix();
  glMaterialfv(GL_FRONT, GL_EMISSION, black);
  
  glPushMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialf(GL_FRONT, GL_SHININESS, 128);
  glTranslatef(0.0, 0.0, 0);
  glutSolidTeapot(0.7);
  glPopMatrix();


  glEnable(GL_TEXTURE_2D);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D,
               0,                    // level 0
               3,                    // use only R, G, and B components
               2, 2,                 // texture has 2x2 texels
               0,                    // no border
               GL_RGB,               // texels are in RGB format
               GL_UNSIGNED_BYTE,     // color components are unsigned bytes
               texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


  glPushMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
  glMaterialfv(GL_FRONT, GL_SPECULAR, black);
  glMaterialf(GL_FRONT, GL_SHININESS, 1.0);
  glTranslatef(2.0, 0.0, 0);
  texturedSolidCube(1.0);
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

  glDepthMask(false);
  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glPushMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tranparent);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialf(GL_FRONT, GL_SHININESS, 1);
  
  glTranslatef(-2.0, 0.0, 0.0);
  glutSolidSphere(0.7, 30, 30);
  
  glPopMatrix();
  glDisable(GL_BLEND);
  glDepthMask(true);

  glPopMatrix();

  glFlush();
}

void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  GLfloat aspect = GLfloat(w) / GLfloat(h);
  glLoadIdentity();
  gluPerspective(60.0, 4.0/3.0, 1, 400);
  gluLookAt(0, -1, 5, 0, 0, 0, 0, 1, 0);
}

void init() {
  glClearColor(0.0, 0.59765625, 0.57421875, 0.5);

  glLightfv(GL_LIGHT0, GL_AMBIENT, black);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_POSITION, lpos);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}

void timer( int value )
{
    glutPostRedisplay();
    glutTimerFunc( 16, timer, 0 );
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 600);
  glutCreateWindow("Soap bubble, green teapot and rubik's cube");
  glutReshapeFunc(reshape);
  glutTimerFunc( 0, timer, 0 );
  glutDisplayFunc(display);
  init();
  glutMainLoop();
}