[VERTEX]
#version 110

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;
uniform vec3 object_scale;

void main()
{
    gl_Position = vec4(gl_Vertex.xyz, 1.0);
}

[FRAGMENT]
#version 110

uniform sampler2D texture_map;
uniform vec4 color;

void main()
{
    gl_FragColor = color;
}
