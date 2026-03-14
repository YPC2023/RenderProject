#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

out vec4 Color;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;

flat out int isEnablePos;
flat out int isEnableColor;
flat out int isEnableNormal;
flat out int isEnableTexCoords;
flat out int isEnableTangent;
flat out int isEnableBitangent;
flat out int isSelected;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool hasPos;
uniform bool hasColor;
uniform bool hasNormal;
uniform bool hasTexCoords;
uniform bool hasTangent;
uniform bool hasBitangent;
uniform bool hasSelected;

void setEnable()
{
    if (hasPos) 
    {
        isEnablePos = 1;
    }
    else 
    {
        isEnablePos = 0;
    }
    if (hasColor) 
    {
        isEnableColor = 1;
    }
    else 
    {
        isEnableColor = 0;
    }
    if (hasNormal) 
    {
        isEnableNormal = 1;
    }
    else 
    {
        isEnableNormal = 0;
    }
    if (hasTexCoords) 
    {
        isEnableTexCoords = 1;
    }
    else 
    {
        isEnableTexCoords = 0;
    }
    if (hasTangent) 
    {
        isEnableTangent = 1;
    }
    else 
    {
        isEnableTangent = 0;
    }
    if (hasBitangent) 
    {
        isEnableBitangent = 1;
    }
    else 
    {
        isEnableBitangent = 0;
    }
    if (hasSelected) 
    {
        isSelected = 1;
    }
    else 
    {
        isSelected = 0;
    }
}

void main()
{
    setEnable();
    if (hasPos) 
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    if (hasColor) 
    {
        Color = aColor;
    }
    if (hasNormal) 
    {
        Normal = aNormal;
    }
    if (hasTexCoords) 
    {
        TexCoords = aTexCoords; 
    }
    if (hasTangent) 
    {
        Tangent = aTangent;
    }
    if (hasBitangent) 
    {
        Bitangent = aBitangent;
    }
}