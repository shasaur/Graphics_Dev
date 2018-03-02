#include "cw1.h"


int renderMode = 1;
bool pause = false;

void Check(const char *where) { // Function to check OpenGL error status
	const char * what;
	int err = glGetError();   //0 means no error
	if (!err)
		return;
	if (err == GL_INVALID_ENUM)
		what = "GL_INVALID_ENUM";
	else if (err == GL_INVALID_VALUE)
		what = "GL_INVALID_VALUE";
	else if (err == GL_INVALID_OPERATION)
		what = "GL_INVALID_OPERATION";
	else if (err == GL_INVALID_FRAMEBUFFER_OPERATION)
		what = "GL_INVALID_FRAMEBUFFER_OPERATION";
	else if (err == GL_OUT_OF_MEMORY)
		what = "GL_OUT_OF_MEMORY";
	else
		what = "Unknown Error";
	fprintf(stderr, "Error (%d) %s  at %s\n", err, what, where);
	exit(1);
}

void CheckShader(int sp, const char *x) {
	int length;
	char text[1001];
	glGetProgramInfoLog(sp, 1000, &length, text);   // Check for errors
	if (length > 0) {
		fprintf(stderr, "Validate Shader Program\nMessage from:%s\n%s\n", x, text);
		exit(1);
	}
}

char* filetobuf(char *file) { /* A simple function that will read a file into an allocated char pointer buffer */
	FILE *fptr;
	long length;
	char *buf;
	fprintf(stderr, "Loading %s\n", file);
#pragma warning (disable : 4996)
	fptr = fopen(file, "rb");   /* Open file for reading */
	if (!fptr) {   /* Return NULL on failure */
		fprintf(stderr, "failed to open %s\n", file);
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);   /* Seek to the end of the file */
	length = ftell(fptr);   /* Find out how many bytes into the file we are */
	buf = (char*)malloc(length + 1);   /* Allocate a buffer for the entire length of the file and a null terminator */
	fseek(fptr, 0, SEEK_SET);   /* Go back to the beginning of the file */
	fread(buf, length, 1, fptr);   /* Read the contents of the file in to the buffer */
	fclose(fptr);   /* Close the file */
	buf[length] = 0;   /* Null terminator */
	return buf;   /* Return the buffer */
}


/* These pointers will receive the contents of our shader source code files */
GLchar *v1source, *v2source, *f1source, *f2source;
/* These are handles used to reference the shaders */
GLuint v1shader, v2shader, f1shader, f2shader;
/* This is a handle to the shader program */
GLuint s1program, s2program;
GLuint vao, vbo[1]; /* Create handles for our Vertex Array Object and One Vertex Buffer Object */

std::vector<Vertex> v;

Scene* scenes[4];
int current_scene = 0;

///*
//Create a simple sphere
//"method" is 0 for quads, 1 for triangles
//(quads look nicer in wireframe mode)/
//*/
//typedef struct {
//	double x, y, z;
//} XYZ;

void Print(glm::mat4 x) {
	x = glm::transpose(x); // cos I got  the storage wrong, and its quicker than retyping.
	printf("\n[[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]\n[%8.4f %8.4f %8.4f %8.4f]]\n",
		x[0][0], x[0][1], x[0][2], x[0][3],
		x[1][0], x[1][1], x[1][2], x[1][3],
		x[2][0], x[2][1], x[2][2], x[2][3],
		x[3][0], x[3][1], x[3][2], x[3][3]);
}

void Print(GLfloat* position) {
	printf("(%f, %f, %f)\n", position[0], position[1], position[2]);
}

void MoveModel(glm::vec3 direction, Entity* entities, GLint components) {
	glm::vec3 direction_r = glm::vec3(direction.x, direction.z, direction.y);
	glm::mat4 rotation = glm::mat4(1.f);
	rotation = glm::rotate(rotation, direction.x, glm::vec3(1.f, 0.f, 0.f));
	rotation = glm::rotate(rotation, direction.y, glm::vec3(0.f, 1.f, 0.f));
	rotation = glm::rotate(rotation, direction.z, glm::vec3(0.f, 0.f, 1.f));

	for (int i = 0; i < components; i++) {
		entities[i].angle += direction_r;
		glm::vec4 temp = rotation * glm::vec4(entities[i].position, 1.f);
		entities[i].position.x = temp.x;
		entities[i].position.y = temp.y;
		entities[i].position.z = temp.z;

	}
}

void ModelFalconHeavy(Scene &s) {
	const int components = 10;

	//Entity rocket[components] = {
	//	// First stage
	//	Entity(Entity::Sphere,{ -2.15f, 0.f, 0.f },{ 0.37f, 0.37f, 0.37f },{ PI / 2, 0.f, 0.f }, 10, false),
	//	Entity(Entity::Sphere,{ 2.15f, 0.f, 0.f },{ 0.37f, 0.37f, 0.37f },{ PI / 2, 0.f, 0.f }, 10, false),
	//	Entity(Entity::Cylinder, {-2.15f, 0.f, 1.f}, { 0.37f, 4.2f, 0.37f}, {PI/2, 0.f, 0.f}, 10, false),
	//	Entity(Entity::Cylinder,{ 0.f, 0.f, 1.f },{ 0.37f, 4.2f, 0.37f },{ PI / 2, 0.f, 0.f }, 10, false),
	//	Entity(Entity::Cylinder,{ 2.15f, 0.f, 1.f },{ 0.37f, 4.2f, 0.37f },{ PI / 2, 0.f, 0.f }, 10, false),

	//	// Second stage
	//	Entity(Entity::Cylinder,{ 0.f, 0.f, -0.4f },{ 0.37f, 2.1f, 0.37f },{ PI / 2, 0.f, 0.f }, 10, false),
	//	Entity(Entity::Sphere,{ 0.f, 0.f, -2.1f },{ 0.43f, 1.4f, 0.43f },{ PI / 2, 0.f, 0.f }, 10, false),

	//	Entity(Entity::Cone,{ -2.65f, 0.f, 27.f },{ 0.3f, 0.3f, 0.3f },{ PI / 2, 0.f, 0.f }, 10, false),
	//	Entity(Entity::Cone,{ 0.f, 0.f, 27.f },{ 0.3f, 0.3f, 0.3f },{ PI / 2, 0.f, 0.f }, 10, false),
	//	Entity(Entity::Cone,{ 2.65f, 0.f, 27.f },{ 0.3f, 0.3f, 0.3f },{ PI / 2, 0.f, 0.f }, 10, false)
	//};

	Entity rocket[10] = {
		// First stage
		Entity(Entity::Sphere,{ -1.f, -0.75f, 0.f },{ 0.48f, 0.48f, 0.48f },{ PI / 2, 0.f, 0.f }, 10, false),
		Entity(Entity::Sphere,{ 1.f, -0.75f, 0.f },{ 0.48f, 0.48f, 0.48f },{ PI / 2, 0.f, 0.f }, 10, false),
		Entity(Entity::Cylinder,{ 0.f, -5.f, 0.f },{ 0.48f, 0.48f, 4.2f },{ PI / 2, 0.f, 0.f }, 10, false),
		Entity(Entity::Cylinder,{ -1.f, -5.f, 0.f },{ 0.48f, 0.48f, 4.2f },{ PI / 2, 0.f, 0.f }, 10, false),
		Entity(Entity::Cylinder,{ 1.f, -5.f, 0.f },{ 0.48f, 0.48f, 4.2f },{ PI / 2, 0.f, 0.f }, 10, false),

		//// Second stage
		Entity(Entity::Cylinder,{ 0.f, 1.3f, 0.f },{ 0.48f, 0.48f, 2.3f },{ PI / 2, 0.f, 0.f }, 10, false),
		Entity(Entity::Sphere,{ 0.f, 3.2f, 0.f },{ 0.60f, 0.60f, 1.5f },{ PI / 2, 0.f, 0.f }, 10, false),

		Entity(Entity::Cone,{ -1.f, -8.9f, 0.f },{ 0.42f, 0.42f, 0.42f },{ PI / 2, 0.f, 0.f }, 10, false),
		Entity(Entity::Cone,{ 0.f, -8.9f, 0.f },{ 0.42f, 0.42f, 0.42f },{ PI / 2, 0.f, 0.f }, 10, false),
		Entity(Entity::Cone,{ 1.f, -8.9f, 0.f },{ 0.42f, 0.42f, 0.42f },{ PI / 2, 0.f, 0.f }, 10, false)
	};

	// Initial positioning/rotation
	//glm::vec3 direction = glm::vec3((PI / 2.f), 0.f, 0.f);
	//glm::vec3 direction = glm::vec3(0.f, (PI / 2.f), 0.f);

	glm::vec3 direction(-(PI / 8.f)*3.f, 0.f, -0.2f);
	MoveModel(direction, rocket, components);
	//MoveModel({ 0.f, -(PI / 4.f), 0.f }, rocket, components);
	//MoveModel({ 0.f, 0.f, (PI / 2.f) }, rocket, components);
	
	GLfloat d = direction.y;
	direction.x = 0.00005f * 1.5f;
	direction.y = 0.00005f * 1.5f;
	direction.z = -0.0001f * 1.5f;
	for (int i = 0; i < components; i++) {
		rocket[i].AddAnimation(1, 125, direction);
		rocket[i].AddAnimation(0, 800, direction);
	}

	s.AddEntities(rocket, 10);

}

void SetupScenes() {
	scenes[0] = new Scene();
	scenes[1] = new Scene();
	scenes[2] = new Scene(glm::vec3(-8.f, -8.f, -5.f));

	Entity e1(Entity::Sphere, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(0.f, 0.f, 0.f), 40, true);
	scenes[0]->AddEntity(e1);

	Entity e2(Entity::Sphere, glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 0.f, 0.f), 40, false);
	scenes[1]->AddEntity(e2);

	ModelFalconHeavy(*scenes[2]);
	scenes[2]->SetBackground(glm::vec3(0.1f, 0.1f, 0.1f));

	//e1.CreateSimpleSphere(centre, 1, 20);
	//center.x = 5.f; center.y = 0.f; center.z = 0.f;
	//CreateSimpleSphere(centre, 1, 20);

	//printf("Size %d\n", v.size());

	//GLfloat center[3] = { 0.f, 0.f, 0.f };
	//CreateCone(center, 2.0f);

	//GLfloat center[3] = { 0.f, 0.f, 0.f };
	//CreateCylinder(center, true);
}

void SetupSquareGeometry() {
	//
	// generate cone
	//
	GLfloat cf = 0.0;
	Vertex t;
	t.color[0] = cf;
	cf = 1. - cf;
	t.color[1] = cf;
	cf = 1. - cf;
	t.color[2] = cf;
	cf = 1. - cf;
	//v.push_back(t); // Apex

	t.position[0] = 0.2f;
	t.position[1] = 0.2f;
	t.position[2] = 2;
	v.push_back(t);

	t.position[0] = 0.8f;
	t.position[1] = 0.2f;
	t.position[2] = 2;
	v.push_back(t);

	t.position[0] = 0.8f;
	t.position[1] = 0.8f;
	t.position[2] = 2;
	v.push_back(t);

	t.position[0] = 0.2f;
	t.position[1] = 0.8f;
	t.position[2] = 2;
	v.push_back(t);

	printf("Size %d\n", v.size());
	//
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	/* Allocate and assign One Vertex Buffer Object to our handle */
	glGenBuffers(1, vbo);
	/* Bind our VBO as being the active buffer and storing vertex attributes (coordinates + colors) */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	/* Copy the vertex data from cone to our buffer */
	/* v,size() * sizeof(GLfloat) is the size of the cone array, since it contains 12 Vertex values */
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(struct Vertex), v.data(), GL_STATIC_DRAW);
	/* Specify that our coordinate data is going into attribute index 0, and contains three doubles per vertex */
	/* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) 0 */
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, position));
	/* Enable attribute index 0 as being used */
	glEnableVertexAttribArray(0);
	/* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
	/* Note stride = sizeof ( struct Vertex ) and pointer = ( const GLvoid* ) ( 3 * sizeof ( GLdouble ) ) i.e. the size (in bytes)
	occupied by the first attribute (position) */
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, color));   // bug );
																																	 /* Enable attribute index 1 as being used */
	glEnableVertexAttribArray(1);  /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
	glBindVertexArray(0);
}

GLuint InitialiseShader(GLchar* vertex_source, GLchar* fragment_source) {
	/* Assign our handles a "name" to new shader objects */
	v1shader = glCreateShader(GL_VERTEX_SHADER);
	f1shader = glCreateShader(GL_FRAGMENT_SHADER);
	/* Associate the source code buffers with each handle */
	glShaderSource(v1shader, 1, (const GLchar**)&vertex_source, 0);
	glShaderSource(f1shader, 1, (const GLchar**)&fragment_source, 0);
	/* Compile our shader objects */
	glCompileShader(v1shader);
	glCompileShader(f1shader);
	/* Assign our program handle a "name" */
	GLuint shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, v1shader);  /* Attach our shaders to our program */
	glAttachShader(shaderprogram, f1shader);
	glBindAttribLocation(shaderprogram, 0, "in_Position");   /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	glBindAttribLocation(shaderprogram, 1, "in_Color");
	glLinkProgram(shaderprogram);  /* Link our program, and set it as being actively used */
	CheckShader(shaderprogram, "Basic Shader");

	return shaderprogram;
}

void SetupShaders(void) {
	s1program = InitialiseShader(filetobuf("./no_light.vert"), filetobuf("./no_light.frag"));
	s2program = InitialiseShader(filetobuf("./light.vert"), filetobuf("./light.frag"));
	glUseProgram(s1program);
}

void Render() {
	if (current_scene == 0)
		scenes[current_scene]->Render(s1program, vao);
	else
		scenes[current_scene]->Render(s2program, vao);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Mode controls
	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		current_scene = 0;
		scenes[0]->FreeGeometry();
		scenes[0]->SetupGeometry();

		glUseProgram(s1program);

	} else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		current_scene = 1;
		scenes[1]->FreeGeometry();
		scenes[1]->SetupGeometry();
		
		glUseProgram(s2program);

	}else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		current_scene = 2;
		scenes[2]->FreeGeometry();
		scenes[2]->SetupGeometry();

		glUseProgram(s2program);

	} else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		/*current_scene = 3;
		FreeGeometry();
		SetupGeometry(3);

		glUseProgram(s2program);*/
	}

	/*else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		cameraRotation[1] = cameraSpeed;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		cameraRotation[1] = -cameraSpeed;
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		cameraRotation[0] = cameraSpeed;
	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
		cameraRotation[0] = -cameraSpeed;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		cameraRotation[0] = 0.f;
		cameraRotation[1] = 0.f;
		cameraRotation[2] = 0.f;
	}

	float scale = 5.f;
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
		cameraPosition[0] += cameraSpeed*scale;
	else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
		cameraPosition[0] -= cameraSpeed*scale;
	else if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
		cameraPosition[2] += cameraSpeed*scale;
	else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
		cameraPosition[2] -= cameraSpeed*scale;*/

	/*else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		renderMode = 1;
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		renderMode = 2;*/
}

void init() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

const GLint WIDTH = 600, HEIGHT = 600;

int main() {
	int k = 0;

	init();


	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr, nullptr);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);


	if (nullptr == window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	glfwSetKeyCallback(window, key_callback);
	fprintf(stderr, "GL INFO %s\n", glGetString(GL_VERSION));
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);

	SetupScenes();
	SetupShaders();

	current_scene = 0;
	scenes[0]->SetupGeometry();

	printf("Ready to render\n");

	glViewport(0, 0, screenWidth, screenHeight);
	//glFrustum(left,right,bottom,top,near,far).
	//glFrustum(0., 100., 100., 100., 0., 1000.);

	//scenes[2]->Rotate(glm::vec3(0.01f, 0.f, 0.f));

	while (!glfwWindowShouldClose(window)) {  // Main loop
		scenes[current_scene]->Update();
		Render();        // OpenGL rendering goes here...
		glfwSwapBuffers(window);        // Swap front and back rendering buffers
		glfwPollEvents();         // Poll for events.

	}
	glfwTerminate();  // Close window and terminate GLFW

	return EXIT_SUCCESS;
}
//
//int main(void) {
//
//#ifndef __APPLE__
//	// IMPORTANT: make window curren must be done so glew recognises OpenGL
//	glewExperimental = GL_TRUE;
//	int err = glewInit();
//	if (GLEW_OK != err) {
//		/* Problem: glewInit failed, something is seriously wrong. */
//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//	}
//#endif
//
//	glfwSetKeyCallback(window, key_callback);
//	fprintf(stderr, "GL INFO %s\n", glGetString(GL_VERSION));
//	glEnable(GL_DEPTH_TEST);
//	SetupGeometry();
//	SetupShaders();
//	printf("Ready to render\n");
//	while (!glfwWindowShouldClose(window)) {  // Main loop
//		Render(k);        // OpenGL rendering goes here...
//		k++;
//		glfwSwapBuffers(window);        // Swap front and back rendering buffers
//		glfwPollEvents();         // Poll for events.
//
//	}
//	glfwTerminate();  // Close window and terminate GLFW
//	exit(EXIT_SUCCESS);  // Exit program
//}
