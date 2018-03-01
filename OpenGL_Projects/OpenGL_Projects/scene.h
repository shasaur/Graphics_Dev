#ifndef SCENE_H
#define SCENE_H

#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Vertex.h"
#include "entity.h"

class Scene {
	std::vector<Entity> en;

	float cameraAngle[3] = { 0,0,0 };
	float cameraRotation[3] = { 0,0,0 };
	float cameraSpeed = 0.01f;
	glm::vec3 cameraPosition;

public:
	std::vector<Vertex> v;

	Scene::Scene();
	void AddEntity(Entity e);
	void Render(GLuint shaderprogram, GLuint vao);
};

#endif /* SCENE_H */