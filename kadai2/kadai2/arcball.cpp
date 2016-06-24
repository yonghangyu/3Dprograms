#include"arcball.h"
#include <math.h>
#include <iostream>





/**
* \ingroup GLVisualization
* Default constructor, it sets the ballRadius to 600
**/
Arcball::Arcball()
{
	this->ballRadius = 600;
	isRotating = false;
	width = height = 0;
	reset();
}

/**
* \ingroup GLVisualization
* Set width and height of the current windows, it's needed every time you resize the window
* \param w Width of the rendering window
* \param h Height of the rendering window
**/
void Arcball::setWidthHeight(int w, int h)
{
	width = w;
	height = h;
	ballRadius = std::min((int)(w / 2), (int)(h / 2));// inscribed to the screen 
}

/**
* \ingroup GLVisualization
* Set the radius of the ball (a typical radius for a 1024x768 window is 600
* \param newRadius The radius of the spherical dragging area
**/

void Arcball::setRadius(float newRadius)
{
	ballRadius = newRadius;
}

/**
* \ingroup GLVisualization
* Start the rotation. Use this method in association with the left click.
* Here you must give directly the coordinates of the mouse as the glut functions extract. This method supposes that the 0,0 is in the upper-left part of the screen
* \param _x Horizontal position of the mouse (0,0) = upperleft corner (w,h) = lower right
* \param _y Vertical position of the mouse (0,0) = upperleft corner (w,h) = lower right
*
**/

void Arcball::startRotation(int _x, int _y)
{
	glGetFloatv(GL_MODELVIEW_MATRIX,startMatrix);

	int sphere_x = _x - (width / 2);
	int sphere_y = (height / 2) - _y;

	startRotationVector=convertXY(sphere_x,sphere_y);
	startRotationVector.normalize();// convert to unit vector

	currentRotationVector = startRotationVector;
	isRotating = true;


   

}

/**
* \ingroup GLVisualization
* Update the rotation. Use this method in association with the drag event.
* Here you must give directly the coordinates of the mouse as the glut functions extract. This method supposes that the 0,0 is in the upper-left part of the screen
* \param _x Horizontal position of the mouse (0,0) = upperleft corner (w,h) = lower right
* \param _y Vertical position of the mouse (0,0) = upperleft corner (w,h) = lower right
**/
void Arcball::updateRotation(int _x, int _y)
{
	int sphere_x = _x - (width / 2);
	int sphere_y =( height / 2)-_y;

	currentRotationVector= convertXY(sphere_x, sphere_y);

	currentRotationVector.normalize();

	//std::cout << _x << ""<<_y ;
	
	glMatrixMode(GL_MODELVIEW);//’ñ‘O•Û‘¶—¹modelview matrix
	glLoadIdentity();
	applyRotationMatrix();//??XV‘l?ˆÊ’u  –çA¥Ž?ŽZo‘Š?˜°startvector “I?“IŠp“x ‘R@Ä‰æo?—¹“ß±Šp“x“I?Œ` ??A‰ÂˆÈ??ŠÅ“žù?“Iî™v

	
}

/**
* \ingroup GLVisualization
* Apply the computed rotation matrix
* This method must be invoked inside the \code glutDisplayFunc() \endcode
*
**/
void Arcball::applyRotationMatrix()
{
	glMultMatrixf(startMatrix);

	if (isRotating)
	{  // Do some rotation according to start and current rotation vectors
	   //cerr << currentRotationVector.transpose() << " " << startRotationVector.transpose() << endl;
		if ((currentRotationVector - startRotationVector).norm() > 1E-6)
		{
			Eigen::Vector3d rotationAxis = currentRotationVector.cross(startRotationVector);
			

			Eigen::Matrix3d currentMatrix;
			for (int i = 0; i < 3;i++) {
				for (int j = 0; j < 3;j++) {
					currentMatrix(i, j) = (double)startMatrix[4 * i + j];
				}
			}
			
			rotationAxis = currentMatrix*rotationAxis;// linear transformation
			
			rotationAxis.normalize();

			double val = currentRotationVector.dot(startRotationVector);
			val > (1 - 1E-10) ? val = 1.0 : val = val;
			double rotationAngle = acos(val) * 180.0f / (float)M_PI;//unit vector 

																	// rotate around the current position
			
			glRotatef(rotationAngle * 2, -rotationAxis.x(), -rotationAxis.y(), -rotationAxis.z());
			
		}
	}
	
}

/**
* \ingroup GLVisualization
* Stop the current rotation and prepare for a new click-then-drag event
*
**/
void Arcball::stopRotation()
{

	
	// set the current matrix as the permanent one
	isRotating = false;
}




/**
* \ingroup GLVisualization
* Maps the mouse coordinates to points on a sphere, if the points lie outside the sphere, the z is 0, otherwise is \f$ \sqrt(r^2 - (x^2+y^2) ) \f$ where \f$ x,y \f$
* are the window centric coordinates of the mouse
* \param x Mouse x coordinate
* \param y Mouse y coordinate
**/
Eigen:: Vector3d Arcball::convertXY(int x, int y)
{

	int d = x*x + y*y;
	float radiusSquared = ballRadius*ballRadius;
	if (d > radiusSquared)
	{
		return Eigen::Vector3d((float)x, (float)y, 0);
	}
	else
	{
		return Eigen:: Vector3d((float)x, (float)y, sqrt(radiusSquared - d));
	}
}

/**
* \ingroup GLVisualization
* Reset the current transformation to the identity
**/
void Arcball::reset()
{

	// reset matrix
	memset(startMatrix, 0, sizeof(startMatrix));
	startMatrix[0] = 1;
	startMatrix[1] = 0;
	startMatrix[2] = 0;
	startMatrix[3] = 0;
	startMatrix[4] = 0;
	startMatrix[5] = 1;
	startMatrix[6] = 0;
	startMatrix[7] = 0;
	startMatrix[8] = 0;
	startMatrix[9] = 0;
	startMatrix[10] = 1;
	startMatrix[11] = 0;
	startMatrix[12] = 0;
	startMatrix[13] = 0;
	startMatrix[14] = 0;
	startMatrix[15] = 1;

}




