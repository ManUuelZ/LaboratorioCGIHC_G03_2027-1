//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";

// Vertex Shaders de colores 
static const char* vShaderRojo = "shaders/shaderrojo.vert";
static const char* vShaderVerde = "shaders/shaderverde.vert";
static const char* vShaderAzul = "shaders/shaderazul.vert";
static const char* vShaderAmarillo = "shaders/shaderamarillo.vert";
static const char* vShaderCafe = "shaders/shadercafe.vert";
static const char* vShaderMorado = "shaders/shadermorado.vert";
static const char* vShaderNegro = "shaders/shadernegro.vert";


//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* piramidetriangular = new Mesh();
	piramidetriangular->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(piramidetriangular);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

//función para crear pirámide cuadrangular unitaria
void CrearPiramideCuadrangular()
{
	unsigned int piramidecuadrangular_indices[] = {
		0,3,4,
		3,2,4,
		2,1,4,
		1,0,4,
		0,1,2,
		0,2,4

	};
	GLfloat piramidecuadrangular_vertices[] = {
		0.5f,-0.5f,0.5f,
		0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		-0.5f,-0.5f,0.5f,
		0.0f,0.5f,0.0f,
	};
	Mesh* piramide = new Mesh();
	piramide->CreateMesh(piramidecuadrangular_vertices, piramidecuadrangular_indices, 15, 18);
	meshList.push_back(piramide);
}

void CrearLetrasyFiguras()
{
	// Triángulo verde
	GLfloat vertices_trianguloVerde[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.1f,	0.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.1f,	0.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.1f,	0.0f,

	};

	MeshColor* trianguloVerde = new MeshColor();
	trianguloVerde->CreateMeshColor(vertices_trianguloVerde, 18);
	meshColorList.push_back(trianguloVerde); // índice 0

	// Triángulo azul
	GLfloat vertices_trianguloAzul[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,			0.0f,	0.0f,	1.0f,
	};

	MeshColor* trianguloAzul = new MeshColor();
	trianguloAzul->CreateMeshColor(vertices_trianguloAzul, 18);
	meshColorList.push_back(trianguloAzul); // índice 1

	// Cuadrado verde
	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde); // índice 2

	// Cuadrado rojo
	GLfloat vertices_cuadradoRojo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			1.0f,	0.0f,	0.0f,

	};

	MeshColor* cuadradoRojo = new MeshColor();
	cuadradoRojo->CreateMeshColor(vertices_cuadradoRojo, 36);
	meshColorList.push_back(cuadradoRojo); // índice 3

	// Cuadrado café
	GLfloat vertices_cuadradoCafe[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	-0.5f,		0.5f,			0.478, 0.255, 0.067,
		0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,
		-0.5f,	0.5f,		0.5f,			0.478, 0.255, 0.067,

	};

	MeshColor* cuadradoCafe = new MeshColor();
	cuadradoCafe->CreateMeshColor(vertices_cuadradoCafe, 36);
	meshColorList.push_back(cuadradoCafe); // índice 4

	// Triángulo amarillo
	GLfloat vertices_trianguloAmarillo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	1.0f,	0.0f,
	};

	MeshColor* trianguloAmarillo = new MeshColor();
	trianguloAmarillo->CreateMeshColor(vertices_trianguloAmarillo, 18);
	meshColorList.push_back(trianguloAmarillo); // índice 5

	// Triángulo morado
	GLfloat vertices_trianguloMorado[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			0.5f,	0.0f,	1.0f,
		1.0f,	-1.0f,		0.5f,			0.5f,	0.0f,	1.0f,
		0.0f,	1.0f,		0.5f,		    0.5f,	0.0f,	1.0f,
	};

	MeshColor* trianguloMorado = new MeshColor();
	trianguloMorado->CreateMeshColor(vertices_trianguloMorado, 18);
	meshColorList.push_back(trianguloMorado); // índice 6

	// Triángulo rojo
	GLfloat vertices_trianguloRojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
	};

	MeshColor* trianguloRojo = new MeshColor();
	trianguloRojo->CreateMeshColor(vertices_trianguloRojo, 18);
	meshColorList.push_back(trianguloRojo); // índice 7

	GLfloat vertices_cuadradoAzul[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	0.0f,	1.0f,

	};

	MeshColor* cuadradoAzul = new MeshColor();
	cuadradoAzul->CreateMeshColor(vertices_cuadradoAzul, 36);
	meshColorList.push_back(cuadradoAzul); // índice 8

	GLfloat vertices_cuadradoAmarillo[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,		1.0f,	1.0f,	0.0f,
		 0.5f,	-0.5f,		0.5f,		1.0f,	1.0f,	0.0f,
		 0.5f,	 0.5f,		0.5f,		1.0f,	1.0f,	0.0f,

		-0.5f,	-0.5f,		0.5f,		1.0f,	1.0f,	0.0f,
		 0.5f,	 0.5f,		0.5f,		1.0f,	1.0f,	0.0f,
		-0.5f,	 0.5f,		0.5f,		1.0f,	1.0f,	0.0f,
	};

	MeshColor* cuadradoAmarillo = new MeshColor();
	cuadradoAmarillo->CreateMeshColor(vertices_cuadradoAmarillo, 36);
	meshColorList.push_back(cuadradoAmarillo); // índice 9

	GLfloat vertices_cuadradoMorado[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,		0.5f,	0.0f,	1.0f,
		 0.5f,	-0.5f,		0.5f,		0.5f,	0.0f,	1.0f,
		 0.5f,	 0.5f,		0.5f,		0.5f,	0.0f,	1.0f,

		-0.5f,	-0.5f,		0.5f,		0.5f,	0.0f,	1.0f,
		 0.5f,	 0.5f,		0.5f,		0.5f,	0.0f,	1.0f,
		-0.5f,	 0.5f,		0.5f,		0.5f,	0.0f,	1.0f,
	};

	MeshColor* cuadradoMorado = new MeshColor();
	cuadradoMorado->CreateMeshColor(vertices_cuadradoMorado, 36);
	meshColorList.push_back(cuadradoMorado); // índice 10

	GLfloat vertices_cuadradoNegro[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		 0.5f,	-0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		 0.5f,	 0.5f,		0.5f,		0.0f,	0.0f,	0.0f,

		-0.5f,	-0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		 0.5f,	 0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
		-0.5f,	 0.5f,		0.5f,		0.0f,	0.0f,	0.0f,
	};

	MeshColor* cuadradoNegro = new MeshColor();
	cuadradoNegro->CreateMeshColor(vertices_cuadradoNegro, 36);
	meshColorList.push_back(cuadradoNegro); // índice 11
}


void CreateShaders()
{
	// Shader rojo
	Shader* shaderRojo = new Shader();
	shaderRojo->CreateFromFiles(vShaderRojo, fShaderColor);
	shaderList.push_back(*shaderRojo); // índice 0

	// Shader verde 
	Shader* shaderVerde = new Shader(); 
	shaderVerde->CreateFromFiles(vShaderVerde, fShaderColor); 
	shaderList.push_back(*shaderVerde); // índice 1 
	 
	// Shader azul 
	Shader* shaderAzul = new Shader();  
	shaderAzul->CreateFromFiles(vShaderAzul, fShaderColor); 
	shaderList.push_back(*shaderAzul); // índice 2 
	 
	// Shader amarillo 
	Shader* shaderAmarillo = new Shader(); 
	shaderAmarillo->CreateFromFiles(vShaderAmarillo, fShaderColor); 
	shaderList.push_back(*shaderAmarillo); // índice 3 
	 
	// Shader café 
	Shader* shaderCafe = new Shader(); 
	shaderCafe->CreateFromFiles(vShaderCafe, fShaderColor); 
	shaderList.push_back(*shaderCafe); // índice 4 
	 
	// Shader morado 
	Shader* shaderMorado = new Shader(); 
	shaderMorado->CreateFromFiles(vShaderMorado, fShaderColor); 
	shaderList.push_back(*shaderMorado); // índice 5 
	 
	// Shader negro 
	Shader* shaderNegro = new Shader(); 
	shaderNegro->CreateFromFiles(vShaderNegro, fShaderColor); 
	shaderList.push_back(*shaderNegro); // índice 6 
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList 
	CrearCubo();//índice 1 en MeshList 
	CrearPiramideCuadrangular();//indice 2 en MeshList 
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;

	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.5f, 0.5f, 1.0f, 1.0f); // Color de fondo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
		//
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//meshColorList[0]->RenderMeshColor();
		//meshColorList[6]->RenderMeshColor();

		/*// Primer letra (A) 

		// Cuadrado rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.8f, -0.05f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.80f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		// Cuadrado rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.5f, -0.05f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.80f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		// Cuadrado rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.4f, -4.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.13f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		// Cuadrado rojo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.65f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.13f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[3]->RenderMeshColor();

		/// Segunda letra (M)

		// Cuadrado verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.20f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.9f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		// Cuadrado verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.20f, 0.0f, -4.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.9f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		// Cuadrado verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.075f, 0.31f, -4.0f));
		model = glm::rotate(model,
			glm::radians(45.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		// Cuadrado verde
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.075f, 0.31f, -4.0f));
		model = glm::rotate(model,
			glm::radians(135.0f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.3f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[2]->RenderMeshColor();

		// Tercera letra (Z)

		// Cuadrado azul
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, -0.39f, -4.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.12f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[8]->RenderMeshColor();

		// Cuadrado azul
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, 0.0f, -4.0f));
		model = glm::rotate(model,
			glm::radians(64.5f),
			glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.79f, 0.12f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[8]->RenderMeshColor();

		// Cuadrado azul
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.65f, 0.39f, -4.0f));
		model = glm::scale(model, glm::vec3(0.45f, 0.12f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshColorList[8]->RenderMeshColor();*/


		//Para el cubo y la pirámide se usa el primer set de shaders con índice 0 en ShaderList
		shaderList[3].useShader(); // Color de la figura rojo -> 0, verde -> 1, azul -> 2, amarillo -> 3, café -> 4, morado -> 5, negro -> 6
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;

		/* Pruebas para ver la rotación de ls figuras y la correcta asignación de los índices de shaders y figuras
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		*/

		// Figura 1 
		// Barra café  
		shaderList[4].useShader();  
		uniformModel = shaderList[4].getModelLocation(); 
		uniformProjection = shaderList[4].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model,glm::vec3(-1.8f, 0.13f, -3.0f)); 
		model = glm::scale(model,glm::vec3(0.1f, 1.8f, 0.3f));
		glUniformMatrix4fv(uniformModel,1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection,1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[1]->RenderMesh();  
		 
		// Barra café  
		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation(); 
		uniformProjection = shaderList[4].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-1.2f, 0.13f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.1f, 1.8f, 0.3f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[1]->RenderMesh(); 
		 
		// Pirámide amarilla 
		shaderList[3].useShader(); 
		uniformModel = shaderList[3].getModelLocation(); 
		uniformProjection = shaderList[3].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-1.5f, 0.5f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.3f)); 
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[0]->RenderMesh(); 
		 
		// Pirámide roja 
		shaderList[0].useShader(); 
		uniformModel = shaderList[0].getModelLocation(); 
		uniformProjection = shaderList[0].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.3f)); 
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[0]->RenderMesh(); 
		 
		// Pirámide verde 
		shaderList[1].useShader(); 
		uniformModel = shaderList[1].getModelLocation(); 
		uniformProjection = shaderList[1].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-1.5f, -0.5f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.3f)); 
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[0]->RenderMesh(); 

		 
		// Figura 2 
		// Cubo café (rombo) 
		shaderList[4].useShader(); 
		uniformModel = shaderList[4].getModelLocation(); 
		uniformProjection = shaderList[4].getProjectLocation(); 	 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-0.18f, -0.24f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 45 * toRadians,glm::vec3(0.0f, 0.0f, 1.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[1]->RenderMesh(); 

		// Cubo azul (rombo)
		shaderList[2].useShader(); 
		uniformModel = shaderList[2].getModelLocation(); 
		uniformProjection = shaderList[2].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-0.18f, -0.24f, -3.0f));
		model = glm::scale(model, glm::vec3(0.64f, 0.64f, 0.3f));
		model = glm::rotate(model, 45 * toRadians,glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[1]->RenderMesh();  
		 
		// Pirámide roja 
		shaderList[0].useShader(); 
		uniformModel = shaderList[0].getModelLocation(); 
		uniformProjection = shaderList[0].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(0.05f, -0.02f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.3f)); 
		model = glm::rotate(model, 90 * toRadians,glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[2]->RenderMesh();
		 
		// Pirámide verde 
		shaderList[1].useShader(); 
		uniformModel = shaderList[1].getModelLocation(); 
		uniformProjection = shaderList[1].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(0.05f, -0.52f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.3f)); 
		model = glm::rotate(model, 90 * toRadians,glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[2]->RenderMesh(); 
		 
		// Pirámide amarilla 
		shaderList[3].useShader(); 
		uniformModel = shaderList[3].getModelLocation(); 
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-0.45f, -0.02f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.3f));
		model = glm::rotate(model, 90 * toRadians,glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[2]->RenderMesh();
		 
		// Pirámide morada 
		shaderList[5].useShader(); 
		uniformModel = shaderList[5].getModelLocation(); 
		uniformProjection = shaderList[5].getProjectLocation(); 
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(-0.45f, -0.52f, -3.0f)); 
		model = glm::scale(model, glm::vec3(0.50f, 0.50f, 0.3f)); 
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[2]->RenderMesh(); 

		// Figura 3
		// Pirámide amarilla
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2f, -0.49f, -3.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Pirámide morada
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2f, 0.005f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Pirámide verde
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.975f, -0.503f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		// Pirámide roja
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.487f, -0.503f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();
		

		// Figura 4
		// Barra negra  
		shaderList[6].useShader(); 
		uniformModel = shaderList[6].getModelLocation();
		uniformProjection = shaderList[6].getProjectLocation();
		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(0.0f, -0.84f, -3.0f)); 
		model = glm::scale(model, glm::vec3(4.0f, 0.15f, 0.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection)); 
		meshList[1]->RenderMesh(); 

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/