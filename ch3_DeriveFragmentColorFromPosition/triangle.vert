#version 430 core

void main(void)
{
	// Declare a hard-coded array of positions
	const vec4 vertices[3] = vec4[3](
		vec4( 0.25, -0.25, 0.5, 1.0),
		vec4(-0.25, -0.25, 0.5, 1.0),
		vec4( 0.25, 0.25, 0.5, 1.0));

	// Add "offset" to our hard-coded vertex position
	gl_Position = vertices[gl_VertexID];
}