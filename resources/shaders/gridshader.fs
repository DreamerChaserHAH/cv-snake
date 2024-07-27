#version 100

precision mediump float;

uniform vec2 numCells;

void main()
{
    vec2 cellSize = 1.0 / numCells;
    vec2 cell = fract(gl_FragCoord.xy / cellSize);
    vec2 grid = mod(cell, 2.0);
    float gridValue = 1.0 - max(grid.x, grid.y);
    
    float gridLineThickness = 0.01;
    vec3 gridLineColor = vec3(0.0, 0.0, 0.0);
    vec3 backgroundColor = vec3(1.0, 1.0, 1.0);

    // Draw grid lines
    vec3 color = mix(backgroundColor, gridLineColor, step(gridLineThickness, cell.x) * step(gridLineThickness, cell.y));
    
    gl_FragColor = vec4(color, 1.0);
}
