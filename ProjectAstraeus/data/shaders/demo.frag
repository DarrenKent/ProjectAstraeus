uniform sampler2D tColorMap;
varying vec3 tVertexNormal, tVertexPosition, tEyePosition;

struct Light {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float constAttenuation, linearAttenuation, quadAttenuation;
	float cutoff, exponent;
	vec3 direction;
};

const int LIGHT_COUNT = 1;
Light LIGHTS[LIGHT_COUNT];

Light tLight0 = Light (
	vec4( -2000.0, -2000.0, 0.0, 1.0 ),
	vec4( 1.0, 1.0, 1.0, 1.0 ),
	vec4( 1.0, 1.0, 1.0, 1.0 ),
	0.0, 0.0001, 0.0,
	180.0, 0.0,
	vec3( 0.0, 0.0, 0.0 )
);

void main (void)
{
	LIGHTS[0] = tLight0;

	vec3 tNormalDirection	= normalize( tVertexNormal );
	vec3 tEyeDirection		= normalize( tEyePosition );
	vec3 tLightDirection	= vec3( 0.0, 0.0, 0.0 );
	float tAttenuation		= 1.0;

	vec3 tSceneAmbient		= vec3( 0.5, 0.5, 0.5 );
	vec3 tFinalLight		= vec3( gl_FrontMaterial.ambient ) * tSceneAmbient;

	for ( int iLight = 0; iLight < LIGHT_COUNT; iLight++ ) {
		vec3 tLightPosition = vec3( LIGHTS[iLight].position.x, LIGHTS[iLight].position.y, LIGHTS[iLight].position.z );

		if ( LIGHTS[iLight].position.w == 0.0 ) {
			tAttenuation	= 1.0;
			tLightDirection = normalize( vec3( tLightPosition ) );
		} else {
			vec3 tPosition	= vec3( tLightPosition - tVertexPosition );
			float tDistance = length( tPosition );
			tLightDirection = normalize( tPosition );
			tAttenuation	= 1.0 / ( LIGHTS[iLight].constAttenuation + 
									  LIGHTS[iLight].linearAttenuation * tDistance + 
									  LIGHTS[iLight].quadAttenuation * tDistance * tDistance );
			if ( LIGHTS[iLight].cutoff <= 90.0 ) {
				float tClamped = max( 0.0, dot( -tLightDirection, normalize( LIGHTS[iLight].direction ) ) );
				if ( tClamped < cos( radians( LIGHTS[iLight].cutoff ) ) ) {
					tAttenuation = 0.0;
				} else {
					tAttenuation += pow( tClamped, LIGHTS[iLight].exponent );
				}
			}
		}
		vec3 tDiffuse	= tAttenuation * vec3( LIGHTS[iLight].diffuse ) * vec3( gl_FrontMaterial.diffuse ) * max( 0.0, dot( tNormalDirection, tLightDirection ) );
		vec3 tSpecular	= vec3( 0.0, 0.0, 0.0 );
		if ( dot( tNormalDirection, tLightDirection ) >= 0.0 ) {
			tSpecular = tAttenuation * vec3( LIGHTS[iLight].specular ) * vec3( gl_FrontMaterial.specular ) *
						pow( max( 0.0, dot( reflect( -tLightDirection, tNormalDirection ), tEyeDirection ) ), gl_FrontMaterial.shininess );
		}

		tFinalLight += tDiffuse + tSpecular;
	}

	gl_FragColor = vec4( tFinalLight * texture2D( tColorMap, gl_TexCoord[0].st), 1.0 );
}