//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/Figuras.h"

#include "Headers/FirstPersonCamera.h"
//Texture includes
//Descomentar
#include "Headers/Texture.h"

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int screenWidth;
int screenHeight;

float r1 = 0.0, r2 = 0.0, r3 = 0.0, r4 = 0.0, r5 = 0.0;

GLFWwindow * window;


//Descomentar
Shader shader;


//TEXTURES
GLuint pisotxID;
GLuint paredtxID;
GLuint camatxID;
GLuint desktxID;
GLuint laptxID;
GLuint almohadatxID;
GLuint libro2txID;
GLuint lamparatxID;
GLuint librotxID;
GLuint ventanatxID;
GLuint guitarratxID;
GLuint colchontxID;
GLuint pesastxID;

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {
	
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.6f, 0.6f, 0.6f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	

	

	//Descomentar
	shader.initialize("../Shaders/texturizado2.vs", "../Shaders/texturizado.fs");

	

	pisoInit();
	paredInit();
	camaInit();
	deskInit();
	lapInit();
	almohadaInit();
	libro2Init();
	lamparaInit();
	libroInit();
	ventanaInit();
	guitarraInit();
	colchonInit();
	pesasInit();


	camera->setPosition(glm::vec3(0.0, 0.0, 6.0));

	// Descomentar
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	Texture pisoTexture("../Textures/piso_texture.png");
	bitmap = pisoTexture.loadImage();
	data = pisoTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &pisotxID);
	glBindTexture(GL_TEXTURE_2D, pisotxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	pisoTexture.freeImage(bitmap);

	Texture paredTexture("../Textures/pared_texture.png");
	bitmap = paredTexture.loadImage();
	data = paredTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &paredtxID);
	glBindTexture(GL_TEXTURE_2D, paredtxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	paredTexture.freeImage(bitmap);

	Texture camaTexture("../Textures/cama_texture.png");
	bitmap = camaTexture.loadImage();
	data = camaTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &camatxID);
	glBindTexture(GL_TEXTURE_2D, camatxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	camaTexture.freeImage(bitmap);

	Texture deskTexture("../Textures/desk_texture.png");
	bitmap = deskTexture.loadImage();
	data = deskTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &desktxID);
	glBindTexture(GL_TEXTURE_2D, desktxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	deskTexture.freeImage(bitmap);

	Texture lapTexture("../Textures/lap_texture.png");
	bitmap = lapTexture.loadImage();
	data = lapTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &laptxID);
	glBindTexture(GL_TEXTURE_2D, laptxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	lapTexture.freeImage(bitmap);

	Texture almohadaTexture("../Textures/almohada_texture.png");
	bitmap = almohadaTexture.loadImage();
	data = almohadaTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &almohadatxID);
	glBindTexture(GL_TEXTURE_2D, almohadatxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	almohadaTexture.freeImage(bitmap);

	Texture libro2Texture("../Textures/libro2_texture.png");
	bitmap = libro2Texture.loadImage();
	data = libro2Texture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &libro2txID);
	glBindTexture(GL_TEXTURE_2D, libro2txID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	libro2Texture.freeImage(bitmap);

	Texture lamparaTexture("../Textures/lampara_texture.png");
	bitmap = lamparaTexture.loadImage();
	data = lamparaTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &lamparatxID);
	glBindTexture(GL_TEXTURE_2D, lamparatxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	lamparaTexture.freeImage(bitmap);

	Texture libroTexture("../Textures/libro_texture.png");
	bitmap = libroTexture.loadImage();
	data = libroTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &librotxID);
	glBindTexture(GL_TEXTURE_2D, librotxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	libroTexture.freeImage(bitmap);

	Texture ventanaTexture("../Textures/ventana_texture.png");
	bitmap = ventanaTexture.loadImage();
	data = ventanaTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &ventanatxID);
	glBindTexture(GL_TEXTURE_2D, ventanatxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	ventanaTexture.freeImage(bitmap);

	Texture guitarraTexture("../Textures/guitarra_texture.png");
	bitmap = guitarraTexture.loadImage();
	data = guitarraTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &guitarratxID);
	glBindTexture(GL_TEXTURE_2D, guitarratxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	guitarraTexture.freeImage(bitmap);

	Texture colchonTexture("../Textures/colchon_texture.png");
	bitmap = colchonTexture.loadImage();
	data = colchonTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &colchontxID);
	glBindTexture(GL_TEXTURE_2D, colchontxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	colchonTexture.freeImage(bitmap);

	Texture pesasTexture("../Textures/pesas_texture.png");
	bitmap = pesasTexture.loadImage();
	data = pesasTexture.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &pesastxID);
	glBindTexture(GL_TEXTURE_2D, pesastxID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	pesasTexture.freeImage(bitmap);
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	

	shader.destroy();
}

void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, 0.005);
}

void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication){
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}
	TimeManager::Instance().CalculateFrameRate(false);
	deltaTime = TimeManager::Instance().DeltaTime;
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);

	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);

	if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		r1 += 0.001;
	if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		r2 += 0.001;
	if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		r3 += 0.001;
	if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		r4 += 0.001;
	if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		r5 += 0.001;
	if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		r1 -= 0.001;

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		

		//Descomentar
		shader.turnOn();
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));
		
		

		glm::mat4 modelRoot = glm::mat4(1.0f);
		glm::mat4 model;

		modelRoot = glm::translate(modelRoot, glm::vec3(0.0f, -1.5f, -1.5f));
		modelRoot = glm::rotate(modelRoot,glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		model = modelRoot;
		model = glm::translate(model, glm::vec3(0.0, 0.0, -0.0));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, pisotxID);
		glBindVertexArray(pisoVAO);
		glDrawElements(GL_TRIANGLES, pisoElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(-0.6624, 1.7545, 1.5833));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, paredtxID);
		glBindVertexArray(paredVAO);
		glDrawElements(GL_TRIANGLES, paredElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(0.6656, 0.2907, 0.7267));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, camatxID);
		glBindVertexArray(camaVAO);
		glDrawElements(GL_TRIANGLES, camaElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(-1.182, 0.766, 1.0009));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, desktxID);
		glBindVertexArray(deskVAO);
		glDrawElements(GL_TRIANGLES, deskElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(-1.2972, 1.0834, 1.6742));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, laptxID);
		glBindVertexArray(lapVAO);
		glDrawElements(GL_TRIANGLES, lapElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(0.6578, 0.9808, 1.7488));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, almohadatxID);
		glBindVertexArray(almohadaVAO);
		glDrawElements(GL_TRIANGLES, almohadaElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(-0.5086, 1.0043, 1.6927));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, libro2txID);
		glBindVertexArray(libro2VAO);
		glDrawElements(GL_TRIANGLES, libro2Elements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(-1.8761, 1.2926, 1.0355));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, lamparatxID);
		glBindVertexArray(lamparaVAO);
		glDrawElements(GL_TRIANGLES, lamparaElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(-0.569, 1.0606, 1.6322));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, librotxID);
		glBindVertexArray(libroVAO);
		glDrawElements(GL_TRIANGLES, libroElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(0.3629, 1.859, 2.4026));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, ventanatxID);
		glBindVertexArray(ventanaVAO);
		glDrawElements(GL_TRIANGLES, ventanaElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(-1.8825, 0.5452, -0.8227));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, guitarratxID);
		glBindVertexArray(guitarraVAO);
		glDrawElements(GL_TRIANGLES, guitarraElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(0.6656, 0.2907, 0.7267));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, colchontxID);
		glBindVertexArray(colchonVAO);
		glDrawElements(GL_TRIANGLES, colchonElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		model = modelRoot;
		model = glm::translate(model, glm::vec3(1.8796, 0.2534, -0.7052));
		shader.setMatrix4("model", 1, false, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, pesastxID);
		glBindVertexArray(pesasVAO);
		glDrawElements(GL_TRIANGLES, pesasElements, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);



		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
