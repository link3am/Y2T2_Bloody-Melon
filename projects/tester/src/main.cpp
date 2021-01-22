#include <Logging.h>
#include <iostream>
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//
#include <filesystem>
#include <json.hpp>
#include <fstream>
//
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
//vector
#include <vector>


#include "entt.hpp"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

#include "Graphics/IBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexArrayObject.h"
#include "Graphics/Shader.h"
#include "Gameplay/Transform.h"
#include "Graphics/MeshBuilder.h"
#include "Graphics/ObjLoader.h"
#include "Graphics/VertexTypes.h"
//textrue
#include "Graphics/Texture2D.h"
#include "Graphics/Texture2DData.h"
//level set
//#include "levelSet.h"
#include "stuff.h"
//Hit Cheak
#include "HitCheck.h"
using namespace std;

void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	std::string sourceTxt;
	switch (source) {
	case GL_DEBUG_SOURCE_API: sourceTxt = "DEBUG"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: sourceTxt = "WINDOW"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceTxt = "SHADER"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: sourceTxt = "THIRD PARTY"; break;
	case GL_DEBUG_SOURCE_APPLICATION: sourceTxt = "APP"; break;
	case GL_DEBUG_SOURCE_OTHER: default: sourceTxt = "OTHER"; break;
	}
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN("[{}] {}", sourceTxt, message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR("[{}] {}", sourceTxt, message); break;
#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO("[{}] {}", sourceTxt, message); break;
#endif
	default: break;
	}
}

GLFWwindow* window;
Camera::sptr camera = nullptr;
void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	//camera->ResizeWindow(width, height);
}

bool initGLFW() {
	if (glfwInit() == GLFW_FALSE) {
		LOG_ERROR("Failed to initialize GLFW");
		return false;
	}

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif

	//Create a new GLFW window
	window = glfwCreateWindow(800, 800, "TT", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set our window resized callback
	glfwSetWindowSizeCallback(window, GlfwWindowResizedCallback);

	return true;
}

bool initGLAD()
{
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		LOG_ERROR("Failed to initialize Glad");
		return false;
	}
	return true;
}


void RenderVAO(const Shader::sptr& shader, const VertexArrayObject::sptr& vao, const Camera::sptr& camera, const Transform::sptr& transform)
{
	shader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform->LocalTransform());
	shader->SetUniformMatrix("u_Model", transform->LocalTransform());
	vao->Render();
}



int main()
{
	vector<Enemy> EnemyList;
	vector<Bullet> BulletList;
	vector<Stuff> stuffList;
	Logger::Init(); // We'll borrow the logger from the toolkit, but we need to initialize it
	if (!initGLFW())
		return 1;
	if (!initGLAD())
		return 1;
	// Let OpenGL know that we want debug output, and route it to our handler function
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, nullptr);
	//textrue
	glEnable(GL_TEXTURE_2D);
	// GL states
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);


	static  entt::registry ecs;

	//////////////////////////////////////////mod and transform
	//melon = 1.6m
	Player p1("watermelon.obj",
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, -10.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),
		"images/melon UV.png");
	//enemyS
	{
		Enemy e1("knife.obj",
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -10.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			"images/knife UV.png");
		e1.setPatrolPoint(glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(8.0f, 0.0f, 0.0f));
		EnemyList.push_back(e1);

		Enemy e2("knife.obj",
			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -10.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			"images/knife UV.png");
		e2.setPatrolPoint(glm::vec3(5.0f, 4.0f, 0.0f), glm::vec3(11.0f, 4.0f, 0.0f));
		EnemyList.push_back(e2);
		Enemy e3("knife.obj",
			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -10.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			"images/knife UV.png");
		e3.setPatrolPoint(glm::vec3(22.f, 0.5f, 0.0f), glm::vec3(23.5f, 0.5f, 0.0f));
		EnemyList.push_back(e3);
		Enemy e4("knife.obj",
			glm::vec3(3.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -10.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			"images/knife UV.png");
		e4.setPatrolPoint(glm::vec3(12.f, 3.0f, 0.0f), glm::vec3(18.0f, 3.0f, 0.0f));
		EnemyList.push_back(e4);
		Enemy e5("knife.obj",
			glm::vec3(26.0f, 1.5f, 0.0f),
			glm::vec3(0.0f, -10.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			"images/knife UV.png");
		e5.setPatrolPoint(glm::vec3(25.6f, 1.5f, 0.0f), glm::vec3(26.4f, 1.5f, 0.0f));
		EnemyList.push_back(e5);
		Enemy e6("knife.obj",
			glm::vec3(26.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -10.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			"images/knife UV.png");
		e6.setPatrolPoint(glm::vec3(25.6f, 0.0f, 0.0f), glm::vec3(26.4f, 0.0f, 0.0f));
		EnemyList.push_back(e6);
		Enemy e7("knife.obj",
			glm::vec3(34.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -10.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, -1.0f),
			"images/knife UV.png");
		e7.setPatrolPoint(glm::vec3(34.6f, 0.0f, 0.0f), glm::vec3(36.4f, 0.0f, 0.0f));
		EnemyList.push_back(e7);
	}
	//bullet
	VertexArrayObject::sptr BulletMod = ObjLoader::LoadFromFile("seeds.obj");
	Texture2D::sptr BulletTex = Texture2D::LoadFromFile("images/seeds UV.png");

	//level set
	{
		Stuff deskTop("mapping.obj",
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/table UV.jpg");
		stuffList.push_back(deskTop);
		Stuff s1("clock.obj",
			glm::vec3(2.0f, 0.0f, -0.7f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/clock UV.png");
		stuffList.push_back(s1);
		Stuff s2("cutting.obj",
			glm::vec3(8.0f, 0.0f, -0.5f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/cutting UV.jpg");
		stuffList.push_back(s2);
		Stuff s3("microwave.obj",
			glm::vec3(15.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/microwave UV.png");
		stuffList.push_back(s3);
		Stuff s4("can.obj",
			glm::vec3(19.4f, 0.0f, -0.5f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/can UV.png");
		stuffList.push_back(s4);
		Stuff s5("dish.obj",
			glm::vec3(23.f, 0.0f, -1.5f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/dish UV.png");
		stuffList.push_back(s5);
		Stuff s6("dish.obj",
			glm::vec3(23.f, 0.5f, -1.5f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/dish UV.png");
		stuffList.push_back(s6);
		Stuff s7("can.obj",
			glm::vec3(26.0f, 0.0f, -0.5f),
			glm::vec3(0.0f, 40.0f, 0.0f),
			"images/can UV.png");
		stuffList.push_back(s7);
		Stuff s8("pan.obj",
			glm::vec3(30.0f, 0.0f, -3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			"images/pan UV.png");
		stuffList.push_back(s8);
	}
	
	//////////////////////////////////////////camera
	camera = Camera::Create();
	glm::vec3 cameraPosition = glm::vec3(0, 3, 7);
	camera->SetPosition(cameraPosition); // Set initial position
	camera->SetUp(glm::vec3(0, 1, 0)); // Use a z-up coordinate system
	camera->LookAt(glm::vec3(0.0f, 1.0f, 0.0f)); // Look at center of the screen
	camera->SetFovDegrees(90.0f); // Set an initial FOV
	camera->SetOrthoHeight(3.0f);

	
	p1.glWindow(window);
	//////////////////////////////////////////delta time
	double lastFrame = glfwGetTime();
	double lastFrameTime = glfwGetTime();
	double timer = glfwGetTime();

	///////////shooting speed limit
	double lastFrame2 = glfwGetTime();
	double lastFrameTime2 = glfwGetTime();
	bool CanShoot = true;
	const double shootLimit = 1.0/3.0;


	const double fpsLimit = 1.0 / 60.0;




	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//DT
		double thisFrame = glfwGetTime();
		float dt = static_cast<float>(thisFrame - lastFrame);// delta time

		double thisFrame2 = glfwGetTime();
		float dt2 = static_cast<float>(thisFrame2 - lastFrame2);// delta time

		glClearColor(0.08f, 0.17f, 0.31f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//shader
		//Lv1.levelRender(camera);
		for (int s = 0; s < stuffList.size(); s++) {
				stuffList[s].Render(camera);

		}
		
		//
		if (!p1.IsDeath()) {
			p1.Render(camera);
			//p1.blocker();

		}
		for (int i = 0; i < EnemyList.size(); i++) {
			EnemyList[i].Render(camera);
			EnemyList[i].AIPatrol();
		}
		for (vector<Bullet>::iterator it = BulletList.begin(); it != BulletList.end();)
		{
			it->Render(camera);
			it->projectile();
			if (it->IsDeath() == true)
				it = BulletList.erase(it);
			else {
				it++;
			}
		}

		for (vector<Enemy>::iterator it = EnemyList.begin(); it != EnemyList.end();)
		{
			if (it->IsDeath() == true)
				it = EnemyList.erase(it);
			else {
				it++;
			}
		}
		if ((thisFrame2 - lastFrameTime2)> shootLimit) {
			CanShoot = true;
		}

		

		//fps limit in this if()
		if ((thisFrame - lastFrameTime) >= fpsLimit)
		{
			//player part
			if (!p1.IsDeath()) {
				p1.phyUpdate(dt);
				camera->cameraMove(window, p1.getPlayervec3());
				if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && CanShoot) {
					if (p1.melonTrans->GetLocalScale().x > 0) {
						Bullet b(BulletMod,
							p1.melonTrans->GetLocalPosition(),
							glm::vec3(0.0f, -10.0f, 0.0f),
							glm::vec3(1.0f, 1.0f, -1.0f),
							BulletTex,
							true);
						BulletList.push_back(b);
					}
					else {
						Bullet b(BulletMod,
							p1.melonTrans->GetLocalPosition(),
							glm::vec3(0.0f, -10.0f, 0.0f),
							glm::vec3(-1.0f, 1.0f, -1.0f),
							BulletTex,
							false);
						BulletList.push_back(b);
					}
					lastFrameTime2 = thisFrame2;
					CanShoot = false;
				}

				for (int y = 0; y < EnemyList.size(); y++) {
					if (HitCheck::AABB(p1.getHitBox(), EnemyList[y].getHitBox())) {
						p1.death = true;
						cout << "Game Over!" << endl;
					}
				}

				
			}
			for (int x = 0; x < BulletList.size(); x++) {
				for (int y = 0; y < EnemyList.size(); y++) {
					if (HitCheck::AABB(BulletList[x].getHitBox(), EnemyList[y].getHitBox())) {
						BulletList[x].death = true;
						EnemyList[y].death = true;
					}
				}
			}

			lastFrameTime = thisFrame;
		}
		glfwSwapBuffers(window);

		timer = glfwGetTime();
		//std::cout << timer2 <<std::endl;
		lastFrame = thisFrame;
		lastFrame2 = thisFrame2;
	}


	// Clean up the toolkit logger so we don't leak memory
	Logger::Uninitialize();
	return 0;
} 
