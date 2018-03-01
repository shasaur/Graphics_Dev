#include "cw1.h"

int renderMode = 1;
bool pause = false;
float cameraAngle[3] = { 0,0,0 };
float cameraRotation[3] = { 0,0,0 };
float cameraSpeed = 0.01f;
glm::vec3 cameraPosition;

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
GLchar *vertexsource, *fragmentsource;
/* These are handles used to reference the shaders */
GLuint vertexshader, fragmentshader;
/* This is a handle to the shader program */
GLuint shaderprogram;
GLuint vao, vbo[1]; /* Create handles for our Vertex Array Object and One Vertex Buffer Object */

std::vector<Vertex> v;
std::vector<Entity> en;

/*
Create a simple sphere
"method" is 0 for quads, 1 for triangles
(quads look nicer in wireframe mode)/
*/
typedef struct {
	double x, y, z;
} XYZ;

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

void CreateCylinder(GLfloat c[3], bool capped) {
	GLfloat cf = 0.0;
	Vertex top_v;
	top_v.position[0] = 0;
	top_v.position[1] = 0;
	top_v.position[2] = 0;
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
		setPosition(v1, Radius * cos(a), Radius * sin(a), 0.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v1, v1.position[0] - c[0], v1.position[1] - c[1], v1.position[2] - c[2]);
		v1.color[0] = cf;
		cf = 1. - cf;
		v1.color[1] = cf;
		cf = 1. - cf;
		v1.color[2] = cf;
		cf = 1. - cf;

		Vertex v2;
		setPosition(v2, Radius * cos(a + step), Radius * sin(a + step), 0.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v2, v2.position[0] - c[0], v2.position[1] - c[1], v2.position[2] - c[2]);
		v2.color[0] = cf;
		cf = 1. - cf;
		v2.color[1] = cf;
		cf = 1. - cf;
		v2.color[2] = cf;

		Vertex v3;
		setPosition(v3, Radius * cos(a), Radius * sin(a), 2.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v3, v3.position[0] - c[0], v3.position[1] - c[1], v3.position[2] - c[2]);
		v3.color[0] = v1.color[0];
		v3.color[1] = v1.color[1];
		v3.color[2] = v1.color[2];

		Vertex v4;
		setPosition(v4, Radius * cos(a + step), Radius * sin(a + step), 2.f); //vertex: width x height x length (set to 0.0 for a circle (flat), >= 1.0 for a cone)
		setNormal(v4, v4.position[0] - c[0], v4.position[1] - c[1], v4.position[2] - c[2]);
		v4.color[0] = v2.color[0];
		v4.color[1] = v2.color[1];
		v4.color[2] = v2.color[2];

		Print(v1.position);
		Print(v2.position);
		Print(v3.position);
		Print(v4.position);

		// Triangle
		v.push_back(v1);
		v.push_back(v2);
		v.push_back(v3);

		v.push_back(v2);
		v.push_back(v3);
		v.push_back(v4);


		//v.push_back(top_v); // Apex
	}

	if (capped) {
		GLfloat normalDirection2[3] = { 0.f, 0.f, -1.f };
		CreateCone(c, 0.f, normalDirection2);

		c[2] = 2.f;
		GLfloat normalDirection[3] = { 0.f, 0.f, 1.f };
		CreateCone(c, 0.f, normalDirection);
	}
}

void CreateCone(GLfloat c[3], GLfloat height, GLfloat n[3]) {

	GLfloat cf = 0.0;
	Vertex top_v;
	setPosition(top_v, c[0], c[1], c[2]);
	if (height!=0)
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
		v.push_back(v1);
		v.push_back(v2);
		v.push_back(top_v); // Apex
	}
}

void SetupGeometry() {
	//
	// generate cone
	//
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Generate an identifier to use for the Vertex Buffer Object, store it in vbo
	glGenBuffers(1, vbo);
	// Make this identifier the active one (storing vertex attributes)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Give vertex data in v to the Vertex Buffer Object in OpenGL
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(struct Vertex), v.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer((GLuint)0, // coordinate data will be in attribute index 0
		3, GL_FLOAT,	// use 3 decimals to represent a vertex
		GL_FALSE,		// not normalised
		sizeof(struct Vertex),	// stride (aka memory to jump to get to the next vertex)
		(const GLvoid*)offsetof(struct Vertex, position));	// coordinates are stored in the vertex.pos space

															// colour data will be in attribute index 1, and the remaining characteristics to read the data
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, color));   // bug );								
																																	 /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
	glEnableVertexAttribArray(1);

	// normal data will be in attribute index 2
	glVertexAttribPointer((GLuint)2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const GLvoid*)offsetof(struct Vertex, normal));   // bug );
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
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

void SetupShaders(void) {
	/* Read our shaders into the appropriate buffers */
	vertexsource = filetobuf("./tutorial3.vert");
	fragmentsource = filetobuf("./tutorial3.frag");
	/* Assign our handles a "name" to new shader objects */
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	/* Associate the source code buffers with each handle */
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	/* Compile our shader objects */
	glCompileShader(vertexshader);
	glCompileShader(fragmentshader);
	/* Assign our program handle a "name" */
	shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, vertexshader);  /* Attach our shaders to our program */
	glAttachShader(shaderprogram, fragmentshader);
	glBindAttribLocation(shaderprogram, 0, "in_Position");   /* Bind attribute 0 (coordinates) to in_Position and attribute 1 (colors) to in_Color */
	glBindAttribLocation(shaderprogram, 1, "in_Color");
	glLinkProgram(shaderprogram);  /* Link our program, and set it as being actively used */
	CheckShader(shaderprogram, "Basic Shader");
	glUseProgram(shaderprogram);
}

//void RenderLines(int i) {
//	GLfloat angle;
//	glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
//	GLfloat t = glfwGetTime();
//	GLfloat p = 400.f;
//	t = fmod(t, p);
//	angle = t * 360.f / p;
//	glm::mat4 View = glm::mat4(1.f);
//	View = glm::translate(View, glm::vec3(0.f, 0.f, -5.0f));
//	View = glm::rotate(View, angle * -1.0f, glm::vec3(1.f, 0.f, 0.f));
//	View = glm::rotate(View, angle * 0.5f, glm::vec3(0.f, 1.f, 0.f));
//	View = glm::rotate(View, angle * 0.5f, glm::vec3(0.f, 0.f, 1.f));
//
//
//	glm::mat4 Model = glm::mat4(1.0f);
//	glm::mat4 MVP = Projection * View * Model;
//
//	glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
//	/* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  /* Make our background black */
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glBindVertexArray(vao);
//	glDrawArrays(GL_LINE_STRIP, 0, v.size());
//	glBindVertexArray(0);
//	/* Invoke glDrawArrays telling that our data consists of a triangle fan */
//}

void Render(int i) {
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
	View = glm::rotate(View, (cameraAngle[0] * 360.f)/p, glm::vec3(1.f, 0.f, 0.f));
	View = glm::rotate(View, (cameraAngle[1] * 360.f) / p, glm::vec3(0.f, 1.f, 0.f));
	View = glm::rotate(View, (cameraAngle[2] * 360.f) / p, glm::vec3(0.f, 0.f, 1.f));

	for (int i = 0; i < en.size(); i++) {
		glm::mat4 Model = en.at(i).model_transform();
		glm::mat4 MVP = Projection * View * Model;

		GLenum mode = NULL;
		switch (renderMode) {
		case 1:
			mode = GL_LINE_STRIP;
			break;
		case 2:
			mode = GL_TRIANGLES;
			break;
		}

		glUniformMatrix4fv(glGetUniformLocation(shaderprogram, "mvpmatrix"), 1, GL_FALSE, glm::value_ptr(MVP));
		/* Bind our modelmatrix variable to be a uniform called mvpmatrix in our shaderprogram */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  /* Make our background black */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(vao);

		glDrawArrays(mode, 0, v.size());
		glBindVertexArray(0);
		/* Invoke glDrawArrays telling that our data consists of a triangle fan */
	}
}

void SetupScenes() {
	Entity e1 (Entity::Sphere, glm::vec3(1.f,0.f,0.f), glm::vec3(1.f, 2.f, 1.f), glm::vec3(3.14f/2.f,0.f,0.f), 20);
	e1.add_my_vertices(v);
	en.push_back(e1);

	//e1.CreateSimpleSphere(centre, 1, 20);
	//center.x = 5.f; center.y = 0.f; center.z = 0.f;
	//CreateSimpleSphere(centre, 1, 20);

	//printf("Size %d\n", v.size());

	//GLfloat center[3] = { 0.f, 0.f, 0.f };
	//CreateCone(center, 2.0f);

	//GLfloat center[3] = { 0.f, 0.f, 0.f };
	//CreateCylinder(center, true);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Mode controls
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		renderMode = 1;
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		renderMode = 2;

	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
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
		cameraPosition[2] -= cameraSpeed*scale;
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

	cameraPosition = glm::vec3(0.f, 0.f, -5.0f);

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

	SetupScenes();
	SetupGeometry();

	SetupShaders();
	printf("Ready to render\n");

	glViewport(0, 0, screenWidth, screenHeight);

	while (!glfwWindowShouldClose(window)) {  // Main loop
		Render(k);        // OpenGL rendering goes here...
		k++;
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
