uniform vec2 gridSize;
uniform vec2 windowSize;

void main() {
    vec2 cell = floor(gl_FragCoord.xy * gridSize / windowSize);
    float checker = mod(cell.x + cell.y, 2.0);

    vec3 finalColor = mix(vec3(0.675, 0.808, 0.369), vec3(0.447, 0.718, 0.416), checker);

    gl_FragColor = vec4(finalColor, 1.0);
}