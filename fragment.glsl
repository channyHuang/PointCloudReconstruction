#version 410 core

precision highp float;

uniform vec3 lightDir;

in vec3 vertex;
in vec3 normal;
out vec4 color;

void main(void) {
    float diffuse = max( dot( lightDir, normal ), 0. );
    color = vec4( vec3( diffuse ), 1. );
    //color = vec4(1.f, 0.f, 0.f, 1.f);
}
