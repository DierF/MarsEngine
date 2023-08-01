//circle texture shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_worldPosition;
layout(location = 1) in vec3 a_localPosition;
layout(location = 2) in vec4 a_color;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in int a_entityID;

uniform mat4 u_viewProjection;

struct VertexOutput
{
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) out VertexOutput outputs;
layout (location = 4) out flat int v_entityID;

void main()
{
	outputs.localPosition = a_localPosition;
	outputs.color = a_color;
	outputs.thickness = a_thickness;
	outputs.fade = a_fade;

	v_entityID = a_entityID;

	gl_Position = u_viewProjection * vec4(a_worldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_color;
layout(location = 1) out int o_entityID;

struct VertexInput
{
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in VertexInput inputs;
layout (location = 4) in flat int v_entityID;

void main()
{
    // Calculate distance and fill circle with white
    float distance = 1.0 - length(inputs.localPosition);
    float circle = smoothstep(0.0, inputs.fade, distance);
    circle *= smoothstep(inputs.thickness + inputs.fade, inputs.thickness, distance);

	if (circle == 0.0)
		discard;

    // Set output color
    o_color = inputs.color;
	o_color.a *= circle;

	o_entityID = v_entityID;
}