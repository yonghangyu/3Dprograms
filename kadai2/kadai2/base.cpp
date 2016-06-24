#include <iostream>
#define GLFW_INCLUDE_GLU
#include<GLFW/glfw3.h>
#include "variable.h"
#include"arcball.h"
#include "saveimage.h"
#include"drawScene.h"

// ----------------------------------------------------------------------------

// the width and the height of the rendering window
int width = 1024;
int height = 768;
static int type3;
int isFlatOrSmooth = 0;//1 is flat,0 is smooth ,flat is default
int ColorCodingType = 1;// extern variable should also be defined in some cpp file  with initialization.
Arcball arcball;
drawScene draw;//initial class object using default constructor 

			   //////////////////////////////////////////////////////////////////////////////////// callback func()

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xd, yd;


	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			isleftKeyPressed = true;
			glfwGetCursorPos(window, &xd, &yd);
			arcball.startRotation(xd, yd);
		}
		else if (action == GLFW_RELEASE) {
			isleftKeyPressed = false;
			arcball.stopRotation();
		}

	}




}

static void cursorpos_callback(GLFWwindow* window, double xd, double yd)
{
	if (isleftKeyPressed) {
		arcball.updateRotation(xd, yd);
	}

}



static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//capture the screen using save image func
	if ((key == GLFW_KEY_P) && (action == GLFW_PRESS)) {

		saveimage();
	}
	//control the switch between 5 objects with any format(line, point ,and faces)

	if ((key == GLFW_KEY_0) && (action == GLFW_PRESS)) {

		if (isTetrahegon) {
			isTetrahegon = false;
			isCube = true;
		}
		else if (isCube) {
			isCube = false;
			isOctahegron = true;
		}
		else if (isOctahegron) {
			isOctahegron = false;
			isIcosahedron = true;
		}
		else if (isIcosahedron) {
			isIcosahedron = false;
			isDodecahedron = true;
		}
		else if (isDodecahedron) {
			isDodecahedron = false;
			isTetrahegon = true;
		}



	}

	if ((key == GLFW_KEY_1) && (action == GLFW_PRESS)) {

		type3 = key - 48;
	}
	else if ((key == GLFW_KEY_2) && (action == GLFW_PRESS))
	{
		type3 = key - 48;
	}
	else if ((key == GLFW_KEY_3) && (action == GLFW_PRESS))
	{
		type3 = key - 48;
	}

	if ((key == GLFW_KEY_S) && (action == GLFW_PRESS)) {
		
		isFlatOrSmooth = 0;
	}
	else if ((key == GLFW_KEY_F) && (action == GLFW_PRESS)) {
	
		isFlatOrSmooth = 1;
	}

	// colorcodingtype key event control

	if ((key == GLFW_KEY_V) && (action == GLFW_PRESS)) {
		ColorCodingType = 1;
	}
	else if ((key == GLFW_KEY_G) && (action == GLFW_PRESS)) {
		ColorCodingType = 2;
	}
	else if ((key g== GLFW_KEY_M) && (action == GLFW_PRESS)) {
		ColorCodingType = 3;
	}
	


}


static void reshape(GLFWwindow* window)
{
	int _width;
	int _height;
	//glfwGetFramebufferSize(window, &_width, &_height);
	//glViewport(0, 0, _width, _height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30.0, (double)width / height, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3, 3, 5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


////////////////////////////////////////////////////////////////////////////////////

void DisPlay(int type) {

	if (isTetrahegon) {

		draw.drawTetrahegon(type);
	}

	if (isCube) {

		draw.drawCube(type);
	}
	if (isOctahegron)
	{
		draw.drawOctahegron(type);
	}
	if (isIcosahedron)
	{
		draw.drawIcosahedron(type);
	}
	if (isDodecahedron)
	{
		draw.drawDodecahedron(type);
	}


}


void switchShading_Line_Point() {

	if (isShading) {
		draw.drawTetrahegon(1);
		std::cout << 1;
	}
	else if (isLine) {
		draw.drawTetrahegon(2);
		std::cout << 2;
	}
	else if (isPoint) {
		draw.drawTetrahegon(3);
		std::cout << 3;
	}




}

void test(int isObjectType) {

	switch (isObjectType) {
	case 1: {
		if (type3 == 1) { draw.drawTetrahegon(1); }
		else if (type3 == 2) { draw.drawTetrahegon(2); }
		else if (type3 == 3) { draw.drawTetrahegon(3); }
	}
			break;
	case 2: {
		if (type3 == 1) { draw.drawCube(1); }
		else if (type3 == 2) { draw.drawCube(2); }
		else if (type3 == 3) { draw.drawCube(3); }
	}
			break;
	case 3: {
		if (type3 == 1) { draw.drawOctahegron(1); }
		else if (type3 == 2) { draw.drawOctahegron(2); }
		else if (type3 == 3) { draw.drawOctahegron(3); }
	}
			break;
	case 4: {
		if (type3 == 1) { draw.drawDodecahedron(1); }
		else if (type3 == 2) { draw.drawDodecahedron(2); }
		else if (type3 == 3) { draw.drawDodecahedron(3); }
	}
			break;
	case 5: {

		if (type3 == 1) { draw.drawIcosahedron(1); }
		else if (type3 == 2) { draw.drawIcosahedron(2); }
		else if (type3 == 3) { draw.drawIcosahedron(3); }

	}
			break;
	default: std::cout << "no type matched";
		break;



	}




}

void ShadingModel() {

	if (isFlatOrSmooth == 1) {
		glShadeModel(GL_FLAT);
		std::cout << "flat" << std::endl;
	}
	else if (isFlatOrSmooth == 0) {
		glShadeModel(GL_SMOOTH);
		std::cout << "smooth" << std::endl;
	}
}

void initGL() {

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);


}

void initShading() {


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	
	
	if (isLightOn == 0) {

		glDisable(GL_LIGHTING);
	}

	GLfloat lightPos0[] = { 10.0f, 10.0f, 10.0f, 1.0f }; //Positioned at (5, 5, 0)
	float lightambient[] = { 0.2,0.6,0.6,1.0 };
	float lightdiffuse[] = { 0.8,0.5,0.2,1 };
	float lightspecualr[] = { 0.9,0.9,0.9,1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightspecualr);





}

// main func()
int main(void)
{



	GLFWwindow* window;


	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello Yu Yonghang ", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	initGL();

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mousebutton_callback);
	glfwSetCursorPosCallback(window, cursorpos_callback);
	reshape(window);//set the eye position 

	arcball.setWidthHeight(width, height);

	//glRotatef(20, 0.0f, 1.0f, 0.0f);



	std::cout << "input whether you want to swith shading ,line and point type(0 is no,1 is yes):";
	std::cin >> isShading_Line_point;
	if (isShading_Line_point) {
		std::cout << "input the object you want to display among three types:";
		std::cin >> isObjectType;
	}
	else if (!isShading_Line_point) {

		std::cout << "input the type you want to switch:";
		std::cin >> typeSwitch;
	}

	std::cout << "input the number to control if the light on ,0 is disabled, 1 is enabled";
	std::cin >> isLightOn;

	initShading();


	/* Loop until the user closes the window */
	
	while (!glfwWindowShouldClose(window))
	{
		//reshape(window);

		//ShadingModel();
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (isShading_Line_point == 1) {

			test(isObjectType);

		}

		else if (isShading_Line_point == 0) {

			if (typeSwitch == 1) {
				DisPlay(1);
			}
			else if (typeSwitch == 2) {
				DisPlay(2);
			}
			else if (typeSwitch) {
				DisPlay(3);
			}

		}
		
		


		/* Render here */

		//capture(window);

		glfwSwapBuffers(window);




		/* Swap front and back buffers */


		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

