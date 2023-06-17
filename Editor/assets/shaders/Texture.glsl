//basic texture shader

#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_textCoord;
layout(location = 3) in float a_textIndex;
layout(location = 4) in float a_tilingFactor;
layout(location = 5) in int a_entityID;

uniform mat4 u_viewProjection;

out vec2 v_textCoord;
out vec4 v_color;
out float v_textIndex;
out float v_tilingFactor;
out flat int v_entityID;

void main()
{
	v_color = a_color;
	v_textCoord = a_textCoord;
	v_textIndex = a_textIndex;
	v_tilingFactor = a_tilingFactor;
	v_entityID = a_entityID;
	gl_Position = u_viewProjection * vec4(a_position, 1.0);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec4 v_color;
in vec2 v_textCoord;
in float v_textIndex;
in float v_tilingFactor;
in flat int v_entityID;

uniform sampler2D u_texture[32];

void main()
{
	color = texture(u_texture[int(v_textIndex)], v_textCoord * v_tilingFactor) * v_color;

	color2 = v_entityID;
}