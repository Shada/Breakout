#version 430 core
in vec2 UV;
in vec3 position_modelspace;
in vec3 normal_viewspace;
in vec3 eyedirection_viewspace;
in vec3 lightdir_viewspace;

out vec3 color;

uniform sampler2D textureSampler;

void main()
{
    vec3 lightCol = vec3(1, 1, 1);

    vec3 diffuseCol = texture2D(textureSampler, UV).rgb;

    // these will have to be sent in to the gpu instead
	vec3 kd = vec3(0.9280f, 0.9280f, 0.9280f);
    vec3 ka = vec3(0.08f, 0.08f, 0.08f);
    vec3 ks = vec3(0.3f, 0.3f, 0.3f);

    //lightDistance
    float dist = length(vec3(0, 0, 0) - position_modelspace); /// TODO: use lightpos instead

    // normal of fragment
    vec3 n = normalize(normal_viewspace);

    // lightdir from fragment to light
    vec3 l = normalize(lightdir_viewspace);

    // cosine of angle between normal and lightdir clamped above 0
    float costheta = max(dot(n, l), 0.0);

    // eye vector
    vec3 e = normalize(eyedirection_viewspace);

    // direction of light reflection
    vec3 r = reflect(-l, n);

    // cosine of angle between eyedir and reflection dir, clamped above 0
    float cosalpha = clamp(dot(e, r), 0, 1);

    color = ka + kd * diffuseCol * lightCol * costheta;// * 50 / (dist);// + specularCol * lightCol * pow(cosalpha, 5)/ (dist * dist);
}
