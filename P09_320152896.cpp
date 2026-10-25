/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
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
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

#include "PlanetaryRaceTrack.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
bool avanza;

float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

// Variables para el rover

float movCoche = 0.0f;
float movOffset = 0.5f; 
bool vaIzquierda = true;

float rotaLlanta = 0.0f;
float rotaLlantaIncremento = 0.06f;

// Variables para el avioncito

float movAvion = 0.0f;
float movAvionOffset = 0.75f;

int estadoAvion = 0;
float rotacionAvion = 0.0f;

float rotaHelice = 0.0f;
float rotaHeliceOffset = 0.5f;

// Variables para el satelite

float movSatelite = 0.0f;
float movSateliteOffset = 0.4f;

int estadoSatelite = 0;
float rotacionSatelite = 0.0f;

float rotaAntena = 0.0f;
float rotaAntenaOffset = 0.025f;

float rotaPanelIzq = 0.0f;
float rotaPanelDer = 0.0f;

float rotaPanelIzqOffset = 0.01f;
float rotaPanelDerOffset = 0.01f;


// Variables para la nave

float movNave = 0.0f;
float movNaveOffset = 0.8f;

int estadoNave = 0;

float rotacionNaveY = 0.0f; // dirección
float rotacionNaveZ = 0.0f; // inclinación lateral
float rotacionNaveX = 0.0f; // inclinación al subir/bajar

float avanceNave = 0.0f;
float velocidadNave = 0.4f;



Window mainWindow;
std::vector<MeshModel*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;


//Lista de Modelos a importar
//Rover 
Model baseRover;
Model pinza;
Model brazo1;
Model brazo2;
Model baseBrazo;
Model brazoLlantaDelDer;
Model brazoLlantaDelIzq;
Model brazoLlantaAtrDer;
Model brazoLlantaAtrIzq;
Model brazoLlantaMedDer;
Model brazoLlantaMedIzq;
Model llantaDelanteraCerca;
Model llantaDelanteraLejos;
Model llantaMedioCerca;
Model llantaMedioLejos;
Model llantaAtrasCerca;
Model llantaAtrasLejos;

//Añadir modelo del avión. 
Model Avion_M;
Model avioncitoHelices;

// Checar donde estará ubicada la pista de carreras para que todos tengamos la misma posición en las prácticas y el proyecto

// Modelo de la nave

// Modelo del satélite
Model cuerpoSat;
Model antenaSat;
Model panelDerSat;
Model panelIzqSat;

// Nave espacial 
Model naveEspacial;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

// ---- SHADERS PARA LA PISTA ----
const char* vCodeTrack = R"(
#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNorm;

out vec2 uvData;
out vec3 normData;
out vec3 fragWorldPos;

uniform mat4 mMat;
uniform mat4 vMat;
uniform mat4 pMat;

void main() {
    fragWorldPos = vec3(mMat * vec4(inPos, 1.0));
    normData = mat3(transpose(inverse(mMat))) * inNorm;
    uvData = inUV;
    gl_Position = pMat * vMat * vec4(fragWorldPos, 1.0);
}
)";

const char* fCodeTrack = R"(
#version 330 core
out vec4 finalColor;

in vec2 uvData;
in vec3 normData;
in vec3 fragWorldPos;

void main() {
    vec3 lDir = normalize(vec3(0.5, 1.0, 0.4));
    float nDotL = max(dot(normalize(normData), lDir), 0.25);
    
    vec3 albedo;
    if (uvData.x >= 0.19 && uvData.x <= 0.81) {
        albedo = vec3(0.20, 0.22, 0.25);
        if (abs(uvData.x - 0.5) < 0.015 && fract(uvData.y * 0.5) < 0.5) {
            albedo = vec3(0.95, 0.80, 0.15); 
        }
    } else {
        float st = step(0.5, fract(uvData.y * 0.5));
        albedo = mix(vec3(0.85, 0.15, 0.15), vec3(0.92, 0.92, 0.92), st);
    }
    finalColor = vec4(albedo * nDotL, 1.0);
}
)";
// -------------------------------

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


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


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	MeshModel* obj1 = new MeshModel();
	obj1->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	MeshModel* obj2 = new MeshModel();
	obj2->CreateMeshModel(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	MeshModel* obj3 = new MeshModel();
	obj3->CreateMeshModel(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	MeshModel* obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	MeshModel* obj5 = new MeshModel();
	obj5->CreateMeshModel(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	MeshModel* obj6 = new MeshModel();
	obj6->CreateMeshModel(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6); // todos los números

	MeshModel* obj7 = new MeshModel();
	obj7->CreateMeshModel(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	// ---- INICIALIZAR PISTA DE CARRERAS ----
	GLuint vsTrack = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsTrack, 1, &vCodeTrack, NULL);
	glCompileShader(vsTrack);
	GLuint fsTrack = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsTrack, 1, &fCodeTrack, NULL);
	glCompileShader(fsTrack);
	GLuint trackProg = glCreateProgram();
	glAttachShader(trackProg, vsTrack);
	glAttachShader(trackProg, fsTrack);
	glLinkProgram(trackProg);

	PlanetaryRaceTrack raceTrack;
	raceTrack.buildMesh();
	// ----------------------------------------

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

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

	brazoLlantaDelDer = Model();
	brazoLlantaDelDer.LoadModel("Models/brazoLlantaDelDer.fbx");

	brazoLlantaDelIzq = Model();
	brazoLlantaDelIzq.LoadModel("Models/brazoLlantaDelIzq.fbx");

	brazoLlantaMedDer = Model();
	brazoLlantaMedDer.LoadModel("Models/brazoLlantaMedDer.fbx");

	brazoLlantaMedIzq = Model();
	brazoLlantaMedIzq.LoadModel("Models/brazoLlantaMedIzq.fbx");

	brazoLlantaAtrDer = Model();
	brazoLlantaAtrDer.LoadModel("Models/brazoLlantaAtrDer.fbx");

	brazoLlantaAtrIzq = Model();
	brazoLlantaAtrIzq.LoadModel("Models/brazoLlantaAtrIzq.fbx");

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

	//Añadir modelo del avión 
	Avion_M = Model();
	Avion_M.LoadModel("Models/avioncito.obj");

	avioncitoHelices = Model();
	avioncitoHelices.LoadModel("Models/avioncitoHelices.obj");

	// Modelo de la nave


	// Modelo del satélite
	cuerpoSat = Model();
	cuerpoSat.LoadModel("Models/cuerpoSat.obj");

	antenaSat = Model();
	antenaSat.LoadModel("Models/antena.obj");

	panelIzqSat = Model();
	panelIzqSat.LoadModel("Models/panelIzq.obj");

	panelDerSat = Model();
	panelDerSat.LoadModel("Models/panelDer.obj");

	//Modelo de la nave espacial 
	naveEspacial = Model();
	naveEspacial.LoadModel("Models/naveEspacial.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luz verde de la pinza del rover
	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
		0.6f, 0.6f,				// radiacion y color (0.3f y 0.6f)
		3.75f, 10.0f, 15.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luz morada para la nave
	pointLights[2] = PointLight(1.0f, 0.0f, 1.0f,
		0.8f, 1.0f,
		0.0f, 0.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	// Luz azul para el motor 1 de la nave
	pointLights[3] = PointLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,				// radiacion y color (0.3f y 0.6f)
		0.0f, 0.0f, 0.0f,
		0.1f, 0.1f, 0.03f);
	pointLightCount++;

	// Luz azul para el motor 2 de la nave
	pointLights[4] = PointLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,				// radiacion y color (0.3f y 0.6f)
		0.0f, 0.0f, 0.0f,
		0.1f, 0.1f, 0.03f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	// 1. Faro Frontal Rover (Índice 1)
	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 2.8f, 0.0f,
		1.0f, 0.0f, 0.0f, // Dirección hacia adelante (+X)
		1.0f, 0.01f, 0.0019f,
		10.0f); // Apertura del faro
	spotLightCount++;

	// 2. Faro hacia abajo del avioncito (Índice 2)
	spotLights[2] = SpotLight(1.0f, 1.0f, 0.8f,
		1.0f, 1.0f,
		0.0f, 2.8f, 0.0f,
		0.0f, -1.0f, 0.0f, // Dirección hacia abajo (-Y)
		1.0f, 0.01f, 0.0019f,
		15.0f); // Apertura del faro
	spotLightCount++;

	// 3. Faro frontal del avioncito (Índice 3)
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, // Dirección hacia adelante (+X)
		1.0f, 0.0005f, 0.00019f,
		20.0f);
	spotLightCount++;

	// 4. Faro frontal de la lave (índice 4)
	spotLights[4] = SpotLight(1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 2.8f, 0.0f,
		1.0f, 0.0f, 0.0f, // Dirección hacia adelante (+X)
		1.0f, 0.0005f, 0.00019f,
		25.0f); // Apertura del faro
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// Matrices y posiciones de las luces para la jerarquia
	glm::mat4 roverMatrix(1.0f);
	glm::mat4 avionMatrix(1.0f);

	glm::vec3 roverPos;
	glm::vec3 roverDir;

	glm::vec3 avionFrentPos;
	glm::vec3 avionFrentDir;

	glm::vec3 avionAbajoPos;
	glm::vec3 avionAbajoDir;

	// Arreglo con los 6 colores para el faro del avioncito
	glm::vec3 coloresFaro[] = {
		glm::vec3(1.0f, 0.0f, 0.0f), // 0: Rojo
		glm::vec3(1.0f, 1.0f, 0.0f), // 1: Amarillo
		glm::vec3(0.0f, 1.0f, 0.0f), // 2: Verde
		glm::vec3(0.0f, 1.0f, 1.0f), // 3: Cian
		glm::vec3(0.0f, 0.0f, 1.0f), // 4: Azul
		glm::vec3(1.0f, 0.0f, 1.0f)  // 5: Magenta
	};

	GLfloat temporizadorColor = 0.0f;
	int indiceColor = 0;

	glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);


	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::mat4 modelaux3(1.0); // Para el avioncito
	glm::mat4 modelaux4(1.0); // Para el satélite
	glm::mat4 modelaux5(1.0); // Para la jerarquia del satélite
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;

		// Animacion básica

		// Para mi rover
		if (vaIzquierda)
		{
			movCoche -= movOffset * deltaTime;
			rotaLlanta += rotaLlantaIncremento * deltaTime;

			if (movCoche <= -293.0f)
			{
				vaIzquierda = false;
			}
		}
		else
		{
			movCoche += movOffset * deltaTime;
			rotaLlanta -= rotaLlantaIncremento * deltaTime;

			if (movCoche >= 293.0f)
			{
				vaIzquierda = true;
			}
		}

		// Para el avioncito

		movAvion += movAvionOffset * deltaTime;
		rotaHelice += rotaHeliceOffset * deltaTime;

		switch (estadoAvion)
		{
		case 0: // Avanza en linea recta
			rotacionAvion = 0.0f;

			if (movAvion >= 560.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 1;
			}
			break;

		case 1: // Avanza girando 45 grados
			rotacionAvion = -45.0f;

			if (movAvion >= 40.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 2;
			}
			break;

		case 2: // Avanza girando 90 grados
			rotacionAvion = -90.0f;

			if (movAvion >= 80.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 3;
			}
			break;

		case 3: // Avanza girando 135 grados
			rotacionAvion = -135.0f;

			if (movAvion >= 40.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 4;
			}
			break;

		case 4: //Avanza girando 180 grados (linea recta de regreso)
			rotacionAvion = 180.0f;

			if (movAvion >= 560.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 5;
			}
			break;

		case 5: // Avanza girando 135 grados
			rotacionAvion = 135.0f;

			if (movAvion >= 40.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 6;
			}
			break;

		case 6: // Avanza girando 90 grados
			rotacionAvion = 90.0f;

			if (movAvion >= 80.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 7;
			}
			break;

		case 7: // Avanza girando 45 grados
			rotacionAvion = 45.0f;

			if (movAvion >= 40.0f)
			{
				movAvion = 0.0f;
				estadoAvion = 0;
			}
			break;
		}

		// Para el satélite

		rotaAntena += rotaAntenaOffset * deltaTime;
		rotaPanelIzq += rotaPanelIzqOffset * deltaTime;
		rotaPanelDer += rotaPanelDerOffset * deltaTime;

		movSatelite += movSateliteOffset * deltaTime;

		switch (estadoSatelite)
		{
		case 0:
			rotacionSatelite = 0.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 1;
			}
			break;

		case 1:
			rotacionSatelite = -36.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 2;
			}
			break;

		case 2:
			rotacionSatelite = -72.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 3;
			}
			break;

		case 3:
			rotacionSatelite = -108.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 4;
			}
			break;

		case 4:
			rotacionSatelite = -144.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 5;
			}
			break;

		case 5:
			rotacionSatelite = -180.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 6;
			}
			break;

		case 6:
			rotacionSatelite = -216.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 7;
			}
			break;

		case 7:
			rotacionSatelite = -252.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 8;
			}
			break;

		case 8:
			rotacionSatelite = -288.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 9;
			}
			break;

		case 9:
			rotacionSatelite = -324.0f;
			if (movSatelite >= 60.0f)
			{
				movSatelite = 0.0f;
				estadoSatelite = 0;
			}
			break;
		}


		// Nave espacial

		movNave += movNaveOffset * deltaTime;
		avanceNave += velocidadNave * deltaTime;

		if (avanceNave >= raceTrack.pathData.size())
		{
			avanceNave = 0.0f;
		}

		

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);

		// ---- RENDER PISTA DE CARRERAS ----
		glUseProgram(trackProg);
		glm::mat4 trackModel = glm::mat4(1.0f);

		// 1. PRIMERO mueve la pista a la coordenada deseada
		trackModel = glm::translate(trackModel, glm::vec3(0.0f, 0.0f, -250.0f));


		glUniformMatrix4fv(glGetUniformLocation(trackProg, "pMat"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(trackProg, "vMat"), 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(trackProg, "mMat"), 1, GL_FALSE, glm::value_ptr(trackModel));
		raceTrack.draw();
		// -----------------------------------

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		// Información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);

		// Creamos un arreglo vacío y un contador para saber cuántas luces enviaremos
		PointLight lucesActivas[MAX_POINT_LIGHTS];
		unsigned int lucesActivasCount = 0;

		// Luz verde (índice 1 original)
		if (mainWindow.getTeclaRoverPresionada())
		{
			lucesActivas[lucesActivasCount] = pointLights[1];
			lucesActivasCount++;
		}
		// Luz morada de la nave (indice 2 original)
		if (mainWindow.getTeclaNavePresionada()) {
			lucesActivas[lucesActivasCount++] = pointLights[2];
		}

		if (mainWindow.getTeclaMotor1())
		{
			lucesActivas[lucesActivasCount++] = pointLights[3];
		}

		if (mainWindow.getTeclaMotor2())
		{
			lucesActivas[lucesActivasCount++] = pointLights[4];
		}

		// Se envía el arreglo final al shader con el contador exacto (puede ser 0, 1, 2, 3 o 4 luces)
		shaderList[0].SetPointLights(lucesActivas, lucesActivasCount);

		// Finalmente, las luces spotlight
		//shaderList[0].SetSpotLights(spotLights, spotLightCount);

		// Hago un arreglo para las luces spotlight como el de las luces puntuales

		SpotLight spotActivas[MAX_SPOT_LIGHTS];
		unsigned int spotActivasCount = 0;

		spotActivas[spotActivasCount++] = spotLights[0]; // Linterna
		spotActivas[spotActivasCount++] = spotLights[1]; // Faro Rover
		spotActivas[spotActivasCount++] = spotLights[2]; // Avion 1
		spotActivas[spotActivasCount++] = spotLights[3]; // Avion 2
		if (mainWindow.getTeclaNavePresionada()) {
			spotActivas[spotActivasCount++] = spotLights[4]; // Faro Nave
		}

		// Se envía el arreglo final al shader con el contador exacto (puede ser 0, 1, 2 o 3 luces)
		shaderList[0].SetSpotLights(spotActivas, spotActivasCount);


		//Reinicializando variables cada ciclo de reloj
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMeshModel();

		// Checar el centro del rover YA
		// Separar llantas YA

		//Instancia del rover
		model = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(movCoche + 0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 1.75f, 10.0f));
		modelaux = model;
		modelaux2 = model;
		
		// Matriz del rover para calcular la posición y dirección del faro
		roverMatrix = modelaux2;

		// Posición del faro del rover
		roverPos = glm::vec3(roverMatrix * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

		// Dirección del faro del rover
		roverDir = glm::normalize(glm::vec3(roverMatrix * glm::vec4(1.5f, -0.25f, 0.0f, 0.0f)));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		baseRover.RenderModel();

		// Faro del rover
		spotLights[1].SetFlash(roverPos, roverDir);

		// -----* Brazo *------
		// Base del brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.75f, 1.5f, -1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		baseBrazo.RenderModel();

		// Brazo 1 (conectado a la base del brazo)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazo1.RenderModel();

		// Brazo 2 (conectado al brazo 1)
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.45f, 2.5f, -0.01f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazo2.RenderModel();

		// Pinza (conectado al brazo 2)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.4f, 3.3f, 0.125f));
		modelaux = model; // Esta matriz contiene todo el brazo jerárquico
		
		// Luz puntual verde de la pinza
		glm::vec3 posPinza = glm::vec3(modelaux * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		pointLights[1].SetPosition(posPinza);
	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pinza.RenderModel();


		//	Brazo de la llanta delantera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.075f, 1.15f, 3.07f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoLlantaDelDer.RenderModel();

		//	Brazo de la llanta delantera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(2.075f, 1.14f, -3.05f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoLlantaDelIzq.RenderModel();

		// Brazo de la llanta media derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.5f, 0.25f, 3.07f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoLlantaMedDer.RenderModel();

		// Brazo de la llanta media izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-2.45f, 0.25f, -3.07f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoLlantaMedIzq.RenderModel();

		//Brazo de la llanta trasera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.5f, 0.25f, 3.07f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoLlantaAtrDer.RenderModel();

		//	Brazo de la llanta trasera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-3.55f, 0.25f, -3.07f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		brazoLlantaAtrIzq.RenderModel();

		// Llanta delantera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(5.6f, -1.42f, -2.0f));
		model = glm::rotate(model, rotaLlanta, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		llantaDelanteraCerca.RenderModel();

		// Llanta delantera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(5.65f, -1.42f, 2.0f));
		model = glm::rotate(model, rotaLlanta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaDelanteraLejos.RenderModel();

		// Llanta medio izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.35f, -1.5f, -3.5f));
		model = glm::rotate(model, rotaLlanta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaMedioCerca.RenderModel();

		// Llanta Medio derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-0.25f, -1.6f, 3.5f));
		model = glm::rotate(model, rotaLlanta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaMedioLejos.RenderModel();

		// Llanta trasera izquierda
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-5.7f, -1.55f, -3.5f));
		model = glm::rotate(model, rotaLlanta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaAtrasCerca.RenderModel();

		// Llanta trasera derecha
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-5.7f, -1.55f, 3.5f));
		model = glm::rotate(model, rotaLlanta, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		llantaAtrasLejos.RenderModel();



		//Añadir modelo del avión
		// Cuerpo del avioncito
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f + movAvion, 10.0f + 4 * sin(movAvionY), 6.0)); // Por el sin() se hace compleja (con las aspas ya girando)
		switch (estadoAvion)
		{
		case 0:  
			model = glm::translate(model, glm::vec3(-280.0f + movAvion, 25.0f, -40.0f));
			break;

		case 1: 
			model = glm::translate(model, glm::vec3(280.0f + movAvion, 25.0f, -40.0f + movAvion));
			break;

		case 2: 
			model = glm::translate(model, glm::vec3(320.0f, 25.0f, 0.0f + movAvion));
			break;

		case 3: 
			model = glm::translate(model, glm::vec3(320.0f - movAvion, 25.0f, 80.0f + movAvion));
			break;

		case 4: 
			model = glm::translate(model, glm::vec3(280.0f - movAvion, 25.0f, 120.0f));
			break;

		case 5: 
			model = glm::translate(model, glm::vec3(-280.0f - movAvion, 25.0f, 120.0f - movAvion));
			break;

		case 6: 
			model = glm::translate(model, glm::vec3(-320.0f, 25.0f, 80.0f - movAvion));
			break;

		case 7: 
			model = glm::translate(model, glm::vec3(-320.0f + movAvion, 25.0f, 0.0f - movAvion));
			break;
		}
		model = glm::rotate(model, rotacionAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux3 = model;
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		
		// Matriz del avion para calcular la posición y dirección de los faros
		avionMatrix = modelaux3;
		avionMatrix = glm::rotate(avionMatrix, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		// 1. Faro hacia abajo: En el mundo real, "abajo" siempre es el eje -Y
		avionAbajoPos = glm::vec3(avionMatrix * glm::vec4(0.0f, -0.25f, 0.0f, 1.0f)); // Posición se queda igual
		avionAbajoDir = glm::vec3(0.0f, -1.0f, 0.0f); // Dirección directa hacia abajo
		spotLights[2].SetFlash(avionAbajoPos, avionAbajoDir);

		// 1. Lógica de animación automática
		if (mainWindow.getCiclarColor())
		{
			temporizadorColor += deltaTime;				// Sumamos el tiempo transcurrido

			// Cambiamos al siguiente color si se cumple la condición
			if (temporizadorColor >= 20.0f)
			{
				indiceColor = (indiceColor + 1) % 6;	// Avanza el color
				temporizadorColor = 0.0f;				// Reiniciamos el cronómetro
			}
		}

		// 2. Aplicamos el color actual (si la animación está apagada, se queda en el último color)
		glm::vec3 colorActual = coloresFaro[indiceColor];
		spotLights[2].SetColor(colorActual.r, colorActual.g, colorActual.b);

		// 3. Actualizamos su posición
		spotLights[2].SetFlash(avionAbajoPos, avionAbajoDir);

		// Faro frontal: Usamos una matriz limpia solo con el giro (yaw) del avión
		glm::mat4 dirAvionMatrix(1.0f);
		dirAvionMatrix = glm::rotate(dirAvionMatrix, rotacionAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		avionFrentPos = glm::vec3(avionMatrix * glm::vec4(8.5f, -0.95f, 0.0f, 1.0f));
		// Cuando rotacionAvion es 0, el avión se mueve hacia +X. Usamos +X como frente, con un ligero declive en Y (-0.2f)
		avionFrentDir = glm::normalize(glm::vec3(dirAvionMatrix * glm::vec4(1.0f, -0.2f, 0.0f, 0.0f)));
		spotLights[3].SetFlash(avionFrentPos, avionFrentDir);
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Avion_M.RenderModel();

		// Helice del avioncito
		model = modelaux3;						// z , x, y
		model = glm::translate(model, glm::vec3(0.0f, 15.05f, -0.925f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotaHelice, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		avioncitoHelices.RenderModel();

		// Satelite
		// Cuerpo del satelite
		model = modelaux4;
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);

		switch (estadoSatelite)
		{
		case 0:
			model = glm::translate(model, glm::vec3(-30.0f + movSatelite, 45.0f, 0.0f));
			break;

		case 1:
			model = glm::translate(model, glm::vec3(30.0f + movSatelite * 0.809f, 45.0f, movSatelite * 0.588f));
			break;

		case 2:
			model = glm::translate(model, glm::vec3(78.5f + movSatelite * 0.309f, 45.0f, 35.3f + movSatelite * 0.951f));
			break;

		case 3:
			model = glm::translate(model, glm::vec3(97.0f - movSatelite * 0.309f, 45.0f, 92.4f + movSatelite * 0.951f));
			break;

		case 4:
			model = glm::translate(model, glm::vec3(78.5f - movSatelite * 0.809f, 45.0f, 149.5f + movSatelite * 0.588f));
			break;

		case 5:
			model = glm::translate(model, glm::vec3(30.0f - movSatelite, 45.0f, 184.8f));
			break;

		case 6:
			model = glm::translate(model, glm::vec3(-30.0f - movSatelite * 0.809f, 45.0f, 184.8f - movSatelite * 0.588f));
			break;

		case 7:
			model = glm::translate(model, glm::vec3(-78.5f - movSatelite * 0.309f, 45.0f, 149.5f - movSatelite * 0.951f));
			break;

		case 8:
			model = glm::translate(model, glm::vec3(-97.0f + movSatelite * 0.309f, 45.0f, 92.4f - movSatelite * 0.951f));
			break;

		case 9:
			model = glm::translate(model, glm::vec3(-78.5f + movSatelite * 0.809f, 45.0f, 35.3f - movSatelite * 0.588f));
			break;
		}

		model = glm::rotate(model, glm::radians(rotacionSatelite), glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cuerpoSat.RenderModel();

		// Antena
		model = modelaux;						//  z , x, y
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.75f));
		model = glm::rotate(model, rotaAntena, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux5 = model;
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		antenaSat.RenderModel();

		// Panel solar 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.7f, 0.0f));
		model = glm::rotate(model, rotaPanelIzq, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux5 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		panelIzqSat.RenderModel();

		// Panel solar 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.65f, 0.0f));
		model = glm::rotate(model, rotaPanelDer, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux5 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		panelDerSat.RenderModel();


		// Nave espacial

		model = glm::mat4(1.0f);

		int idx = (int)avanceNave;

		if (idx >= raceTrack.pathData.size())
		{
			idx = 0;
		}

		PlanetaryRaceTrack::PathSample sample = raceTrack.pathData[idx];

		// La pista está desplazada -250 en Z
		glm::vec3 posicionNave = sample.pos +glm::vec3(0.0f, 3.0f, -250.0f);

		glm::vec3 forward = glm::normalize(sample.fwd);
		glm::vec3 up = glm::normalize(sample.up);
		glm::vec3 right = glm::normalize(sample.right);

		// Construcción de matriz de orientación usando los ejes de la pista
		glm::mat4 orientacion(1.0f);

		orientacion[0] = glm::vec4(right, 0.0f);
		orientacion[1] = glm::vec4(up, 0.0f);
		orientacion[2] = glm::vec4(-forward, 0.0f);

		model = glm::translate(glm::mat4(1.0f),posicionNave);

		model *= orientacion;

		// Ajustes propios del modelo
		model = glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f, 1.0f, 0.0f));

		// MATRIZ PARA LUCES
		glm::mat4 naveMatrixLuces = model;

		model = glm::scale(model,glm::vec3(7.0f, 7.0f, 7.0f));

		// ================== LUCES DE LA NAVE ==================

		glm::vec3 navePuntualPos =glm::vec3(naveMatrixLuces * glm::vec4(0.0f, 2.25f, -3.0f, 1.0f));

		pointLights[2].SetPosition(navePuntualPos);

		glm::vec3 naveSpotPos = glm::vec3( naveMatrixLuces * glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));

		glm::vec3 naveSpotDir = glm::normalize( glm::vec3( naveMatrixLuces * glm::vec4(0.0f, -0.4f, 1.0f, 0.0f)));

		spotLights[4].SetFlash(naveSpotPos,naveSpotDir);

		// Motor izquierdo
		glm::vec3 motor1Pos =glm::vec3(naveMatrixLuces *glm::vec4(-7.35f, 3.0f, -14.5f, 1.0f));

		pointLights[3].SetPosition(motor1Pos);

		// Motor derecho
		glm::vec3 motor2Pos = glm::vec3( naveMatrixLuces * glm::vec4(7.5f, 3.0f, -14.5f, 1.0f));

		pointLights[4].SetPosition(motor2Pos);

		// ================== RENDER ==================

		glUniformMatrix4fv(uniformModel,1,GL_FALSE,glm::value_ptr(model));

		color = glm::vec3(1.0f, 1.0f, 1.0f);

		glUniform3fv(uniformColor,1,glm::value_ptr(color));

		naveEspacial.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del Rover?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMeshModel();

		//textura con movimiento
		//Importantes porque la variable uniform no podemos modificarla directamente
		toffsetflechau += 0.001;
		toffsetflechav = 0.000;
		//para que no se desborde la variable
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		//if (toffsetv > 1.0)
		//	toffsetv = 0;
		//printf("\ntfosset %f \n", toffsetu);
		//pasar a la variable uniform el valor actualizado
		toffset = glm::vec2(toffsetflechau, toffsetflechav);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMeshModel();

		//plano con todos los números
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();

		//número 1
		//toffsetnumerou = 0.0;
		//toffsetnumerov = 0.0;
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 2.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		for (int i = 1; i < 4; i++)
		{
			//números 2-4
			toffsetnumerou += 0.25;
			toffsetnumerov = 0.0;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-10.0f - (i * 3.0), 2.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();

		}

		for (int j = 1; j < 5; j++)
		{
			//números 5-8
			toffsetnumerou += 0.25;
			toffsetnumerov = -0.33;
			toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-7.0f - (j * 3.0), 5.0f, -6.0f));
			model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			color = glm::vec3(1.0f, 1.0f, 1.0f);
			glUniform3fv(uniformColor, 1, glm::value_ptr(color));
			NumerosTexture.UseTexture();
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			meshList[6]->RenderMeshModel();
		}


		//número cambiante 
		/*
		¿Cómo hacer para que sea a una velocidad visible?
		*/
		toffsetnumerocambiau += 0.25;
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMeshModel();

		//cambiar automáticamente entre textura número 1 y número 2
		toffsetnumerou = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerou, toffsetnumerov);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Numero1Texture.UseTexture();
		//if 
		//Numero1Texture.UseTexture();
		//Numero2Texture.UseTexture();

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMeshModel();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

/*
----- Animacion básica -----

- Variables Incrementales/Decrementales afectadas por deltaTime (multiplicadas)
- Tenemos banderas que las limitan

Para que sea animación:
1. Afecta a más de una transformación geométrica a la vez
2. Contexto de la animación -> ej de la puerta: necesita una entidad que inserte la llave, gire la llave, retire la llave
												y abra la puerta
	2.1 Elementos que se rigen por el contexto: puertas, ventanas, ventiladores, etc...

Para el rover: separar las llantas para que giren
Para el aviocito: separar las helices para que giren

----- Animación compleja -----

ej. el rover avanzando hasta el final del piso y cayendo, dónde la caida depende de la velocidad y conforme cae, el rover va girando

- Valores de incrementos/decrementos se obtienen o afectan a ecuaciones y algoritmos (ya existentes)
	ej. rebote, tiro parabólico, MRU, fluidos, humo, aceleración, etc...

Para que sea animación:
1. Afecta a más de una transformación geométrica a la vez
2. Contexto de la animación
*/