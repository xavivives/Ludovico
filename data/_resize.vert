#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform float offsetX = 0.0;
uniform float offsetY = 0.0;	
uniform float scale = 0.0;

void main() {
	//Get original position of the vertex
	vec3 v = gl_Vertex.xyz;

	v.x = offsetX + v.x * scale;
	v.y = offsetY + v.y * scale;
	v.z = 1;

	//Set output vertex position
	vec4 posHomog = vec4( v, 1.0 );
	gl_Position = gl_ModelViewProjectionMatrix * posHomog;

	//Set output texture coordinate and color in a standard way
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
