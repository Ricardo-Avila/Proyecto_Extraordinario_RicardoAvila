//
// Profesor: Ing. Jose Roque Roman Guadarrama
// Materia: Extraordinario II 2025-1
// Semestre 2025-1
// Alumnos:
// Avila Laguna Ricardo




// Cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <chrono>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

// Iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;


// Variables de animación 
float ciclo = 0.0f;
float luminucidad;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;


// Modelos
Model piso_M;
Model agua_M;
Model palmera_01_M;
Model palmera_02_M;
Model puerto_M;
Model faro_M;
Model ventanasfaro_M;
Model arco_M;
Model gaviota_M;
Model cangrejo_M;
Model atun_M;
Model salmon_M;
Model coral_M;
Model bote_M;
Model balde_M;
Model quinque_M;
Model vidrio_quinque_M;
Model caña_M;
Model animacion_gaviota_M;


//Skybox
Skybox skybox;

// Materiales
Material Material_brillante;
Material Material_opaco;


//Variables Tiempo
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Luz direccional
DirectionalLight mainLight;

// Luces de tipo pointlight y spotlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearObjetos()
{
	unsigned int pisoIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat pisoVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(pisoVertices, pisoIndices, 32, 12);
	meshList.push_back(obj1);

}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateShaders();
	CrearObjetos();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.9f, 0.6f);

	// Modelos
	piso_M = Model();
	piso_M.LoadModel("Modelos/Piso/piso.obj");
	agua_M = Model();
	agua_M.LoadModel("Modelos/Piso/plano.obj");
	
	palmera_01_M = Model();
	palmera_01_M.LoadModel("Modelos/Objetos/palmera_01.obj");

	palmera_02_M = Model();
	palmera_02_M.LoadModel("Modelos/Objetos/palmera_02.obj");

	puerto_M = Model();
	puerto_M.LoadModel("Modelos/Objetos/puerto.obj");

	faro_M = Model();
	faro_M.LoadModel("Modelos/Faro/faro.obj");
	ventanasfaro_M = Model();
	ventanasfaro_M.LoadModel("Modelos/Faro/ventanas.obj");

	arco_M = Model();
	arco_M.LoadModel("Modelos/Arco/arco.obj");

	gaviota_M = Model();
	gaviota_M.LoadModel("Modelos/Aire/gaviota.obj");

	cangrejo_M = Model();
	cangrejo_M.LoadModel("Modelos/Agua/cangrejo.obj");

	atun_M = Model();
	atun_M.LoadModel("Modelos/Agua/atun.obj");

	salmon_M = Model();
	salmon_M.LoadModel("Modelos/Agua/salmon.obj");

	coral_M = Model();
	coral_M.LoadModel("Modelos/Agua/coral.obj");

	bote_M = Model();
	bote_M.LoadModel("Modelos/Bote/bote.obj");

	balde_M = Model();
	balde_M.LoadModel("Modelos/Bote/balde.obj");

	quinque_M = Model();
	quinque_M.LoadModel("Modelos/Bote/quinque.obj");

	vidrio_quinque_M = Model();
	vidrio_quinque_M.LoadModel("Modelos/Bote/vidrio_quinque.obj");

	caña_M = Model();
	caña_M.LoadModel("Modelos/Bote/caña.obj");

	animacion_gaviota_M = Model();
	animacion_gaviota_M.LoadModel("Modelos/gaviota_animacion.fbx");
	//animacion_gaviota_M.RenderModel(&Ligth);


	// Skybox
	std::vector<std::string> skyboxFaces;

	skyboxFaces.push_back("Skybox/der.tga");
	skyboxFaces.push_back("Skybox/izq.tga");
	skyboxFaces.push_back("Skybox/abajo.tga");
	skyboxFaces.push_back("Skybox/arriba.tga");
	skyboxFaces.push_back("Skybox/atras.tga");
	skyboxFaces.push_back("Skybox/frente.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		//0.0f + luminucidad, 0.0f + luminucidad,
		0.6f, 0.6f,
		0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 0.5f, 0.0f,
		5.0f, 3.0f,
		-18.0f, 2.0f, 63.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	/*
	pointLights[1] = PointLight(1.0f, 0.0f, 0.0f,
		5.0f, 3.0f,
		-59.0f, 42.5f, 11.0f, 
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[2] = PointLight(0.0f, 1.0f, 0.0f,
		5.0f, 3.0f,
		-54.0f, 41.5f, -15.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	*/
	unsigned int spotLightCount = 0;
	//linterna
	
	spotLights[0] = SpotLight(0.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	spotLights[1] = SpotLight(1.0f, 1.0f, 0.5f,
		3.0f, 0.5f,
		29.0f, 98.0f, 146.0f,
		-0.5f, 0.0f, 0.5f,
		1.0f, 0.0f, 0.0f,
		28.0f);
	spotLightCount++;

	/*
	spotLights[2] = SpotLight(1.0f, 0.5f, 0.0f,
		2.0f, 2.0f,
		-20.0f, 0.0f, 50.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	spotLights[3] = SpotLight(1.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		-10.0f, 30.0f, 55.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	*/

	/////////////////////////
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);

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
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);


		// Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piso_M.RenderModel();


		//Objetos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		palmera_01_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		palmera_02_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		puerto_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		faro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		arco_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		gaviota_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cangrejo_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		atun_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		salmon_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		coral_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		bote_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		balde_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		quinque_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		caña_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		animacion_gaviota_M.RenderModel();


		//ANIMACIONES
		
		ciclo = fmod(lastTime, 10.0); // Cada ciclo completo dura 80 unidades de tiempo

		if (mainWindow.getAnimacionLuminocidad() && ciclo <= 40.0) {
			// Fase creciente de iluminocidad (de 0.1 a 0.7)
			luminucidad = 0.1 + (0.7 - 0.1) * (ciclo / 40.0);
		}
		else {
			// Fase decreciente de iluminocidad (de 0.7 a 0.1)
			luminucidad = 0.7 - (0.7 - 0.1) * ((ciclo - 40.0) / 40.0);
		}


		// Opcional: imprimir el valor de la variable para verificar el resultado 
		std::cout << "lastTime: " << lastTime << ", iluminocidad: " << luminucidad << std::endl;
		
		// Objetos traslucidos 
		

		//Agua Traslucida
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		ventanasfaro_M.RenderModel();
		glDisable(GL_BLEND);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		agua_M.RenderModel();
		glDisable(GL_BLEND);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		vidrio_quinque_M.RenderModel();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
