#version 330 core

out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gShaded;
uniform sampler2D gBlur;
uniform sampler2D gMetallicRoughnessAO;

uniform vec2 gScreenSize;
uniform vec3 viewPos;

uniform float maxRayDistance;
uniform float stepResolution;
uniform int stepIterations;
uniform float tolerance;

layout(shared) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

in vec2 exTexcoord;

void main() {
	
	
    vec2 texSize  = textureSize(gShaded, 0).xy;

	int searchSteps = 10;

	vec4 texPos = vec4(0);
	float visibility = 0;

	vec2 finalLookupPos;
	vec4 finalLookupFragPositionView;
	vec4 finalLookupFragPositionWs;
	
	
	float roughness = texture(gMetallicRoughnessAO, exTexcoord).g;
	
	float metallic = texture(gMetallicRoughnessAO, exTexcoord).r;

	vec4 fragPos = texture(gPosition, exTexcoord);
	
	
	bool range = (length(fragPos.xyz - viewPos) <= 50);

	if (fragPos.x != 0 && fragPos.y != 0 && fragPos.z != 0 && range){

		vec3 fragN = normalize(texture(gNormal, exTexcoord).rgb);
	
		vec3 viewRay = (fragPos.xyz - viewPos.xyz);
		vec3 viewDir = normalize(viewRay);

		vec3 reflectionRay = normalize(reflect(viewDir, fragN));
	
		vec4 reflectionRayStartWs = vec4(fragPos.xyz + reflectionRay * 0.1, 1);
		vec4 reflectionRayEndWs = vec4(fragPos.xyz + reflectionRay * maxRayDistance, 1);

		vec4 reflectionRayStartView = ViewMatrix * reflectionRayStartWs;
		vec4 reflectionRayEndView = ViewMatrix * reflectionRayEndWs;

		vec4 reflectionRayStart = reflectionRayStartView;
		reflectionRayStart = ProjectionMatrix * reflectionRayStart;
		reflectionRayStart.xyz /= reflectionRayStart.w;
		reflectionRayStart.xy = reflectionRayStart.xy * 0.5 + 0.5;
		reflectionRayStart.xy *= texSize;

		vec4 reflectionRayEnd = reflectionRayEndView;
		reflectionRayEnd = ProjectionMatrix * reflectionRayEnd;
		reflectionRayEnd.xyz /= reflectionRayEnd.w;
		reflectionRayEnd.xy = reflectionRayEnd.xy * 0.5 + 0.5;
		reflectionRayEnd.xy *= texSize;

		vec2 lookupFrag = reflectionRayStart.xy;
		texPos.xy = lookupFrag / texSize;

		float deltaX = reflectionRayEnd.x - reflectionRayStart.x;
		float deltaY = reflectionRayEnd.y - reflectionRayStart.y;

		float useDeltaX = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
		float delta = mix(abs(deltaY), abs(deltaX), useDeltaX) * clamp(stepResolution, 0.0, 1.0);

		vec2 stepSize = vec2(deltaX, deltaY) / max(delta, 0.001);
		float lastSearchStep = 0;
		float nextSearchStep = 0;

		int firstPassHit = 0;
		int secondPassHit = 0;

		float viewDistance = -reflectionRayStartView.z;

		float depthDiff = tolerance;
	
		vec4 lookupFragPositionWs = vec4(0);
		vec4 lookupFragPositionView = vec4(0);

		for (int i = 0; i < stepIterations; ++i){
			lookupFrag += stepSize;
			texPos.xy = lookupFrag / texSize;
			lookupFragPositionWs = texture(gPosition, texPos.xy);
			lookupFragPositionView = ViewMatrix * lookupFragPositionWs;
	
			nextSearchStep = mix((lookupFrag.y - reflectionRayStart.y) / deltaY, 
						  (lookupFrag.x - reflectionRayStart.x) / deltaX,
						  useDeltaX);
			
			nextSearchStep = clamp(nextSearchStep, 0, 1);

			viewDistance = -(reflectionRayStartView.z * (reflectionRayEndView.z)) / mix(reflectionRayEndView.z, reflectionRayStartView.z, nextSearchStep);
		
			depthDiff = (viewDistance) - (-lookupFragPositionView.z);
			if (lookupFragPositionWs.r == 0 && lookupFragPositionWs.g == 0)
				depthDiff = 99999999;
	
			if (depthDiff > 0 && depthDiff < tolerance){
				firstPassHit = 1;
				break;
			} else {
				lastSearchStep = nextSearchStep;
			};
		}
	

		nextSearchStep = lastSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
	
		searchSteps *= firstPassHit;
		
		finalLookupPos = texPos.xy;
		finalLookupFragPositionView = lookupFragPositionView;
		finalLookupFragPositionWs = lookupFragPositionWs;

		for (int i = 0; i < searchSteps; ++i){
			lookupFrag = mix(reflectionRayStart.xy, reflectionRayEnd.xy, nextSearchStep);
			texPos.xy = lookupFrag / texSize;
			lookupFragPositionWs = texture(gPosition, texPos.xy);
			lookupFragPositionView = ViewMatrix * lookupFragPositionWs;

			viewDistance = -(reflectionRayStartView.z * reflectionRayEndView.z) / mix(reflectionRayEndView.z, reflectionRayStartView.z, nextSearchStep);
		
			depthDiff = (viewDistance) - (-lookupFragPositionView.z);
	
			if (depthDiff > 0 &&  depthDiff < tolerance ) {
				secondPassHit = 1;
				nextSearchStep = lastSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
			} else {
				float searchTemp = nextSearchStep;
				nextSearchStep = nextSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
				lastSearchStep = searchTemp;
			}
		}
		if (secondPassHit == 1){
			finalLookupPos = texPos.xy;
			finalLookupFragPositionView = lookupFragPositionView;
			finalLookupFragPositionWs = lookupFragPositionWs;
		}

		vec2 dCoords = smoothstep(0.2, 0.6, abs(vec2(0.5, 0.5) - texPos.xy));
		float screenEdgefactor = clamp(1.0 - (dCoords.x + dCoords.y), 0.0, 1.0);
		
		float angle = dot(reflectionRay, texture(gNormal,texPos.xy).rgb);

		visibility =((secondPassHit == 1 || firstPassHit == 1) ? 1 : 0)	// check if any geometry has been hit
					* finalLookupFragPositionWs.w							// discard background reflections
					* fragPos.r == 0 ? 0 : 1								// discard ray reflected from background
					* (1 - max(angle >= 0 ? 1 : angle , 0))					// discard ray that hit geometry from the inside
					* (1 - max(dot(-viewDir, reflectionRay), 0))			// if we look at the geometry from behind
					* (1 - clamp(depthDiff / tolerance, 0, 1))		// if we did not hit exactly
					* (1 - clamp(length(finalLookupFragPositionView - reflectionRayStartView) / maxRayDistance, 0, 1)) // fade out reflection strength depending on travelled ray distance
					* (texPos.x < 0 || texPos.x > 1 ? 0 : 1)				// if texture coordinate is outside of texture
					* (texPos.y < 0 || texPos.y > 1 ? 0 : 1)
					* screenEdgefactor										
					* metallic												// reflectivity of material 
					;
	
		visibility = clamp(visibility, 0, 1);
	}

	
	vec4 baseColor = texture(gShaded, exTexcoord);
	vec4 reflectionColorBlur = texture(gBlur, finalLookupPos.xy); 
	vec4 reflectionColor = texture(gShaded, finalLookupPos.xy);
	vec3 ref = mix(reflectionColorBlur.rgb, reflectionColor.rgb, roughness);

	fragColor = vec4(reflectionColor.rgb * visibility, visibility);
}