#include "scene.h"

Scene::Scene() {
	background = glm::vec3(0.f, 0.f, 0.f);
	cameraPosition = glm::vec3(0.f, 0.f, -5.0f);
	cameraRotation = glm::vec3(0.f, 0.f, 0.f);
	cameraAngle = glm::vec3(0.f, 0.f, 0.f);
	cameraSpeed = 0.f;
}

Scene::Scene(glm::vec3 cam) {
	background = glm::vec3(0.f, 0.f, 0.f);
	cameraPosition = glm::vec3(0.f, 0.f, -5.0f);
	cameraRotation = glm::vec3(0.f, 0.f, 0.f);
	cameraAngle = glm::vec3(0.f, 0.f, 0.f);
	cameraPosition = cam;
}

void Scene::SetBackground(glm::vec3 b) {
	background = b;
}

void Scene::Rotate(glm::vec3 rot) {
	cameraRotation = rot;
}

void Scene::AddEntity(Entity e) {
	e.add_my_vertices(v);
	en.push_back(e);
}

void Scene::AddEntities(Entity* e, int n) {
	for (int i = 0; i < n; i++) {
		e[i].add_my_vertices(v);
		en.push_back(e[i]);
	}
}

void Scene::FreeGeometry() {
	for (int i = 0; i < en.size(); i++) {
		en[i].FreeGeometry();
	}
}

void Scene::SetupGeometry() {
	for (int i = 0; i < en.size(); i++) {
		en[i].SetupGeometry();
	}
}

void Scene::Update() {
	for (int i = 0; i < en.size(); i++) {
		en.at(i).Animate();
	}
}

void Scene::Render(GLuint shaderprogram, GLuint vao) {
	GLfloat angle;
	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	GLfloat t = glfwGetTime();
	GLfloat p = 400.f;
	t = fmod(t, p);
	angle = t * 360.f / p;
	glm::mat4 View = glm::mat4(1.f);

	cameraAngle[0] += cameraRotation.x;
	cameraAngle[1] += cameraRotation.y;
	cameraAngle[2] += cameraRotation.z;

	View = glm::translate(View, cameraPosition);
	View = glm::rotate(View, cameraAngle.x, glm::vec3(1.f, 0.f, 0.f));
	View = glm::rotate(View, cameraAngle.y, glm::vec3(0.f, 1.f, 0.f));
	View = glm::rotate(View, cameraAngle.z, glm::vec3(0.f, 0.f, 1.f));

	/* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
	glClearColor(background.r, background.g, background.b, 1.0f);  /* Make our background black */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < en.size(); i++) {
		glBindVertexArray(en.at(i).vao);

		glm::mat4 Model = en.at(i).model_transform();
		glm::mat4 Angle = en.at(i).angle_matrix();

		//glm::mat4 Model = glm::mat4(1.f);
		//Model = glm::translate(Model, en.at(i).position);
		//Model = glm::rotate(Model, en.at(i).angle);
		glm::mat4 MVP = Projection * View * Model;

		GLenum mode = NULL;
		/*switch (renderMode) {
		case 1:
		mode = GL_LINE_STRIP;
		break;
		case 2:
		mode = GL_TRIANGLES;
		break;
		}*/

		if (en.at(i).wiremesh)
			mode = GL_LINE_STRIP;

		else
			mode = GL_TRIANGLES;


		glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
		glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "modelmatrix"), 1, GL_FALSE, glm::value_ptr(Angle));
		

		glDrawArrays(mode, 0, en.at(i).v.size());
		/* Invoke glDrawArrays telling that our data consists of a triangle fan */
		glBindVertexArray(0);
	}
}