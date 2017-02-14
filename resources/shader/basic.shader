[VERTEX]
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;
uniform vec3 object_scale;

varying highp vec2 v_uv;

void main()
{
    gl_Position = projection_matrix * camera_matrix * object_matrix * vec4(position.xyz * object_scale, 1.0);
    v_uv = uv;
}

[FRAGMENT]
uniform sampler2D texture_map;
uniform vec4 color;

varying highp vec2 v_uv;

out vec4 out_color;

void main()
{
    out_color = texture2D(texture_map, v_uv) * color;
}
