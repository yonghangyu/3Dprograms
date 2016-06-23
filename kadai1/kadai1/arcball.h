#ifndef ARCBALL_H
#define ARCBALL_H




/*
* \class Arcball
* \ingroup GLVisualization
* \brief Arcball is a method to manipulate and rotate objects in 3D intuitively.
*
* The motivation behind the trackball (aka arcball) is to provide an intuitive user interface for complex 3D
* object rotation via a simple, virtual sphere - the screen space analogy to the familiar input device bearing
* the same name.
*
* The sphere is a good choice for a virtual trackball because it makes a good enclosure for most any object;
* and its surface is smooth and continuous, which is important in the generation of smooth rotations in response
* to smooth mouse movements.
* Any smooth, continuous shape, however, could be used, so long as points on its surface can be generated
*  in a consistent way.
* The algorithm for accomplishing this, needs to perform the following steps (not neseccarily in order).
*
*
*
*
* \b ArcBall Algorithm made easy
*  - Detect the left-button of the mouse being depressed.
*  - Keep track of the last known mouse position.
*  - Treat the mouse position as the projection of a point on the hemi-sphere down to the image plane (along the z-axis), and determine that point on the hemi-sphere.
*  - Detect the mouse movement
*  - Determine the great circle connecting the old mouse-hemi-sphere point to the current mouse-hemi-sphere point.
*  - Calculate the normal to this plane. This will be the axis about which to rotate.
*  - Set the OpenGL state to modify the MODELVIEW matrix.
*  - Read off the current matrix, since we want this operation to be the last transformation, not the first, and OpenGL does things LIFO.
*  - Reset the model-view matrix to the identity
*  - Rotate about the axis
*  - Multiply the resulting matrix by the saved matrix.
*  - Force a redraw of the scene.
*
* An example of the code for using successfully this class is the following:
* First set the radius of the arcball by using the appropriate method when you call the handle to the GLUT resize method (this is usually accomplished with a
* \code
* void handleResize(int w, int h)
* \endcode
* function in GLUT (if you are using Qt or other, please refer to the relative API ).
* An example of a \code handleResize(int w, int h) \endcode function is the following:
* \code
* void handleResize(int w, int h)
* {
*   arcball.setWidthHeight(w, h);
*   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
*   glViewport(0, 0, w, h);
*
*   glMatrixMode(GL_PROJECTION);
*   glLoadIdentity();
*   gluPerspective(viewAngle, (float)w / (float)w, zNear, zFar );
*
*   glMatrixMode(GL_MODELVIEW);
* }
* \endcode
*
*
These are the two functions (GLUT) that control the mouse events:
* \code
* void mouseFunc(int state, int button, int _x , int _y)
* {
*   if ( button == GLUT_LEFT_BUTTON )
*       arcball.startRotation(_x,_y);
*   else
*       arcball.stopRotation();
*
*
*   glutPostRedisplay();
* }
*
* void mouseDrag(int _x, int _y)
* {
*
*   arcball.updateRotation(_x,_y);
*   glutPostRedisplay();
* }
*
* \endcode
*
*/

#include <iostream>
#include <Eigen/core>
#include <Eigen/Geometry>
#include <GLFW/glfw3.h>

#define M_PI 3.1415926;
typedef float GLMatrix[16];


class Arcball
{
private:
	// window size 
	int width, height;

	// get the current modelview matrix
	GLMatrix startMatrix;
	
	Eigen::Vector3d startRotationVector;
	Eigen::Vector3d currentRotationVector;
	
	bool isRotating;
	float ballRadius;
	
	
	Eigen::Vector3d convertXY(int x, int y);//mapping the screen coordinate to sphere coordinates
	
public:
	Arcball();//default constructor func

	void setWidthHeight(int w, int h);//compute the ball radius interms of window width and height 
	
	//object rotation 
	void startRotation(int x, int y);
	void updateRotation(int x, int y);
	void stopRotation();


	// rotation transformation 
	void applyRotationMatrix();

	//set trackball radius
	void setRadius(float newRadius);
	
	//reset the startmatrix to identity matrix
	void reset();
};
#endif