#ifndef ENTITY_H
#define ENTITY_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"

// Sphere-related constants
#define TWOPI           6.283185307179586476925287
#define PID2            1.570796326794896619231322




class Entity {
	void push(Vertex &vertex);

public:
	GLuint vao, vbo;

	void FreeGeometry();
	void SetupGeometry();

	enum Shape { Sphere, Cone, Cylinder, Cube };

	glm::dvec3 position;
	glm::vec3 size;
	glm::vec3 angle;

	bool wiremesh;

	std::vector<Vertex> v;

	// Animation
	std::vector<GLuint> an_types, an_lengths;
	std::vector<glm::vec3> an_magnitudes;
	glm::vec3 mom_angular, mom_movement;
	GLint frame, animation;

	// - Functions -
	Entity(Shape shape);
	Entity(Shape shape, glm::vec3 p, glm::vec3 s, glm::vec3 a, int res, bool wiremesh);

	void CreateSimpleSphere(int n, bool wiremesh);
	void CreateCylinder(bool capped);
	void CreateCone(glm::vec3 c, GLfloat height, glm::vec3 n);

	void add_my_vertices(std::vector<Vertex> &vertices);

	glm::mat4 angle_matrix();
	glm::mat4 model_transform();

	void AddAnimation(GLuint type, GLuint length, glm::vec3 mag);
	void Animate(glm::vec3 pivot);

	void MoveModel(glm::dvec3 direction, glm::dvec3 p);
};

#endif /* ENTITY_H */