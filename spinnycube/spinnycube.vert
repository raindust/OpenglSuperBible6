#version 430 core            
                                       
in vec3 position;               
                                       
out VS_OUT                     
{                                      
    vec4 color;                   
} vs_out;                         
                                       
uniform mat4 mv_matrix; 
uniform mat4 proj_matrix;
                                       
void main(void)                
{                                      
	sition = proj_matrix * mv_matrix * vec4(position.xyz, 1.0);   
    vs_out.color = position * 2.0 + vec4(0.5, 0.5, 0.5, 0.0);
}
 