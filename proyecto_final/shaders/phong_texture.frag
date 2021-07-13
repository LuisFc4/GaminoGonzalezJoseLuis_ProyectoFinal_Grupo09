#version 330 core
struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform float shininess;
uniform float alpha;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform DirLight light;


out vec4 FragColor;

void main()
{
	//Text color
	vec4   texColor=texture(texture_diffuse1, TexCoords);
    if(texColor.a < 0.1)
        discard;
	vec3 texture_color=vec3(texColor.r,texColor.g,texColor.b);
	
	//Properties
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);

	//Specular shadding 
	vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	//Ambient light
	vec3 ambient = light.ambient * texture_color;

    //Diffuselight
    vec3 diffuse = light.diffuse * diff * texture_color;
    
	//SpecularLight
    vec3 specular = light.specular * pow(spec,1) * texture_color;
	
	vec3 lightPower = vec3(1.0f,1.0f,1.0f);
	vec3 result =  ambient + diffuse + specular;
	FragColor = vec4(result,alpha);
}