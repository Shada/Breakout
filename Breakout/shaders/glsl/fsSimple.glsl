#version 430 core
in vec2 UV;
in vec3 position_modelspace;
in vec3 normal_viewspace;
in vec3 eyedirection_viewspace;
in vec3 lightdir_viewspace;

out vec3 color;

uniform sampler2D textureSampler;
//uniform vec3 lightpos;

void main()
{
    vec3 lightCol = vec3(1, 1, 1);

    vec3 diffuseCol = texture2D(textureSampler, UV).rgb;

    // these will have to be sent in to the gpu instead
    vec3 ambientCol = vec3(0.1f, 0.1f, 0.1f);
    vec3 specularCol = vec3(0.3f, 0.3f, 0.3f);

    //lightDistance
    float dist = length(vec3(0, 0, 0) - position_modelspace); /// TODO: use lightpos instead

    // normal of fragment
    vec3 n = normalize(normal_viewspace);

    // lightdir from fragment to light
    vec3 l = normalize(lightdir_viewspace);

    // cosine of angle between normal and lightdir clamped above 0
    float costheta = dot(n, l);

    // eye vector
    vec3 e = normalize(eyedirection_viewspace);

    // direction of light reflection
    vec3 r = reflect(-l, n);

    // cosine of angle between eyedir and reflection dir, clamped above 0
    float cosalpha = clamp(dot(e, r), 0, 1);

    color = ambientCol + diffuseCol * lightCol * costheta;// * 50 / (dist);// + specularCol * lightCol * pow(cosalpha, 5)/ (dist * dist);
}
