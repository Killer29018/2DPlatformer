#version 430

layout (location = 0) in vec2 i_Position;
layout (location = 1) in vec2 i_Texcoords;

out vec2 v_Texcoords;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(i_Position, 15.0, 1.0);

    v_Texcoords = i_Texcoords;
}
