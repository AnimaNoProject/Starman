#version 430 core

in float color;
out vec4 FragColor;

void main()
{
/*
	if(color < 0.2)
		FragColor = mix(vec4(vec3(0.0),1.0), vec4(0.0,0.5,1.0,1.0), color*10.0); 
	else if(color > 0.9)
		FragColor = mix(vec4(0.6,0.05,0.0,1.0), vec4(vec3(0.0),1.0), (color-0.9)*10.0); 
	else
		FragColor = mix(vec4(0.0,0.5,1.0,1.0), vec4(0.6,0.05,0.0,1.0), color);*/
	
	FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}	