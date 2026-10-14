/*
Práctica 7: Iluminación 1 
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
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<MeshModel*> meshListModel; // recibe xyz uv nx ny nz
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture CuboTexture;
Texture HolocronTexture;
// Añadir Texturas del dado y del holocron


Model Blackhawk_M;
Model Rover_Cuerpo; //Sustituir por sus propios Rovers Con jerarquía
Model Base_Brazo;
Model Brazo_1;
Model Brazo_2;
Model Garra;
Model Rueda_DelanteraDer;
Model Rueda_DelanteraIzq;
Model Rueda_MediaDer;
Model Rueda_MediaIzq;
Model Rueda_TraseraDer;
Model Rueda_TraseraIzq;
Model Pata_DelanteraDer;
Model Pata_DelanteraIzq;
Model Pata_MediaDer;
Model Pata_MediaIzq;
Model Pata_TraseraDer;
Model Pata_TraseraIzq;
Model Avion; //Añadir modelo del avión.

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


//función de calculo de normales por promedio de vértices 
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
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, -1.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, -1.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, -1.0f,


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

	MeshModel *obj4 = new MeshModel();
	obj4->CreateMeshModel(vegetacionVertices, vegetacionIndices, 64, 12);
	meshListModel.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado() //sustituir con su propio dado, para que tenga la textura de la práctica 6
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

	};	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.19f,  0.25f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.32f,	0.25f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.32f,	0.41f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.19f,	0.41f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.05f,  0.62f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.19f,	0.62f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.19f,	0.78f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.05f,	0.78f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.21f,  0.62f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.34f,	0.62f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.34f,	0.79f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.21f,	0.79f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.55f,  0.82f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.66f,	0.82f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.66f,	0.98f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.55f,	0.98f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.69f,  0.63f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.78f,	0.63f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.78f,	0.79f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.69f,	0.79f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.81f,  0.62f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.92f,	0.62f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.92f,	0.79f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.81f,	0.79f,		0.0f,	-1.0f,	0.0f,

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}


void CrearHolocron()
{
	unsigned int holocron_indices[] = {
		// FRONT
		0, 2, 1,
		2, 0, 3,
		// RIGHT
		4, 5, 6,
		6, 7, 4,
		// BACK
		8, 9, 10,
		10, 11, 8,
		// LEFT
		12, 13, 14,
		14, 15, 12,
		// BOTTOM
		16, 18, 17,
		18, 16, 19,
		// TOP
		20, 21, 22,
		22, 23, 20,
		// 1. Frente - Arriba - Derecha
		24, 25, 26,
		// 2. Frente - Arriba - Izquierda
		27, 28, 29,
		// 3. Frente - Abajo - Derecha
		30, 31, 32,
		// 4. Frente - Abajo - Izquierda
		33, 34, 35,
		// 5. Atrás - Arriba - Derecha
		36, 37, 38,
		// 6. Atrás - Arriba - Izquierda
		39, 40, 41,
		// 7. Atrás - Abajo - Derecha
		42, 43, 44,
		// 8. Atrás - Abajo - Izquierda
		45, 46, 47,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
	// average normals
	GLfloat holocron_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f,  0.5f,		0.19f,  0.25f,		0.0f,	0.0f,	-1.0f,
		-0.5f, 0.0f,  0.5f,		0.19f,	0.41f,		0.0f,	0.0f,	-1.0f,
		0.0f,  0.5f,  0.5f,		0.06f,	0.41f,		0.0f,	0.0f,	-1.0f,
		0.5f,  0.0f,  0.5f,		0.06f,	0.25f,		0.0f,	0.0f,	-1.0f,
		// right
		//x		y		z		S		T			NX		NY		NZ
		0.5f, -0.5f,  0.0f,		0.66f,  0.62f,		-1.0f,	0.0f,	0.0f,
		0.5f, 0.0f,  -0.5f,		0.66f,	0.78f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.0f,		0.53f,	0.78f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.0f,  0.5f,		0.53f,	0.62f,		-1.0f,	0.0f,	0.0f,
		// back
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, -0.5f,		0.47f,  0.26f,		0.0f,	0.0f,	1.0f,
		-0.5f, 0.0f, -0.5f,		0.47f,	0.42f,		0.0f,	0.0f,	1.0f,
		0.0f,  0.5f, -0.5f,		0.34f,	0.42f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.0f, -0.5f,		0.34f,	0.26f,		0.0f,	0.0f,	1.0f,
		// left
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.0f,	0.81f, 0.82f,		1.0f, 0.0f, 0.0f,
		-0.5f,  0.0f,  0.5f,	0.92f, 0.82f,		1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.0f,	0.92f, 0.97f,		1.0f, 0.0f, 0.0f,
		-0.5f,  0.0f, -0.5f,	0.81f, 0.97f,		1.0f, 0.0f, 0.0f,
		// bottom
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f,  0.5f,		0.21f,  0.62f,		0.0f,	1.0f,	0.0f,
		0.5f, -0.5f,  0.0f,		0.34f,	0.62f,		0.0f,	1.0f,	0.0f,
		0.0f, -0.5f, -0.5f,		0.34f,	0.79f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f, 0.0f,		0.21f,	0.79f,		0.0f,	1.0f,	0.0f,
		//UP
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, 0.5f,		0.66f,  0.82f,		0.0f,	-1.0f,	0.0f,
		0.5f, 0.5f, 0.0f,		0.66f,	0.98f,		0.0f,	-1.0f,	0.0f,
		0.0f, 0.5f, -0.5f,		0.55f,	0.98f,		0.0f,	-1.0f,	0.0f,
		-0.5f, 0.5f, 0.0f,		0.55f,	0.82f,		0.0f,	-1.0f,	0.0f,
		//Esq 1 Frente - Arriba - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, 0.5f,		0.76f,	0.78f,		-0.57f,	-0.57f,	-0.57f,
		0.5f, 0.0f, 0.5f,		0.86f,	0.62f,		-0.57f,	-0.57f,	-0.57f,
		0.5f, 0.5f, 0.0f,		0.96f,	0.78f,		-0.57f,	-0.57f,	-0.57f,
		//Esq 2 Frente - Arriba - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, 0.5f,		0.2f,	0.2f,		0.57f,	-0.57f,	-0.57f,
		-0.5f, 0.5f, 0.0f,		0.05f,	0.2f,		0.57f,	-0.57f,	-0.57f,
		-0.5f, 0.0f, 0.5f,		0.12f,	0.02f,		0.57f,	-0.57f,	-0.57f,
		//Esq 3 Frente - Abajo - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, 0.5f,		0.16f,	0.4f,		-0.57f,	0.57f,	-0.57f,
		0.5f, -0.5f, 0.0f,		0.36f,	0.4f,		-0.57f,	0.57f,	-0.57f,
		0.5f, 0.0f, 0.5f,		0.26f,	0.20f,		-0.57f,	0.57f,	-0.57f,
		//Esq 4 Frente - Abajo - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, 0.5f,		0.7f,	0.44f,		0.57f,	0.57f,	-0.57f,
		-0.5f, 0.0f, 0.5f,		0.57f,	0.6f,		0.57f,	0.57f,	-0.57f,
		-0.5f, -0.5f, 0.0f,		0.45f,	0.44f,		0.57f,	0.57f,	-0.57f,
		//Esq 5 Atras - Arriba - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, -0.5f,		0.66f,	0.2f,		-0.57f,	-0.57f,	0.57f,
		0.5f, 0.5f, 0.0f,		0.83f,	0.2f,		-0.57f,	-0.57f,	0.57f,
		0.5f, 0.0f, -0.5f,		0.75f,	0.07f,		-0.57f,	-0.57f,	0.57f,
		//Esq 6 Atras - Arriba - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, -0.5f,		0.83f,	0.78f,		0.57f,	-0.57f,	0.57f,
		-0.5f, 0.0f, -0.5f,		0.73f,	0.63f,		0.57f,	-0.57f,	0.57f,
		-0.5f, 0.5f, 0.0f,		0.62f,	0.78f,		0.57f,	-0.57f,	0.57f,
		//Esq 7 Atras - Abajo - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, -0.5f,		0.75f,	0.25f,		-0.57f,	0.57f,	0.57f,
		0.5f, 0.0f, -0.5f,		0.85f,	0.43f,		-0.57f,	0.57f,	0.57f,
		0.5f, -0.5f, 0.0f,		0.95f,	0.25f,		-0.57f,	0.57f,	0.57f,
		//Esq 8 Atras - Abajo - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, -0.5f,		0.95f,	0.2f,		0.57f,	0.57f,	0.57f,
		-0.5f, -0.5f, 0.0f,		0.77f,	0.2f,		0.57f,	0.57f,	0.57f,
		-0.5f, 0.0f, -0.5f,		0.87f,	0.02f,		0.57f,	0.57f,	0.57f,
	};
	MeshModel* holocron = new MeshModel();
	holocron->CreateMeshModel(holocron_vertices, holocron_indices, 384, 60);
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
	CuboTexture = Texture("Textures/logos.jpg");
	CuboTexture.LoadTextureA();
	HolocronTexture = Texture("Textures/logos.jpg");
	HolocronTexture.LoadTextureA();

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Rover_Cuerpo = Model();
	Rover_Cuerpo.LoadModel("Models/Cuerpo.obj");
	Base_Brazo = Model();
	Base_Brazo.LoadModel("Models/BaseBrazo.obj");
	Brazo_1 = Model();
	Brazo_1.LoadModel("Models/Brazo1.obj");
	Brazo_2 = Model();
	Brazo_2.LoadModel("Models/Brazo2.obj");
	Garra = Model();
	Garra.LoadModel("Models/Garra.obj");
	Rueda_DelanteraDer = Model();
	Rueda_DelanteraDer.LoadModel("Models/RuedaDelanteraDer.obj");
	Rueda_DelanteraIzq = Model();
	Rueda_DelanteraIzq.LoadModel("Models/RuedaDelanteraIzq.obj");
	Rueda_MediaDer = Model();
	Rueda_MediaDer.LoadModel("Models/RuedaMediaDer.obj");
	Rueda_MediaIzq = Model();
	Rueda_MediaIzq.LoadModel("Models/RuedaMediaIzq.obj");
	Rueda_TraseraDer = Model();
	Rueda_TraseraDer.LoadModel("Models/RuedaTraseraDer.obj");
	Rueda_TraseraIzq = Model();
	Rueda_TraseraIzq.LoadModel("Models/RuedaTraseraIzq.obj");
	Pata_DelanteraDer = Model();
	Pata_DelanteraDer.LoadModel("Models/PataDelanteraDer.obj");
	Pata_DelanteraIzq = Model();
	Pata_DelanteraIzq.LoadModel("Models/PataDelanteraIzq.obj");
	Pata_MediaDer = Model();
	Pata_MediaDer.LoadModel("Models/PataMediaDer.obj");
	Pata_MediaIzq = Model();
	Pata_MediaIzq.LoadModel("Models/PataMediaIzq.obj");
	Pata_TraseraDer = Model();
	Pata_TraseraDer.LoadModel("Models/PataTraseraDer.obj");
	Pata_TraseraIzq = Model();
	Pata_TraseraIzq.LoadModel("Models/PataTraseraIzq.obj");
	
	Avion = Model();
	Avion.LoadModel("Models/avion.obj");

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
		0.2f, 1.0f,
		0.0f, -1.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.5f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		7.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	//Faro frontal del Rover
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.5f,
		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.05f, 0.01f,
		25.0f);
	spotLightCount++;

	//Faro inferior del avion
	spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.05f, 0.01f,
		30.0f);
	spotLightCount++;

	//Faro frontal del avion
	spotLights[4] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 1.5f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.05f, 0.01f,
		25.0f);
	spotLightCount++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 matrizRover(1.0f);
	glm::mat4 matrizPata(1.0f);
	glm::mat4 matrizAvion(1.0f);

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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//Movimiento del Rover
		GLfloat avanceRover = 0.05f * (mainWindow.getarticulacion10() - mainWindow.getarticulacion11());
		matrizRover = glm::mat4(1.0f);
		matrizRover = glm::translate(matrizRover, glm::vec3(1.0f - avanceRover, 0.3f, 6.0f));
		matrizRover = glm::scale(matrizRover, glm::vec3(0.4f, 0.4f, 0.4f));
		matrizRover = glm::translate(matrizRover, glm::vec3(0.0f, 1.0f, 0.0f));

		//Faro frontal del Rover
		glm::vec3 posicionFaroRover = glm::vec3(matrizRover * glm::vec4(-5.0f, 1.0f, 0.0f, 1.0f));
		glm::vec3 direccionFaroRover = glm::normalize(glm::vec3(matrizRover * glm::vec4(-1.0f, -0.5f, 0.0f, 0.0f)));
		spotLights[2].SetFlash(posicionFaroRover, direccionFaroRover);

		//Movimiento del avion
		GLfloat alturaAvion = 0.05f * (mainWindow.getarticulacion12() - mainWindow.getarticulacion13());
		GLfloat avanceAvion = 0.05f * (mainWindow.getarticulacion14() - mainWindow.getarticulacion15());
		glm::mat4 giroAvion = glm::rotate(glm::mat4(1.0f), 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 frenteLocalAvion = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 frenteAvion = glm::normalize(glm::vec3(giroAvion * glm::vec4(frenteLocalAvion, 0.0f)));
		matrizAvion = glm::mat4(1.0f);
		matrizAvion = glm::translate(matrizAvion, glm::vec3(0.0f, 5.0f + alturaAvion, 6.0f) + frenteAvion * avanceAvion);
		matrizAvion = glm::scale(matrizAvion, glm::vec3(0.3f, 0.3f, 0.3f));
		matrizAvion = glm::rotate(matrizAvion, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//Faro inferior del avion
		glm::vec3 posicionFaroInferior = glm::vec3(matrizAvion * glm::vec4(0.0f, -2.0f, 0.0f, 1.0f));
		glm::vec3 direccionFaroInferior = glm::normalize(glm::vec3(matrizAvion * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f)));
		spotLights[3].SetFlash(posicionFaroInferior, direccionFaroInferior);

		//Faro frontal del avion
		glm::vec3 posicionFaroFrontal = glm::vec3(matrizAvion * glm::vec4(0.0f, 0.0f, -3.0f, 1.0f));
		glm::vec3 direccionFaroFrontal = glm::normalize(glm::vec3(matrizAvion * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
		spotLights[4].SetFlash(posicionFaroFrontal, direccionFaroFrontal);


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[2]->RenderMeshModel();

		//Rover: Sustituir por su propio Rover con jerarquía
		//Modelo Inicial
		model = matrizRover;
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rover_Cuerpo.RenderModel();

		//Brazo
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.5f, 0.75f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Base_Brazo.RenderModel();

		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion1()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brazo_1.RenderModel();

		model = glm::translate(model, glm::vec3(2.7f, 2.7f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion2()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Brazo_2.RenderModel();

		model = glm::translate(model, glm::vec3(-2.75f, 2.75f, 0.1f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion3()), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Garra.RenderModel();

		//Pata delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.75f, -0.1f, 3.5f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion4()), glm::vec3(0.0f, 0.0f, 1.0f));
		matrizPata = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_DelanteraDer.RenderModel();

		//Rueda delantera derecha
		model = matrizPata;
		model = glm::translate(model, glm::vec3(-3.6f, -2.7f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_DelanteraDer.RenderModel();

		//Pata delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.75f, -0.1f, -2.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion5()), glm::vec3(0.0f, 0.0f, 1.0f));
		matrizPata = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_DelanteraIzq.RenderModel();

		//Rueda delantera izquierda
		model = matrizPata;
		model = glm::translate(model, glm::vec3(-3.6f, -2.7f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_DelanteraIzq.RenderModel();

		//Pata media derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, 3.4f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion6()), glm::vec3(0.0f, 0.0f, 1.0f));
		matrizPata = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_MediaDer.RenderModel();

		//Rueda media derecha
		model = matrizPata;
		model = glm::translate(model, glm::vec3(-1.5f, -2.1f, 1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_MediaDer.RenderModel();

		//Pata media izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -1.0f, -2.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion7()), glm::vec3(0.0f, 0.0f, 1.0f));
		matrizPata = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_MediaIzq.RenderModel();

		//Rueda media izquierda
		model = matrizPata;
		model = glm::translate(model, glm::vec3(-1.5f, -2.1f, -1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_MediaIzq.RenderModel();

		//Pata trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.8f, -1.0f, 3.4f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(0.0f, 0.0f, 1.0f));
		matrizPata = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_TraseraDer.RenderModel();

		//Rueda trasera derecha
		model = matrizPata;
		model = glm::translate(model, glm::vec3(1.5f, -2.1f, 1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_TraseraDer.RenderModel();

		//Pata trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(2.8f, -1.0f, -2.7f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(0.0f, 0.0f, 1.0f));
		matrizPata = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pata_TraseraIzq.RenderModel();

		//Rueda trasera izquierda
		model = matrizPata;
		model = glm::translate(model, glm::vec3(1.5f, -2.1f, -1.2f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion11()), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rueda_TraseraIzq.RenderModel();




		//Dado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 1.0f, 6.0));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CuboTexture.UseTexture();
		meshListModel[4]->RenderMeshModel();

		//Holocron
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 8.0f, 6.0));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HolocronTexture.UseTexture();
		meshListModel[5]->RenderMeshModel();


		//Añadir modelo del avión
		model = matrizAvion;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshListModel[3]->RenderMeshModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
