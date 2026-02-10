#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;

// Output fragment color
out vec4 finalColor;

void main() {
    finalColor = vec4(fragTexCoord, fragPosition.z * 0.125, 1.);
}
