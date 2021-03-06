#ifndef CW1_H
#define CW1_H

#include <iostream>
#include <stdio.h>

#include <cstdlib>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include <vector>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// My base classes
#include "Vertex.h"
#include "entity.h"
#include "scene.h"
#include "entitygroup.h"

#define PI 3.14159265359f

void CreateCone(GLfloat c[3], GLfloat, GLfloat n[3]);

#endif /* CW1_H */