#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform float alpha;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
    // FragColor = vec4(ourColor, alpha);
    // FragColor = texture(ourTexture, texCoord)* vec4(ourColor, alpha);
    FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(1. - texCoord.x, texCoord.y)), alpha);
}