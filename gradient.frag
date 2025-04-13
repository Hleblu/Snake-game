uniform vec4 startColor;
uniform vec4 endColor;

void main() {
	float position = gl_TexCoord[0].y;
	gl_FragColor = mix(startColor, endColor, position);
}