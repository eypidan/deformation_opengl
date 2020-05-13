#version 330 core
out vec4 FragColor;

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
uniform vec3 lightColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // change to [-1,1]
    projCoords.xyz = projCoords.xyz * 0.5 + 0.5;
    // get depth from light view
    float closestDepth = texture(shadowMap, projCoords.xy).g; 
    // get depth of current
    float currentDepth = projCoords.z;

    // chech if in shadow, use pcf to soft
	//calculate bias
	vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.05);
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;
	
	if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

void main()
{           
//     vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
//     vec3 normal = normalize(fs_in.Normal);
    
//     // ambient
//     vec3 ambient = 0.6 * color;
//     // diffuse
//     vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//     float diff = max(dot(lightDir, normal), 0.0);
//     vec3 diffuse = diff * lightColor;
//     // specular
//     vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//     vec3 reflectDir = reflect(-lightDir, normal);
//     float spec = 0.0;
//     vec3 halfwayDir = normalize(lightDir + viewDir);  
//     spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
//     vec3 specular = spec * lightColor;    
//     // calculate shadow
//     float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
// //	if(shadow == 0) FragColor = vec4(1.0,0.0,0.0,1.0);
// //	else FragColor = vec4(0.0,0.0,0.0,1.0);
//     vec3 lighting = (ambient + (0.8 - shadow) * (diffuse + specular)) * color;    
    
//     FragColor = vec4(lighting, 1.0);
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}