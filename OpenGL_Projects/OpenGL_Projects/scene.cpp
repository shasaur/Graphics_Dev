#include "scene.h"

Scene::Scene() {
	cameraPosition = glm::vec3(0.f, 0.f, -5.0f);
}

void Scene::AddEntity(Entity e) {
	e.add_my_vertices(v);
	en.push_back(e);
}

void Scene::Render(GLuint shaderprogram, GLuint vao) {
	GLfloat angle;
	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	GLfloat t = glfwGetTime();
	GLfloat p = 400.f;
	t = fmod(t, p);
	angle = t * 360.f / p;
	glm::mat4 View = glm::mat4(1.f);

	cameraAngle[0] += cameraRotation[0];
	cameraAngle[1] += cameraRotation[1];
	cameraAngle[2] += cameraRotation[2];

	View = glm::translate(View, cameraPosition);
	View = glm::rotate(View, (cameraAngle[0] * 360.f) / p, glm::vec3(1.f, 0.f, 0.f));
	View = glm::rotate(View, (cameraAngle[1] * 360.f) / p, glm::vec3(0.f, 1.f, 0.f));
	View = glm::rotate(View, (cameraAngle[2] * 360.f) / p, glm::vec3(0.f, 0.f, 1.f));

	for (int i = 0; i < en.size(); i++) {
		glm::mat4 Model = en.at(i).model_transform();
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
		/* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  /* Make our background black */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(vao);
		glDrawArrays(mode, 0, en.at(i).v.size());
		glBindVertexArray(0);
		/* Invoke glDrawArrays telling that our data consists of a triangle fan */
	}
}