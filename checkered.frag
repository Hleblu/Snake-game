uniform vec2 gridSize;
uniform vec2 windowSize;
uniform vec3 fColor;
uniform vec3 sColor;

void main() {
    vec2 cell = floor(gl_FragCoord.xy * gridSize / windowSize);
    float checker = mod(cell.x + cell.y, 2.0);

    vec3 finalColor = mix(fColor, sColor, checker);

    gl_FragColor = vec4(finalColor, 1.0);
}   