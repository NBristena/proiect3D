#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include "SOIL.h"


enum {
	NONE, NSHADOW, BSHADOW, WSHADOW
};

int rendermode = NONE;

void menu(int mode)
{
	rendermode = mode;
	glutPostRedisplay();
}

enum {
	SPHERE = 1, CONE, LIGHT, LEFTWALL, BLAT
};

enum {
	X, Y, Z, W
};
enum {
	A, B, C, D
};

GLfloat leftwallshadow[4][4];
GLfloat floorshadow[4][4];
GLfloat lightpos[] = { 0.f, 80.f, -350.f, 1.f };

/* Matricea pentru umbre */
void shadowmatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4])
{
	GLfloat dot;

	dot = groundplane[X] * lightpos[X] +
		  groundplane[Y] * lightpos[Y] +
	      groundplane[Z] * lightpos[Z] +
		  groundplane[W] * lightpos[W];

	shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
	shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
	shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
	shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

	shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
	shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
	shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
	shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

	shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
	shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
	shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
	shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

	shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
	shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
	shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
	shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

/* Ecuatia planului cu ajutorul a 3 puncte */
void findplane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3])
{
	GLfloat vec0[3], vec1[3];

	vec0[X] = v1[X] - v0[X];
	vec0[Y] = v1[Y] - v0[Y];
	vec0[Z] = v1[Z] - v0[Z];

	vec1[X] = v2[X] - v0[X];
	vec1[Y] = v2[Y] - v0[Y];
	vec1[Z] = v2[Z] - v0[Z];

	plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
	plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
	plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

	plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}

/* Culorile materialelor obiectelor din scena */
static GLfloat wall_mat[] = { 1.f, 1.f, 1.f, 1.f };
static GLfloat sphere_mat[] = { 0.43f, 0.85f, 0.85f, 1.f };
static GLfloat masa1_mat[] = { 0.5f, 0.1f, 0.1f, 1.f };
static GLfloat masa2_mat[] = { 0.2f, 0.2f, 0.9f, 1.f };
static GLfloat masa3_mat[] = { 0.42f, 0.42f, 0.1f, 1.f };
static GLfloat masa4_mat[] = { 0.1f, 0.4f, 0.1f, 1.f };
static GLfloat scaun_mat[] = { 0.2f, 0.3f, .4f, 1.f };

void lumina(void) {
	glPushMatrix();
	glTranslatef(lightpos[X], lightpos[Y], lightpos[Z]);
	glDisable(GL_LIGHTING);
	glColor3f(1.f, 1.f, .8f);
	glCallList(LIGHT);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
void masa1(void)
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa1_mat);
	glTranslatef(-130.f, -100.f, -480.f);
	glCallList(CONE);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa1_mat);
	glTranslatef(-130.f, -70.f, -480.f);
	glCallList(MASA);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
	glTranslatef(-130.f, -40.f, -480.f);
	glCallList(SPHERE);
	glPopMatrix();
}
void masa2(void)
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa2_mat);
	glTranslatef(-50.f, -100.f, -370.f);
	glCallList(CONE);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa2_mat);
	glTranslatef(-50.f, -70.f, -370.f);
	glCallList(MASA);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
	glTranslatef(-50.f, -40.f, -370.f);
	glCallList(SPHERE);
	glPopMatrix();
}
void masa3(void)
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa3_mat);
	glTranslatef(30.f, -100.f, -450.f);
	glCallList(CONE);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa3_mat);
	glTranslatef(30.f, -70.f, -450.f);
	glCallList(MASA);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
	glTranslatef(30.f, -40.f, -450.f);
	glCallList(SPHERE);
	glPopMatrix();
}
void masa4(void)
{
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa4_mat);
	glTranslatef(110.f, -100.f, -400.f);
	glCallList(CONE);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, masa4_mat);
	glTranslatef(110.f, -70.f, -400.f);
	glCallList(MASA);
	glPopMatrix();

	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
	glTranslatef(110.f, -40.f, -400.f);
	glCallList(SPHERE);
	glPopMatrix();
}
void perna(void)
{
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, scaun_mat);
	glTranslatef(-180.f, -72.f, -360.f);
	glRotatef(90.f, 20.f, 90.f, 40.f);
	glutSolidTorus(10.0, 20.0, 100, 100);
	glPopMatrix();
}

void LoadTextureParchet(void)
{
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image("parchet.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}
void LoadTexturePerete(void)
{
	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image("perete.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}
void LoadTextureTavan(void)
{
	GLuint texture3;
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image = SOIL_load_image("tavan.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}

void drawTextureParchet() {
	LoadTextureParchet();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200.f, -100.f, -320.f);
	glTexCoord2f(0.0, 1.0); glVertex3f(200.f, -100.f, -320.f);
	glTexCoord2f(1.0, 1.0); glVertex3f(200.f, -100.f, -620.f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-200.f, -100.f, -620.f);
	glEnd();
}
void drawTexturePereteDreapta() {
	LoadTexturePerete();

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3f(200.f, -100.f, -630.f); 
		glTexCoord2f(0.0, 0.0); glVertex3f(200.f, 130.f, -630.f);
		glTexCoord2f(1.0, 0.0); glVertex3f(200.f, 130.f, -320.f);
		glTexCoord2f(1.0, 1.0); glVertex3f(200.f, -100.f, -320.f);
	glEnd();
}
void drawTexturePereteSpate() {
	LoadTexturePerete();

	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3f(-200.f, -100.f, -600.f);
		glTexCoord2f(0.0, 0.0); glVertex3f(-200.f, 120.f, -600.f);
		glTexCoord2f(1.0, 0.0); glVertex3f(210.f, 120.f, -600.f);
		glTexCoord2f(1.0, 1.0); glVertex3f(210.f, -100.f, -600.f);
	glEnd();
}
void drawTexturePereteStanga() {
	LoadTexturePerete();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-200.f, -100.f, -320.f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200.f, 120.f, -320.f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-200.f, 120.f, -620.f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-200.f, -100.f, -620.f);
	glEnd();
}
void drawTextureTavan() {
	LoadTextureTavan();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0); glVertex3f(-200.f, 120.f, -620.f);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200.f, 120.f, -320.f);
	glTexCoord2f(1.0, 0.0); glVertex3f(200.f, 120.f, -320.f);
	glTexCoord2f(1.0, 1.0); glVertex3f(200.f, 120.f, -620.f);
	glEnd();
}

void drawShadowPodea() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/* Obiectele pe care se aplica umbra */
	glPushMatrix();
	glMultMatrixf((GLfloat *)floorshadow);
	masa1();
	masa2();
	masa3();
	masa4();
	perna();
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
void drawShadowPerete() {
	glStencilFunc(GL_EQUAL, 1, 1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/* Obiectele pe care se aplica umbra */
	glPushMatrix();
	glMultMatrixf((GLfloat *)leftwallshadow);
	masa1();
	perna();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void draw(void)
{	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat);
	
	/* Texturi aplicate pe podea, pereti si tavan */
	glEnable(GL_TEXTURE_2D);
		drawTextureParchet();
		drawTexturePereteDreapta();
		drawTexturePereteSpate();
		drawTextureTavan();
	glDisable(GL_TEXTURE_2D);

	/* Umbra de pe podea */
	if (rendermode == NSHADOW) {
		glColor3f(0.26f, 0.15f, 0.1f);
		drawShadowPodea();
	}
	if (rendermode == BSHADOW) {
		glColor3f(0.f, 0.f, 0.f);
		drawShadowPodea();
	}
	if (rendermode == WSHADOW) {
		glColor3f(1.f, 1.f, 1.f);
		drawShadowPodea();
	}

	/* Pentru a putea aplica umbra pe peretele stang */
	if (rendermode != NONE) {
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}
	
	/* Textura peretelui si culoarea materialului */
	glEnable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_mat);
	drawTexturePereteStanga();
	glDisable(GL_TEXTURE_2D);

	/* Umbra de pe peretele stang */
	if (rendermode == NSHADOW) {
		glColor3f(0.26f, 0.15f, 0.1f);
		drawShadowPerete();
	}
	if (rendermode == BSHADOW) {
		glColor3f(0.f, 0.f, 0.f);
		drawShadowPerete();
	}
	if (rendermode == WSHADOW) {
		glColor3f(1.f, 1.f, 1.f);
		drawShadowPerete();
	}
	
	/* Lumina (becul) din scena */
	lumina();
	
	/* Obiectele din scena */
	masa1();
	masa2();
	masa3();
	masa4();
	perna();
	
	glutSwapBuffers();    
}

int main(int argc, char *argv[])
{
	GLfloat *tex;
	GLUquadricObj *sphere, *cone, *base, *blat;
	GLfloat plane[4];
	GLfloat v0[3], v1[3], v2[3];

	glutInit(&argc, argv);
	glutInitWindowSize(1080, 520);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE);
	(void)glutCreateWindow("Sala de lectura pentru copii");
	glutDisplayFunc(draw);
	glutKeyboardFunc(key);

	int subMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Normal", NSHADOW);
	glutAddMenuEntry("Black", BSHADOW);
	glutAddMenuEntry("White", WSHADOW);
	glutCreateMenu(menu);
	glutAddMenuEntry("No Shadows", NONE);
	glutAddSubMenu("Shadows", subMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Perspectiva scenei */
	glMatrixMode(GL_PROJECTION);
	glFrustum(-200., 200., -100., 100., 320., 640.);
	glMatrixMode(GL_MODELVIEW);

	/* Aplicarea proprietatilor */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* Setarea pozitiei si a culorii luminii */
	float alb[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, alb);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	/* Matricile de umbre */
	/* Cele 3 pct de pe podea */
	v0[X] = -200.f; v0[Y] = -100.f; v0[Z] = -320.f;
	v1[X] = 200.f;  v1[Y] = -100.f; v1[Z] = -320.f;
	v2[X] = 200.f;  v2[Y] = -100.f; v2[Z] = -620.f;
	findplane(plane, v0, v1, v2);
	shadowmatrix(floorshadow, plane, lightpos);

	/* Cele 3 pct de pe peretele stang */
	v0[X] = -200.f; v0[Y] = -100.f; v0[Z] = -320.f;
	v1[X] = -200.f; v1[Y] = -100.f; v1[Z] = -620.f;
	v2[X] = -200.f; v2[Y] = 120.f;  v2[Z] = -620.f;
	findplane(plane, v0, v1, v2);
	shadowmatrix(leftwallshadow, plane, lightpos);
	
	glCullFace(GL_BACK);

	/* Liste de display pentru fiecare obiect */

	glNewList(LIGHT, GL_COMPILE);
	sphere = gluNewQuadric();
	gluSphere(sphere, 5.f, 20, 20);
	gluDeleteQuadric(sphere);
	glEndList();

	glNewList(CONE, GL_COMPILE);
	cone = gluNewQuadric();
	base = gluNewQuadric();
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	gluDisk(base, 0., 10., 20, 1);
	gluCylinder(cone, 10., 1., 55., 20, 20);
	gluDeleteQuadric(cone);
	gluDeleteQuadric(base);
	glEndList();

	glNewList(BLAT, GL_COMPILE);
	blat = gluNewQuadric();
	base = gluNewQuadric();
	glRotatef(-90.f, 1.f, 0.f, 0.f);
	glPushMatrix();
	glTranslatef(0. ,0., 10. );
	gluDisk(base, 0., 40., 100, 1);
	glPopMatrix();
	gluDisk(base, 0., 40., 100, 1);
	gluCylinder(blat, 40., 40., 10., 100, 100);
	gluDeleteQuadric(blat);
	gluDeleteQuadric(base);
	glEndList();

	glNewList(SPHERE, GL_COMPILE);
	sphere = gluNewQuadric();
	gluSphere(sphere, 10.f, 50, 20);
	gluDeleteQuadric(sphere);
	glEndList();

	glutMainLoop();
	return 0;
}