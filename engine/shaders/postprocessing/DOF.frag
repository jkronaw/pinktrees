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

uint wangHash(uint n)
{
	n  = (n ^ 61u) ^ (n >> 16u);
	n *= 9u;
	n  = n ^ (n >> 4u);
	n *= 0x27d4eb2du;
	n  = n ^ (n >> 15u);
	return n;
}

uint xorShift(inout uint state)
{
	state ^= (state << 13u);
	state ^= (state >> 17u);
	state ^= (state <<  5u);
	return state;
}

// inout to change state after every sample
float randomFloat(inout uint state)
{
	uint value = xorShift(state);

	//account for float precision 
	return float(value & 0x00ffffffu) / float(0x01000000u);
}

vec2 sampleDisc(inout uint seed)
{
	float radius = sqrt(randomFloat(seed));
	float angle  = randomFloat(seed) * 2.0 * 3.141592654;

	return radius * vec2(cos(angle), sin(angle));
}

// random seed generator
uint initializeSeed()
{
	uint id = uint(gl_FragCoord.x) + 2048u * uint(gl_FragCoord.y);
	uint seed = wangHash(id);
	return seed;
}


void main()
{   
    
	// Without DOF:
	vec3 color = texture(gBloom, exTexcoord).rgb;
	FragmentColor = vec4(color, 1.0);

	// With DOF:
	if(useDOF){

		uint seed = initializeSeed();

		// retrieve fragment properties
		vec3 position = texture(gPosition, exTexcoord).rgb;
		vec3 normal = texture(gNormal, exTexcoord).rgb;

		// get distance to camera
		float z = (ViewMatrix * vec4(position ,1.0)).z;
		
		// calculate difference to focal depth
		float depthDiff = abs(z - focalDepth);

		// set difference for background framgent manually
		if(position == vec3(0,0,0) || (normal == vec3(0,0,0))){
			depthDiff = abs(50 - focalDepth);
			z = 999999999;
		}

		// calculate sample radius
		float discRadius = depthDiff * 0.00009;

		vec4 accumulatedColor = vec4(0.0);
		float validSamples = 0;
		
		for(int i = 0; i < dofSamples; i++){
			
			// sample a direction according to radis of confusion
			vec2 samplePos = sampleDisc(seed);
			samplePos = samplePos * discRadius;

			// get distance to geometry at sampled coordinate
			vec3 samplePosWs = texture(gPosition, exTexcoord + samplePos).rgb;
			float samplePosVsZ = (ViewMatrix * vec4(samplePosWs, 1.0)).z;

			// manually set value for background
			if(samplePosWs == vec3(0,0,0)){
				samplePosVsZ = 999999999;
			}

			// only accumulate color of fragments that are behind center fragment (+ tolerance)
			if (z <= samplePosVsZ + 100){
				vec4 sampleColor = vec4(texture(gBloom, exTexcoord + samplePos).rgb , 1.0);
				
				accumulatedColor += sampleColor;
				validSamples++;
			}
		}

		// normalize
		FragmentColor = accumulatedColor / validSamples;
	}
}
