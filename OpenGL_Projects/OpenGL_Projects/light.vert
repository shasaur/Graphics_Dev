#version 400

precision highp float;

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec3 in_Angle;

uniform mat4 mvpmatrix;  // mvpmatrix is the result of multiplying the model, view, and projection matrices
uniform mat4 modelmatrix;

out vec3 ex_Color;
void main(void) {

    gl_Position = mvpmatrix * vec4(in_Position, 1.0f); // Multiply the mvp matrix by the vertex to obtain our final vertex position
    
	vec4 lightDirection = normalize(vec4(0.0,0.0,100.0, 1.0));

	vec4 gl_Normal = modelmatrix * vec4(in_Normal, 1.0f);
	gl_Normal = normalize(gl_Normal);

	vec3 diffuseReflection2 = in_Color * max(0.0, dot(gl_Normal, lightDirection));

	vec3 diffuseReflection = normalize(vec3(1.0,1.0,1.0));

    ex_Color = diffuseReflection2;
}
