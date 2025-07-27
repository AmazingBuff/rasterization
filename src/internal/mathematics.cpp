//
// Created by AmazingBuff on 25-7-27.
//

RASTERIZER_NAME_SPACE_BEGIN
    // right hand, [0 - 1]
template <bool RowMajor>
mat4 perspective_rh_zero(Float fov_y, Float aspect, Float near, Float far)
{
    Float const inv_tan = 1 / std::tan(fov_y / 2);
    Float const range = 1 / (far - near);
    mat4 mat;
    mat.setZero();
    mat(0, 0) = inv_tan / aspect;
    mat(1, 1) = inv_tan;
    mat(2, 2) = -far * range;
    if constexpr (RowMajor)
    {
        mat(3, 2) = -1;
        mat(2, 3) = -far * near * range;
    }
    else
    {
        mat(2, 3) = -1;
        mat(3, 2) = -far * near * range;
    }

    return mat;
}

// right hand, [-1 - 1]
template <bool RowMajor>
mat4 perspective_rh_one(Float fov_y, Float aspect, Float near, Float far)
{
    Float const inv_tan = 1 / std::tan(fov_y / 2);
    Float const range = 1 / (far - near);
    mat4 mat;
    mat.setZero();
    mat(0, 0) = inv_tan / aspect;
    mat(1, 1) = inv_tan;
    mat(2, 2) = -(far + near) * range;
    if constexpr (RowMajor)
    {
        mat(3, 2) = -1;
        mat(2, 3) = -2 * far * near * range;
    }
    else
    {
        mat(2, 3) = -1;
        mat(3, 2) = -2 * far * near * range;
    }
    return mat;
}

// left hand, [0 - 1]
template <bool RowMajor>
mat4 perspective_lh_zero(Float fov_y, Float aspect, Float near, Float far)
{
    Float const inv_tan = 1 / std::tan(fov_y / 2);
    Float const range = 1 / (far - near);
    mat4 mat;
    mat.setZero();
    mat(0, 0) = inv_tan / aspect;
    mat(1, 1) = inv_tan;
    mat(2, 2) = far * range;
    if constexpr (RowMajor)
    {
        mat(3, 2) = 1;
        mat(2, 3) = -far * near * range;
    }
    else
    {
        mat(2, 3) = 1;
        mat(3, 2) = -far * near * range;
    }

    return mat;
}

// left hand, [-1 - 1]
template <bool RowMajor>
mat4 perspective_lh_one(Float fov_y, Float aspect, Float near, Float far)
{
    Float const inv_tan = 1 / std::tan(fov_y / 2);
    Float const range = 1 / (far - near);
    mat4 mat;
    mat.setZero();
    mat(0, 0) = inv_tan / aspect;
    mat(1, 1) = inv_tan;
    mat(2, 2) = (far + near) * range;
    if constexpr (RowMajor)
    {
        mat(3, 2) = 1;
        mat(2, 3) = -2 * far * near * range;
    }
    else
    {
        mat(2, 3) = 1;
        mat(3, 2) = -2 * far * near * range;
    }
    return mat;
}

mat4 perspective(Float fov_y, Float aspect, Float near, Float far)
{
    return perspective_rh_zero<true>(fov_y, aspect, near, far);
}


// right hand, [0 - 1]
template <bool RowMajor>
mat4 orthographic_rh_zero(Float left, Float right, Float bottom, Float top, Float near, Float far)
{
    Float const rl = 1 / (right - left);
    Float const tb = 1 / (top - bottom);
    Float const fn = 1 / (far - near);

    mat4 mat(mat4::Identity());
    mat(0, 0) = 2 * rl;
    mat(1, 1) = 2 * tb;
    mat(2, 2) = -fn;
    if constexpr (RowMajor)
    {
        mat(0, 3) = -(right + left) * rl;
        mat(1, 3) = -(top + bottom) * tb;
        mat(2, 3) = -near * fn;
    }
    else
    {
        mat(3, 0) = -(right + left) * rl;
        mat(3, 1) = -(top + bottom) * tb;
        mat(3, 2) = -near * fn;
    }

    return mat;
}

// right hand, [-1 - 1]
template <bool RowMajor>
mat4 orthographic_rh_one(Float left, Float right, Float bottom, Float top, Float near, Float far)
{
    Float const rl = 1 / (right - left);
    Float const tb = 1 / (top - bottom);
    Float const fn = 1 / (far - near);

    mat4 mat(mat4::Identity());
    mat(0, 0) = 2 * rl;
    mat(1, 1) = 2 * tb;
    mat(2, 2) = -2 * fn;
    if constexpr (RowMajor)
    {
        mat(0, 3) = -(right + left) * rl;
        mat(1, 3) = -(top + bottom) * tb;
        mat(2, 3) = -(far + near) * fn;
    }
    else
    {
        mat(3, 0) = -(right + left) * rl;
        mat(3, 1) = -(top + bottom) * tb;
        mat(3, 2) = -(far + near) * fn;
    }

    return mat;
}

// left hand, [0 - 1]
template <bool RowMajor>
mat4 orthographic_lh_zero(Float left, Float right, Float bottom, Float top, Float near, Float far)
{
    Float const rl = 1 / (right - left);
    Float const tb = 1 / (top - bottom);
    Float const fn = 1 / (far - near);

    mat4 mat(mat4::Identity());
    mat(0, 0) = 2 * rl;
    mat(1, 1) = 2 * tb;
    mat(2, 2) = fn;
    if constexpr (RowMajor)
    {
        mat(0, 3) = -(right + left) * rl;
        mat(1, 3) = -(top + bottom) * tb;
        mat(2, 3) = -near * fn;
    }
    else
    {
        mat(3, 0) = -(right + left) * rl;
        mat(3, 1) = -(top + bottom) * tb;
        mat(3, 2) = -near * fn;
    }

    return mat;
}

// left hand, [-1 - 1]
template <bool RowMajor>
mat4 orthographic_lh_one(Float left, Float right, Float bottom, Float top, Float near, Float far)
{
    Float const rl = 1 / (right - left);
    Float const tb = 1 / (top - bottom);
    Float const fn = 1 / (far - near);

    mat4 mat(mat4::Identity());
    mat(0, 0) = 2 * rl;
    mat(1, 1) = 2 * tb;
    mat(2, 2) = 2 * fn;
    if constexpr (RowMajor)
    {
        mat(0, 3) = -(right + left) * rl;
        mat(1, 3) = -(top + bottom) * tb;
        mat(2, 3) = -(far + near) * fn;
    }
    else
    {
        mat(3, 0) = -(right + left) * rl;
        mat(3, 1) = -(top + bottom) * tb;
        mat(3, 2) = -(far + near) * fn;
    }

    return mat;
}

mat4 orthographic(Float left, Float right, Float bottom, Float top, Float near, Float far)
{
    return orthographic_rh_zero<true>(left, right, bottom, top, near, far);
}


template <bool RowMajor>
mat4 look_at_rh(vec3 const& eye, vec3 const& center, vec3 const& up)
{
    vec3 const f((center - eye).normalized());
    vec3 const s(f.cross(up).normalized());
    vec3 const u(s.cross(f));

    mat4 mat(mat4::Identity());
    if constexpr (RowMajor)
    {
        mat.row(0) = vec4(s.x(), s.y(), s.z(), -s.dot(eye));
        mat.row(1) = vec4(u.x(), u.y(), u.z(), -u.dot(eye));
        mat.row(2) = vec4(-f.x(), -f.y(), -f.z(), f.dot(eye));
    }
    else
    {
        mat.col(0) = vec4(s.x(), s.y(), s.z(), -s.dot(eye));
        mat.col(1) = vec4(u.x(), u.y(), u.z(), -u.dot(eye));
        mat.col(2) = vec4(-f.x(), -f.y(), -f.z(), f.dot(eye));
    }

    return mat;
}

template <bool RowMajor>
mat4 look_at_lh(vec3 const& eye, vec3 const& center, vec3 const& up)
{
    vec3 const f((center - eye).normalized());
    vec3 const s(up.cross(f).normalized());
    vec3 const u(f.cross(s));

    mat4 mat(mat4::Identity());
    if constexpr (RowMajor)
    {
        mat.row(0) = vec4(s.x(), s.y(), s.z(), -s.dot(eye));
        mat.row(1) = vec4(u.x(), u.y(), u.z(), -u.dot(eye));
        mat.row(2) = vec4(f.x(), f.y(), f.z(), -f.dot(eye));
    }
    else
    {
        mat.col(0) = vec4(s.x(), s.y(), s.z(), -s.dot(eye));
        mat.col(1) = vec4(u.x(), u.y(), u.z(), -u.dot(eye));
        mat.col(2) = vec4(f.x(), f.y(), f.z(), -f.dot(eye));
    }

    return mat;
}

mat4 look_at(vec3 const& eye, vec3 const& center, vec3 const& up)
{
    return look_at_rh<true>(eye, center, up);
}


mat4 translate(vec3 const& v)
{
    mat4 mat(mat4::Identity());
    mat(0, 3) = v.x();
    mat(1, 3) = v.y();
    mat(2, 3) = v.z();
    return mat;
}

mat4 rotate(vec3 const& v, Float angle)
{
    Float const a = angle * EIGEN_PI / 180;
    Float const c = std::cos(a);
    Float const s = std::sin(a);

    vec3 axis = v.normalized();
    vec3 temp = (1 - c) * axis;

    mat4 rotate(mat4::Identity());
    rotate(0, 0) = c + temp[0] * axis[0];
    rotate(1, 0) = temp[0] * axis[1] + s * axis[2];
    rotate(2, 0) = temp[0] * axis[2] - s * axis[1];

    rotate(0, 1) = temp[1] * axis[0] - s * axis[2];
    rotate(1, 1) = c + temp[1] * axis[1];
    rotate(2, 1) = temp[1] * axis[2] + s * axis[0];

    rotate(0, 2) = temp[2] * axis[0] + s * axis[1];
    rotate(1, 2) = temp[2] * axis[1] - s * axis[0];
    rotate(2, 2) = c + temp[2] * axis[2];

    return rotate;
}

RASTERIZER_NAME_SPACE_END