#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform int u_Collision;

uniform int u_TotalRows;
uniform int u_TotalCols;
uniform int u_Row;
uniform int u_Col;

uniform int u_WidthMul;
uniform int u_HeightMul;

uniform sampler2D u_Texture;

void main()
{
    vec2 texCoords = v_Texcoords;
    texCoords.x = mod(texCoords.x * u_WidthMul, 1);
    texCoords.y = mod(texCoords.y * u_HeightMul, 1);

    float height = 1 / float(u_TotalRows);
    float width = 1 / float(u_TotalCols);

    vec2 tileSize = vec2(width, height);

    texCoords = (texCoords * tileSize) + tileSize * vec2(u_Col, u_Row);
    // vec2 newTexCoords = v_Texcoords * tileSize;

    if (u_Collision <= 0)
    {
        // vec2 texCoords = newTexCoords;

        f_Colour = texture(u_Texture, texCoords);
        // f_Colour = texture(u_Texture, v_Texcoords);
        // f_Colour = vec4(texCoords.x, texCoords.y, 0.0, 1.0);
    }
    else
    {
        f_Colour = vec4(1.0, 0.0, 0.0, 1.0);
    }

    f_Colour = pow(f_Colour, vec4(1/2.2));
}
