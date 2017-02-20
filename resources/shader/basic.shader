[VERTEX]
#version 110

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;
uniform vec3 object_scale;

varying vec2 v_uv;

void main()
{
    gl_Position = projection_matrix * camera_matrix * object_matrix * vec4(gl_Vertex.xyz * object_scale, 1.0);
    v_uv = gl_MultiTexCoord0.xy;
}

[FRAGMENT]
#version 110
uniform sampler2D texture_map;
uniform vec4 color;

varying vec2 v_uv;

void main()
{
    gl_FragColor = texture2D(texture_map, v_uv) * color;
}
