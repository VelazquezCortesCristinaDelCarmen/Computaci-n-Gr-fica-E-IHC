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
Texture holocronLogos;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model Holocron_M;
Model Holocron;
Model Avion;

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
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.26f,  0.34f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.49f,	0.34f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.49f,	0.66f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.26f,	0.66f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.0f,  0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	1.0f,	0.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	1.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.0f,	1.0f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		1.0f,	0.0f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		1.0f,	1.0f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	1.0f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,  0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	1.0f,	0.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	1.0f,	1.0f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	1.0f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.0f,  0.0f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,	0.0f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	1.0f,	1.0f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.0f,	1.0f,		0.0f,	-1.0f,	0.0f,

	};

	MeshModel* dado = new MeshModel();
	dado->CreateMeshModel(cubo_vertices, cubo_indices, 192, 36);
	meshListModel.push_back(dado);

}

void CrearHolocron()
{
	unsigned int holocron_indices[] = {
		// FRONT
		0, 1, 2,
		2, 3, 0,
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
		16, 17, 18,
		18, 19, 16,
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
		//x		y		z		S		T
		0.5f, -0.5f,  0.0f,		0.66f,  0.62f,		-1.0f,	0.0f,	0.0f,
		0.5f, 0.0f,  -0.5f,		0.66f,	0.78f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.0f,		0.53f,	0.78f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.0f,  0.5f,		0.53f,	0.62f,		-1.0f,	0.0f,	0.0f,
		// back
		0.0f, -0.5f, -0.5f,		0.47f,  0.26f,		0.0f,	0.0f,	1.0f,
		-0.5f, 0.0f, -0.5f,		0.47f,	0.42f,		0.0f,	0.0f,	1.0f,
		0.0f,  0.5f, -0.5f,		0.34f,	0.42f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.0f, -0.5f,		0.34f,	0.26f,		0.0f,	0.0f,	1.0f,
		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  0.0f,	0.81f, 0.82f,		-1.0f, 0.0f, 0.0f,
		-0.5f,  0.0f,  0.5f,	0.92f, 0.82f,		-1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.0f,	0.92f, 0.97f,		-1.0f, 0.0f, 0.0f,
		-0.5f,  0.0f, -0.5f,	0.81f, 0.97f,		-1.0f, 0.0f, 0.0f,
		// bottom
		//x		y		z		S		T
		0.0f, -0.5f,  0.5f,		0.21f,  0.62f,		0.0f,	1.0f,	0.0f,
		0.5f, -0.5f,  0.0f,		0.34f,	0.62f,		0.0f,	1.0f,	0.0f,
		0.0f, -0.5f, -0.5f,		0.34f,	0.79f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f, 0.0f,		0.21f,	0.79f,		0.0f,	1.0f,	0.0f,
		//UP
		//x		y		z		S		T
		0.0f, 0.5f, 0.5f,		0.66f,  0.82f,		0.0f,	-1.0f,	0.0f,
		0.5f, 0.5f, 0.0f,		0.66f,	0.98f,		0.0f,	-1.0f,	0.0f,
		0.0f, 0.5f, -0.5f,		0.55f,	0.98f,		0.0f,	-1.0f,	0.0f,
		-0.5f, 0.5f, 0.0f,		0.55f,	0.82f,		0.0f,	-1.0f,	0.0f,
		//Esq 1 Frente - Arriba - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, 0.5f,		0.76f,	0.78f,		0.577f,	0.577f,	0.577f,
		0.5f, 0.0f, 0.5f,		0.86f,	0.62f,		0.577f,	0.577f,	0.577f,
		0.5f, 0.5f, 0.0f,		0.96f,	0.78f,		0.577f,	0.577f,	0.577f,
		//Esq 2 Frente - Arriba - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, 0.5f,		0.2f,	0.2f,		-0.577f,	0.577f,	0.577f,
		-0.5f, 0.5f, 0.0f,		0.05f,	0.2f,		-0.577f,	0.577f,	0.577f,
		-0.5f, 0.0f, 0.5f,		0.12f,	0.02f,		-0.577f,	0.577f,	0.577f,
		//Esq 3 Frente - Abajo - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, 0.5f,		0.16f,	0.4f,		0.577f,	-0.577f,	0.577f,
		0.5f, -0.5f, 0.0f,		0.36f,	0.4f,		0.577f,	-0.577f,	0.577f,
		0.5f, 0.0f, 0.5f,		0.26f,	0.20f,		0.577f,	-0.577f,	0.577f,
		//Esq 4 Frente - Abajo - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, 0.5f,		0.7f,	0.44f,		-0.577f,	-0.577f,	0.577f,
		-0.5f, 0.0f, 0.5f,		0.57f,	0.6f,		-0.577f,	-0.577f,	0.577f,
		-0.5f, -0.5f, 0.0f,		0.45f,	0.44f,		-0.577f,	-0.577f,	0.577f,
		//Esq 5 Atras - Arriba - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, -0.5f,		0.66f,	0.2f,		0.577f,	0.577f,	-0.577f,
		0.5f, 0.5f, 0.0f,		0.83f,	0.2f,		0.577f,	0.577f,	-0.577f,
		0.5f, 0.0f, -0.5f,		0.75f,	0.07f,		0.577f,	0.577f,	-0.577f,
		//Esq 6 Atras - Arriba - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 0.5f, -0.5f,		0.83f,	0.78f,		-0.577f,	0.577f,	-0.577f,
		-0.5f, 0.0f, -0.5f,		0.73f,	0.63f,		-0.577f,	0.577f,	-0.577f,
		-0.5f, 0.5f, 0.0f,		0.62f,	0.78f,		-0.577f,	0.577f,	-0.577f,
		//Esq 7 Atras - Abajo - Derecha
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, -0.5f,		0.75f,	0.25f,		0.577f,	-0.577f,	-0.577f,
		0.5f, 0.0f, -0.5f,		0.85f,	0.43f,		0.577f,	-0.577f,	-0.577f,
		0.5f, -0.5f, 0.0f,		0.95f,	0.25f,		0.577f,	-0.577f,	-0.577f,
		//Esq 8 Atras - Abajo - Izquierda
		//x		y		z		S		T			NX		NY		NZ
		0.0f, -0.5f, -0.5f,		0.95f,	0.2f,		-0.577f,	-0.577f,	-0.577f,
		-0.5f, -0.5f, 0.0f,		0.77f,	0.2f,		-0.577f,	-0.577f,	-0.577f,
		-0.5f, 0.0f, -0.5f,		0.87f,	0.02f,		-0.577f,	-0.577f,	-0.577f,
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

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado-de-numeros.png");
	dadoTexture.LoadTextureA();
	holocronTexture = Texture("Textures/holocronunwrap.png");
	holocronTexture.LoadTextureA();
	holocronLogos = Texture("Textures/logos.jpg");
	holocronLogos.LoadTextureA();
	Holocron_M = Model();
	Holocron_M.LoadModel("Models/holocron_simple.obj");
	Holocron = Model();
	Holocron.LoadModel("Models/holocron.obj");
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


		/*Reporte de práctica :
		
		Ejercicio 1: Crear o modificar el holocron y texturizarlo por medio de código
		Ejercicio 2: Importar el modelo del holocron texturizardo en el programa de modelado
		Ejercicio 3: Importar un modelo de avión con con la textura de la cara del personaje de la imagen del previo:
		Vidrio fonrtal: OJOS
		Frente del avión: Nariz y Sonrisa
		Alas: Logos del universo del personaje
		*/

		/*
		//Holocrones
		color = glm::vec3(1.0f, 0.0f, 1.0f);//color rosa que multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, 6.5f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		holocronLogos.UseTexture();
		meshListModel[5]->RenderMeshModel();
		
		color = glm::vec3(0.5f, 0.0f, 0.5f);//color rojo, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-16.5f, 6.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Holocron.RenderModel();
		*/

		//Avion
		color = glm::vec3(1.0f, 1.0f, 1.0f);//color rojo, multiplica a la información de color de la textura
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.5f, 6.5f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Avion.RenderModel();
	
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