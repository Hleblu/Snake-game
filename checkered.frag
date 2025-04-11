uniform float tileSize;
void main() {
    vec2 cell = floor(gl_FragCoord.xy / tileSize);
    float alpha = 1.0 - mod(cell.x + cell.y, 2.0);
    gl_FragColor = vec4(1.0, 1.0, 1.0, alpha);
}  