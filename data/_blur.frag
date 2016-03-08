#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable


uniform sampler2D inputImageTexture;
uniform  float texelWidthOffset;
uniform  float texelHeightOffset;
 
varying  vec2 blurCoordinates[5];
 
void main()
{
    vec4 sum = vec4(0.0);
    sum += texture2D(inputImageTexture, blurCoordinates[0]) * 0.204164;
    sum += texture2D(inputImageTexture, blurCoordinates[1]) * 0.304005;
    sum += texture2D(inputImageTexture, blurCoordinates[2]) * 0.304005;
    sum += texture2D(inputImageTexture, blurCoordinates[3]) * 0.093913;
    sum += texture2D(inputImageTexture, blurCoordinates[4]) * 0.093913;
    gl_FragColor = sum;
}