#version 330 core
out vec4 FragColor;
  
in vec2 texcoord;

uniform sampler2D gBloom;
 
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{   
    float newWeights[5];
    for (int i = 0; i < weight.length(); i++){
        newWeights[i] = weight[i] * 1;
    }

    vec2 tex_offset = 1.0 / textureSize(gBloom, 0); 
    vec3 result = texture(gBloom, texcoord).rgb * newWeights[0]; 
    for(int i = 1; i < 5; ++i)
    {
        result += texture(gBloom, texcoord + vec2(0.0, tex_offset.y * i)).rgb * newWeights[i];
        result += texture(gBloom, texcoord - vec2(0.0, tex_offset.y * i)).rgb * newWeights[i];
    }
    FragColor = vec4(result, 1.0);
}
