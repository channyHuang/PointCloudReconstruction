#version 410 core

precision highp float;

uniform mat4 osg_ModelViewProjectionMatrix;

in vec3 in_vertex;
in vec3 in_normal;
out vec3 vertex;
out vec3 normal;

void main(void) {
    vertex = in_vertex;
    normal = in_normal;
    gl_Position = osg_ModelViewProjectionMatrix * vec4(in_vertex, 1.f);
}
