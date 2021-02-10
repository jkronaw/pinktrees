#version 330 core

out vec4 fragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gShaded;
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

	// defines number of refinement search steps
	int searchSteps = 10;

	vec4 texPos = vec4(0);
	float visibility = 0;

	vec2 finalLookupPos;
	vec4 finalLookupFragPositionView;
	vec4 finalLookupFragPositionWs;
	
	// get fragment pr0perties
	float roughness = texture(gMetallicRoughnessAO, exTexcoord).g;
	float metallic = texture(gMetallicRoughnessAO, exTexcoord).r;
	vec4 fragPos = texture(gPosition, exTexcoord);
	
	//define max range for fragments to be regarded as reflective
	bool range = (length(fragPos.xyz - viewPos) <= 50);

	//discard fragments of background, those that aren't fully metallic (this is implemented to limit SSR on the water surface) / out of range 
	if (fragPos.x != 0 && fragPos.y != 0 && fragPos.z != 0 && metallic == 1.0 && range){
		
		vec3 fragN = normalize(texture(gNormal, exTexcoord).rgb);
		
		// calculate view direction
		vec3 viewRay = (fragPos.xyz - viewPos.xyz);
		vec3 viewDir = normalize(viewRay);

		// calculate reflection direction
		vec3 reflectionRay = normalize(reflect(viewDir, fragN));
	
		// define start and end point of reflection ray 
		vec4 reflectionRayStartWs = vec4(fragPos.xyz + reflectionRay * 0.1, 1);
		vec4 reflectionRayEndWs = vec4(fragPos.xyz + reflectionRay * maxRayDistance, 1);

		vec4 reflectionRayStartView = ViewMatrix * reflectionRayStartWs;
		vec4 reflectionRayEndView = ViewMatrix * reflectionRayEndWs;

		// calculate start point of ray in texture space
		vec4 reflectionRayStart = reflectionRayStartView;
		reflectionRayStart = ProjectionMatrix * reflectionRayStart;
		reflectionRayStart.xyz /= reflectionRayStart.w;
		reflectionRayStart.xy = reflectionRayStart.xy * 0.5 + 0.5;
		reflectionRayStart.xy *= texSize;

		// calculate end point of ray in texture space
		vec4 reflectionRayEnd = reflectionRayEndView;
		reflectionRayEnd = ProjectionMatrix * reflectionRayEnd;
		reflectionRayEnd.xyz /= reflectionRayEnd.w;
		reflectionRayEnd.xy = reflectionRayEnd.xy * 0.5 + 0.5;
		reflectionRayEnd.xy *= texSize;

		// define fragment position for lookup
		vec2 lookupFrag = reflectionRayStart.xy;
		texPos.xy = lookupFrag / texSize;

		// calculate ray length in x and y direction
		float deltaX = reflectionRayEnd.x - reflectionRayStart.x;
		float deltaY = reflectionRayEnd.y - reflectionRayStart.y;

		// set delta to highest between the two 
		float useDeltaX = abs(deltaX) >= abs(deltaY) ? 1.0 : 0.0;
		float delta = mix(abs(deltaY), abs(deltaX), useDeltaX) * clamp(stepResolution, 0.0, 1.0);

		//define stepsize with regards to delta
		vec2 stepSize = vec2(deltaX, deltaY) / max(delta, 0.001);

		float lastSearchStep = 0;
		float nextSearchStep = 0;
		int firstPassHit = 0;
		int secondPassHit = 0;
		float viewDistance = -reflectionRayStartView.z;
		float depthDiff = tolerance;
		vec4 lookupFragPositionWs = vec4(0);
		vec4 lookupFragPositionView = vec4(0);

		// travel along reflection ray
		for (int i = 0; i < stepIterations; ++i){

			// move along ray in texture space according to stepsize
			lookupFrag += stepSize;
			texPos.xy = lookupFrag / texSize;

			// get position data for respective fragment
			lookupFragPositionWs = texture(gPosition, texPos.xy);
			lookupFragPositionView = ViewMatrix * lookupFragPositionWs;
	
			// get ratio of ray length that has been travelled 
			nextSearchStep = mix((lookupFrag.y - reflectionRayStart.y) / deltaY, 
						  (lookupFrag.x - reflectionRayStart.x) / deltaX,
						  useDeltaX);
			
			nextSearchStep = clamp(nextSearchStep, 0, 1);

			// get distance to that ray progress in view space
			viewDistance = -(reflectionRayStartView.z * (reflectionRayEndView.z)) / mix(reflectionRayEndView.z, reflectionRayStartView.z, nextSearchStep);
			
			// calculate difference in distance from camera of ray position and fragment position from geometry
			depthDiff = (viewDistance) - (-lookupFragPositionView.z);

			// discard background fragments
			if (lookupFragPositionWs.r == 0 && lookupFragPositionWs.g == 0)
				depthDiff = 99999999;
	
			// if ray position is closely behind geometry ("hit" geometry) stop search, otherwise progress further
			if (depthDiff > 0 && depthDiff < tolerance){
				firstPassHit = 1;
				break;
			} else {
				lastSearchStep = nextSearchStep;
			};
		}
	
		// start between last miss and hit
		nextSearchStep = lastSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
		
		// discard ray that have not hit anything
		searchSteps *= firstPassHit;
		
		finalLookupPos = texPos.xy;
		finalLookupFragPositionView = lookupFragPositionView;
		finalLookupFragPositionWs = lookupFragPositionWs;

		// refinement of hit position
		for (int i = 0; i < searchSteps; ++i){
			
			lookupFrag = mix(reflectionRayStart.xy, reflectionRayEnd.xy, nextSearchStep);
			texPos.xy = lookupFrag / texSize;
			lookupFragPositionWs = texture(gPosition, texPos.xy);
			lookupFragPositionView = ViewMatrix * lookupFragPositionWs;

			viewDistance = -(reflectionRayStartView.z * reflectionRayEndView.z) / mix(reflectionRayEndView.z, reflectionRayStartView.z, nextSearchStep);
		
			depthDiff = (viewDistance) - (-lookupFragPositionView.z);
			
			// if ray position still behind geometry -> go back half the distance to last miss, otherwise advance half the distance
			if (depthDiff > 0 && depthDiff < tolerance ) {
				secondPassHit = 1;
				nextSearchStep = lastSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
			} else {
				float searchTemp = nextSearchStep;
				nextSearchStep = nextSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
				lastSearchStep = searchTemp;
			}
		}
		// only update lookup positions if refinement was successfull
		if (secondPassHit == 1){
			finalLookupPos = texPos.xy;
			finalLookupFragPositionView = lookupFragPositionView;
			finalLookupFragPositionWs = lookupFragPositionWs;
		}

		// smoothly discard fragments close to edge of image to hide discontinuities
		vec2 smoothCoords = smoothstep(0.25, 0.55, abs(vec2(0.5, 0.5) - texPos.xy));
		float screenEdgefactor = clamp(1.0 - (smoothCoords.x + smoothCoords.y), 0.0, 1.0);
		
		// calculate cos of angle between rflection and normal of hit geometry
		float angle = dot(reflectionRay, texture(gNormal,texPos.xy).rgb);

		visibility =((secondPassHit == 1) ? 1 : 0)		// check if any geometry has been hit
					* finalLookupFragPositionWs.w							// discard background reflections
					* fragPos.r == 0 ? 0 : 1								// discard ray reflected from background
					* (1 - max(angle >= 0 ? 1 : angle , 0))					// discard ray that hit geometry from the inside
					* (1 - max(dot(-viewDir, reflectionRay), 0))			// if we look at the geometry from behind
					* (1 - clamp(depthDiff / tolerance, 0, 1))				// if we did not hit exactly
					* (1 - clamp(length(finalLookupFragPositionView - reflectionRayStartView) / maxRayDistance, 0, 1)) // fade out reflection strength depending on travelled ray distance
					* (texPos.x < 0 || texPos.x > 1 ? 0 : 1)				// if texture coordinate is outside of texture
					* (texPos.y < 0 || texPos.y > 1 ? 0 : 1)				// if texture coordinate is outside of texture
					* screenEdgefactor										// if we are close to edge of reflectable fragments
					* metallic												// reflectivity of material 
					;
	
		visibility = clamp(visibility, 0, 1);
	}

	vec4 reflectionColor = texture(gShaded, finalLookupPos.xy);

	// save visibility in alpha channel for blending in different shader
	fragColor = vec4(reflectionColor.rgb * visibility, visibility);
}