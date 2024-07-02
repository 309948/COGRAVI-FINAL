#version 460 core

out vec4 FragColor; // Salida del color del fragmento.

// Estructura para definir las propiedades del material.
struct Material {
    sampler2D diffuse; // Textura difusa.
    sampler2D specular; // Textura especular.
    float shininess; // Brillo del material.
};

// Estructura para definir las propiedades de la luz de la linterna.
struct Light {
    vec3 position; // Posición de la luz.
    vec3 direction; // Dirección de la luz.
    float cutOff; // Ángulo de corte para el cono de luz.
    float outerCutOff; // Ángulo de corte externo para suavizar los bordes del cono de luz.

    vec3 ambient; // Color de la luz ambiental.
    vec3 diffuse; // Color de la luz difusa.
    vec3 specular; // Color de la luz especular.

    float constant; // Atenuación constante.
    float linear; // Atenuación lineal.
    float quadratic; // Atenuación cuadrática.

    sampler2D flashlight; // Textura de la máscara de la linterna.
};

in vec3 FragPos; // Posición del fragmento.
in vec3 Normal; // Normal del fragmento.
in vec2 TexCoords; // Coordenadas de textura del fragmento.

uniform vec3 viewPos; // Posición de la cámara/vista.
uniform Material material; // Material del objeto.
uniform Light light; // Luz de la linterna.

uniform vec2 viewPort; // Dimensiones del viewport.

uniform float time; // Tiempo, puede ser usado para efectos que varían con el tiempo.

void main()
{
    // Calcula la luz ambiental.
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // Calcula la luz difusa.
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // Calcula la luz especular.
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // Calcula la intensidad de la luz de la linterna con bordes suaves.
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Aplica la máscara de la linterna (cookie) para crear patrones de luz.
    float ratio = viewPort.x / viewPort.y;
    vec2 uv = (2.0 * gl_FragCoord.xy) / viewPort.xy - 1.0;
    uv.x *= ratio;
    uv *= 0.5;
    uv += 0.5;
    intensity *= length(vec3(texture(light.flashlight, uv)));

    // Aplica la intensidad a la luz difusa y especular.
    diffuse  *= intensity;
    specular *= intensity;
    
    // Calcula la atenuación de la luz basada en la distancia.
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // Aplica la atenuación a la luz ambiental, difusa y especular.
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // Combina las componentes de luz para obtener el color final.
    vec3 result = ambient + diffuse + specular;

    // Aplica un efecto de posterización al color final.
    float color_num = 16;
    vec3 final = floor(result * (color_num - 1.0) + 0.5) / (color_num - 1.0);

    // Establece el color final del fragmento.
    FragColor = vec4(final, 1.0);
}