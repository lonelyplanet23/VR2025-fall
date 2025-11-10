#include "transform.h"

namespace Utils::Transform {

matf4 perspective(float fov_y, float aspect, float z_near, float z_far) noexcept {
    assert(fov_y > 0 && aspect > 0 && z_near >= 0 && z_far > z_near);
	// TODO 2.1.2 : Create the perspective projection matrix.
    //* 步骤：先将视锥体变换为正交投影体，再进行正交投影 */
    
    float tan_half_fovy = tan(fov_y / 2.0f);
    matf4 m = matf4::Zero();

    m(0,0) = 1.0f / (aspect * tan_half_fovy); //* zoom_X
    m(1,1) = 1.0f / tan_half_fovy; //* zoom_Y
    m(2,2) = -(z_far + z_near) / (z_far - z_near);
    m(2,3) = -2.0f * z_far * z_near / (z_far - z_near);
    m(3,2) = -1.0f;

    return m;
}

matf4 orthographic(float width, float height, float z_near, float z_far) noexcept {
    assert(width > 0 && height > 0 && z_near >= 0 && z_far > z_near);

	// TODO 2.1.2 : Create the orthographic projection matrix.
	matf4 m = matf4::Identity();
    //! opengl 用的是列矢量
    //* 映射到标准立方体中
    m(0,0) = 2.0f/width;
    m(1,1) = 2.0f/height;
    m(2,2) = -2.0f/fabs(z_far - z_near); //* 注意-Z轴方向
    m(2,3) = -(z_far + z_near) / (z_far - z_near); //! debug
    return m;
}

matf4 get_scale_matrix(const vecf3& scale) noexcept {
    // TODO 2.1.1 : Implement the function that returns a scale matrix.
    matf4 m = matf4::Identity();
    //* Scale(scale.x(), scale.y(), scale.z());
    m(0,0) = scale.x();
    m(1,1) = scale.y();
    m(2,2) = scale.z();
    return m;
}

matf4 get_trans_matrix(const vecf3& trans) noexcept {
    // TODO 2.1.1 : Implement the function that returns a translation matrix.
    //* 沿着给定向量平移！所以第四维数为1
    //!行列搞反了
    matf4 m = matf4::Identity();
    m(0,3) = trans.x();
    m(1,3) = trans.y();
    m(2,3) = trans.z();
    return m;
}
matf4 look_at(const vecf3& pos, const vecf3& target, const vecf3& up) noexcept {
    assert(abs(up.dot(up) - 1.0f) < 1e-6);

    //* 平移到原点
    matf4 T = get_trans_matrix(-pos);

    //* 计算坐标轴旋转到相机坐标系的旋转矩阵，再转置
    matf4 R = matf4::Identity();
    vecf3 z_axis = (pos - target).normalized(); //* forward -g
    vecf3 x_axis = up.cross(z_axis).normalized(); //* right
    vecf3 y_axis = z_axis.cross(x_axis); //* up
   
    R.block<1,3>(0,0) = x_axis.transpose();
    R.block<1,3>(1,0) = y_axis.transpose();
    R.block<1,3>(2,0) = z_axis.transpose();
    //! Eigan左边和右边引用同一个内存块，就会触发“别名（aliasing）”问题。
    matf4 m = R * T;
    

    return m;
}

matf4 rotate_with(float theta, const vecf3& axis) noexcept {
    assert(abs(axis.dot(axis) - 1.0f) < 1e-6);

	// TODO 2.1.1 : Create the rotation matrix with the given theta and axis.
    // Note that the parameter 'theta' is expressed in the radian system.
	matf4 m = matf4::Identity();
    //* Rotational symmetry, 需要用罗德里格公式
        float c = cos(theta);
    float s = sin(theta);
    float x = axis.x(), y = axis.y(), z = axis.z();

    m(0,0) = c + (1 - c) * x * x;
    m(0,1) = (1 - c) * x * y - s * z;
    m(0,2) = (1 - c) * x * z + s * y;

    m(1,0) = (1 - c) * y * x + s * z;
    m(1,1) = c + (1 - c) * y * y;
    m(1,2) = (1 - c) * y * z - s * x;

    m(2,0) = (1 - c) * z * x - s * y;
    m(2,1) = (1 - c) * z * y + s * x;
    m(2,2) = c + (1 - c) * z * z;
    
    return m;
}

matf4 generate_model_matrix(const vecf3& pos, const vecf3& scale, float theta, const vecf3& axis) noexcept {
	// TODO 2.1.1 : Implement the function that returns a model matrix.
	//* LocalPoition -> WorldPosition
    matf4 m = matf4::Identity();
    //* first, need scaling
    matf4 S = get_scale_matrix(scale);
    //* then, rotate
    matf4 R = rotate_with(theta, axis);
    //* finaly, Translate acoording to the known position in the world space 整体平移到的点
    matf4 T = get_trans_matrix(pos);
    //* multiply them
    m = T * R * S;

	return m;
}

matf4 generate_model_matrix(const vecf3& pos, const vecf3& scale, const matf4& rotate) noexcept {
    matf4 m = get_trans_matrix(pos) *  get_scale_matrix(scale) * rotate;

    //* 函数重载，已知旋转矩阵
    
    return m;
}

} // namespace Utils::Transform