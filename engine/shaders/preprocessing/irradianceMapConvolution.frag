#version 330 core

// position on cube in world space
in vec3 exPosition;

out vec4 FragmentColor;

// HDR environment map
uniform samplerCube cubemap;

const float PI = 3.14159265359;

void main()
{
    // use position as direction vector
    vec3 normal = normalize(exPosition);

    // derive coordinate system from normal and up
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up         = cross(normal, right);

    float sampleDelta = 0.025;
    int nrSamples = 0;

    vec3 irradiance = vec3(0.0);
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // calculate point on hemisphere (result is in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

            // transform from tangent space to world space
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 

            // take sample from the environment map
            // weighted by cos(theta) to account for the light's incident angle
            // weighted by sin(theta) to compensate for inequally distributed samples
            irradiance += texture(cubemap, sampleVec).rgb * cos(theta) * sin(theta);

            nrSamples++;
        }
    }

    FragmentColor = vec4(PI * irradiance / float(nrSamples), 1.0);
}