[VERTEX]
layout (location = 0) in highp vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection_matrix;
uniform mat4 camera_matrix;
uniform mat4 object_matrix;
uniform vec3 object_scale;

uniform highp vec2 light_source_position;

void main()
{
    highp vec4 pos = object_matrix * vec4(position.xy * object_scale, 0.0, 1.0);
    highp vec2 diff = normalize(pos.xy - light_source_position);
    pos.xy += diff * position.z;
    gl_Position = projection_matrix * camera_matrix * pos;
}

[FRAGMENT]
uniform sampler2D texture_map;
uniform vec4 color;

out vec4 out_color;

void main()
{
    out_color = color;
}
