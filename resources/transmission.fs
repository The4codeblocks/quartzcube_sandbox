#version 330

// Input vertex attributes (from vertex shader)
in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragColor;
in vec3 fragNormal;
in vec4 fragScreenPos;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D viewport;
uniform vec4 colDiffuse;
uniform vec4 colTransmit;


// Output fragment color
out vec4 finalColor;

vec3 blendRGBHAE(vec3 rgb1, vec3 rgb2, vec3 hae) {
    return rgb1 + hae * (rgb2 - rgb1);
}

void main() {
    vec2 screenPos = ((0.5*fragScreenPos.xy)/(fragScreenPos.w)) + vec2(0.5);

    //finalColor = fragColor + 0.*vec4(mod((fragPosition.xy-vec2(0.5,1)),vec2(1.,1.)),0.,0.) - 0.5 * texture(rFrame, abs(mod(mod((fragPosition.xy-vec2(0.5,1)),vec2(1.,1.)), textureSize(rFrame, 0))));
    vec3 fragHAE = (texture(texture1, fragTexCoord) * colTransmit).xyz;
    vec4 texelColor = texture(texture0, fragTexCoord) * colDiffuse;
    finalColor = vec4(blendRGBHAE(texture(viewport, screenPos).xyz, fragColor * texelColor.xyz, fragHAE * texelColor.w), 1.);
}
