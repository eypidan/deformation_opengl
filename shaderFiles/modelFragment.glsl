#version 460 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D texture_diffuse0;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() {           
     //vec3 color = texture(texture_diffuse0, fs_in.TexCoords).rgb;
     vec3 color = vec3(0.1,0.2,0.3);
     vec3 normal = normalize(fs_in.Normal);
    
     // ambient
     vec3 ambient = 0.6 * lightColor;
     // diffuse
     vec3 lightdir = normalize(lightPos - fs_in.FragPos);
     float diff = max(dot(lightdir, normal), 0.0);
     vec3 diffuse = diff * lightColor;
     // specular
     vec3 viewdir = normalize(viewPos - fs_in.FragPos);
     vec3 reflectdir = reflect(-lightdir, normal);
     float spec = 0.0;
     vec3 halfwaydir = normalize(lightdir + viewdir);  
     spec = pow(max(dot(normal, halfwaydir), 0.0), 64.0);
     vec3 specular = spec * lightColor;    
     vec3 lighting = (ambient + diffuse + specular) * color;    

     FragColor = vec4(lighting, 1.0);
}