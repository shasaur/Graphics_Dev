#ifndef ENTITY_H
#define ENTITY_H

#define GLEW_STATIC
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include "Vertex.h"

// Sphere-related constants
#define TWOPI           6.283185307179586476925287
#define PID2            1.570796326794896619231322




class Entity {
public:

	enum Shape { Sphere, Cone, Cylinder, Cube };

	glm::vec3 position;
	glm::vec3 size;
	glm::vec3 angle;

	std::vector<Vertex> v;

	Entity(Shape shape);
	Entity(Shape shape, glm::vec3 p, glm::vec3 s, glm::vec3 a, int res);

	void CreateSimpleSphere(glm::vec3 c, double r, int n);

	void add_my_vertices(std::vector<Vertex> &vertices);
	glm::mat4 model_transform();
};

#endif /* ENTITY_H */