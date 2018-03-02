#include "entity.h"

Entity::Entity(Shape shape) {
	position = glm::vec3(0.f, 0.f, 0.f);
	size = glm::vec3(1.f, 1.f, 1.f);
	angle = glm::vec3(1.f, 1.f, 1.f);

	wiremesh = false;

	switch (shape) {
	case Shape::Cone:
	{break; };
	case Shape::Cube:
	{break; };
	case Shape::Cylinder:{
		break; };
	case Shape::Sphere:{
		CreateSimpleSphere(10, false);
		break;};
	}
}

Entity::Entity(Shape shape, glm::vec3 p, glm::vec3 s, glm::vec3 a, int res, bool w) {
	position = p;
	size = s;
	angle = a;

	wiremesh = w;

	switch (shape) {
	case Shape::Cone:{
		CreateCone(glm::vec3(0.f, 0.f, 0.f), 2.f, { 0.f, 0.f, 1.f });
		break; };
	case Shape::Cube:
	{break; };
	case Shape::Cylinder:{
		CreateCylinder(true);
		break; };
	case Shape::Sphere:{
		CreateSimpleSphere(res, w);
		break; };
	}

	// Animation
	an_types = std::vector<GLuint>();
	an_lengths = std::vector<GLuint>();
	an_magnitudes = std::vector<glm::vec3>();
	mom_angular = glm::vec3(0.f,0.f,0.f);
	mom_movement = glm::vec3(0.f,0.f,0.f);
	frame = 0;
	animation = 0;
}

void Entity::FreeGeometry() {
	glDeleteBuffers(1, &vbo);
}

void Entity::SetupGeometry() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate an identifier to use for the Vertex Buffer Object, store it in vbo
	glGenBuffers(1, &vbo);
	// Make this identifier the active one (storing vertex attributes)
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Give vertex data in v to the Vertex Buffer Object in OpenGL
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(struct Vertex), v.data(), GL_STATIC_DRAW);

	// usual setting up of memory locations to cycle through for attributes of vertices during rendering
	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, // coordinate data will be in attribute index 0
		3, GL_FLOAT,	// use 3 decimals to represent a vertex
		GL_FALSE,		// not normalised
		sizeof(struct Vertex),	// stride (aka memory to jump to get to the next vertex)
		(const GLvoid*)offsetof(struct Vertex, position));	// coordinates are stored in the vertex.pos space

															// colour data will be in attribute index 1, and the remaining characteristics to read the data
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, color));   // bug );
	glEnableVertexAttribArray(1);

	// normal data will be in attribute index 2
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, normal));   // bug );
	glEnableVertexAttribArray(2);

	// angle data to fix the light after the object has been rotated, so that it diffuses the right way
	glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, normal));   // bug );
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

/* Note:
	Because the task is to make the objects be illuminated by light at infinity, they are not off-centered
	by their distance in the lighting calculation. This will therefore make all entities render lights
	in parallel, much like the light rays potray when coming from infinity. */


void Entity::CreateSimpleSphere(int n, bool wiremesh)
{
	int i, j;
	double theta1, theta2, theta3;
	glm::vec3 e, p;

	GLfloat r = 1.f;
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
			setPosition(v1, r * cos(theta2) * cos(theta3), sin(theta2), r * cos(theta2) * sin(theta3));
			setNormal(v1, v1.position[0], v1.position[1], v1.position[2]);

			e.x = cos(theta1) * cos(theta3);
			e.y = sin(theta1);
			e.z = cos(theta1) * sin(theta3);

			Vertex v2;
			setColour(v2, v1);
			setPosition(v2, r * e.x, r * e.y, r * e.z);
			setNormal(v2, v2.position[0], v2.position[1], v2.position[2]);


			theta3 = (i + 1) * TWOPI / n;

			e.x = cos(theta2) * cos(theta3);
			e.y = sin(theta2);
			e.z = cos(theta2) * sin(theta3);

			Vertex v3;
			setColour(v3, v1);
			setPosition(v3, r * e.x, r * e.y, r * e.z);
			setNormal(v3, v3.position[0], v3.position[1], v3.position[2]);


			e.x = cos(theta1) * cos(theta3);
			e.y = sin(theta1);
			e.z = cos(theta1) * sin(theta3);

			Vertex v4;
			setColour(v4, v1);
			setPosition(v4, r * e.x, r * e.y, r * e.z);
			setNormal(v4, v4.position[0], v4.position[1], v4.position[2]);

			if (wiremesh) {
				push(v1);
				push(v2);
				push(v4);
				push(v3);
			}
			else {
				push(v1);
				push(v2);
				push(v3);

				push(v2);
				push(v3);
				push(v4);
			}
		}
		glEnd();
	}
}

void Entity::CreateCylinder(bool capped) {
	GLfloat cf = 0.0;/*
	Vertex top_v;
	top_v.position[0] = 0;
	top_v.position[1] = 0;
	top_v.position[2] = 0;
	top_v.color[0] = cf;
	cf = 1. - cf;
	top_v.color[1] = cf;
	cf = 1. - cf;
	top_v.color[2] = cf;
	cf = 1. - cf;*/


	GLint lod = 32;
	GLfloat step = 2.f * 3.141596f / GLfloat(lod);
	GLfloat Radius = 1.f;
	for (GLfloat a = 0; a <= (2.f * 3.141596f + step); a += step) {

		Vertex v1;
		setPosition(v1, Radius * cos(a), Radius * sin(a), -1.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v1, v1.position[0], v1.position[1], v1.position[2]);
		v1.color[0] = cf;
		cf = 1. - cf;
		v1.color[1] = cf;
		cf = 1. - cf;
		v1.color[2] = cf;
		cf = 1. - cf;

		Vertex v2;
		setPosition(v2, Radius * cos(a + step), Radius * sin(a + step), -1.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v2, v2.position[0], v2.position[1], v2.position[2]);
		v2.color[0] = cf;
		cf = 1. - cf;
		v2.color[1] = cf;
		cf = 1. - cf;
		v2.color[2] = cf;

		Vertex v3;
		setPosition(v3, Radius * cos(a), Radius * sin(a), 1.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v3, v3.position[0], v3.position[1], v3.position[2]);
		v3.color[0] = v1.color[0];
		v3.color[1] = v1.color[1];
		v3.color[2] = v1.color[2];

		Vertex v4;
		setPosition(v4, Radius * cos(a + step), Radius * sin(a + step), 1.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v4, v4.position[0], v4.position[1], v4.position[2]);
		v4.color[0] = v2.color[0];
		v4.color[1] = v2.color[1];
		v4.color[2] = v2.color[2];
/*
		Print(v1.position);
		Print(v2.position);
		Print(v3.position);
		Print(v4.position);*/

		// Lower left triangle
		push(v1);
		push(v2);
		push(v3);

		// Upper right triangle
		push(v2);
		push(v3);
		push(v4);
	}

	// If the container should have 'lids'
	if (capped) {
		glm::vec3 c(0.f, 0.f, -1.f);
		glm::vec3 normalDirection2 = { 0.f, 0.f, -1.f };
		CreateCone(c, 0.f, normalDirection2);

		c[2] = 1.f;
		glm::vec3 normalDirection = { 0.f, 0.f, 1.f };
		CreateCone(c, 0.f, normalDirection);
	}
}

void Entity::CreateCone(glm::vec3 c, GLfloat height, glm::vec3 n) {

	GLfloat cf = 0.0;
	Vertex top_v;
	setPosition(top_v, c[0], c[1], c[2]);
	if (height != 0)
		setNormal(top_v, top_v.position[0] - c[0], top_v.position[1] - c[1], top_v.position[2] - c[2]);
	else
		setNormal(top_v, n[0], n[1], n[2]);

	//setColour(top_v, 0.5f, 0.5f, 0.5f);

	top_v.color[0] = cf;
	cf = 1. - cf;
	top_v.color[1] = cf;
	cf = 1. - cf;
	top_v.color[2] = cf;
	cf = 1. - cf;


	GLint lod = 32;
	GLfloat step = 2.f * 3.141596f / GLfloat(lod);
	GLfloat Radius = 1.f;
	for (GLfloat a = 0; a <= (2.f * 3.141596f + step); a += step) {

		Vertex v1;
		setPosition(v1, Radius * cos(a) + c[0], Radius * sin(a) + c[1], height + c[2]); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		if (height != 0)
			setNormal(v1, v1.position[0] - c[0], v1.position[1] - c[1], v1.position[2] - c[2]);
		else
			setNormal(v1, n[0], n[1], n[2]);

		//setColour(v1, 0.5f, 0.5f, 0.5f);
		v1.color[0] = cf;
		cf = 1. - cf;
		v1.color[1] = cf;
		cf = 1. - cf;
		v1.color[2] = cf;
		cf = 1. - cf;

		Vertex v2;
		setPosition(v2, Radius * cos(a + step) + c[0], Radius * sin(a + step) + c[1], height + c[2]); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		if (height != 0)
			setNormal(v2, v2.position[0] - c[0], v2.position[1] - c[1], v2.position[2] - c[2]);
		else
			setNormal(v2, n[0], n[1], n[2]);


		//setColour(v2, 0.5f, 0.5f, 0.5f);
		v2.color[0] = cf;
		cf = 1. - cf;
		v2.color[1] = cf;
		cf = 1. - cf;
		v2.color[2] = cf;

		// Triangle
		push(v1);
		push(v2);
		push(top_v); // Apex
	}
}

glm::mat4 Entity::angle_matrix() {
	glm::mat4 t = glm::mat4(1.f);
	
	t = glm::rotate(t, angle.x, glm::vec3(1, 0, 0));
	t = glm::rotate(t, angle.y, glm::vec3(0, 1, 0));
	t = glm::rotate(t, angle.z, glm::vec3(0, 0, 1));

	return t;
}

glm::mat4 Entity::model_transform() {
	glm::mat4 r = glm::mat4(1.f);
	r = glm::rotate(r, angle.x, glm::vec3(1, 0, 0));
	r = glm::rotate(r, angle.y, glm::vec3(0, 1, 0));
	r = glm::rotate(r, angle.z, glm::vec3(0, 0, 1));
	
	glm::mat4 Model =
		glm::translate(glm::mat4(1.), position) *
		r *
		glm::scale(glm::mat4(1.), size);


	return Model;



	/*t = glm::scale(t, size);
	t = glm::translate(t, position);*/

	/*t[0][0] = size.x;
	t[1][1] = size.y;
	t[2][2] = size.z;

	t[3][0] = position.x;
	t[3][1] = position.y;
	t[3][2] = position.z;*/
	
	/*if (angle.x != 0.f) {
		glm::mat4 new_t = glm::mat4(1.f);
		new_t[1][1] = cos(angle.x);
		new_t[2][1] = -sin(angle.x);
		new_t[1][2] = sin(angle.x);
		new_t[2][2] = cos(angle.x);
		new_t *= t;
		t = new_t;
		t *= new_t;
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
	}*/

	//return t;
}

void Entity::add_my_vertices(std::vector<Vertex> &vertices) {
	for (int i = 0; i < v.size(); i++)
		vertices.push_back(v.at(i));
}

// Pushes and normalises the vertex such that lighting is correctly displayed in the new angle
void Entity::push(Vertex &vertex) {
	vertex.angle[0] = angle.x;
	vertex.angle[1] = angle.y;
	vertex.angle[2] = angle.z;
	v.push_back(vertex);
}

void Entity::AddAnimation(GLuint type, GLuint length, glm::vec3 mag) {
	an_types.push_back(type);
	an_lengths.push_back(length);
	an_magnitudes.push_back(mag);
}

void Entity::Animate() {
	
	// If the entity is to be animated
	if (an_types.size() != 0) {

		// If not at the end of the animation sequence
		if (animation < an_types.size()) {

			// Take action depending on what the animation is supposed to do
			switch (an_types.at(animation)) {
			case 0:
				// do nothing
				break;
			case 1:
				mom_movement += an_magnitudes.at(animation);
				break;
			case 2:
				mom_angular += an_magnitudes.at(animation);
				break;
			}

			// Update
			position.z += mom_movement.z;
			position.y += mom_movement.y;
			position.x += mom_movement.x;
			/*position += mom_movement/position;
			angle += mom_angular;
*/
			// Repeating and continuation mechanics
			frame++;

			if (frame > an_lengths.at(animation)) {
				animation++;
				frame = 0;
			}
		}
	}

}