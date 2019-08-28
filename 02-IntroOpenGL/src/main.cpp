//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

int screenWidth;
int screenHeight;

// Vertices struct
typedef struct _Vertex {
	float m_pos[3];
	float m_color[3];
} Vertex;

GLFWwindow * window;

bool exitApp = false;
int lastMousePosX;
int lastMousePosY;

double deltaTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow* Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

GLint vertexShaderID, fragmentShaderID, shaderProgramID;
//Variables para VAOs,VBOs,EBOs
GLuint VAO[2], VBO[2], EBO[2];
int currentVao = 1;
int nElements[2];

// Codigo de los shaders, por ahora se crean en una cadena de texto
// Shader de vertices
const GLchar * vertexShaderSource = "#version 330 core\n"
		"layout (location=0) in vec3 in_position;\n"
		"layout(location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main(){\n"
		"gl_Position = vec4(in_position, 1.0);\n"
		"ourColor = aColor;\n"
		"}\0";
// Shader de fragmento
const GLchar * fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;\n"
		"in vec3 ourColor;\n"
		"void main(){\n"
		"color = vec4(ourColor, 1.0);\n"
		"}\0";

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
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

	// Compilacion de los shaders

	// Se crea el id del Vertex Shader
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	// Se agrega el codigo fuente al ID
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
	// Compilación de Vertex Shader
	glCompileShader(vertexShaderID);
	GLint success;
	GLchar infoLog[512];
	// Se obtiene el estatus de la compilacion del vertex shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if(!success){
		// En caso de error se obtiene el error y lanza mensaje con error
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el VERTEX_SHADER." << infoLog << std::endl;
	}

	// Se crea el id del fragment Shader
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	// Se agrega el codigo fuente al ID
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
	// Compilación de Fragment Shader
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		// En caso de error se obtiene el error y lanza mensaje con error
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "Error al compilar el FRAGMENT_SHADER." << infoLog << std::endl;
	}

	// Programa con los shaders
	shaderProgramID = glCreateProgram();
	// Se agregan el vertex y fragment shader al program
	glAttachShader(shaderProgramID, vertexShaderID);
	glAttachShader(shaderProgramID, fragmentShaderID);
	// Proceso de linkeo
	glLinkProgram(shaderProgramID);
	// Revision de error de linkeo del programa
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR al linkear el programa." << infoLog << std::endl;
	}

	// Se definen los vertices de la geometria a dibujar
	Vertex verticesEstrella[] =
		{
		{ { 0.0f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f }},
		{ { 0.2f, 0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ { 0.0f, 0.8f, 0.0f}, { 0.0f, 0.5f, 1.0f }},
		{ {-0.2f, 0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ {-0.6f, 0.2f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ {-0.8f, 0.0f, 0.0f}, { 0.0f, 0.5f, 1.0f }},
		{ {-0.6f,-0.2f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ {-0.2f,-0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ { 0.0f,-0.8f, 0.0f}, { 0.0f, 0.5f, 1.0f }},
		{ { 0.2f,-0.6f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ { 0.6f,-0.2f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		{ { 0.8f, 0.0f, 0.0f}, { 0.0f, 0.5f, 1.0f }},
		{ { 0.6f, 0.2f, 0.0f}, { 0.0f, 0.0f, 1.0f }}
		};
	unsigned int indicesEstrella[] = {
		0, 1, 2,
		2, 3, 0,
		0, 4, 5,
		5, 6, 0,
		0, 7, 8,
		8, 9, 0,
		0, 10, 11,
		11, 12, 0
	};

	nElements[0] = sizeof(indicesEstrella) / sizeof(int);

	Vertex verticesCasa[] =
	{
		//Casa
	{ {-0.4f,-0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f }},
	{ { 0.4f,-0.5f, 0.0f}, { 1.0f, 1.0f, 1.0f }},
	{ { 0.4f, 0.2f, 0.0f}, { 1.0f, 1.0f, 1.0f }},
	{ {-0.4f, 0.2f, 0.0f}, { 1.0f, 1.0f, 1.0f }},
		//Chimenea
	{ { 0.16f, 0.24f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
	{ { 0.26f, 0.24f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
	{ { 0.26f, 0.44f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
	{ { 0.16f, 0.44f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
		//Chimeneatope
	{ { 0.14f, 0.44f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
	{ { 0.28f, 0.44f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
	{ { 0.28f, 0.47f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
	{ { 0.14f, 0.47f, 0.0f}, { 0.8f, 0.2f, 0.2f }},
		//techo
	{ { 0.45f, 0.175f, 0.0f}, { 1.0f, 0.7f, 0.5f }},
	{ { 0.0f, 0.5f, 0.0f}, { 1.0f, 0.7f, 0.5f }},
	{ {-0.45f, 0.175f, 0.0f}, { 1.0f, 0.8f, 0.5f }},
		//ventanaTecho
	{ {-0.06f, 0.25f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
	{ { 0.06f, 0.25f, 0.0f}, { 0.5f, 0.5f, 1.0f }},
	{ { 0.06f, 0.35f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
	{ {-0.06f, 0.35f, 0.0f}, { 0.5f, 0.5f, 1.0f }},
		//Puerta
	{ {-0.3f,-0.5f, 0.0f}, { 0.6f, 0.3f, 0.1f }},
	{ {-0.1f,-0.5f, 0.0f}, { 0.6f, 0.3f, 0.1f }},
	{ {-0.1f,-0.1f, 0.0f}, { 0.6f, 0.3f, 0.1f }},
	{ {-0.3f,-0.1f, 0.0f}, { 0.6f, 0.3f, 0.1f }},
		//Ventana
	{ { 0.1f,-0.3f, 0.0f}, { 0.0f, 0.5f, 1.0f }},
	{ { 0.3f,-0.3f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
	{ { 0.3f,-0.1f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
	{ { 0.1f,-0.1f, 0.0f}, { 0.0f, 0.0f, 1.0f }},
		//PuertaInt
	{ {-0.28f,-0.48f, 0.0f}, { 0.8f, 0.7f, 0.5f }},
	{ {-0.12f,-0.48f, 0.0f}, { 0.8f, 0.7f, 0.5f }},
	{ {-0.12f,-0.12f, 0.0f}, { 0.8f, 0.7f, 0.5f }},
	{ {-0.28f,-0.12f, 0.0f}, { 0.8f, 0.7f, 0.5f }},
		//VentanaRepisa
	{ { 0.05f,-0.35f, 0.0f}, { 0.8f, 0.7f, 0.7f }},
	{ { 0.35f,-0.35f, 0.0f}, { 0.8f, 0.7f, 0.7f }},
	{ { 0.35f,-0.30f, 0.0f}, { 0.8f, 0.7f, 0.7f }},
	{ { 0.05f,-0.30f, 0.0f}, { 0.8f, 0.7f, 0.7f }},
	};

	unsigned int indicesCasa[] = {
		//Casa
		0, 1, 2,
		2, 3, 0,
		//chimenea
		4, 5, 6,
		6, 7, 4,
		//chimeneatope
		8, 9, 10,
		10, 11, 8,
		//techo
		12, 13, 14,
		//ventanaTecho
		15, 16, 17,
		17, 18, 15,
		//Puerta
		19, 20, 21,
		21, 22, 19,
		//ventana
		23, 24, 25,
		25, 26, 23,
		//puertaInt
		27, 28, 29,
		29, 30, 27,
		//Ventana Repisa
		31, 32, 33,
		33, 34, 31
		
		
	};

	nElements[1] = sizeof(indicesCasa) / sizeof(int);
	// Se crea el ID del VAO
	/*
	El VAO es un objeto que nos permite almacenar la estructura de nuestros datos,
	Esto es de gran utilidad debido a que solo se configura la estructura una vez
	y se puede utilizar en el loop de renderizado
	*/
	glGenVertexArrays(2, &VAO[0]);
	// Cambiamos el estado para indicar que usaremos el id del VAO
	glBindVertexArray(VAO[0]);
	// Se crea el VBO (buffer de datos) asociado al VAO
	glGenBuffers(2, &VBO[0]);
	glGenBuffers(2, &EBO[0]);

	// Cambiamos el estado para indicar que usaremos el id del VBO como Arreglo de vertices (GL_ARRAY_BUFFER)
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	// Copiamos los datos de los vertices a memoria del procesador grafico
	//           TIPO DE BUFFER     TAMANIO          DATOS    MODO (No cambian los datos)
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesEstrella), verticesEstrella, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesEstrella), indicesEstrella, GL_STATIC_DRAW);

	// Se crea un indice para el atributo del vertice posicion, debe corresponder al location del atributo del shader
	// indice del atributo, Cantidad de datos, Tipo de dato, Normalizacion, Tamanio del bloque (Stride), offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	// Se habilita el atributo del vertice con indice 0 (posicion)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCasa), verticesCasa, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCasa), indicesCasa, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Ya que se configuro, se regresa al estado original
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	glUseProgram(0);
	glDetachShader(shaderProgramID, vertexShaderID);
	glDetachShader(shaderProgramID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glDeleteProgram(shaderProgramID);

	glBindVertexArray(VAO[0]);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER,VBO[0]);
	glDeleteBuffers(1, &VBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO[0]);
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
		case GLFW_KEY_E:
			currentVao = 0;
			break;
		case GLFW_KEY_C:
			currentVao = 1;
			break;
		}
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	lastMousePosX = xpos;
	lastMousePosY = ypos;
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
	deltaTime = 1 / TimeManager::Instance().CalculateFrameRate(false);
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;
	while (psi) {
		psi = processInput(true);
		glClear(GL_COLOR_BUFFER_BIT);

		// Esta linea esta comentada debido a que de momento no se usan los shaders
		glUseProgram(shaderProgramID);

		// Se indica el buffer de datos y la estructura de estos utilizando solo el id del VAO
		glBindVertexArray(VAO[currentVao]);
		// Primitiva de ensamble
		glDrawElements(GL_TRIANGLES, nElements[currentVao], GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
}

int main(int argc, char ** argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
