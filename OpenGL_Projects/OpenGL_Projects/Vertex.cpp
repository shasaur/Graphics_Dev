#include "Vertex.h"


void randomiseColour(Vertex &vertex) {
	vertex.color[0] = (float)(rand() % 100) / 100.f;
	vertex.color[1] = (float)(rand() % 100) / 100.f;
	vertex.color[2] = (float)(rand() % 100) / 100.f;
}

void setColour(Vertex &vertex, GLfloat r, GLfloat g, GLfloat b) {
	vertex.color[0] = r;
	vertex.color[1] = g;
	vertex.color[2] = b;
}

void setColour(Vertex &vertex, Vertex &sampleVertex) {
	vertex.color[0] = sampleVertex.color[0];
	vertex.color[1] = sampleVertex.color[1];
	vertex.color[2] = sampleVertex.color[2];
}

void setPosition(Vertex &vertex, GLfloat x, GLfloat y, GLfloat z) {
	vertex.position[0] = x;
	vertex.position[1] = y;
	vertex.position[2] = z;
}

void setNormal(Vertex &vertex, GLfloat x, GLfloat y, GLfloat z) {
	vertex.normal[0] = x;
	vertex.normal[1] = y;
	vertex.normal[2] = z;
}