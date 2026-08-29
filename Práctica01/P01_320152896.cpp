#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glew.h>
#include <glfw3.h>

//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 800;

GLuint VAO, VBO, shader; // variables propias de OpenGL de tipo uint -> unsigned int
// VAO
// VBO
// shader

//LENGUAJE DE SHADER (SOMBRAS) GLSL

//Vertex Shader
//recibir color, salida Vcolor                      in vc3 almacena 3 datos (se hace el cast a float)
static const char* vShader = " \n\
	#version 330 \n\
	layout (location =0) in vec3 pos; \n\
	void main() \n\
	{ \n\
	gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); \n\
	}";
// gl_Position convierte los valores en coordenadas espaciales (lo compila el GPU)

//Fragment Shader
//recibir Vcolor y dar de salida color
static const char* fShader = " \n\
	#version 330 \n\
	out vec4 color; \n\
	void main() \n\
	{ \n\
	color = vec4(0.0f,0.0f,0.0f,1.0f); \n\
	}";
// color de 4 tipos diferentes: plano, por variable uniform, de una textura, por medio de iluminación


void CrearTriangulo()
{

	GLfloat vertices[] = {
		-0.7f, 0.4f, 0.0f,
		-0.5f, 0.4f, 0.0f,
		-0.5f, 0.3f, 0.0f,

		-0.7f, 0.4f, 0.0f,
		-0.5f, 0.3f, 0.0f,
		-0.7f, 0.3f, 0.0f,

		-0.8f, 0.4f, 0.0f,
		-0.7f, 0.4f, 0.0f,
		-0.8f, -0.3f, 0.0f,

		-0.8f, -0.3f, 0.0f,
		-0.7f, 0.4f, 0.0f,
		-0.7f, -0.3f, 0.0f,

		-0.5f, 0.4f, 0.0f,
		-0.4f, 0.4f, 0.0f,
		-0.5f, -0.3f, 0.0f,

		-0.5f, -0.3f, 0.0f,
		-0.4f, 0.4f, 0.0f,
		-0.4f, -0.3f, 0.0f,

		-0.7f, 0.1f, 0.0f,
		-0.5f, 0.1f, 0.0f,
		-0.5f, 0.0f, 0.0f,

		-0.7f, 0.1f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		-0.7f, 0.0f, 0.0f,

		-0.3f, 0.4f, 0.0f,
		-0.2f, 0.4f, 0.0f,
		-0.3f, -0.3f, 0.0f,

		-0.3f, -0.3f, 0.0f,
		-0.2f, 0.4f, 0.0f,
		-0.2f, -0.3f, 0.0f,

		0.3f, 0.4f, 0.0f,
		0.2f, 0.4f, 0.0f,
		0.3f, -0.3f, 0.0f,

		0.3f, -0.3f, 0.0f,
		0.2f, 0.4f, 0.0f,
		0.2f, -0.3f, 0.0f,

		-0.2f, 0.4f, 0.0f,
		-0.2f, 0.25f, 0.0f,
		0.0f, 0.2f, 0.0f,

		-0.2f, 0.25f, 0.0f,
		0.0f, 0.2f, 0.0f,
		0.0f, 0.05f, 0.0f,

		0.0f, 0.05f, 0.0f,
		0.0f, 0.2f, 0.0f,
		0.2f, 0.25f, 0.0f,

		0.0f, 0.2f, 0.0f,
		0.2f, 0.25f, 0.0f,
		0.2f, 0.4f, 0.0f,

		0.4f, 0.4f, 0.0f,
		0.4f, 0.3f, 0.0f,
		0.8f, 0.3f, 0.0f,

		0.4f, 0.4f, 0.0f,
		0.8f, 0.4f, 0.0f,
		0.8f, 0.3f, 0.0f,

		0.4f, -0.2f, 0.0f,
		0.4f, -0.3f, 0.0f,
		0.8f, -0.3f, 0.0f,

		0.4f, -0.2f, 0.0f,
		0.8f, -0.2f, 0.0f,
		0.8f, -0.3f, 0.0f,

		0.4f, -0.2f, 0.0f,
		0.5f, -0.2f, 0.0f,
		0.7f, 0.3f, 0.0f,

		0.7f, 0.3f, 0.0f,
		0.8f, 0.3f, 0.0f,
		0.5f, -0.2f, 0.0f,
	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO (Vertex Array Object)
	glBindVertexArray(VAO);//asignar VAO para enviarlo al GPU

	glGenBuffers(1, &VBO); // VBO (Vertex Buffer Object)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores
	//GL_STATIC_DRAW -> arreglo de datos estáticos (no se modifican en tiempo de ejecución)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride (tipo offset) en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
	// Almacena 3 datos, no los normaliza y quiero que guarde 3 datos de un cierto número de datos
	glEnableVertexAttribArray(0); // envía los datos al vertex shader, convierte los 9 flotantes en 3 vec3 y se convierten en 3 puntos en el espacio
	//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
// No le metemos mano a AddShader y CompileShader, nos apegamos a la documentación

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica

//the Program recibe los datos de theShader

{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader, 1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n", shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

// CompileShader crea el programa
void CompileShaders() {
	shader = glCreateProgram(); //se crea un programa
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica y valida que el paquete recibido sea el adecuado (versión de sahder compatible)
	//verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}



}
int main()
{
	//Definimos las variables para que el color del fondo se modifique aleatoreamente
	srand(time(NULL));
	float rojo = (float)rand() / (float)RAND_MAX;
	float verde = (float)rand() / (float)RAND_MAX;
	float azul = (float)rand() / (float)RAND_MAX;
	double ultimoCambio = 0.0f;

	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}

	//****  LAS SIGUIENTES 4 LÍNEAS SE COMENTAN EN DADO CASO DE QUE AL USUARIO NO LE FUNCIONE LA VENTANA Y PUEDA CONOCER LA VERSIÓN DE OPENGL QUE TIENE ****/

	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Llamada a las funciones creadas antes del main
	CrearTriangulo();
	CompileShaders();

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		double tiempoActual = glfwGetTime();

		if (tiempoActual - ultimoCambio >= 2.0)
		{	
			rojo = (float)rand() / (float)RAND_MAX;
			verde = (float)rand() / (float)RAND_MAX;
			azul = (float)rand() / (float)RAND_MAX;
			ultimoCambio = tiempoActual;
		}

		//Imprime en la terminal los valores de las variables de color para ver la 'aleatoriedad' 
		printf("Valores random: Rojo -> %.3f | Verde -> %.3f | Azul -> %.3f\n",rojo, verde, azul);

		glClearColor(rojo, verde, azul, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		// Para dibujar más de 1 triangulo, hay que ir sumando de 3 en 3
		// por lo tanto puse 66 en el argumento, para asó dibujar 22 triángulos
		glDrawArrays(GL_TRIANGLES, 0, 66); 
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);

		//NO ESCRIBIR NINGUNA LÍNEA DESPUÉS DE glfwSwapBuffers(mainWindow);
	}


	return 0;
}