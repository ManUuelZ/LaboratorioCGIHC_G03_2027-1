#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getarticulacion7() { return articulacion7; }
	GLfloat getarticulacion8() { return articulacion8; }
	GLfloat getarticulacion9() { return articulacion9; }
	GLfloat getarticulacion10() { return articulacion10; }
	GLfloat getarticulacion11() { return articulacion11; }
	GLfloat getarticulacion12() { return articulacion12; }
	GLfloat getarticulacion13() { return articulacion13; }
	GLfloat getarticulacion14() { return articulacion14; }
	GLfloat getarticulacion15() { return articulacion15; }
	GLfloat getarticulacion16() { return articulacion16; }
	GLfloat getarticulacion17() { return articulacion17; }
	GLfloat getarticulacion18() { return articulacion18; }
	GLfloat getarticulacion19() { return articulacion19; }
	GLfloat getarticulacion20() { return articulacion20; }
	GLfloat getarticulacion21() { return articulacion21; }
	GLfloat getarticulacion22() { return articulacion22; }
	GLfloat getarticulacion23() { return articulacion23; }
	GLfloat getarticulacion24() { return articulacion24; }
	GLfloat getarticulacion25() { return articulacion25; }
	GLfloat getarticulacion26() { return articulacion26; }
	GLfloat getarticulacion27() { return articulacion27; }
	GLfloat getarticulacion28() { return articulacion28; }
	GLfloat getarticulacion29() { return articulacion29; }
	GLfloat getarticulacion30() { return articulacion30; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, 
			articulacion4, articulacion5, articulacion6, articulacion7,
			articulacion8, articulacion9, articulacion10, articulacion11,
			articulacion12, articulacion13, articulacion14, articulacion15,
			articulacion16, articulacion17, articulacion18, articulacion19,
			articulacion20, articulacion21, articulacion22, articulacion23,
			articulacion24, articulacion25, articulacion26, articulacion27,
			articulacion28, articulacion29, articulacion30;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

