#version 330 core
in vec3 color;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform float mixVal;
uniform sampler2D texture2;

void main() {
    // FragColor = vec4(1.0f, 0.2, 0.6f, 1.0f);
    //  FragColor = vec4(color, 1.0);
   FragColor = vec4(color, 1.0f) * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixVal);
}
