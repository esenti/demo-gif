#version 130

uniform float timeStep;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

in vec3 position;
in vec3 normal;

out vec3 Color;
out vec3 Normal;
out vec3 Position;

void main()
{
    Color = vec3(1.0, 1.0, 1.0);
    Normal = normalize(vec3(transpose(inverse(V * M)) * vec4(normal, 0.0)));
    Position = vec3(V * M * vec4(position, 1.0));

    gl_Position = P * V * M * vec4(position.x, position.y, position.z, 1.0);
}

