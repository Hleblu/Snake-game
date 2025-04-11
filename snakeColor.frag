uniform vec4 color;

void main() {
	float position = gl_TexCoord[0].y;
	vec4 finalColor = mix(color, color * 0.7 , position);
	gl_FragColor = vec4(finalColor.r, finalColor.g, finalColor.b, 1.0);
}