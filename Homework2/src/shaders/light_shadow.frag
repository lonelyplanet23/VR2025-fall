#version 330 core

#define PI 3.1415926
#define EPSILON 0.0001

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
	//* 如果片元在光源的远裁剪面之外，则不进行阴影计算
	if(proj_coords.z > 1.0)
    	return 0.0;

	float closest_depth = texture(shadow_map, proj_coords.xy).r;
	float current_depth = proj_coords.z;

	//* 进行深度比较，判断片元是否在阴影中
    // 关键修复3：PCF采样（3x3核），柔化阴影边缘
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_map, 0);  // 阴影图纹理像素尺寸

    // 对周围9个像素采样
    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            float pcf_depth = texture(shadow_map, proj_coords.xy + vec2(x, y) * texel_size).r;
            if(proj_coords.z - EPSILON > pcf_depth)
                shadow += 1.0;
        }
    }
    shadow /= 9.0;  // 平均9个样本


	return shadow;
}

void main() {
	vec3 color = texture(color_texture, vs_out.TexCoord).rgb;

	//* 将片元坐标转换到光源空间坐标系下
	vec4 fragPosLightSpace = light_space_matrix * vec4(vs_out.WorldPos, 1.0); 

	// TODO 2.2.1 : Complete the following shadow mapping calculations
	vec3 light_dir = normalize(point_light_pos - vs_out.WorldPos);
	vec3 view_dir = normalize(camera_pos - vs_out.WorldPos);
	vec3 reflect_dir = reflect(-light_dir, vs_out.Normal);//! debug1: 反射向量计算时注意取负号

	float shadow = 0.0;	
	if(have_shadow)
	{
		shadow = ShadowCalculation(fragPosLightSpace);
	}

	// TODO 2.2.1 :add lighting, Modify the output of frag color 
	vec3 ambient_term = ambient * color;
	//! 注意点积为负时，漫反射光为0
	vec3 diffuse_term = 0.3 * point_light_radiance * max(dot(light_dir, vs_out.Normal),0) * color;
	vec3 reflect_term = 0.35 * point_light_radiance * pow(max(dot(reflect_dir, view_dir),0), specular)* color; //! debug2: 是反射向量与视线向量的点积


	color = ambient_term + (1.0 - shadow) * (diffuse_term + reflect_term);	


	FragColor = vec4(color, 1.0);
	
}
