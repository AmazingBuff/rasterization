//
// Created by AmazingBuff on 25-7-25.
//

#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include "core.h"

RASTERIZER_NAME_SPACE_BEGIN

template <typename T>
T cross(Eigen::Vector2<T> const& l, Eigen::Vector2<T> const& r)
{
    return l.x() * r.y() - l.y() * r.x();
}

template <typename T, int Rows, int Cols, typename U>
Eigen::Matrix<T, Rows, Cols> clamp(Eigen::Matrix<T, Rows, Cols> const& mat, U min_val, U max_val)
{
    return mat.cwiseMax(min_val).cwiseMin(max_val);
}

template <typename T, int Rows, int Cols, typename U>
Eigen::Matrix<T, Rows, Cols> remap(Eigen::Matrix<T, Rows, Cols> const& mat, U old_min, U old_max, U new_min, U new_max)
{
    Eigen::Matrix<T, Rows, Cols> ret;
    for (int i = 0; i < Rows; ++i)
        for (int j = 0; j < Cols; ++j)
            ret(i, j) = (mat(i, j) - old_min) / (old_max - old_min) * (new_max - new_min) + new_min;

    return ret;
}


RASTERIZER_NAME_SPACE_END
#endif //MATHEMATICS_H
