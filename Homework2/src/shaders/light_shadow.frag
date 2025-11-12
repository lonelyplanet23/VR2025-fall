#version 330 core

#define PI 3.1415926
#define EPSILON 0.0003

out vec4 FragColor;	//最终的输出片元颜色

uniform vec3 point_light_pos;
uniform vec3 point_light_radiance; //光源的辐射率
uniform sampler2D shadow_map;				
uniform bool have_shadow; 

uniform mat4 light_space_matrix;

uniform float ambient; //环境光强度
uniform float specular; //镜面反射强度
uniform sampler2D color_texture;

uniform vec3 camera_pos;

in VS_OUT {
    vec3 WorldPos;
    vec2 TexCoord;
    vec3 Normal;
} vs_out;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// TODO 2.2.2 : Complete the following shadow mapping calculations
	//* 将片元坐标转换到[0,1]范围内，便于与深度贴图进行比较
	vec3 proj_coords = fragPosLightSpace.xyz / fragPosLightSpace.w; //标准化了
	proj_coords = proj_coords * 0.5 + 0.5;


	float closest_depth = texture(shadow_map, proj_coords.xy).r;

    // 动态偏移（Slope-Scaled Bias），减少阴影acne
    vec3 light_dir = normalize(point_light_pos - vs_out.WorldPos);
	float bias = max(EPSILON* (1.0 - dot(normalize(vs_out.Normal),
                                     normalize(point_light_pos - vs_out.WorldPos))), 0.005);

    float current_depth = proj_coords.z - bias;  // 应用偏移

    // PCF采样（3x3核），柔化阴影边缘
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);  // 阴影图纹理像素尺寸

    // 对周围9个像素采样
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            shadow += current_depth > pcf_depth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;  // 平均9个样本


	return shadow;
}

void main() {
	vec3 color = texture(color_texture, vs_out.TexCoord).rgb;
	vec3 light_color = vec3(1.0, 1.0, 1.0);
	//* 将片元坐标转换到光源空间坐标系下
	vec4 fragPosLightSpace = light_space_matrix * vec4(vs_out.WorldPos, 1.0); 

	// TODO 2.2.1 : Complete the following shadow mapping calculations
	vec3 light_dir = normalize(point_light_pos - vs_out.WorldPos);
	vec3 view_dir = normalize(camera_pos - vs_out.WorldPos);
	vec3 reflect_dir = reflect(-light_dir, vs_out.Normal);//! debug1: 反射向量计算时注意取负号
	vec3 norm = normalize(vs_out.Normal);

	float shadow = 0.0;	
	if(have_shadow)
	{
		shadow = ShadowCalculation(fragPosLightSpace);
	}

	// TODO 2.2.1 :add lighting, Modify the output of frag color 
	vec3 ambient_term = ambient * light_color;
	
	//! 注意点积为负时，漫反射光为0	
	vec3 diffuse_term =  0.3 * point_light_radiance * max(dot(norm, light_dir), 0) * light_color;
	
	float specularStrength = 0.2;
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular);
	vec3 reflect_term = specularStrength * spec * light_color;				 //! debug2: 是反射向量与视线向量的点积


	color = (ambient_term + (diffuse_term + reflect_term)*(1.0 - shadow)) * color;	


	FragColor = vec4(color, 1.0);
	
}
