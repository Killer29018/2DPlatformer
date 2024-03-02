#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform ivec2 u_TilemapSize;
uniform ivec2 u_TileTextureMapping[3];
uniform int u_TileMappingSize;

uniform vec3 u_GlobalPosition;
uniform vec2 u_TileSize;

uniform ivec2 u_BlockSize;

uniform int u_ShowBlock;
uniform int u_BlockID;

uniform sampler2DArray u_Texture;

float floatConstruct(uint m)
{
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

int hash(int x)
{
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return int(x);
}

int hash(ivec2 v) { return hash(v.x ^ hash(v.y)); }
int hash(ivec3 v) { return hash(v.x ^ hash(v.y) ^ hash(v.z)); }

int hash(vec3 v) { return int(floatConstruct(hash(floatBitsToInt(v))) * 1000000); }

void main()
{
    ivec2 tilePosInBlock = ivec2(floor(vec2(0.0, 1.0) - v_Texcoords * u_BlockSize));
    vec3 globalPosition = u_GlobalPosition + vec3(tilePosInBlock * u_TileSize, 0);

    int subIndex = hash(globalPosition) % u_TileMappingSize;
    ivec2 index = u_TileTextureMapping[subIndex];

    int tileIndex = index.y * u_TilemapSize.x + index.x;
    vec2 texcoord_2d = mod(v_Texcoords * u_BlockSize, 1.);
    vec3 texcoord = vec3(texcoord_2d.xy, tileIndex);

    if (u_ShowBlock == 1)
    {
        float a = 1;
        float b = ((u_BlockID >>  0) & 0xFF) / 255.;
        float g = ((u_BlockID >>  8) & 0xFF) / 255.;
        float r = ((u_BlockID >> 16) & 0xFF) / 255.;
        f_Colour = vec4(r, g, b, a);
    }
    else
    {
        f_Colour = texture(u_Texture, texcoord);

        if (f_Colour.a == 0)
            discard;
    }

}
