#version 410 core
out vec3 FragColor;

in vec2 exTexcoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

layout(shared) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


uniform vec3 samples[64];

uniform vec3 viewPos;

uniform float radius;
uniform float bias;

int kernelSize = 64;

void main()
{   
    
    vec2 texSize = textureSize(gPosition, 0);
    vec2 noiseScale = texSize/4.0;
    
    // get Fragment Position in World Space
    vec4 fragPos = vec4(texture(gPosition, exTexcoord));
    // transform to View Space
    fragPos = ViewMatrix * fragPos;

    // get View Space Normals
    vec3 normal = normalize(texture(gNormal, exTexcoord).rgb);
    normal = vec3(transpose(inverse(ViewMatrix)) * vec4(normal,1)).xyz;

    // calculate random direction
    vec3 randomVec = normalize(texture(texNoise, exTexcoord * noiseScale).xyz);

    // calculate TBN 
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    vec3 samplePos;
    vec4 offsetPosition;
    float sampleDepth;
    int rejectedSamples = 0;
    for(int i = 0; i < kernelSize; ++i)
    {   
        // Test for Background
        if (fragPos.x == 0 && fragPos.y == 0 && fragPos.z == 0) {
            occlusion = 1.0;
            break;
        }

        // get sample position
        samplePos = TBN * samples[i]; 
        samplePos = fragPos.xyz + samplePos * radius; 
        
        // calclate texture coordinates for sample position
        vec4 offset = vec4(samplePos, 1.0);
        offset = ProjectionMatrix * offset; 
        offset.xyz /= offset.w; 
        offset.xyz = offset.xyz * 0.5 + 0.5; 
        
        // get Position in World Space
        offsetPosition = texture(gPosition, offset.xy);
        vec4 testPosition = offsetPosition;

        // to View Space
        offsetPosition = ViewMatrix * offsetPosition;

        // test for background incontinuities
        bool add = true;
        if (testPosition.x == 0 && testPosition.y == 0 && testPosition.z == 0){
            add = false;
            rejectedSamples++;
        }

        
        // add to occlusion if geometry detected
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - offsetPosition.z));

        occlusion += (offsetPosition.z >= samplePos.z + bias ? 1.0 : 0.0) 
                     * rangeCheck 
                     * (add ? 1 : 0)
                     * (offset.x < 0 || offset.x > 1 ? 0 : 1)				// if texture coordinate is outside of texture
					 * (offset.y < 0 || offset.y > 1 ? 0 : 1);  
        
    }
    // account for misses (background samples)
    int leftOver = kernelSize - rejectedSamples;
    occlusion = (leftOver <= 0) ? 1.0 : (1.0 - (occlusion / leftOver));
    
    FragColor = vec3(occlusion);
}