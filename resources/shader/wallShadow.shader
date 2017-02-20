[VERTEX]
#version 110

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;
uniform vec3 object_scale;

uniform vec2 light_source_position;

void main()
{
    vec4 pos = object_matrix * vec4(gl_Vertex.xy * object_scale, 0.0, 1.0);
    vec2 diff = normalize(pos.xy - light_source_position);
    pos.xy += diff * gl_Vertex.z;
    gl_Position = projection_matrix * camera_matrix * pos;
}

[FRAGMENT]
#version 110

uniform sampler2D texture_map;
uniform vec4 color;

void main()
{
    gl_FragColor = color;
}
