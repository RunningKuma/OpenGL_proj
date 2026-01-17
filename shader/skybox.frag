#version 330 core
in vec3 TexCoords;

out vec4 FragColor;

uniform sampler2D skybox;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    uv.y = 1.0 - uv.y;  // Flip Y to correct orientation
    return uv;
}

void main()
{
    vec2 uv = SampleSphericalMap(normalize(TexCoords));
    FragColor = texture(skybox, uv);
}
