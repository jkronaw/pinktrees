#version 330 core

out vec4 fragColor;

layout(shared) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gShaded;

uniform vec2 gScreenSize;
uniform vec3 viewPos;

in vec2 exTexcoord;

void main() {

	float maxRayDistance = 15;
	float stepResolution = 0.3;
	int searchSteps = 10;
	float tolerance = 0.5;

	vec4 texPos = vec4(0);

	vec4 fragPos = texture(gPosition, exTexcoord);
	vec3 fragN = normalize(texture(gNormal, exTexcoord).rgb);

	vec3 viewRay = (fragPos.xyz - viewPos.xyz);
	vec3 viewDir = normalize(viewRay);

	vec3 reflectionRay = normalize(reflect(viewDir, fragN));

	vec4 reflectionRayStartWs = vec4(fragPos.xyz, 1);
	vec4 reflectionRayEndWs = vec4(fragPos.xyz + reflectionRay * maxRayDistance, 1);

	vec4 reflectionRayStartView = ViewMatrix * reflectionRayStartWs;
	vec4 reflectionRayEndView = ViewMatrix * reflectionRayEndWs;

	vec4 reflectionRayStart = reflectionRayStartView;
	reflectionRayStart = ProjectionMatrix * reflectionRayStart;
	reflectionRayStart.xyz /= reflectionRayStart.w;
	reflectionRayStart.xy = reflectionRayStart.xy * 0.5 + 0.5;
	reflectionRayStart.xy *= gScreenSize;

	vec4 reflectionRayEnd = reflectionRayEndView;
	reflectionRayEnd = ProjectionMatrix * reflectionRayEnd;
	reflectionRayEnd.xyz /= reflectionRayEnd.w;
	reflectionRayEnd.xy = reflectionRayEnd.xy * 0.5 + 0.5;
	reflectionRayEnd.xy *= gScreenSize;

	vec2 lookupFrag = reflectionRayStart.xy;
	texPos.xy = lookupFrag / gScreenSize;

	float deltaX = reflectionRayEnd.x - reflectionRayStart.x;
	float deltaY = reflectionRayEnd.y - reflectionRayStart.y;

	float useDeltaX = abs(deltaX) >= abs(deltaY) ? 1 : 0;
	float delta = mix(abs(deltaY), abs(deltaX), useDeltaX) * clamp(stepResolution, 0, 1);

	vec2 stepSize = vec2(deltaX, deltaY) / max(delta, 0.001);

	float lastSearchStep = 0;
	float nextSearchStep = 0;

	int firstPassHit = 0;
	int secondPassHit = 0;

	float viewDistance = reflectionRayStartView.z;
	float depthDiff = tolerance;

	vec4 lookupFragPositionWs = vec4(0);
	vec4 lookupFragPositionView = vec4(0);

	for (float i = 0; i < int(delta); ++i){
		lookupFrag += stepSize;
		texPos.xy = lookupFrag / gScreenSize;
		lookupFragPositionWs = texture(gPosition, texPos.xy);
		lookupFragPositionView = ViewMatrix * lookupFragPositionWs;

		nextSearchStep = mix((lookupFrag.y - reflectionRayStart.y) / deltaY, 
					  (lookupFrag.x - reflectionRayStart.x) / deltaX,
					  useDeltaX);
		
		viewDistance = (reflectionRayStartView.z * reflectionRayEndView.z) / mix(reflectionRayEndView.z, reflectionRayStartView.z, nextSearchStep);
		
		depthDiff = viewDistance - lookupFragPositionView.z;

		if (depthDiff > 0 && depthDiff < tolerance){
			firstPassHit = 1;
			break;
		} else {
			lastSearchStep = nextSearchStep;
		};
	}

	nextSearchStep = lastSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);

	searchSteps *= firstPassHit;

	for (float i = 0; i < searchSteps; ++i){
		lookupFrag = mix(reflectionRayStart.xy, reflectionRayEnd.xy, nextSearchStep);
		texPos.xy = lookupFrag / gScreenSize;
		lookupFragPositionWs = texture(gPosition, texPos.xy);
		lookupFragPositionView = ViewMatrix * lookupFragPositionWs;

		viewDistance = (reflectionRayStartView.z * reflectionRayEndView.z) / mix(reflectionRayEndView.z, reflectionRayStartView.z, nextSearchStep);
		
		depthDiff = viewDistance - lookupFragPositionView.z;

		if (depthDiff > 0 && depthDiff < tolerance){
			secondPassHit = 1;
			nextSearchStep = lastSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
		} else {
			float searchTemp = nextSearchStep;
			nextSearchStep = lastSearchStep + ((nextSearchStep - lastSearchStep) * 0.5);
			lastSearchStep = searchTemp;
		}
	}

	float visibility = secondPassHit * lookupFragPositionView.w				// if there is a fragment at all
					   * (1 - max(dot(-viewDir, reflectionRay), 0))			// if we look at the geometry from behind
					   * (1 - clamp(depthDiff / tolerance, 0, 1))			// if we did not hit exactly
					   * (1 - clamp(length(lookupFragPositionView - reflectionRayStartView) / maxRayDistance, 0, 1)) // fade out reflection strength depending on travelled ray distance
					   * (texPos.x < 0 || texPos.x > 1 ? 0 : 1)		// if texture coordinate is outside of texture
					   * (texPos.y < 0 || texPos.y > 1 ? 0 : 1);

	visibility = clamp(visibility, 0, 1);


	vec4 baseColor = texture(gShaded, exTexcoord);
	vec4 reflectionColor = texture(gShaded, texPos.xy);
	
	fragColor = vec4(mix(baseColor.rgb, reflectionColor.rgb, visibility), 1);