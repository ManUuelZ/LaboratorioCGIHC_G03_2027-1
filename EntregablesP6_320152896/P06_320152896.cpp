/*
Práctica 6: Texturizado
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

#include "Window.h"
#include "Mesh_tn.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList; //solo recibe xyz
std::vector<MeshColor*> meshListColor; // recibe xyz rgb
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz

std::vector<Shader> shaderList;

Camera camera;

Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture holocronTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;
Model Avioncito; 

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";





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

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
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

	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,

		// right
		4, 5, 6,
		6, 7, 4,

	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
	// average normals
	GLfloat cubo_vertices[] = {
		// front  Old Galactic Republic
		//x		y		z		S		T				NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.2000f,  0.8000f,		0.0f,	0.0f,	-1.0f,	//0
		 0.5f, -0.5f,  0.5f,	0.3750f,  0.8000f,		0.0f,	0.0f,	-1.0f,	//1
		 0.5f,  0.5f,  0.5f,	0.3750f,  0.6100f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.2000f,  0.6100f,		0.0f,	0.0f,	-1.0f,	//3

		// right Rebel Alliance
		//x		y		z		S		T
		 0.5f, -0.5f,  0.5f,	0.0879f,  0.9709f,		-1.0f,	0.0f,	0.0f,
		 0.5f, -0.5f, -0.5f,	0.2100f,  0.9709f,		-1.0f,	0.0f,	0.0f,
		 0.5f,  0.5f, -0.5f,	0.2100f,  0.805f,		-1.0f,	0.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.0879f,  0.805f,		-1.0f,	0.0f,	0.0f,

		 // back  Galactic Empire
		 //x		y		z		S		T
		 -0.5f, -0.5f, -0.5f,	0.4150f,  0.9709f,		0.0f,	0.0f,	1.0f,
		  0.5f, -0.5f, -0.5f,	0.5420f,  0.9709f,		0.0f,	0.0f,	1.0f,
		  0.5f,  0.5f, -0.5f,	0.5420f,  0.8293f,		0.0f,	0.0f,	1.0f,
		 -0.5f,  0.5f, -0.5f,	0.4150f,  0.8293f,		0.0f,	0.0f,	1.0f,

		 // left  Mando
		 //x		y		z		S		T
		 -0.5f, -0.5f, -0.5f,	0.8643f,  0.210f,		1.0f,	0.0f,	0.0f,
		 -0.5f, -0.5f,  0.5f,	0.9521f,  0.210f,		1.0f,	0.0f,	0.0f,
		 -0.5f,  0.5f,  0.5f,	0.9521f,  0.0154f,		1.0f,	0.0f,	0.0f,
		 -0.5f,  0.5f, -0.5f,	0.8643f,  0.0154f,		1.0f,	0.0f,	0.0f,

		 // bottom First Order
		 //x		y		z		S		T
		 -0.5f, -0.5f,  0.5f,	0.7031f,  0.5818f,		0.0f,	1.0f,	0.0f,
		 0.5f, -0.5f,  0.5f,	0.8105f,  0.5818f,		0.0f,	1.0f,	0.0f,
		 0.5f, -0.5f, -0.5f,	0.8105f,  0.4502f,		0.0f,	1.0f,	0.0f,
		 -0.5f, -0.5f, -0.5f,	0.7031f,  0.4502f,		0.0f,	1.0f,	0.0f,

		 // UP   Sith Empire
		  //x		y		z		S		T
		 -0.5f,  0.5f,  0.5f,	0.0488f,  0.5918f,		0.0f,	-1.0f,	0.0f,
		  0.5f,  0.5f,  0.5f,	0.1562f,  0.5918f,		0.0f,	-1.0f,	0.0f,
		  0.5f,  0.5f, -0.5f,	0.1562f,  0.4302f,		0.0f,	-1.0f,	0.0f,
		 -0.5f,  0.5f, -0.5f,	0.0488f,  0.4302f,		0.0f,	-1.0f,	0.0f,
	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	unsigned int holocron_indices[] = {

		// CARAS DEL CUBO PRINCIPAL

		// Frontal (First Order)
		17, 16, 18,
		17, 19, 16,

		// Trasera (Old Galactic Republic)
		21, 20, 22,
		21, 23, 20,

		// Izquierda (Galactic Republic)
		26, 25, 27,
		26, 27, 24,

		// Derecha (Galactic Empire)
		29, 31, 28,
		29, 28, 30,

		// Superior (Mando)
		32, 33, 35,
		32, 34, 33,

		// Inferior (Sith Empire) - Octágono
		36, 37, 38,
		38, 39, 36,
		37, 40, 38,
		36, 41, 37,
		39, 42, 36,
		38, 43, 39,


		// TRIANGULOS QUE UNEN LAS CARAS DEL CUBO

		15, 4, 3,
		12, 1, 0,
		14, 9, 2,
		2, 12, 15,
		3, 6, 14,
		4, 5, 8,
		6, 7, 13,
		9, 10, 11,
		12, 11, 1,
		4, 0, 5,
		6, 8, 7,
		9, 13, 10

	};

	GLfloat holocron_vertices[] = {

		//x					y				z				S		T		NX	NY	NZ
		-2.517274f,		-2.579095f,		 0.002115f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//0
		-1.185055f,		-2.530499f,		-1.198261f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//1
		 0.049649f,		 2.440128f,		-2.417217f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//2
		 0.051152f,		 2.399781f,		 2.545043f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//3
		-2.646786f,		-0.139047f,		 2.493757f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//4
		-1.328995f,		-2.578386f,		 1.297838f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//5
		 2.593003f,		-0.085222f,		 2.505755f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//6
		 1.385028f,		-2.549476f,		 1.308677f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//7
		-0.140696f,		-2.601934f,		 2.558561f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//8
		 2.534944f,		-0.061146f,		-2.580280f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//9
		 1.344182f,		-2.542571f,		-1.294387f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//10
		 0.081432f,		-2.513204f,		-2.426005f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//11
		-2.453980f,		-0.063388f,		-2.477451f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//12
		 2.632201f,		-2.553892f,		 0.007226f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//13
		 2.521206f,		 2.448241f,		 0.063388f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//14
		-2.451372f,		 2.421759f,		 0.077538f,		0.0f,	0.0f,	0.0f,	0.0f,	0.0f,	//15


		// CARA FRONTAL (First Order) - Más pequeña y centrada
		 0.051152f,		 2.399781f,		 2.545043f,		0.871f,	0.597f,		0.0f,	0.0f,	0.0f,	//16 
		-0.140696f,		-2.601934f,		 2.558561f,		0.660f,	0.397f,		0.0f,	0.0f,	0.0f,	//17 
		-2.646786f,		-0.139047f,		 2.493757f,		0.687f,	0.573f,		0.0f,	0.0f,	0.0f,	//18 
		 2.593003f,		-0.085222f,		 2.505755f,		0.845f,	0.419f,		0.0f,	0.0f,	0.0f,	//19 

		// CARA TRASERA (Old Galactic Republic) - Más pequeña y centrada
		0.049649f,		 2.440128f,		-2.417217f,		0.370f,	0.802f,		0.0f,	0.0f,	0.0f,	//20 
		0.081432f,		-2.513204f,		-2.426005f,		0.166f,	0.599f,		0.0f,	0.0f,	0.0f,	//21 
		2.534944f,		-0.061146f,		-2.580280f,		0.193f,	0.773f,		0.0f,	0.0f,	0.0f,	//22 
		-2.453980f,		-0.063388f,		-2.477451f,		0.341f,	0.625f,		0.0f,	0.0f,	0.0f,	//23 

		// CARA IZQUIERDA (Galactic Republic) - Más pequeña y centrada
		-2.451372f,		 2.421759f,		 0.077538f,		0.539f,	0.797f,		0.0f,	0.0f,	0.0f,	//24 
		-2.517274f,		-2.579095f,		 0.002115f,		0.331f,	0.594f,		0.0f,	0.0f,	0.0f,	//25 
		-2.453980f,		-0.063388f,		-2.477451f,		0.361f,	0.771f,		0.0f,	0.0f,	0.0f,	//26 
		-2.646786f,		-0.139047f,		 2.493757f,		0.505f,	0.620f,		0.0f,	0.0f,	0.0f,	//27 

		// CARA DERECHA (Galactic Empire) - Más pequeña y desplazada
		2.521206f,		 2.448241f,		 0.063388f,		0.595f,	1.002f,		0.0f,	0.0f,	0.0f,	//28 
		2.632201f,		-2.553892f,		 0.007226f,		0.391f,	0.795f,		0.0f,	0.0f,	0.0f,	//29 
		2.593003f,		-0.085222f,		 2.505755f,		0.418f,	0.971f,		0.0f,	0.0f,	0.0f,	//30 
		2.534944f,		-0.061146f,		-2.580280f,		0.570f,	0.821f,		0.0f,	0.0f,	0.0f,	//31 

		// CARA SUPERIOR (Mando) - Intacta
		0.049649f,		 2.440128f,		-2.417217f,		1.024f,	0.192f,		0.0f,	0.0f,	0.0f,	//32 
		0.051152f,		 2.399781f,		 2.545043f,		0.831f,	-0.001f,	0.0f,	0.0f,	0.0f,	//33 
		-2.451372f,		 2.421759f,		 0.077538f,		0.830f,	0.192f,		0.0f,	0.0f,	0.0f,	//34 
		2.521206f,		 2.448241f,		 0.063388f,		1.024f,	-0.001f,	0.0f,	0.0f,	0.0f,	//35 
		
		// CARA INFERIOR (Sith Empire) - Intacta
		1.344182f,		-2.542571f,		-1.294387f,		0.085f,	0.397f,		0.0f,	0.0f,	0.0f,	//36
		1.385028f,		-2.549476f,		 1.308677f,		0.188f,	0.497f,		0.0f,	0.0f,	0.0f,	//37
		-1.328995f,		-2.578386f,		 1.297838f,		0.082f,	0.602f,		0.0f,	0.0f,	0.0f,	//38
		-1.185055f,		-2.530499f,		-1.198261f,		-0.010f,0.499f,		0.0f,	0.0f,	0.0f,	//39
		-0.140696f,		-2.601934f,		 2.558561f,		0.177f,	0.605f,		0.0f,	0.0f,	0.0f,	//40
		2.632201f,		-2.553892f,		 0.007226f,		0.186f,	0.398f,		0.0f,	0.0f,	0.0f,	//41
		0.081432f,		-2.513204f,		-2.426005f,		-0.008f,0.402f,		0.0f,	0.0f,	0.0f,	//42
		-2.517274f,		-2.579095f,		 0.002115f,		-0.015f,0.598f,		0.0f,	0.0f,	0.0f	//43

	};

	MeshModel* holocron = new MeshModel();

	// 48 vértices en total * 8 floats por vértice = 384 de tamaño
	// 84 índices para los triángulos
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 84);

	meshListModel.push_back(holocron);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CrearHolocron();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/starWarsModif.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/starWarsModif.png");
	holocronTexture.LoadTextureA();
	
	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocron_simple_modif.obj");

	Dado_M = Model();
	Dado_M.LoadModel("Models/dadoStarWars.obj");

	Avioncito = Model(); 
	Avioncito.LoadModel("Models/avioncito.obj"); 

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
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
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshListModel[2]->RenderMeshModel();


		
		/*
		//Dado de Opengl
		//Ejercicio 1: Texturizar su dado con la imagen ya optimizada por ustedes con logos de star wars
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 5.0f, -6.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();
		
		//Ejercicio 2:Importar el cubo texturizado en el programa de modelado con 
		//la imagen ya optimizada por ustedes
		
		//Dado importado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado_M.RenderModel();
		*/



		/*Reporte de práctica :
		
		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avión con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avión: Nariz y Sonrisa
		Alas: Logos del universo del personaje
		
		*/

		
		// ----- Holocrones -----
		
		// Holocron texturizado por código 
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color que multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(-4.5f, 4.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		holocronTexture.UseTexture(); 
		meshListModel[5]->RenderMeshModel(); 
		 
		// Holocron texturizado en Blender 
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(4.5f, 4.5f, 0.0f)); 
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); 
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Holocron_M.RenderModel(); 
		
		// Avioncito del previo
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura 
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); 
		model = glm::mat4(1.0); 
		model = glm::translate(model, glm::vec3(30.0f, 4.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 
		Avioncito.RenderModel();
		 
	
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/