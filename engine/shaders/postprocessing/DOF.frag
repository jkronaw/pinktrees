#version 430 core

out vec4 FragmentColor;

in vec2 exTexcoord;

// GBuffer
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gBloom;

uniform bool useDOF;
uniform vec2 gScreenSize;
uniform vec3 viewPos;

uniform float focalDepth;
uniform int dofSamples;

layout (shared) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


// functions to generate pseudo random numbers: Found on http://www.reedbeta.com/blog/quick-and-easy-gpu-random-numbers-in-d3d11/

uint wang_hash(uint n)
{
	n  = (n ^ 61u) ^ (n >> 16u);
	n *= 9u;
	n  = n ^ (n >> 4u);
	n *= 0x27d4eb2du;
	n  = n ^ (n >> 15u);
	return n;
}

uint xor_shift(inout uint state)
{
	state ^= (state << 13u);
	state ^= (state >> 17u);
	state ^= (state <<  5u);
	return state;
}

// inout to change state after every sample
float random_float(inout uint state)
{
	uint value = xor_shift(state);
	return float(value & 0x00ffffffu) / float(0x01000000u);
}

vec2 sample_disc(inout uint state)
{
	float radius = sqrt(random_float(state));
	float angle  = random_float(state) * 2.0 * 3.141592654;

	return radius * vec2( cos( angle ), sin( angle ) );
}

// random seed generator
uint init_rng()
{
	uint pixelID = uint(gl_FragCoord.x) + 2048u * uint(gl_FragCoord.y);
	uint seed = wang_hash(pixelID * 32u + uint(23456) % 32u);
	return seed;
}


void main()
{   
    
	// Without DOF:
	vec3 color = texture(gBloom, exTexcoord).rgb;
	FragmentColor = vec4(color, 1.0);

	if(useDOF){

		uint state = init_rng();
		vec3 position = texture(gPosition, exTexcoord).rgb;
		vec3 normal = texture(gNormal, exTexcoord).rgb;
		float z = (ViewMatrix * vec4(position ,1.0)).z;
		
		float depth_diff = abs(z - focalDepth);
		if(position == vec3(0,0,0) || (normal == vec3(0,0,0))){
			depth_diff = abs(50 - focalDepth);
			z = 999999999;
		}
		float disc_radius = depth_diff * 0.00009;
		vec4 color_sum = vec4(0.0);// vec4(color, 1.0);
		float num_valid = 0;
		
		for(int i = 0; i < dofSamples; i++){
			
			vec2 sample_pos = sample_disc(state);
			sample_pos = sample_pos * disc_radius;
			vec3 sample_pos_ws = texture(gPosition, exTexcoord + sample_pos).rgb;
			float sample_z = (ViewMatrix * vec4(sample_pos_ws, 1.0)).z;
			if(sample_pos_ws == vec3(0,0,0)){
				sample_z = 999999999;
			}
			if (z <= sample_z + 100){
				vec4 sample_color = vec4(texture(gBloom, exTexcoord + sample_pos).rgb , 1.0);
				
				color_sum += sample_color;
				num_valid++;
			}
		}
		FragmentColor = color_sum / num_valid;
	}
}
