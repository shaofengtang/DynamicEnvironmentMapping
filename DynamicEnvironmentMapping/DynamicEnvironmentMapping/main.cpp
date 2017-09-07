#include <iostream>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Camera.h"
#include "stb_image.h"

#include "mesh.h"
#include "model.h"
#include "light.h"
#include "TextRender.h"
#include "cubeTexture.h"


float deltaTime = 0.0f;
float lastFrame = 0.0f;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// test mesh data
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

Camera camera(glm::vec3(0.0f, 1.0f, 10.0f));

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void RenderWithBorder(Shader& objShader, Shader& borderShader, Model& model)
{
	borderShader.use();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	model.Draw(borderShader);

	glDisable(GL_CULL_FACE);
	objShader.use();
	model.Draw(objShader);
}

int main()
{
	// init flfw
	glfwInit();

	// set up opengl context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create main window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dynamic Environment Mapping", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init GLAD, load function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// register callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// shader configuration
	Shader texShader("Shaders//VertexShaderWithTex.vs", "Shaders//FragmentShaderWithTex.fs");
	Shader noTexShader("Shaders//VertexShaderWithoutTex.vs", "Shaders//FragmentShaderWithoutTex.fs");
	Shader textShader("Shaders//Char2D.vs", "Shaders//Char2D.fs");
	Shader ndcShader("Shaders//NDC.vs", "Shaders//NDC.fs");
	Shader skyboxShader("Shaders//Skybox.vs", "Shaders//Skybox.fs");
	Shader environmentShader("Shaders//EnvironmentMapping.vs", "Shaders//EnvironmentMapping.fs");

	// test mesh openGL setup
	// VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// push data to GPU
	// VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// linking vertex attributs
	// position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinate attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// texture
	unsigned int diffuseMap;
	glGenTextures(1, &diffuseMap);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("Texture//container2.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int specularMap;
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//int width, height, nrChannels;
	data = stbi_load("Texture//container2_specular.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// 
	float quad[] =
	{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int screenVAO;
	glGenVertexArrays(1, &screenVAO);
	glBindVertexArray(screenVAO);

	unsigned int screenVBO;
	glGenBuffers(1, &screenVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	// text renderer
	TextRender textRender;
	textRender.LoadFont("Fonts//arial.ttf");

	// model loading
	Model bunny("Models//bunny.obj");
	Model plane("Models//chopper.obj");
	Model box("Models//cube.obj");

	bunny.setMaterial(0, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.0f, 0.0f, 0.8f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);
	plane.setMaterial(0, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);

	texShader.use();
	texShader.setInt("material.texture_diffuse1", 0);
	texShader.setInt("material.texture_specular1", 1);
	texShader.setFloat("material.shininess", 32.0f);

	ndcShader.setInt("screenTexture", 0);

	// lights settings
	DirLight dirLight(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
	dirLight.SetupInShader(noTexShader, "dirLight");
	dirLight.SetupInShader(texShader, "dirLight");

	
	// create and bind FBO
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	unsigned int cubeBuffer;
	glGenTextures(1, &cubeBuffer);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeBuffer);
	for (int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach it to currently bound FBO
	for (int i = 0; i < 6; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeBuffer, 0);
	}
	

	// generate texture
	/*unsigned int rightBuffer;
	glGenTextures(1, &rightBuffer);
	glBindTexture(GL_TEXTURE_2D, rightBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rightBuffer, 0);

	unsigned int leftBuffer;
	glGenTextures(1, &leftBuffer);
	glBindTexture(GL_TEXTURE_2D, leftBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, leftBuffer, 0);

	unsigned int topBuffer;
	glGenTextures(1, &topBuffer);
	glBindTexture(GL_TEXTURE_2D, topBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, topBuffer, 0);

	unsigned int bottomBuffer;
	glGenTextures(1, &bottomBuffer);
	glBindTexture(GL_TEXTURE_2D, bottomBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, bottomBuffer, 0);

	unsigned int backBuffer;
	glGenTextures(1, &backBuffer);
	glBindTexture(GL_TEXTURE_2D, backBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, backBuffer, 0);

	unsigned int frontBuffer;
	glGenTextures(1, &frontBuffer);
	glBindTexture(GL_TEXTURE_2D, frontBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 600, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach it to currently bound FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, frontBuffer, 0);*/

	// generate renderbuffer (as depth buffer and stencil buffer)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 600, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// attach it to currently bound FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// check framebuffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// skybox
	/*std::vector<std::string> faces =
	{
		"Texture//mp_alpha//alpha-island_rt.tga",
		"Texture//mp_alpha//alpha-island_lf.tga",
		"Texture//mp_alpha//alpha-island_up.tga",
		"Texture//mp_alpha//alpha-island_dn.tga",
		"Texture//mp_alpha//alpha-island_bk.tga",
		"Texture//mp_alpha//alpha-island_ft.tga"
	};*/
	std::vector<std::string> faces =
	{
		"Texture//skybox//right.jpg",
		"Texture//skybox//left.jpg",
		"Texture//skybox//top.jpg",
		"Texture//skybox//bottom.jpg",
		"Texture//skybox//back.jpg",
		"Texture//skybox//front.jpg"
	};
	CubeTexture skybox(faces);

	glm::vec3 insideCameraPos;
	insideCameraPos.x = (bunny.m_minX + bunny.m_maxX) / 2.0f;
	insideCameraPos.y = (bunny.m_minY + bunny.m_maxY) / 2.0f;
	insideCameraPos.z = (bunny.m_minZ + bunny.m_maxZ) / 2.0f;
	Camera rightCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f);
	Camera leftCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -180.0f, 0.0f);
	Camera topCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -90.0f, 90.0f);
	Camera bottomCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -90.0f, -90.0f);
	Camera backCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), 90.0f, 0.0f);
	Camera frontCamera(insideCameraPos, glm::vec3(0.0f, -1.0f, 0.0f), -90.0f, 0.0f);

	std::vector<Camera*> cameraList = { &rightCamera, &leftCamera, &topCamera, &bottomCamera, &backCamera, &frontCamera };

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// model, view, projection matrix setup
		glm::mat4 projection;
		glm::mat4 view;
		glm::mat4 model;

		for (int i = 0; i < 6; ++i)
		{
			projection = glm::perspective(glm::radians(cameraList[i]->Zoom), (float)600 / (float)600, 0.1f, 100.0f);
			view = cameraList[i]->GetViewMatrix();
			model = glm::mat4();

			texShader.use();
			texShader.setMat4("projection", glm::value_ptr(projection));
			texShader.setMat4("view", glm::value_ptr(view));
			texShader.setMat4("model", glm::value_ptr(model));
			texShader.setVec3("viewPos", cameraList[i]->Position.x, cameraList[i]->Position.y, cameraList[i]->Position.z);

			noTexShader.use();
			noTexShader.setMat4("projection", glm::value_ptr(projection));
			noTexShader.setMat4("view", glm::value_ptr(view));
			noTexShader.setMat4("model", glm::value_ptr(model));
			noTexShader.setVec3("viewPos", cameraList[i]->Position.x, cameraList[i]->Position.y, cameraList[i]->Position.z);

			skyboxShader.use();
			skyboxShader.setMat4("projection", glm::value_ptr(projection));
			glm::mat4 skyboxView = glm::mat4(glm::mat3(cameraList[i]->GetViewMatrix()));
			skyboxShader.setMat4("view", glm::value_ptr(skyboxView));

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			// draw box on the right
			texShader.use();
			model = glm::mat4();
			model = glm::scale(model, glm::vec3(2.0f));
			model = glm::translate(model, glm::vec3(3.0f, 1.0f, 0.0f));
			texShader.setMat4("model", glm::value_ptr(model));
			glBindVertexArray(VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// draw bunny on the left
			noTexShader.use();
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
			noTexShader.setMat4("model", glm::value_ptr(model));
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			bunny.Draw(noTexShader);

			// draw the helicopter
			model = glm::mat4();
			float y = 10 * sin(currentFrame);
			float z = 10 * cos(currentFrame);
			model = glm::translate(model, glm::vec3(0.0f, y, z));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			noTexShader.setMat4("model", glm::value_ptr(model));
			plane.Draw(noTexShader);
			glDisable(GL_CULL_FACE);

			// sky box
			glDepthFunc(GL_LEQUAL);
			glDepthMask(GL_FALSE);
			skyboxShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.m_textureID);
			skybox.Draw(skyboxShader);
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);
			glDisable(GL_DEPTH_TEST);
		}
		
		// render final scene
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		model = glm::mat4();

		texShader.use();
		texShader.setMat4("projection", glm::value_ptr(projection));
		texShader.setMat4("view", glm::value_ptr(view));
		texShader.setMat4("model", glm::value_ptr(model));
		texShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		noTexShader.use();
		noTexShader.setMat4("projection", glm::value_ptr(projection));
		noTexShader.setMat4("view", glm::value_ptr(view));
		noTexShader.setMat4("model", glm::value_ptr(model));
		noTexShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		skyboxShader.use();
		skyboxShader.setMat4("projection", glm::value_ptr(projection));
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", glm::value_ptr(skyboxView));

		environmentShader.use();
		environmentShader.setMat4("projection", glm::value_ptr(projection));
		environmentShader.setMat4("view", glm::value_ptr(view));
		environmentShader.setMat4("model", glm::value_ptr(model));
		environmentShader.setVec3("cameraPos", camera.Position.x, camera.Position.y, camera.Position.z);
		environmentShader.setInt("skybox", 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// draw box on the right
		texShader.use();
		model = glm::mat4();
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::translate(model, glm::vec3(3.0f, 1.0f, 0.0f));
		texShader.setMat4("model", glm::value_ptr(model));
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// draw bunny on the left
		noTexShader.use();
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
		noTexShader.setMat4("model", glm::value_ptr(model));
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		bunny.Draw(noTexShader);

		// draw the helicopter
		model = glm::mat4();
		float y = 10 * sin(currentFrame);
		float z = 10 * cos(currentFrame);
		model = glm::translate(model, glm::vec3(0.0f, y, z));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		noTexShader.setMat4("model", glm::value_ptr(model));
		plane.Draw(noTexShader);
		glDisable(GL_CULL_FACE);

		// dynamic environment mapped bunny
		glEnable(GL_CULL_FACE);
		environmentShader.use();
		model = glm::mat4();
		environmentShader.setMat4("model", glm::value_ptr(model));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeBuffer);
		bunny.Draw(environmentShader);
		glDisable(GL_CULL_FACE);

		// sky box
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		skyboxShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.m_textureID);
		skybox.Draw(skyboxShader);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_DEPTH_TEST);

		// text rendering
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		std::string strFPS = "FPS: ";
		std::stringstream ss;
		float fps = 1.0f / deltaTime;
		ss << fps;
		textRender.RenderText(textShader, (strFPS + ss.str()).c_str(), 5.0f, 580.0f, 0.4f);
		glDisable(GL_BLEND);

		// check and call evnets and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	// clean up
	glfwTerminate();
	return 0;
}