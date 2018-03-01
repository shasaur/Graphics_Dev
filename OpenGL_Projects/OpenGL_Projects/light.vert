#version 400

precision highp float;

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;

uniform mat4 mvpmatrix;  // mvpmatrix is the result of multiplying the model, view, and projection matrices

out vec3 ex_Color;
void main(void) {

    gl_Position = mvpmatrix * vec4(in_Position, 1.0f); // Multiply the mvp matrix by the vertex to obtain our final vertex position
    
	vec3 lightDirection = normalize(vec3(0.0,0.0,1.0));
	vec3 normalDirection = normalize(in_Normal);

	vec3 diffuseReflection2 = in_Color * max(0.0, dot(normalDirection, lightDirection));

	vec3 diffuseReflection = normalize(vec3(1.0,1.0,1.0));

    ex_Color = diffuseReflection2;
}
