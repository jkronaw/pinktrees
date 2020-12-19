#version 430 core

out vec4 FragmentColor;

in vec2 texcoord;

// GBuffer
uniform sampler2D gPosition;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D gMetallicRoughnessAO;
uniform sampler2D gTexCoord;
uniform sampler2D gShaded;
uniform sampler2D gBloom;

uniform bool useDOF;
uniform vec2 gScreenSize;
uniform vec3 viewPos;

layout (shared) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

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

uint init_rng()
{
	uint pixelID = uint(gl_FragCoord.x) + 2048u * uint(gl_FragCoord.y);
	uint seed = wang_hash(pixelID * 32u + uint(23456) % 32u);
	return seed;
}


void main()
{   
    
	// Without Post Processing:

	vec3 color = texture(gShaded, texcoord).rgb;
	FragmentColor = vec4(color, 1.0);


	// very simple DOF:
	
	if(useDOF){

		uint state = init_rng();
		vec3 position = texture(gPosition, texcoord).rgb;
		float z = -(ViewMatrix * vec4(position ,1.0)).z;
		
		float depth_diff = abs(z - 4);
		float disc_radius = depth_diff * 0.0005;
		vec4 color_sum = vec4(0.0);
		float num_valid = 0;
		
		for(int i = 0; i < 150; i++){
			
			vec2 sample_pos = sample_disc(state);
			sample_pos = sample_pos * disc_radius;
			vec3 sample_pos_ws = texture(gPosition, texcoord + sample_pos).rgb;
			float sample_z = (viewPos - sample_pos_ws).z;
		
			vec4 sample_color = vec4(texture(gShaded, texcoord + sample_pos).rgb , 1.0);
			
			color_sum += sample_color;
			num_valid++;
		}
	FragmentColor = color_sum / num_valid;
	}
}
