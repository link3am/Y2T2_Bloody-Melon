#pragma once
#include "Graphics/VertexArrayObject.h"
#include "Gameplay/Transform.h"
#include <Graphics\Shader.h>
#include <entt\entt.hpp>
#include <GLFW\glfw3.h>
#include <Gameplay\Camera.h>
#include "Graphics/Texture2D.h"
class Stuff
{
public:
	Stuff(std::string obj,
		glm::vec3 location,
		glm::vec3 rotation,
		std::string UV
	);
	

	void Render(Camera::sptr cam, glm::vec3 lightpos, glm::vec3 camerapos);


	Transform::sptr stuffTrans = nullptr;
	VertexArrayObject::sptr stuffMod = nullptr;
	Shader::sptr stuffShader = nullptr;

	Texture2D::sptr stuffTex = nullptr;



};
