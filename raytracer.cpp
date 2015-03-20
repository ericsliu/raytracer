#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits.h>
#include <sstream>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <unistd.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#ifndef LIGHT_H
#define LIGHT_H
#include "light.h"
#endif
#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "geometry.h"
#endif
#ifndef MATRIX_H
#define MATRIX_H
#include "matrix.h"
#endif

#define PI 3.14159265  // Should be used from mathlib
#define WINDOW_WIDTH 800.0
#define WINDOW_HEIGHT 800.0
inline float sqr(float x) { return x*x; }

using namespace std;

//****************************************************
// Some Classes
//****************************************************

class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};


//****************************************************
// Global Variables
//****************************************************
Viewport viewport;
Point camera = Point(0, 0, 0);
Point ul = Point(-0.5, 0.5, 0.5);
Point ll = Point(-0.5, -0.5, 0.5);
Point ur = Point(0.5, 0.5, 0.5);
Point lr = Point(0.5, -0.5, 0.5);
Color ambient;
Color diffuse;
Color specular;
Color shade;
Vector point;
float p;
std::vector<Light*> lights;
std::vector<Point> objPoints;
//std::vector<

//****************************************************
// Simple init function
//****************************************************
void initScene() {

  // Nothing to do here for this simple example.

}

//****************************************************
// Load necessary objects
//****************************************************
void loadObjs() {
  std::ifstream infile("icosahedron.obj");
  std::string line;
  /*
  while (std::getline(infile, line)) {
    //cout << "\n" << line;
    if (line.size() > 0 && line.at(0) == 'v') {
      std::istringstream split(line);
      std::string type;
      float x, y, z;
      if (!(split >> type >> x >> y >> z)) {
        break;
      }
      objPoints.push_back(Point(x, y, z));
    }
  }*/
  // CODE TO ENSURE PARSING .obj WORKS
  /*
  for (int i = 0; i < objPoints.size(); i++) {
    cout << objPoints[i].x << " " << objPoints[i].y << " " << objPoints[i].z << endl;
  }
  */
}

//****************************************************
// Tests that can be run
//****************************************************
void tests() {
  // point/vector tests
  printf("declaring initial variables\n");
  Point testPoint = Point(0.0, 1.0, 2.0);
  printf("testPoint is (%f, %f, %f)\n", testPoint.x, testPoint.y, testPoint.z);
  Vector testVector = Vector(0.6, 0.8, 1.0);
  printf("testVector is (%f, %f, %f)\n", testVector.x, testVector.y, testVector.z);
  Vector testVectorTwo = testVector;
  printf("created new testVectorTwo = testVector, normalizing testVectorTwo\n");
  testVectorTwo.normalize();
  printf("testVector is (%f, %f, %f)\n", testVector.x, testVector.y, testVector.z);
  printf("testVectorTwo is (%f, %f, %f)\n", testVectorTwo.x, testVectorTwo.y, testVectorTwo.z);

  // intersection tests
  printf("testing intersection:\n");
  printf("spheres\n");
  Ray testRay = Ray(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0, 8.0);
  Sphere testSphere = Sphere(Point(0.0, 0.0, 5.0), 2.0);
  float hitPoint = testSphere.intersect(testRay);
  if (hitPoint != -1.0) {
    printf("HIT\n");
  }
  printf("hitPoint = %f\n", hitPoint);
  printf("triangles\n");
  Triangle testTriangle = Triangle(Point(-1.0, -1.0, 4.0), Point(0.0, 2.0, 4.0), Point(1.0, -1.0, 4.0));
  LocalGeo geoTest = LocalGeo();
  LocalGeo* geoTestPointer = &geoTest;
  hitPoint = testTriangle.intersect(testRay, geoTestPointer);
  if (hitPoint != -1.0) {
    printf("HIT\n");
  }
  Vector geoNormal = geoTest.getNormal();
  printf("hitPoint = %f\n", hitPoint);
  printf("normal = (%f, %f, %f)\n", geoNormal.x, geoNormal.y, geoNormal.z);

  // matrix tests
  Matrix testVectorMatrix = Matrix(testVectorTwo);
  testVectorMatrix.printMatrix();
  Matrix testPointMatrix = Matrix(testPoint);
  testPointMatrix.printMatrix();
  testVectorMatrix.mul(testPointMatrix).printMatrix();
  Matrix testInvMatrix = Matrix();
  testInvMatrix.array[0][0] = 2;
  testInvMatrix.array[0][1] = 3;
  testInvMatrix.array[0][2] = 1;
  testInvMatrix.array[0][3] = 5;
  testInvMatrix.array[1][0] = 1;
  testInvMatrix.array[1][1] = 0;
  testInvMatrix.array[1][2] = 3;
  testInvMatrix.array[1][3] = 1;
  testInvMatrix.array[2][0] = 0;
  testInvMatrix.array[2][1] = 2;
  testInvMatrix.array[2][2] = -3;
  testInvMatrix.array[2][3] = 2;
  testInvMatrix.array[3][0] = 0;
  testInvMatrix.array[3][1] = 2;
  testInvMatrix.array[3][2] = 3;
  testInvMatrix.array[3][3] = 1;
  testInvMatrix.printMatrix();
  testInvMatrix.inv().printMatrix();
}

//****************************************************
// Mouse click callback
//****************************************************
void mouseClick(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    //lights.push_back(Light((float)(5*(x - (WINDOW_WIDTH / 2))/WINDOW_WIDTH), (float)(5*(-y + (WINDOW_HEIGHT / 2))/WINDOW_HEIGHT), pointZ, pointR, pointG, pointB, true));
    glutPostRedisplay();
  }
}

//****************************************************
// Deletes point lights
//****************************************************
void deletePoints() {
  for (int i = 0; i < lights.size(); i++) {
    if (lights[i]->type == "point") {
      lights.erase(lights.begin() + i);
      i--;
    }
  }
  glutPostRedisplay();
  printf("Deleting all point lights\n");
}

//****************************************************
// Function to parse input arguments to program
//****************************************************
void argParser(int argc, char *argv[]){ // TODO: Handle errors if input configuration is incorrect (e.g. r,g,b values missing)
  /*
  for (int i=1 ; i < argc ; ) {
    string arg;
    stringstream argstream;
    argstream << argv[i];
    argstream >> arg;
    if (arg == "-ka") {
      ambient = Color(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]));
      i += 4;
    } else if (arg == "-kd") {
      diffuse = Color(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]));
      i += 4;
    } else if (arg == "-ks") {
      specular = Color(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]));
      i += 4;
    } else if (arg == "-sp") {
      p = atof(argv[i+1]);
      i+=2;
    } else if (arg == "-pl") {
      lights.push_back(Light(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]),atof(argv[i+5]),atof(argv[i+6]),true));
      i += 7;
    } else if (arg == "-dl") {
      lights.push_back(Light(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]),atof(argv[i+4]),atof(argv[i+5]),atof(argv[i+6]),false));
      i += 7;
    }
    else {
      cout << "Invalid input argument '" << argv[i] << "' ignored \n";
      i++;
    }
  }
  */
}

//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, viewport.w, 0, viewport.h);

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// Sample the image
//****************************************************


void circle(float centerX, float centerY, float radius) {
  glBegin(GL_POINTS);

  int i,j;  // Pixel indices

  int minI = max(0,(int)floor(centerX-radius));
  int maxI = min(viewport.w-1,(int)ceil(centerX+radius));

  int minJ = max(0,(int)floor(centerY-radius));
  int maxJ = min(viewport.h-1,(int)ceil(centerY+radius));

  float xStep = 1.0 / viewport.w;
  float yStep = 1.0 / viewport.h;

  printf("w = %d, h = %d", viewport.w, viewport.h);
  printf("xStep = %f, yStep = %f\n", xStep, yStep);

  Vector cameraDir = Vector(0, 0, 1.0);
  Sphere testSphere = Sphere(Point(0.0, 0.0, 5.0), 2.0);
  DirecLight testLight = DirecLight(0.25, 0.25, 0.25, 1, 0, 1);
  LocalGeo geo = LocalGeo();
  LocalGeo* geoPointer = &geo;

  for (i = 0; i < viewport.w; i++) {
    /*
    if (i % 10 == 0) {
      printf("i = %d\n", i);
    }*/
    for (j = 0; j < viewport.h; j++) {
      // send out a ray
      Point viewPlanePoint = Point(i * xStep - 0.5, j * yStep - 0.5, 1);
      Ray sampleRay = Ray(camera, viewPlanePoint.sub(camera), 8.0);
      Ray lightRay;
      Color lightColor;
      float hitPoint = testSphere.intersect(sampleRay, geoPointer);
      float hitLight;
      if (hitPoint != -1.0) {
        testLight.generateLightRay(geoPointer, lightRay, lightColor);
        hitLight = testSphere.intersect(lightRay);
        //printf("hitlight = %f\n", hitLight);
        if (hitLight == -1.0) {
          setPixel(i, j, (geoPointer->normal.x / 2) + 0.25, (geoPointer->normal.y / 2) + 0.25, geoPointer->normal.z);
        } else {
          setPixel(i, j, 1, 0, 1);
        }
      } else {
        setPixel(i, j, 0, 0, 0);
      }
      /*
      if (sqrt(pow(i - 400, 2) + pow(j - 400, 2)) <= 400) {
        setPixel(i, j, 1, 1, 1);
      } else {
        setPixel(i, j, 0, 0, 0);
      }
      */
    }
  }

/*
  Ray testRay = Ray(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0, 8.0);
  Sphere testSphere = Sphere(Point(0.0, 0.0, 5.0), 2.0);
  float hitPoint = testSphere.intersect(testRay);
  */

/*
  Vector center = Vector(centerX,centerY,0);

  for (i=0;i<viewport.w;i++) {
    for (j=0;j<viewport.h;j++) {
      // Location of the center of pixel relative to center of sphere
      float x = (i+0.5-centerX);
      float y = (j+0.5-centerY);

      float dist = sqrt(sqr(x) + sqr(y));

      if (dist<=radius) {
        shade = Color();
        // This is the front-facing Z coordinate
        float z = sqrt(radius*radius-dist*dist);

        point = Vector(x,y,z);
        Light light;
        Vector r;
        Vector v = Vector(0,0,1);
        Vector normal;
        Vector vector;
        for (int i = 0; i < lights.size() ; i++) {
          light = lights[i];
          normal = point;
          normal.normalize();
          if (light.type == "point") {
            vector = light.vector + (-1*normal);
          } else {
            vector = -1*light.vector;
          }
          vector.normalize();
          r = (-1*vector) + normal*2*(vector*normal);
          r.normalize();
          shade = shade + ambient*light.color + diffuse*light.color * std::max(vector*normal,0.0f) + specular*light.color * pow(std::max(r*v,0.0f),p);
        }

        setPixel(i,j, shade.r, shade.g, shade.b);
      }
    }
  }*/
  glEnd();
}
//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT);				// clear the color buffer

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations
  glLoadIdentity();				        // make sure transformation is "zero'd"

  // Start drawing
  circle(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) * 0.45);

  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// called by glut when there are no messages to handle
//****************************************************
void myIdle() {
  #ifdef OSX
    usleep(10000); // give ~10ms back to OS to save CPU
  #else
    usleep(10000); // give ~10ms back to OS to save CPU
  #endif
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {
  // This parses the input arguments to set variables/params
  argParser(argc, argv);

  // Run some tests (comment out as necessary)
  //tests();

  // Load up .obj files
  loadObjs();

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  // Initalize theviewport size
  viewport.w = WINDOW_WIDTH;
  viewport.h = WINDOW_HEIGHT;

  //The size and position of the window
  glutInitWindowSize(viewport.w, viewport.h);
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutDisplayFunc(myDisplay);				// function to run when its time to draw something
  glutReshapeFunc(myReshape);				// function to run when the window gets resized
  glutIdleFunc(myIdle);             // function to run when not handling any other task
  glutMouseFunc(mouseClick);        // function to run when mouse is clicked

  // ********* Tests *********

  // // Color Tests
  // Color v = Color(0.5,0.5,0.5);
  // Color u = Color(0.3,0.3,0.3);
  // Color t = Color(100, 40000, 200000);
  // cout << v;
  // cout << u;
  // cout << t;
  // cout << u*v;

  // // Vector tests
  // Vector v = Vector(1.0, 2.0, 3.0);
  // cout << sum(v) << '\n';
  // cout << v;
  // v.normalize();
  // cout << v;
  // cout << sum(v) << '\n';

  // // Light tests
  // for (int i = 0; i < lights.size() ; i++) {
  //   cout << lights[i];
  // }

  // *************************

  glutMainLoop(); // infinite loop that will keep drawing and resizing and whatever else

  return 0;
}
