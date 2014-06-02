varying vec3 tVertexNormal, tVertexPosition, tEyePosition;

void main()
{	
	tVertexNormal	= normalize( vec3( gl_NormalMatrix * gl_Normal ) );
	tVertexPosition = vec3( gl_ModelViewMatrix * gl_Vertex );
	tEyePosition	= vec3( gl_ModelViewMatrix * gl_Vertex );

	gl_Position		= ftransform();		
}
