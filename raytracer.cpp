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

#include "light.h"
#include "geometry.h"
#include "matrix.h"

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
Color ambientColor = Color(0.3, 0.3, 0.3);
Color diffuseColor = Color(0.7, 0.7, 0.7);
Color specularColor = Color(0.7, 0.7, 0.7);
float specularPower = 64;
int REF_BOUNCES = 5;
Color reflectiveColor = Color(0.7, 0.7, 0.7);
Vector point;
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
void loadObjs(std::string filename, Matrix& transform) {
  std::vector< Point > objPoints;
  std::vector< Shape* > triangles;
  std::ifstream infile(filename.c_str());
  std::string line;

  if (infile.is_open()) {
    while (std::getline(infile,line)) {
      std::istringstream lineStream(line);
      std::vector< std::string > params;
      std::string token;
      while(std::getline(lineStream, token, ' ')) {
        if (token.size()==0) continue;
        else if (token[0]=='#') break;
        else params.push_back(token);
      }

      if (params.size()<1) continue;
      
      if (params[0].compare("v") == 0) {
        float z;
        if (params.size()>3) z = std::atof(params[3].c_str());
        else z = 0.0;
        float x = std::atof(params[1].c_str());
        float y = std::atof(params[2].c_str());
        objPoints.push_back(Point(x,y,z));
      }
      else if (params[0].compare("f") == 0) {
        int indices[3];
        int count = 0;
        for (unsigned int k=1; k < params.size(); ++k){
          std::stringstream triangleStream(params[k]);
          std::string index;
          if (std::getline(triangleStream, index, '/')){
            indices[count] = std::atoi(index.c_str())-1;
            count += 1;
          }
        }
        if (count > 3) std::cout<<"Parser doesn't support polygons with more than 3 vertices!"<<std::endl;
        Triangle* triangle = new Triangle(objPoints[indices[0]],objPoints[indices[1]],objPoints[indices[2]]);
        triangles.push_back(triangle);
      }
      else std::cout<<"Unrecognized parameter: "<<params[0]<<std::endl;
    }
    infile.close();
    if (triangles.size() > 0) {
      Object obj = Object(triangles, ambientColor, diffuseColor, specularColor, specularPower, reflectiveColor);
      obj.setTransform(transform);
      objects.push_back(obj);
    }
  } else {
    std::cout<<"Can't open file!"<<std::endl; 
  }
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
void argParser(int argc, char *argv[]){
  /*
  for (int i=1 ; i < argc ; ) {
    string arg;
    stringstream argstream;
    argstream << argv[i];
    argstream >> arg;
    if (arg == "-ka") {
      ambientColor = Color(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]));
      i += 4;
    } else if (arg == "-kd") {
      diffuseColor = Color(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]));
      i += 4;
    } else if (arg == "-ks") {
      specularColor = Color(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]));
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
    else {
      cout << "Invalid input argument '" << argv[i] << "' ignored \n";
      i++;
    }
  }
  */
}

void loadScene() {
  std::ifstream infile("raytracer.scn");
  std::string line;

  Matrix transform = Matrix();

  if (infile.is_open()) {
    while (std::getline(infile,line)) {
      std::istringstream lineStream(line);
      std::vector< std::string > params;
      std::string token;
      while(std::getline(lineStream, token, ' ')) {
        if (token.size()==0) continue;
        else if (token[0]=='#') break;
        else params.push_back(token);
      }

      if (params.size()<1) continue;
      
      // Parser the arguments
      if (params[0].compare("cam") == 0) {
        camera = Point(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
        ll = Point(atof(params[4].c_str()), atof(params[5].c_str()), atof(params[6].c_str()));
        lr = Point(atof(params[7].c_str()), atof(params[8].c_str()), atof(params[9].c_str()));
        ul = Point(atof(params[10].c_str()), atof(params[11].c_str()), atof(params[12].c_str()));
        ur = Point(atof(params[13].c_str()), atof(params[14].c_str()), atof(params[15].c_str()));
        std::cout << "Eye: " << camera.x << " " << camera.y << " " << camera.z << std::endl;
        std::cout << "LL: " << ll.x << " " << ll.y << " " << ll.z << std::endl;
        std::cout << "LR: " << lr.x << " " << lr.y << " " << lr.z << std::endl;
        std::cout << "UL: " << ul.x << " " << ul.y << " " << ul.z << std::endl;
        std::cout << "UR: " << ur.x << " " << ur.y << " " << ur.z << std::endl;
      }
      else if (params[0].compare("sph") == 0) {
        Sphere* sph;
        Point center = Point(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
        float radius = atof(params[4].c_str());
        sph = new Sphere(center,radius);
        Object obj = Object(sph, ambientColor, diffuseColor, specularColor, specularPower, reflectiveColor);
        obj.setTransform(transform);
        std::cout << "Sphere Transform:" << std::endl;
        std::cout << obj.transform.array[0][0] << " " << obj.transform.array[0][1] << " " << obj.transform.array[0][2] << " " << obj.transform.array[0][3] << std::endl;
        std::cout << obj.transform.array[1][0] << " " << obj.transform.array[1][1] << " " << obj.transform.array[1][2] << " " << obj.transform.array[1][3] << std::endl;
        std::cout << obj.transform.array[2][0] << " " << obj.transform.array[2][1] << " " << obj.transform.array[2][2] << " " << obj.transform.array[2][3] << std::endl;
        objects.push_back(obj);
        std::cout << "Sphere center: " << center.x << " " << center.y << " " << center.z << std::endl;
        std::cout << "Sphere radius: " << radius << std::endl;
      }
      else if (params[0].compare("tri") == 0) {
        Triangle* tri;
        Point point1 = Point(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
        Point point2 = Point(atof(params[4].c_str()), atof(params[5].c_str()), atof(params[6].c_str()));
        Point point3 = Point(atof(params[7].c_str()), atof(params[8].c_str()), atof(params[9].c_str()));
        tri = new Triangle(point1,point2,point3);
        Object obj = Object(tri, ambientColor, diffuseColor, specularColor, specularPower, reflectiveColor);
        obj.setTransform(transform);
        std::cout << "Triangle Transform:" << std::endl;
        std::cout << obj.transform.array[0][0] << " " << obj.transform.array[0][1] << " " << obj.transform.array[0][2] << " " << obj.transform.array[0][3] << std::endl;
        std::cout << obj.transform.array[1][0] << " " << obj.transform.array[1][1] << " " << obj.transform.array[1][2] << " " << obj.transform.array[1][3] << std::endl;
        std::cout << obj.transform.array[2][0] << " " << obj.transform.array[2][1] << " " << obj.transform.array[2][2] << " " << obj.transform.array[2][3] << std::endl;
        objects.push_back(obj);
      }
      else if (params[0].compare("obj") == 0) {
        loadObjs(params[1], transform);
      }
      else if (params[0].compare("ltp") == 0) {
        PointLight* pl;
        float x = atof(params[1].c_str());
        float y = atof(params[2].c_str());
        float z = atof(params[3].c_str());
        float r = atof(params[4].c_str());
        float g = atof(params[5].c_str());
        float b = atof(params[6].c_str());
        pl = new PointLight(x, y, z, r, g, b);
        lights.push_back(pl);
      }
      else if (params[0].compare("ltd") == 0) {
        DirecLight* dl;
        float x = atof(params[1].c_str());
        float y = atof(params[2].c_str());
        float z = atof(params[3].c_str());
        float r = atof(params[4].c_str());
        float g = atof(params[5].c_str());
        float b = atof(params[6].c_str());
        dl = new DirecLight(x, y, z, r, g, b); 
        lights.push_back(dl);
      }
      else if (params[0].compare("lta") == 0) {
        AmbieLight* al;
        float r = atof(params[1].c_str());
        float g = atof(params[2].c_str());
        float b = atof(params[3].c_str());
        al = new AmbieLight(r, g, b); 
        lights.push_back(al);
      }
      else if (params[0].compare("mat") == 0) {
        // These are globally accessible
        ambientColor.r = atof(params[1].c_str());
        ambientColor.g = atof(params[2].c_str());
        ambientColor.b = atof(params[3].c_str());

        diffuseColor.r = atof(params[4].c_str());
        diffuseColor.g = atof(params[5].c_str());
        diffuseColor.b = atof(params[6].c_str());

        specularColor.r = atof(params[7].c_str());
        specularColor.g = atof(params[8].c_str());
        specularColor.b = atof(params[9].c_str());

        float specularPower = atof(params[10].c_str());

        reflectiveColor.r = atof(params[11].c_str());
        reflectiveColor.g = atof(params[12].c_str());
        reflectiveColor.b = atof(params[13].c_str());
      }
      else if (params[0].compare("xft") == 0) {
        Vector t = Vector(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
        Matrix translation = Matrix(t, true);
        transform = transform.mul(translation);
        std::cout << "Translation:" << std::endl;
        std::cout << translation.array[0][0] << " " << translation.array[0][1] << " " << translation.array[0][2] << " " << translation.array[0][3] << std::endl;
        std::cout << translation.array[1][0] << " " << translation.array[1][1] << " " << translation.array[1][2] << " " << translation.array[1][3] << std::endl;
        std::cout << translation.array[2][0] << " " << translation.array[2][1] << " " << translation.array[2][2] << " " << translation.array[2][3] << std::endl;
        std::cout << "New Transform:" << std::endl;
        std::cout << transform.array[0][0] << " " << transform.array[0][1] << " " << transform.array[0][2] << " " << transform.array[0][3] << std::endl;
        std::cout << transform.array[1][0] << " " << transform.array[1][1] << " " << transform.array[1][2] << " " << transform.array[1][3] << std::endl;
        std::cout << transform.array[2][0] << " " << transform.array[2][1] << " " << transform.array[2][2] << " " << transform.array[2][3] << std::endl;
      }
      else if (params[0].compare("xfr") == 0) {
        // this is the rotation matrix
        Vector r_hat = Vector(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
        float theta = r_hat.norm(); // /180.0*PI
        r_hat.normalize();
        std::cout << "Axis of Rotation:" << std::endl;
        std::cout << r_hat.x << " " << r_hat.y << " " << r_hat.z << std::endl;
        std::cout << "Rotation Amount (Radians):" << std::endl;
        std::cout << theta << std::endl;
        Matrix r_x = Matrix(0, r_hat.z*-1, r_hat.y, r_hat.z, 0, r_hat.x*-1, r_hat.y*-1, r_hat.x, 0);
        Matrix dyad = Matrix(r_hat.x*r_hat.x, r_hat.x*r_hat.y, r_hat.x*r_hat.z, r_hat.x*r_hat.y, r_hat.y*r_hat.y, r_hat.y*r_hat.z, r_hat.x*r_hat.z, r_hat.y*r_hat.z, r_hat.z*r_hat.z);
        Matrix rotation = dyad.add(r_x.scale(sin(theta))).add(r_x.mul(r_x).scale(-1*cos(theta)));
        // Matrix rotation = Matrix().add(r_x.scale(sin(theta))).add(r_x.mul(r_x).scale(1+(-1*cos(theta))));
        transform = transform.mul(rotation);
        std::cout << "Rotation:" << std::endl;
        std::cout << rotation.array[0][0] << " " << rotation.array[0][1] << " " << rotation.array[0][2] << " " << rotation.array[0][3] << std::endl;
        std::cout << rotation.array[1][0] << " " << rotation.array[1][1] << " " << rotation.array[1][2] << " " << rotation.array[1][3] << std::endl;
        std::cout << rotation.array[2][0] << " " << rotation.array[2][1] << " " << rotation.array[2][2] << " " << rotation.array[2][3] << std::endl;
        std::cout << "New Transform:" << std::endl;
        std::cout << transform.array[0][0] << " " << transform.array[0][1] << " " << transform.array[0][2] << " " << transform.array[0][3] << std::endl;
        std::cout << transform.array[1][0] << " " << transform.array[1][1] << " " << transform.array[1][2] << " " << transform.array[1][3] << std::endl;
        std::cout << transform.array[2][0] << " " << transform.array[2][1] << " " << transform.array[2][2] << " " << transform.array[2][3] << std::endl;
    }
      else if (params[0].compare("xfs") == 0) {
        Vector s = Vector(atof(params[1].c_str()), atof(params[2].c_str()), atof(params[3].c_str()));
        Matrix scaling = Matrix(s);
        transform = transform.mul(scaling);
        std::cout << "Scaling:" << std::endl;
        std::cout << scaling.array[0][0] << " " << scaling.array[0][1] << " " << scaling.array[0][2] << " " << scaling.array[0][3] << std::endl;
        std::cout << scaling.array[1][0] << " " << scaling.array[1][1] << " " << scaling.array[1][2] << " " << scaling.array[1][3] << std::endl;
        std::cout << scaling.array[2][0] << " " << scaling.array[2][1] << " " << scaling.array[2][2] << " " << scaling.array[2][3] << std::endl;
        std::cout << "New Transform:" << std::endl;
        std::cout << transform.array[0][0] << " " << transform.array[0][1] << " " << transform.array[0][2] << " " << transform.array[0][3] << std::endl;
        std::cout << transform.array[1][0] << " " << transform.array[1][1] << " " << transform.array[1][2] << " " << transform.array[1][3] << std::endl;
        std::cout << transform.array[2][0] << " " << transform.array[2][1] << " " << transform.array[2][2] << " " << transform.array[2][3] << std::endl;
      }
      else if (params[0].compare("xfz") == 0) {
        transform = Matrix();
        std::cout << "New Transform:" << std::endl;
        std::cout << transform.array[0][0] << " " << transform.array[0][1] << " " << transform.array[0][2] << " " << transform.array[0][3] << std::endl;
        std::cout << transform.array[1][0] << " " << transform.array[1][1] << " " << transform.array[1][2] << " " << transform.array[1][3] << std::endl;
        std::cout << transform.array[2][0] << " " << transform.array[2][1] << " " << transform.array[2][2] << " " << transform.array[2][3] << std::endl;
      }
      else std::cout<<"Unrecognized parameter: "<<params[0]<<std::endl;
    }
    infile.close();
  } else {
    std::cout<<"Can't open file!"<<std::endl; 
  }
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


void sample(float centerX, float centerY) {
  glBegin(GL_POINTS);

  int i,j;  // Pixel indices

  float yStep = (ul.y - ll.y) / viewport.h;
  float xStep = (lr.x - ll.x) / viewport.w;

  for (i = 0; i < viewport.w; i++) {
    for (j = 0; j < viewport.h; j++) {
      // send out a ray
      Point viewPlanePoint = Point((i - centerX + 0.5) * xStep, (j - centerY + 0.5) * yStep, 0);
      Ray sampleRay = Ray(camera, viewPlanePoint.sub(camera));
      
      Color pixColor = Color();
      float bestHit = std::numeric_limits<float>::infinity();
      int nearestObjIndex = -1;
      // loop to find nearest object
      for (int o = 0; o < objects.size(); o++) {
        float hitPoint = objects[o].intersect(sampleRay);
        if (hitPoint != -1.0 && hitPoint < bestHit) {
          bestHit = hitPoint;
          nearestObjIndex = o;
        }
      }
      if (nearestObjIndex != -1) {
        // Calculate collision point and normal on the nearest object
        LocalGeo geo = LocalGeo();
        objects[nearestObjIndex].intersect(sampleRay, &geo);

        Ray shadowRay = Ray();
        Color lightColor = Color();
        // Iterate over lights and compute color
        for (int l=0; l < lights.size(); l++) {
          lights[l]->generateLightRay(&geo, shadowRay, lightColor); // Shadow Ray (Ray from object towards light source)

          Color tempColor;
          // Compute ambient color
          tempColor = objects[nearestObjIndex].ambient;
          tempColor.mul(lightColor);
          pixColor.add(tempColor);

          if (lights[l]->type.compare("ambient") == 0) continue;
          else {
            // Check if shadowRay isn't obstructed
            bool blocked = false;
            for (int o = 0; o < objects.size(); o++) {
              if (objects[o].intersect(shadowRay) != -1.0) {
                blocked = true;
                break;
              }
            }
            if (!blocked) {
              Vector lightDirection = shadowRay.dir;

              // Compute diffuse color
              double c2 = std::max(0.0f, (lightDirection).dot(geo.normal));
              tempColor = objects[nearestObjIndex].diffuse;
              tempColor.mul(lightColor);
              tempColor.scale(c2);
              pixColor.add(tempColor);

              // Compute specular color
              Vector projection = geo.normal;
              projection.scale(lightDirection.dot(geo.normal)*2.0);
              Vector reflection = lightDirection;
              reflection.scale(-1);
              reflection.add(projection);
              double c3 = pow(std::max(0.0f, -reflection.dot(sampleRay.dir)), objects[nearestObjIndex].specularPow);
              tempColor = objects[nearestObjIndex].specular;
              tempColor.mul(lightColor);
              tempColor.scale(c3);


              pixColor.add(tempColor);

              // Linear falloff (UNUSED)
              // if (lights[l]->type.compare("point") == 0){
              //   color = color.scale(1/shadowRay.tMax)
              // }
            }

            // Compute reflections
            if (objects[nearestObjIndex].reflective.mag() != 0.0) {
              Color totalRefColor = Color();
              Ray refRay = Ray(geo.pos, geo.normal);
              Color refColor = Color();
              Color curMultiplier = objects[nearestObjIndex].reflective;
              for (int bounce = 0; bounce < REF_BOUNCES; bounce++) {
                float bestRefHit = std::numeric_limits<float>::infinity();
                float nearestRef = -1;
                for (int ro = 0; ro < objects.size(); ro++) {
                  float refHit = objects[ro].intersect(refRay);
                  if (refHit != -1.0 && refHit < bestRefHit) {
                    bestRefHit = refHit;
                    nearestRef = ro;
                  }
                }
                if (nearestRef != -1) {
                  objects[nearestRef].intersect(refRay, &geo);
                  for (int rl = 0; rl < lights.size(); rl++) {
                    Color tempRefColor = Color();
                    lights[rl]->generateLightRay(&geo, refRay, refColor);
                    tempRefColor = objects[nearestRef].ambient;
                    tempRefColor.mul(refColor);
                    tempRefColor.mul(curMultiplier);
                    totalRefColor.add(tempRefColor);

                    if (lights[rl]->type.compare("ambient") == 0) continue;
                    else {
                      // Check if refRay isn't obstructed
                      blocked = false;
                      for (int ro = 0; ro < objects.size(); ro++) {
                        if (objects[ro].intersect(refRay) != -1.0) {
                          blocked = true;
                          break;
                        }
                      }
                      if (!blocked) {
                        Vector refDirection = refRay.dir;
        
                        // Compute diffuse color
                        double refc2 = std::max(0.0f, (refDirection).dot(geo.normal));
                        tempRefColor = objects[nearestRef].diffuse;
                        tempRefColor.mul(refColor);
                        tempRefColor.scale(refc2);
                        tempRefColor.mul(curMultiplier);
                        totalRefColor.add(tempRefColor);
        
                        // Compute specular color
                        Vector refProjection = geo.normal;
                        refProjection.scale(refDirection.dot(geo.normal)*2.0);
                        Vector refReflection = refDirection;
                        refReflection.scale(-1);
                        refReflection.add(refProjection);
                        double c3 = pow(std::max(0.0f, -refReflection.dot(refRay.dir)), objects[nearestRef].specularPow);
                        tempRefColor = objects[nearestRef].specular;
                        tempRefColor.mul(refColor);
                        tempRefColor.scale(c3);
                        tempRefColor.mul(curMultiplier);
                        totalRefColor.add(tempRefColor);

                        // Computer next reflection
                        curMultiplier.mul(objects[nearestRef].reflective);
                      }
                    }

                  }
                } else {
                  break;
                }
              }
              pixColor.add(totalRefColor);
            }

            //pixColor.add(tempColor);

          }
        }
      } else {
        pixColor = Color(0,0,0);
      }
      pixColor.r = std::min(1.0f, std::max(0.0f, pixColor.r));
      pixColor.g = std::min(1.0f, std::max(0.0f, pixColor.g));
      pixColor.b = std::min(1.0f, std::max(0.0f, pixColor.b));
      setPixel(i, j, pixColor.r, pixColor.g, pixColor.b);
    }
  }

  std::cout <<"DONE" <<std::endl;

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
  sample(viewport.w / 2.0 , viewport.h / 2.0);

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
  // loadObjs();

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

  // initScene();							// quick function to set up scene
  loadScene();

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
