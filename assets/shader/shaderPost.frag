#version 430 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

mat3 sobel_x = mat3( 
    1.0, 0.0, -1.0, 
    1.0, 0.0, -1.0, 
    1.0, 0.0, -1.0 
);

mat3 sobel_y = mat3( 
    1.0, 1.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -1.0, -1.0 
);

void main()
{ 
	vec3 colorDiff = texture(screenTexture, TexCoords).rgb;
	mat3 I;
	vec3 texel;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            texel  = texelFetch(screenTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0).rgb;
            I[i][j] = length(texel); 
		}
	}

	float gx = dot(sobel_x[0], I[0]) + dot(sobel_x[1], I[1]) + dot(sobel_x[2], I[2]); 
	float gy = dot(sobel_y[0], I[0]) + dot(sobel_y[1], I[1]) + dot(sobel_y[2], I[2]);


	float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));
	FragColor = vec4(colorDiff - vec3(g), 1.);
}















