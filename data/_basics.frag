#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable


uniform sampler2DRect texture0;

uniform float lightness; 
uniform float saturation; 
uniform float red; 
uniform float green; 
uniform float blue; 
uniform float brightness; 

uniform float contrast; 
uniform float hue;
uniform float invert;

uniform float stereoFx;


void main(){
	//Getting coordinates of the current pixel in texture
	vec2 pos = gl_TexCoord[0].xy;

	//Getting the pixel color from the texture texture0 in pos
	vec4 color = texture2DRect(texture0, pos);

    //STEREO FX
    color.r = texture2DRect(texture0, vec2(pos.x +stereoFx, pos.y)).r;
    color.g = texture2DRect(texture0, vec2(pos.x, pos.y)).g;
    color.b = texture2DRect(texture0, vec2(pos.x -stereoFx, pos.y)).b;


    //BRIGHTNESS
    color.r = color.r * brightness;
	color.g = color.g * brightness;
	color.b = color.b * brightness;

    //CONTRAST
    //float c = contrast *2;
    color.r = (color.r - 0.5) * contrast+ 0.5;
    color.g = (color.g - 0.5) * contrast+ 0.5;
    color.b = (color.b - 0.5) * contrast+ 0.5;


    //LIGHTNESS
    color.r = color.r + lightness - 1;
    color.g = color.g + lightness - 1;
    color.b = color.b + lightness - 1;

    //RGB
    color.r = color.r * red;
    color.g = color.g * green;
    color.b = color.b * blue;
    
    //SATURATION
    float P = sqrt((color.r)*(color.r) * 0.299 + (color.b)*(color.b) * 0.587 + (color.g)*(color.g) * 0.114 ) ;
	color.r = P+(color.r-P)*saturation;
    color.g = P+(color.g-P)*saturation;
    color.b = P+(color.b-P)*saturation;

    //INVERT
    if(invert !=0)
    {
        color.r = 1 - color.r;
        color.g = 1 - color.g;
        color.b = 1 - color.b;
    }

	//Output the color of shader
	gl_FragColor = color;
}
