#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 objectColor;
uniform vec3 viewPos;

// Light 1 - Ceiling lantern
uniform vec3 light1Pos;
uniform vec3 light1Color;
uniform bool light1On;

// Light 2 - Window moonlight
uniform vec3 light2Pos;
uniform vec3 light2Color;
uniform bool light2On;

// Excavator lights
uniform vec3 excavatorLight1Pos;
uniform vec3 excavatorLight2Pos;
uniform vec3 excavatorLightColor;
uniform bool excavatorLightsOn;

// for texture
uniform sampler2D texture1;
uniform bool useTexture;

vec3 calculatePointLight(vec3 lightPos, vec3 lightColor, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient
    vec3 ambient = 0.1 * lightColor;
    
    // Diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;
    
    // Attenuation
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    
    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    // Get base color from texture or uniform
    vec3 baseColor;
    if (useTexture) {
        baseColor = texture(texture1, TexCoord).rgb;
    } else {
        baseColor = objectColor;
    }

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0);
    
    // Add ambient lighting
    result += 0.15 * baseColor;
    
    // Light 1
    if (light1On) {
        result += calculatePointLight(light1Pos, light1Color, norm, FragPos, viewDir) * baseColor;
    }
    
    // Light 2
    if (light2On) {
        result += calculatePointLight(light2Pos, light2Color, norm, FragPos, viewDir) * baseColor;
    }
    
    // Excavator lights
    if (excavatorLightsOn) {
        result += calculatePointLight(excavatorLight1Pos, excavatorLightColor, norm, FragPos, viewDir) * baseColor;
        result += calculatePointLight(excavatorLight2Pos, excavatorLightColor, norm, FragPos, viewDir) * baseColor;
    }
    
    FragColor = vec4(result, 1.0);
}