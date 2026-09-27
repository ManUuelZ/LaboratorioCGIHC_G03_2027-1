#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	rotax = 0.0f;
	rotay = 0.0f;
	rotaz = 0.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	articulacion7 = 0.0f;
	articulacion8 = 0.0f;
	articulacion9 = 0.0f;
	articulacion10 = 0.0f;
	articulacion11 = 0.0f;
	articulacion12 = 0.0f;
	articulacion13 = 0.0f;
	articulacion14 = 0.0f;
	articulacion15 = 0.0f;
	articulacion16 = 0.0f;
	articulacion17 = 0.0f;
	articulacion18 = 0.0f;
	articulacion19 = 0.0f;
	articulacion20 = 0.0f;
	articulacion21 = 0.0f;
	articulacion22 = 0.0f;
	articulacion23 = 0.0f;
	articulacion24 = 0.0f;
	articulacion25 = 0.0f;
	articulacion26 = 0.0f;
	articulacion27 = 0.0f;
	articulacion28 = 0.0f;
	articulacion29 = 0.0f;
	articulacion30 = 0.0f;

	limiteMaximo = true;

	
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 05: Optimización y Carga de Modelos", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	
	/*if (key == GLFW_KEY_E) 
	{
		theWindow->rotax += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->rotay += 10.0; //rotar sobre el eje y 10 grados
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->rotaz += 10.0;
	}
	*/ 
	if (key == GLFW_KEY_F)
	{
		theWindow->articulacion1 += 10.0;
	}

	if (key == GLFW_KEY_G)
	{
		theWindow->articulacion2 += 10.0;
	}
	if (key == GLFW_KEY_H)
	{
		theWindow->articulacion3 += 10.0;
	}
	if (key == GLFW_KEY_J)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion4 += 5.0f;

			if (theWindow->articulacion4 >= 45.0f)
			{
				theWindow->articulacion4 = 45.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion4 -= 5.0f;

			if (theWindow->articulacion4 <= -45.0f)
			{
				theWindow->articulacion4 = -45.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_K)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion5 += 5.0f;

			if (theWindow->articulacion5 >= 45.0f)
			{
				theWindow->articulacion5 = 45.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion5 -= 5.0f;

			if (theWindow->articulacion5 <= -45.0f)
			{
				theWindow->articulacion5 = -45.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_L)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion6 += 5.0f;

			if (theWindow->articulacion6 >= 45.0f)
			{
				theWindow->articulacion6 = 45.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion6 -= 5.0f;

			if (theWindow->articulacion6 <= -45.0f)
			{
				theWindow->articulacion6 = -45.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_P)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion7 += 5.0f;

			if (theWindow->articulacion7 >= 45.0f)
			{
				theWindow->articulacion7 = 45.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion7 -= 5.0f;

			if (theWindow->articulacion7 <= -45.0f)
			{
				theWindow->articulacion7 = -45.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_O)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion8 += 5.0f;

			if (theWindow->articulacion8 >= 45.0f)
			{
				theWindow->articulacion8 = 45.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion8 -= 5.0f;

			if (theWindow->articulacion8 <= -45.0f)
			{
				theWindow->articulacion8 = -45.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_I)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion9 += 5.0f;

			if (theWindow->articulacion9 >= 45.0f)
			{
				theWindow->articulacion9 = 45.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion9 -= 5.0f;

			if (theWindow->articulacion9 <= -45.0f)
			{
				theWindow->articulacion9 = -45.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->articulacion10 += 10.0;
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow->articulacion11 += 10.0;
	}
	if (key == GLFW_KEY_T)
	{
		theWindow->articulacion12 += 10.0;
	}
	if (key == GLFW_KEY_R)
	{
		theWindow->articulacion13 += 10.0;
	}
	if (key == GLFW_KEY_E)
	{
		theWindow->articulacion14 += 10.0;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->articulacion15 += 10.0;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->articulacion16 += 10.0;
	}
	if (key == GLFW_KEY_C)
	{
		theWindow->articulacion17 += 10.0;
	}
	if (key == GLFW_KEY_V)
	{
		theWindow->articulacion18 += 10.0;
	}
	if (key == GLFW_KEY_B)
	{
		theWindow->articulacion19 += 10.0;
	}
	if (key == GLFW_KEY_N)
	{
		theWindow->articulacion20 += 10.0;
	}
	if (key == GLFW_KEY_M)
	{
		theWindow->articulacion21 += 10.0;
	}
	if (key == GLFW_KEY_1)
	{
		theWindow->articulacion22 += 10.0;
	}
	if (key == GLFW_KEY_2)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion23 += 5.0f;

			if (theWindow->articulacion23 >= 85.0f)
			{
				theWindow->articulacion23 = 85.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion23 -= 5.0f;

			if (theWindow->articulacion23 <= -85.0f)
			{
				theWindow->articulacion23 = -85.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_3)
	{
		theWindow->articulacion24 += 10.0;
	}
	if (key == GLFW_KEY_4)
	{
		if (theWindow->limiteMaximo)
		{
			theWindow->articulacion25 += 5.0f;

			if (theWindow->articulacion25 >= 85.0f)
			{
				theWindow->articulacion25 = 85.0f;
				theWindow->limiteMaximo = false;
			}
		}
		else
		{
			theWindow->articulacion25 -= 5.0f;

			if (theWindow->articulacion25 <= -85.0f)
			{
				theWindow->articulacion25 = -85.0f;
				theWindow->limiteMaximo = true;
			}
		}
	}
	if (key == GLFW_KEY_5)
	{
		theWindow->articulacion26 += 10.0;
	}
	if (key == GLFW_KEY_6)
	{
		theWindow->articulacion27 += 10.0;
	}
	if (key == GLFW_KEY_7)
	{
		theWindow->articulacion28 += 10.0;
	}
	if (key == GLFW_KEY_8)
	{
		theWindow->articulacion29 += 10.0;
	}
	if (key == GLFW_KEY_9)
	{
		theWindow->articulacion30 += 10.0;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		//printf("se presiono la tecla: %s\n",key_name);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
