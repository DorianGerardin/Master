// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include <math.h> 
#include "src/Vec3.h"
#include "src/Camera.h"

enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };

bool display_normals;
bool display_smooth_normals;
bool display_mesh;
bool display_basis;
DisplayMode displayMode;

int nbU = 20;
int nbV = 20;

std::vector<Vec3> TabControlPoint;

std::vector<Vec3> buildingPoints;

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glDisable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    display_normals = false;
    display_mesh = true;
    display_smooth_normals = true;
    displayMode = LIGHTED;
    display_basis = false;
}

void DrawCurve(std::vector<Vec3> TabPointsOfCurve, long nbPoints) {

	glBegin(GL_LINE_STRIP);
	for (long i = 0; i < nbPoints; ++i)
	{
		glVertex3f(TabPointsOfCurve[i][0],
				   TabPointsOfCurve[i][1],
				   TabPointsOfCurve[i][2]);
	}
	glEnd();
}

void drawPoints(std::vector<Vec3> TabPointsOfCurve, long nbPoints) {

    glBegin(GL_POINTS);
    for (long i = 0; i < nbPoints; ++i)
    {
        glVertex3f(TabPointsOfCurve[i][0],
                   TabPointsOfCurve[i][1],
                   TabPointsOfCurve[i][2]);
    }
    glEnd();
}

std::vector<Vec3> HermiteCubicCurve(Vec3 P0, Vec3 P1, Vec3 v0, Vec3 v1, long nbU) {

	std::vector<Vec3> points;

	for (long i = 0; i < nbU; ++i)
	{

		float u = ((float)i)/nbU;

		float F1 = (2*pow(u, 3)) - (3*pow(u, 2)) + 1;
		float F2 = (-2*pow(u, 3)) + (3*pow(u, 2));
		float F3 = (pow(u, 3)) - (2*pow(u, 2)) + u;
		float F4 = (pow(u, 3)) - (pow(u, 2));

		Vec3 point;
		point[0] = F1*P0[0] + F2*P1[0] + F3*v0[0] + F4*v1[0];
		point[1] = F1*P0[1] + F2*P1[1] + F3*v0[1] + F4*v1[1];
		point[2] = F1*P0[2] + F2*P1[2] + F3*v0[2] + F4*v1[2];
		std::cout << point << std::endl;
		points.push_back(point);

	}

	return points;

}

int fact(int n) {
	if (n == 0) {
		return 1; 
	} else return n * fact(n-1);
}

std::vector<Vec3> BezierCurveByBernstein(std::vector<Vec3> TabControlPoint, long nbControlPoint, long nbU) {

	std::vector<Vec3> points;
    int n = nbControlPoint - 1;

	for (long i = 0; i < nbU; ++i)
	{
		float u = ((float)i)/(nbU-1);

		Vec3 B = Vec3(0., 0., 0.);

		for (int j = 0; j < nbControlPoint; ++j)
		{
			float polynome = fact(n) / (fact(j)*fact((n-j))) *
					pow(u, j) * pow((1-u), (n-j));
			B +=  polynome * TabControlPoint[j];

			std::cout << "B : " << B << std::endl;
			std::cout << "u : " << u << std::endl;
			std::cout << "polynome : " << polynome << std::endl;
		}
		points.push_back(B);
	}
	return points;

}

Vec3 BezierCurveByCasteljauAux(std::vector<Vec3> TabControlPoint, int k, int i, float u) {

    if(k == 0) {
        return TabControlPoint[i];
    } else {
        Vec3 p1 = BezierCurveByCasteljauAux(TabControlPoint, k-1, i, u);
        Vec3 p2 = BezierCurveByCasteljauAux(TabControlPoint, k-1, i+1, u);
        Vec3 p = ((1-u) * p1) + (u * p2);
        buildingPoints.push_back(p);
        return p;
    }

}

std::vector<Vec3> BezierCurveByCasteljau(std::vector<Vec3> TabControlPoint, long nbControlPoint, long nbU) {

    std::vector<Vec3> points;
    int n = nbControlPoint - 1;

    for (long i = 0; i < nbU; ++i)
    {
        float u = ((float)i)/(nbU-1);

        Vec3 B = Vec3(0., 0., 0.);
        
        B = BezierCurveByCasteljauAux(TabControlPoint, n, 0, u);
        points.push_back(B);
        
    }
    return points;

}

std::vector<std::vector<Vec3>> surfaceCylindrique(std::vector<Vec3> BezierCurve, Vec3 vDroite, long nbV) {

    std::vector<std::vector<Vec3>> ensembleCourbes;
    std::vector<Vec3> courbe;

    for (size_t k = 0; k < BezierCurve.size(); ++k) {
        courbe.clear();
        for (long j = 0; j < nbV; ++j) {
            float v = ((float)j)/(nbV-1);
            courbe.push_back(BezierCurve[k] + (v*vDroite));
        }
        ensembleCourbes.push_back(courbe);
    }
    
    return ensembleCourbes;

}

std::vector<std::vector<Vec3>> surfaceReglee(std::vector<Vec3> BezierCurve1, std::vector<Vec3> BezierCurve2, long nbV) {

    std::vector<std::vector<Vec3>> ensembleCourbes;
    std::vector<Vec3> courbe;

    for (size_t i = 0; i < BezierCurve1.size(); ++i) {
        courbe.clear();
        for (long k = 0; k < nbV; ++k) {
            float v = ((float)k)/(nbV-1);
            courbe.push_back((1-v)*BezierCurve1[i] + v*BezierCurve2[i]);
        }
        ensembleCourbes.push_back(courbe);
    }
    
    return ensembleCourbes;

}


std::vector<std::vector<Vec3>> BezierSurfaceByBernstein(std::vector<std::vector<Vec3>> GrilleControlPoint, long nbControlPointU, long nbControlPointV, long nbU, long nbV) {

    std::vector<std::vector<Vec3>> ensembleCourbes;
    std::vector<Vec3> courbe;

    int n = nbControlPointU - 1;
    int m = nbControlPointV - 1;

    for (int k = 0; k < nbU; ++k) {

        courbe.clear();
        float u = ((float)k)/(nbU-1);

        for (int l = 0; l < nbV; ++l) {

            float v = ((float)l)/(nbV-1);
            Vec3 B = Vec3(0., 0., 0.);

            for (int i = 0; i < nbControlPointU; ++i)
            {
                for (int j = 0; j < nbControlPointV; ++j)
                {

                    float polynomeI = fact(n) / (fact(i)*fact((n-i))) *
                            pow(u, i) * pow((1-u), (n-i));

                    float polynomeJ = fact(m) / (fact(j)*fact((m-j))) *
                            pow(v, j) * pow((1-v), (m-j));

                    B += polynomeI * polynomeJ * GrilleControlPoint[i][j]; 
                }
            }
            courbe.push_back(B);
        }
        ensembleCourbes.push_back(courbe);
    }
    return ensembleCourbes;
}


void drawSurface(std::vector<std::vector<Vec3>> ensembleDroites) {

    size_t m = ensembleDroites[0].size();
    unsigned int size = ensembleDroites.size();

    for (size_t i = 0; i < size; ++i)
    {
        DrawCurve(ensembleDroites[i], m);
    }

    std::vector<Vec3> curves;

    for (size_t i = 0; i < m; ++i)
    {
        curves.clear();
        for (size_t j = 0; j < size; ++j){
            curves.push_back(ensembleDroites[j][i]);
        }
        DrawCurve(curves, size);
    } 

}


void initTabControlPoint() {
    
    Vec3 v1 = Vec3(0.,0.,1.);
    Vec3 v2 = Vec3(2.,0.,1.);
    Vec3 v3 = Vec3(2.,2.,1.);
    Vec3 v4 = Vec3(0.,2.,1.);
    Vec3 v5 = Vec3(-2.,2.,-1.);
    Vec3 v6 = Vec3(-2.,4.,-1.);
    Vec3 v7 = Vec3(0.,4.,-1.);

    TabControlPoint.push_back(v1);
    TabControlPoint.push_back(v2);
    TabControlPoint.push_back(v3);
    TabControlPoint.push_back(v4);
    TabControlPoint.push_back(v5);
    TabControlPoint.push_back(v6);
    TabControlPoint.push_back(v7);
}



void draw () {

	if(displayMode == LIGHTED || displayMode == LIGHTED_WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);

    }  else if(displayMode == WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glDisable (GL_LIGHTING);

    }  else if(displayMode == SOLID ){
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    }

    glColor3f(0.8,1,0.8);

    if(displayMode == SOLID || displayMode == LIGHTED_WIRE){
        glEnable (GL_POLYGON_OFFSET_LINE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (1.0f);
        glPolygonOffset (-2.0, 1.0);

        glColor3f(0.,0.,0.);

        glDisable (GL_POLYGON_OFFSET_LINE);
        glEnable (GL_LIGHTING);
    }

    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHTING);

    initTabControlPoint();

    std::vector<Vec3> BezierCurve = BezierCurveByBernstein(TabControlPoint, 7, nbU);

    Vec3 p1D = Vec3(-1.,0.,1.);
    Vec3 p2D = Vec3(-1.,0.,-1.);
    Vec3 vDroite = p2D - p1D;

    std::vector<std::vector<Vec3>> ensembleDroites = surfaceCylindrique(BezierCurve, vDroite, nbV);

    //DrawCurve(TabControlPoint, 7);

    drawSurface(ensembleDroites);

    //---------Question 2----------

/*    std::vector<Vec3> TabControlPoint2;

    Vec3 v1 = Vec3(0.,0.,0.);
    Vec3 v2 = Vec3(1.,3.,1.);
    Vec3 v3 = Vec3(3.,1.,2.);
    Vec3 v4 = Vec3(-2.,2.,3.);
    Vec3 v5 = Vec3(-4.,1.,-3.);
    Vec3 v6 = Vec3(-1.,2.,-1.);
    Vec3 v7 = Vec3(-2.,1.,-1.);

    TabControlPoint2.push_back(v1);
    TabControlPoint2.push_back(v2);
    TabControlPoint2.push_back(v3);
    TabControlPoint2.push_back(v4);
    TabControlPoint2.push_back(v5);
    TabControlPoint2.push_back(v6);
    TabControlPoint2.push_back(v7);

    std::vector<Vec3> BezierCurve1 = BezierCurveByBernstein(TabControlPoint, 7, nbU);
    std::vector<Vec3> BezierCurve2 = BezierCurveByBernstein(TabControlPoint2, 7, nbU);

    std::vector<std::vector<Vec3>> ensembleDroites = surfaceReglee(BezierCurve1, BezierCurve2, nbV);

    drawSurface(ensembleDroites);*/

    //---------Question 3----------

/*    std::vector <std::vector<Vec3>> points =
                {
                        {
                                Vec3(0, 1, 0),
                                Vec3(0, 0, 1),
                                Vec3(0, 0, 2),
                                Vec3(0, 1, 3),
                        },
                        {
                                Vec3(1, 0, 0),
                                Vec3(1, 1, 1),
                                Vec3(1, 1, 2),
                                Vec3(1, 0, 3),
                        },
                        {
                                Vec3(1, 0, 0),
                                Vec3(1, 1, 1),
                                Vec3(1, 1, 2),
                                Vec3(1, 0, 3),
                        },
                        {
                                Vec3(2, 1, 0),
                                Vec3(2, 0, 1),
                                Vec3(2, 0, 2),
                                Vec3(2, 1, 3),
                        }
                };

    std::vector <std::vector<Vec3>> ensembleCourbes = BezierSurfaceByBernstein(points, 4, 4, nbU, nbV);         
    //drawSurface(points);
    drawSurface(ensembleCourbes);*/
}

void changeDisplayMode(){
    if(displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if(displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if(displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
}

void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    std::vector<std::vector<Vec3>> ensembleDroites;
    std::vector<Vec3> BezierCurve;
    Vec3 p1D,p2D,vDroite;
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;


    case 'w': //Change le mode d'affichage
        changeDisplayMode();
        break;

    case 'U': //Augmente u
        nbU++;
        break;

    case 'V': //Augmente v
        nbV++;
        break;

    case 'u': //Diminue u
        nbU--;
        break;

    case 'v': //Diminue v
        nbV--;
        break;

    default:
        break;
    }
    idle ();
}

void reshape(int w, int h) {
    camera.resize (w, h);
}

//Mouse events
void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }

    idle ();
}

//Mouse motion, update camera
void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}

int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("TP HAI702I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    glutMainLoop ();
    return EXIT_SUCCESS;
}