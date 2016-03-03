#version 130

uniform float timeStep;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 lightPos;

in vec3 Color;
in vec3 Normal;
in vec3 Position;

out vec4 outColor;

void main()
{
    vec3 mvLightPos = vec3(V * vec4(lightPos, 1.0));

    vec3 lightVector = normalize(mvLightPos - Position);

    float lightDistance = length(mvLightPos - Position);

    float diffuse = max(dot(Normal, lightVector), 0.0);
    diffuse = diffuse * (1.0 / (0.025 * lightDistance * lightDistance));

    outColor = vec4(Color.r, Color.g, Color.b, 1.0) * diffuse;
}

