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

	glm::vec3 cameraAngle;
	glm::vec3 cameraRotation;
	float cameraSpeed = 0.01f;
	glm::vec3 cameraPosition;
	glm::vec3 background;

public:
	std::vector<Vertex> v;

	Scene::Scene();
	Scene::Scene(glm::vec3 cam);

	void FreeGeometry();
	void SetupGeometry();

	void Rotate(glm::vec3 rot);

	void AddEntity(Entity e);
	void AddEntities(Entity* e, int n);

	void SetBackground(glm::vec3 background);

	void Update();
	void Render(GLuint shaderprogram, GLuint vao);
};

#endif /* SCENE_H */