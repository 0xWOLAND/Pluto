#version 330 core
in vec3 color;
out vec4 FragColor;

void main() {
    // FragColor = vec4(1.0f, 0.2, 0.6f, 1.0f);
    FragColor = vec4(color, 1.0);
}