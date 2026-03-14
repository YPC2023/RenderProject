#version 330 core
out vec4 FragColor;

uniform vec4 objectID;

void main()
{
    //FragColor = vec4(objectID.x, objectID.y, objectID.z, objectID.w);
    FragColor = vec4(objectID.x, objectID.y, objectID.z, objectID.w);
}