#version 430 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform float far;
uniform float near;

mat3 sobel_y = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

mat3 sobel_x = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);

float LinearizeDepth(float z)
{
  float n = near;
  float f = far;
  return (2.0 * n) / (f + n - z * (f - n));	
}

void main()
{ 
	vec3 colorDiff = texture(colorTexture, TexCoords).rgb;

	mat3 I;
	vec3 texel;

    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
			vec3 test = texelFetch(depthTexture, ivec2(gl_FragCoord) + ivec2(i-1, j-1), 0).rgb;
			vec3 tempvec = vec3(LinearizeDepth(test.x));
            I[i][j] = length(tempvec); 
		}
	}

	float gx = dot(sobel_x[0], I[0]) + dot(sobel_x[1], I[1]) + dot(sobel_x[2], I[2]); 
	float gy = dot(sobel_y[0], I[0]) + dot(sobel_y[1], I[1]) + dot(sobel_y[2], I[2]);

	float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));

	FragColor = vec4(colorDiff - vec3(g), 1.0);
}

