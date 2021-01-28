#pragma once

#include "Graphics/VertexArrayObject.h"
#include "Gameplay/Transform.h"
#include <Graphics\Shader.h>
#include <entt\entt.hpp>
#include <GLFW\glfw3.h>
#include <Gameplay\Camera.h>
#include "Graphics/Texture2D.h"
class UI
{
public:
	UI(VertexArrayObject::sptr UIM,
		glm::vec3 location,
		glm::vec3 rotation,
		glm::vec3 scale,
		Texture2D::sptr UIUV
	);

	void Render(Camera::sptr cam);

	Transform::sptr UITrans = nullptr;
	VertexArrayObject::sptr UIMod = nullptr;
	Shader::sptr UIShader = nullptr;

	Texture2D::sptr UITex = nullptr;
};

