#include "entity.h"

//class Entity {
//	glm::vec3 position;
//	glm::vec3 size;
//	glm::vec3 angle;
//
//	std::vector<Vertex> v;
//
//
	//enum Shape { Sphere, Cone, Cylinder, Cube };

Entity::Entity(Shape shape) {
	position = glm::vec3(0.f, 0.f, 0.f);
	size = glm::vec3(1.f, 1.f, 1.f);
	angle = glm::vec3(0.f, 0.f, 0.f);

	switch (shape) {
	case Shape::Cone:
	{break; };
	case Shape::Cube:
	{break; };
	case Shape::Cylinder:
	{break; };
	case Shape::Sphere:
	{
		CreateSimpleSphere(position, size.x, 10);
		break;};
	}
}

Entity::Entity(Shape shape, glm::vec3 p, glm::vec3 s, glm::vec3 a, int res) {
	position = p;
	size = s;
	angle = a;

	switch (shape) {
	case Shape::Cone:
	{break; };
	case Shape::Cube:
	{break; };
	case Shape::Cylinder:
	{break; };
	case Shape::Sphere:{
		CreateSimpleSphere(position, 1.f, res);
		break; };
	}
}

void Entity::CreateSimpleSphere(glm::vec3 c, double r, int n)
{
	int i, j;
	double theta1, theta2, theta3;
	glm::vec3 e, p;

	// vertical
	for (j = 0; j<n / 2; j++) {
		theta1 = j * TWOPI / n - PID2;
		theta2 = (j + 1) * TWOPI / n - PID2;

		// horizontal
		for (i = 0; i <= n; i++) {

			theta3 = i * TWOPI / n;

			// SIMPLIFY ALL OF THIS

			Vertex v1;
			randomiseColour(v1);
			setPosition(v1, c.x + r * (cos(theta2) * cos(theta3)), c.y + (sin(theta2)), c.z + r * (cos(theta2) * sin(theta3)));
			setNormal(v1, v1.position[0] - c.x, v1.position[1] - c.y, v1.position[2] - c.z);

			e.x = cos(theta1) * cos(theta3);
			e.y = sin(theta1);
			e.z = cos(theta1) * sin(theta3);

			Vertex v2;
			setColour(v2, v1);
			setPosition(v2, c.x + r * e.x, c.y + r * e.y, c.z + r * e.z);
			setNormal(v2, v2.position[0] - c.x, v2.position[1] - c.y, v2.position[2] - c.z);


			theta3 = (i + 1) * TWOPI / n;

			e.x = cos(theta2) * cos(theta3);
			e.y = sin(theta2);
			e.z = cos(theta2) * sin(theta3);

			Vertex v3;
			setColour(v3, v1);
			setPosition(v3, c.x + r * e.x, c.y + r * e.y, c.z + r * e.z);
			setNormal(v3, v3.position[0] - c.x, v3.position[1] - c.y, v3.position[2] - c.z);


			e.x = cos(theta1) * cos(theta3);
			e.y = sin(theta1);
			e.z = cos(theta1) * sin(theta3);

			Vertex v4;
			setColour(v4, v1);
			setPosition(v4, c.x + r * e.x, c.y + r * e.y, c.z + r * e.z);
			setNormal(v4, v4.position[0] - c.x, v4.position[1] - c.y, v4.position[2] - c.z);


			v.push_back(v1);
			v.push_back(v2);
			v.push_back(v3);

			v.push_back(v2);
			v.push_back(v3);
			v.push_back(v4);
		}
		glEnd();
	}
}

glm::mat4 Entity::model_transform() {
	glm::mat4 t = glm::mat4(1.f);

	t[0][0] = size.x;
	t[1][1] = size.y;
	t[2][2] = size.z;

	t[3][0] = position.x;
	t[3][1] = position.y;
	t[3][2] = position.z;
	
	if (angle.x != 0.f) {
		glm::mat4 new_t = glm::mat4(1.f);
		new_t[1][1] = cos(angle.x);
		new_t[2][1] = -sin(angle.x);
		new_t[1][2] = sin(angle.x);
		new_t[2][2] = cos(angle.x);
		new_t *= t;
		t = new_t;
		//t *= new_t;
	}
	if (angle.y != 0.f) {
		glm::mat4 new_t = glm::mat4(1.f);
		new_t[0][0] = cos(angle.y);
		new_t[2][0] = sin(angle.y);
		new_t[0][2] = -sin(angle.y);
		new_t[2][2] = cos(angle.y);
		new_t *= t;
		t = new_t;
	}
	if (angle.z != 0.f) {
		glm::mat4 new_t = glm::mat4(1.f);
		new_t[0][0] = cos(angle.x);
		new_t[1][0] = -sin(angle.x);
		new_t[0][1] = sin(angle.x);
		new_t[1][1] = cos(angle.x);
		new_t *= t;
		t = new_t;
	}

	return t;
}

void Entity::add_my_vertices(std::vector<Vertex> &vertices) {
	for (int i = 0; i < v.size(); i++)
		vertices.push_back(v.at(i));
}
//};