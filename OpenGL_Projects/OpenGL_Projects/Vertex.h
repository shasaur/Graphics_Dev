#ifndef VERTEX_H
#define VERTEX_H

#define GLEW_STATIC
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


struct Vertex {
	GLfloat position[3];
	GLfloat color[3];
	GLfloat normal[3];
	GLfloat angle[3];
};


void randomiseColour(Vertex &vertex);
void setColour(Vertex &vertex, GLfloat r, GLfloat g, GLfloat b);
void setColour(Vertex &vertex, Vertex &sampleVertex);
void setPosition(Vertex &vertex, GLfloat x, GLfloat y, GLfloat z);
void setNormal(Vertex &vertex, GLfloat x, GLfloat y, GLfloat z);

#endif /* VERTEX_H */