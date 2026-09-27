/*
Práctica 5: Optimización y Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;
//float angulocola = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

//Lista de Modelos a importar
//Rover
Model baseRover;
Model pinza;
Model brazo1;
Model brazo2;
Model baseBrazo;
Model llantaDelanteraCerca;
Model llantaDelanteraLejos;
Model llantaMedioCerca;
Model llantaMedioLejos;
Model llantaAtrasCerca;
Model llantaAtrasLejos;

//Holocron
Model centroHolocron;
Model piramide1;
Model piramide2;
Model piramide3;
Model piramide4;
Model piramide5;
Model piramide6;
Model piramide7;
Model piramide8;

//Satelite
Model cuerpoSatelite;
Model antena;
Model panel1;
Model panel2;


//Lista de Skybox a crear
Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// Vertex Shader
static const char* vShader = "shaders/shader_m.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_m.frag";


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	
	MeshModel *obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj1);

	MeshModel *obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshListModel.push_back(obj2);

	MeshModel *obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshListModel.push_back(obj3);


}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.5f, 7.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);

	//Cargar modelos
	//Rover
	baseRover = Model();
	baseRover.LoadModel("Models/baseRover.fbx");

	baseBrazo = Model();
	baseBrazo.LoadModel("Models/baseBrazo.fbx");

	brazo1 = Model();
	brazo1.LoadModel("Models/brazo1.fbx");

	brazo2 = Model();
	brazo2.LoadModel("Models/brazo2.fbx");

	pinza = Model();
	pinza.LoadModel("Models/pinza.fbx");

	llantaDelanteraCerca = Model();
	llantaDelanteraCerca.LoadModel("Models/llantaDelanteraCerca.fbx");

	llantaDelanteraLejos = Model();
	llantaDelanteraLejos.LoadModel("Models/llantaDelanteraLejos.fbx");

	llantaMedioCerca = Model();
	llantaMedioCerca.LoadModel("Models/llantaMedioCerca.fbx");

	llantaMedioLejos = Model();
	llantaMedioLejos.LoadModel("Models/llantaMedioLejos.fbx");

	llantaAtrasCerca = Model();
	llantaAtrasCerca.LoadModel("Models/llantaAtrasCerca.fbx");

	llantaAtrasLejos = Model();
	llantaAtrasLejos.LoadModel("Models/llantaAtrasLejos.fbx");

	//Holocron
	centroHolocron = Model();
	centroHolocron.LoadModel("Models/centroHolocron.fbx");

	piramide1 = Model();
	piramide1.LoadModel("Models/piramide1.fbx");

	piramide2 = Model();
	piramide2.LoadModel("Models/piramide2.fbx");

	piramide3 = Model();
	piramide3.LoadModel("Models/piramide3.fbx");

	piramide4 = Model();
	piramide4.LoadModel("Models/piramide4.fbx");

	piramide5 = Model();
	piramide5.LoadModel("Models/piramide5.fbx");

	piramide6 = Model();
	piramide6.LoadModel("Models/piramide6.fbx");

	piramide7 = Model();
	piramide7.LoadModel("Models/piramide7.fbx");

	piramide8 = Model();
	piramide8.LoadModel("Models/piramide8.fbx");
	
	//Satelite
	cuerpoSatelite = Model();
	cuerpoSatelite.LoadModel("Models/cuerpoSatelite.fbx");

	antena = Model();
	antena.LoadModel("Models/antena.fbx");

	panel1 = Model();
	panel1.LoadModel("Models/panel1.fbx");

	panel2 = Model();
	panel2.LoadModel("Models/panel2.fbx");

	//Crear Skybox con sus 6 texturas
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0); // para la jerarquía de la base del rover
	glm::mat4 modelaux3(1.0); // para la jerarquía del holocron
	glm::mat4 modelaux4(1.0); // para la jerarquía de la base del satelite
	glm::mat4 modelaux5(1.0); // para la jerarquía de las extensiones de la base del satélite
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Se dibuja el Skybox
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		// INICIA DIBUJO DEL PISO
		color = glm::vec3(0.5f, 0.5f, 0.5f); //piso de color gris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshListModel[2]->RenderMeshModel();

		//------------*INICIA DIBUJO DE NUESTROS DEMÁS OBJETOS-------------------*
		
		//Rover
		//Modelo Inicial
		color = glm::vec3(0.0f, 0.0f, 1.0f); //modelo de color azul

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -1.5f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		baseRover.RenderModel();//modificar por el modelo de solo cuerpo del Rover, 
								//para que se pueda separar el brazo y las llantas
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		//En sesión se separara una parte del modelo y se unirá por jeraquía al cuerpo

		// -----* Brazo *------
		// Base del brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.75f, 1.5f, -1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		baseBrazo.RenderModel();

		// Brazo 1 (conectado a la base del brazo)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazo1.RenderModel();

		// Brazo 2 (conectado al brazo 1)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.45f, 2.5f, -0.01f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazo2.RenderModel();

		// Pinza (conectado al brazo 2)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.4f, 3.3f, 0.125f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pinza.RenderModel();

		//Siguientes modelos
		/* Ejercicio:
		1.- Separar las llantas
		2.- Hacer que al presionar una tecla cada pata de rueda pueda rotar un máximo de 45° 
		    "hacia adelante y hacia atrás"
		*/

		//	Llanta delantera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.1f, 2.105f, 1.55f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaDelanteraLejos.RenderModel();
		
		//	Llanta delantera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.1f, 2.05f, -4.55f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaDelanteraCerca.RenderModel();

		//Llanta media derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.5f, 1.25f, 1.55f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaMedioLejos.RenderModel();
		
		// Llanta media izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.45f, 1.25f, -4.55f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaMedioCerca.RenderModel();

		//Llanta trasera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.5f, 1.25f, 1.55f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaAtrasLejos.RenderModel();

		//	Llanta trasera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.55f, 1.25f, -4.55f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f)); 
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaAtrasCerca.RenderModel();


		// Holocron
		// Centro del holocron
		model = modelaux3;
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 5.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		centroHolocron.RenderModel();
		
		// Piramide 1
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-18.05f, 6.9f, 1.95f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 1.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide1.RenderModel();
		
		// Piramide 2
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-18.05f, 6.95f, -1.95f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(-1.0f, -1.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide2.RenderModel();
		
		// Piramide 3
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-21.95f, 6.9f, 1.95f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion12()), glm::vec3(1.0f, -1.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide3.RenderModel();
		
		// Piramide 4
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-21.95f, 6.95f, -1.95f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion13()), glm::vec3(-1.0f, 1.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide4.RenderModel();

		// Piramide 5
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-21.95f, 3.1f, 1.95f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion14()), glm::vec3(-1.0f, -1.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide5.RenderModel();

		// Piramide 6
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-21.95f, 3.1f, -1.95f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion15()), glm::vec3(-1.0f, -1.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide6.RenderModel();

		// Piramide 7
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-18.05f, 3.1f, 1.9f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion16()), glm::vec3(-1.0f, 1.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide7.RenderModel();

		// Piramide 8
		model = modelaux3;
		model = glm::translate(model, glm::vec3(-18.05f, 3.1f, -1.95f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion17()), glm::vec3(1.0f, -1.0f, -1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piramide8.RenderModel();

		// Satelite
		// Cuerpo del satelite
		model = modelaux4;
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(25.0f, 5.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion18()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion19()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion20()), glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cuerpoSatelite.RenderModel();

		// Antena
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion21()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux5 = model;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.6f, 0.6f, 0.6f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		antena.RenderModel();

		// Panel solar 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion22()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion23()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux5 = model;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.05f, 0.08f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		panel1.RenderModel();

		// Panel solar 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.7f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion24()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion25()), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux5 = model;
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.05f, 0.08f, 0.25f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		panel2.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
