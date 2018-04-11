#version 330 core

uniform samplerCube cubemap;
uniform float minY;
uniform float maxY;

in vec3 pos;
in float y;
in vec3 norm;

in vec3 viewV;

out vec4 color;

vec3 calcDirLight(vec3 col, vec3 normal);

void main()
{
    float col = abs((y - minY) / (maxY - minY));
    color = vec4(col, col, col, 1.0f);

    if (pos.y > 15.0f && pos.y < 50.0f) {
        color = vec4(1.0f/255.0f, 152.0f/255.0f, 117.0f/255.0f, 1.0f);
    }
    
    else if (pos.y < 0.0f) {
        color = vec4(col, col, col, 1.0f);
        
    }
    
    else if (pos.y < 15.0f ) {
        color = vec4(1.0f, 1.0f, col, 1.0f);
        
    }
    
    else if (pos.y > 90.0f) {
        color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    } 
    
    vec3 normal = normalize(norm);

    vec4 lighting = vec4(calcDirLight(vec3(color.x, color.y, color.z), norm), 1.0);
    color = mix(color, lighting, 0.05f);
}

vec3 calcDirLight(vec3 newCol, vec3 normal) {    
    vec3 dir = vec3(-0.5f, -1.0f, 0.0f);
    vec3 lightV = normalize(-dir);
    vec3 reflectV = reflect(lightV,normal);
    reflectV = normalize(reflectV);

    vec3 diffuse = newCol * dot(lightV,normal);
    vec3 specular = newCol * pow(dot(reflectV,vec3(0.0f,.0f,-1.0f)),3.0f);

    return (diffuse+newCol);
}