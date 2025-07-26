//
// Created by AmazingBuff on 25-7-25.
//

#ifndef CORE_H
#define CORE_H

#include <Eigen/Eigen>
#include <astd/astd.h>

#define RASTERIZER_NAME_SPACE_BEGIN AMAZING_NAMESPACE_BEGIN NAMESPACE_BEGIN(Rasterizer)
#define RASTERIZER_NAME_SPACE_END AMAZING_NAMESPACE_END NAMESPACE_END(Rasterizer)

RASTERIZER_NAME_SPACE_BEGIN

using Float = float;

using vec2 = Eigen::Vector2f;
using vec3 = Eigen::Vector3f;
using vec4 = Eigen::Vector4f;

using ivec2 = Eigen::Vector2i;
using ivec3 = Eigen::Vector3i;
using ivec4 = Eigen::Vector4i;

using mat2 = Eigen::Matrix2f;
using mat3 = Eigen::Matrix3f;
using mat4 = Eigen::Matrix4f;

RASTERIZER_NAME_SPACE_END

#endif //CORE_H
