#version 400 core
out vec4 FragColour;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // Perform perspective divide.
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0, 1] range.
    projCoords = projCoords * 0.5f + 0.5f;
    // Get closes depth value from light's perspective (using [0, 1] range fragPosLightSpace as coords).
    float closeDepth = texture(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective.
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope).
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float bias = max(0.05f * (1.0f - dot(normal, lightDir)), 0.005f);

    // Check wether current frag pos is in shadow.
    //float shadow = currentDepth - bias > closeDepth ? 1.0f : 0.0f;
    // PCF
    float shadow = 0.0f;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }

    shadow /= 9.0f;

    if (projCoords.z > 1.0f)
        shadow = 0.0f;

    return shadow;
}

void main() {
    vec3 colour = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColour = vec3(1.0f);

    // Ambient
    vec3 ambient = 0.15 * colour;

    // Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir, normal), 0.0f);
    vec3 diffuse = diff * lightColour;

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = 0.0f;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0f);
    vec3 specular = spec * lightColour;

    // Calculate shadow.
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * colour;

    FragColour = vec4(lighting, 1.0f);
}