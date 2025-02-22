uniform vec2 gridSize;
uniform vec2 windowSize;

void main() {
    vec2 uv = gl_FragCoord.xy * gridSize / windowSize; // Scale coordinates
    vec2 cell = floor(uv); // Get integer cell indices

    float checker = mod(cell.x + cell.y, 2.0);

    vec3 fColor = vec3(172.0 / 255.0, 206.0 / 255.0, 94.0 / 255.0);
    vec3 sColor = vec3(114.0 / 255.0, 183.0 / 255.0, 106.0 / 255.0);
    
    vec3 finalColor = mix(fColor, sColor, vec3(checker));

    gl_FragColor = vec4(finalColor, 1.0);
}
