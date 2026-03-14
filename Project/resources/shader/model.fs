#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;
in vec3 Bitangent;

flat in int isEnablePos;
flat in int isEnableColor;
flat in int isEnableNormal;
flat in int isEnableTexCoords;
flat in int isEnableTangent;
flat in int isEnableBitangent;
flat in int isSelected;

uniform bool renderID;
uniform vec4 objectID;

uniform sampler2D texture_diffuse1;

void main()
{
    if (0 != isEnableTexCoords) {
        FragColor = texture(texture_diffuse1, TexCoords);
    }
    if (0 != isEnableColor) {
        FragColor = Color;
    }
    if (0 != isSelected) {
        FragColor += 0.5;
    }
    if (renderID) {
        FragColor = vec4(objectID.x, objectID.y, objectID.z, objectID.w);
    }
}