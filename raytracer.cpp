#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
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
Point ul = Point(-0.5, 0.5, -0.5);
Point ll = Point(-0.5, -0.5, -0.5);
Point ur = Point(0.5, 0.5, -0.5);
Point lr = Point(0.5, -0.5, -0.5);
Color ambient;
Color diffuse;
Color specular;
Color shade;
Vector point;
float power = 64;
std::vector<Light*> lights;
std::vector<Object> objects;
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
  /*
  Sphere testSphere = Sphere(Point(0.0, 0.0, 5.0), 2.0);
  Object sphereObj = Object(&testSphere, 0.1, 0.1, 0.1, 0.5, 0.1, 0.5, 0.0, 1.0, 0.0, 0, 0, 0);
  objects.reserve(sizeof(Object));
  objects.push_back(sphereObj);
  DirecLight testLight = DirecLight(0.25, 0.25, 0.25, 1, 1, 1);
  lights.reserve(sizeof(Light));
  lights.push_back(&testLight);
  printf("run 1: size %d\n", lights.size());
  for (int l = 0; l < lights.size(); l++) {
    printf("run 1: item %d\n", l);
    cout << "type is " << lights[l]->type << endl;
    //printf("type is %s\n", lights[l]->type);
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
  /*
  Ray testRay = Ray(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0), 1.0, 8.0);
  Sphere testSphere = Sphere(Point(0.0, 0.0, 5.0), 2.0);
  float hitPoint = testSphere.intersect(testRay);
  if (hitPoint != -1.0) {
    printf("HIT\n");
  }
  printf("hitPoint = %f\n", hitPoint);
  */
  Ray testRay = Ray(Point(0.718590, 1.654307, 4.135769), Vector(-0.577350,-0.577350,-0.577350), 1.0, 8.0);
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


void sample(float centerX, float centerY, float radius) {
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
  
  /*
  printf("run 2: size %d\n", lights.size());
  for (int l = 0; l < lights.size(); l++) {
    printf("run 2: item %d\n", l);
    cout << "type is " << lights[l]->type << endl;
  }
  */

  objects.clear();
  lights.clear();
  Vector cameraDir = Vector(0, 0, -1.0);
  Sphere testSphere = Sphere(Point(0.0, 0.0, -5.0), 2.0);
  Object sphereObj = Object(&testSphere, 0.1, 0.1, 0.1, 0.5, 0.1, 0.5, 0.0, 1.0, 0.0, 0, 0, 0);
  objects.push_back(sphereObj);
  Triangle testTriangle = Triangle(Point(-3, 0, -2), Point(0, 0, -2), Point(-2, 4, -6));
  Object triangleObj = Object(&testTriangle, 0.1, 0.1, 0.1, 0.5, 0.1, 0.5, 0.0, 1.0, 0.0, 0, 0, 0);
  objects.push_back(triangleObj);
  DirecLight testDLight = DirecLight(0.25, 0.25, -0.25, 0, 1, 1);
  lights.push_back(&testDLight);
  PointLight testPLight = PointLight(7, 0, -5.0, 1, 1, 0);
  lights.push_back(&testPLight);

  /*
  Ray testRay = Ray(Point(0, 0, 0), Vector(-0.1, -0.1, -1));
  LocalGeo geoTest = LocalGeo();
  LocalGeo* geoTestPointer = &geoTest;
  hitPoint = testTriangle.intersect(testRay, geoTestPointer);
  */

  printf("objects is %d\n", objects.size());
  LocalGeo geo = LocalGeo();
  LocalGeo* geoPointer = &geo;
  for (i = 0; i < viewport.w; i++) {
    for (j = 0; j < viewport.h; j++) {
      // send out a ray
      Point viewPlanePoint = Point(i * xStep - 0.5, j * yStep - 0.5, -1);
      Ray sampleRay = Ray(camera, viewPlanePoint.sub(camera), 8.0);
      Color pixColor = Color();
      float bestHit = std::numeric_limits<float>::infinity();
      int nearestObj = 0;
      bool hit = false;
      for (int o = 0; o < objects.size(); o++) {
        Color lightColor = Color();
        float hitPoint = objects[o].shape->intersect(sampleRay, geoPointer);
        float hitLight;
        if (hitPoint != -1.0 && hitPoint < bestHit) {
          bestHit = hitPoint;
          nearestObj = o;
          hit = true;
        }
      }
      if (hit) {
        Color lightColor = Color();
        float hitPoint = objects[nearestObj].shape->intersect(sampleRay, geoPointer);
        float hitLight;
        Ray lightRay;
        for (int l = 0; l < lights.size(); l++) {
          bool blocked = false;
          lights[l]->generateLightRay(geoPointer, lightRay, lightColor);
          // check to see if light hits object
          for (int lo = 0; lo < objects.size(); lo++) {
            hitLight = objects[nearestObj].shape->intersect(lightRay);
            if (hitLight != -1.0) {
              blocked = true;
              break;
            }
          }
          if (!blocked) {
            Vector lightNeg = lightRay.dir;
            lightNeg.normalize();
            //diffuse
            Color difColor = objects[nearestObj].diffuse;
            difColor.mul(lightColor);
            difColor.scale(std::max(lightNeg.dot(geoPointer->normal), 0.0f));
            pixColor.add(difColor);
            //specular
            Color speColor = objects[nearestObj].specular;
            speColor.mul(lightColor);
            Vector r = geoPointer->normal;
            Vector v = sampleRay.dir;
            v.scale(-1);
            r.scale(lightNeg.dot(geoPointer->normal) * 2);
            r.sub(lightNeg);
            r.normalize();
            speColor.scale(pow(std::max(r.dot(v), 0.0f), power));
            pixColor.add(speColor);
          }
        }
      }

      /*
      for (int o = 0; o < objects.size(); o++) {
        Ray lightRay;
        Color lightColor = Color();
        float hitPoint = objects[o].shape->intersect(sampleRay, geoPointer);
        float hitLight;
        if (hitPoint != -1.0) {
          //ambient
          pixColor.add(objects[nearestObj].ambient);
          //Point tempPoint = sampleRay.at(hitPoint);
          for (int l = 0; l < lights.size(); l++) {
            bool blocked = false;
            lights[l]->generateLightRay(geoPointer, lightRay, lightColor);
            // check to see if light hits object
            for (int lo = 0; lo < objects.size(); lo++) {
              hitLight = objects[o].shape->intersect(lightRay);
              if (hitLight != -1.0) {
                blocked = true;
                break;
              }
            }
            if (!blocked) {
              //lightNeg.scale(-1);
              Vector lightNeg = lightRay.dir;
              lightNeg.normalize();
              //diffuse
              Color difColor = objects[o].diffuse;
              difColor.mul(lightColor);
              difColor.scale(std::max(lightNeg.dot(geoPointer->normal), 0.0f));
              pixColor.add(difColor);
              //specular
              Color speColor = objects[o].specular;
              speColor.mul(lightColor);
              Vector r = geoPointer->normal;
              Vector v = sampleRay.dir;
              v.scale(-1);
              r.scale(lightNeg.dot(geoPointer->normal) * 2);
              r.sub(lightNeg);
              r.normalize();
              speColor.scale(pow(std::max(r.dot(v), 0.0f), power));
              pixColor.add(speColor);
            }
          }
          break;
        }
      }
      */
      setPixel(i, j, pixColor.r, pixColor.g, pixColor.b);
      /*
      Ray lightRay;
      Color lightColor = Color();
      float hitPoint = testSphere.intersect(sampleRay, geoPointer);
      float hitLight;
      //ambient
      Color amb = Color(0.1, 0.1, 0.1);
      amb.mul(lightColor);
      pixColor.add(amb);
      if (hitPoint != -1.0) {
        Point tempPoint = sampleRay.at(hitPoint);
        testLight.generateLightRay(geoPointer, lightRay, lightColor);
        //printf("lightRay: origin = (%f, %f, %f)  dir = (%f, %f, %f)\n", lightRay.origin.x, lightRay.origin.y, lightRay.origin.z, lightRay.dir.x, lightRay.dir.y, lightRay.dir.z);
        hitLight = testSphere.intersect(lightRay);
        if (hitLight == -1.0) {
          Vector lightNeg = testLight.vector;
          lightNeg.scale(-1);
          lightNeg.normalize();
          if (i == 400 && j == 400) {
            printf("ambi: Color in center is (%f, %f, %f)\n", pixColor.r, pixColor.g, pixColor.b);
          }
          //diffuse
          Color difColor = Color(0.7, 0.1, 0.7);
          difColor.mul(lightColor);
          difColor.scale(std::max(lightNeg.dot(geoPointer->normal), 0.0f));
          pixColor.add(difColor);
          if (i == 400 && j == 400) {
            printf("diff: Color in center is (%f, %f, %f)\n", pixColor.r, pixColor.g, pixColor.b);
          }
          //specular
          Color speColor = Color(0.8, 0.8, 0.4);
          speColor.mul(lightColor);
          Vector speVector = geoPointer->normal;
          speVector.scale(lightNeg.dot(geoPointer->normal) * 2);
          speVector.add(testLight.vector);
          speColor.scale(pow(std::max(speVector.dot(sampleRay.dir), 0.0f), p));
          pixColor.add(speColor);
          if (i == 400 && j == 400) {
            printf("spec: Color in center is (%f, %f, %f)\n", pixColor.r, pixColor.g, pixColor.b);
          }
        }
        setPixel(i, j, pixColor.r, pixColor.g, pixColor.b);
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
  sample(viewport.w / 2.0 , viewport.h / 2.0 , min(viewport.w, viewport.h) * 0.45);

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
