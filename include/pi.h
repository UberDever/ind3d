//
// Created by uberdever on 04.08.2020.
//
// Pi contains all pure-functional math, primarily for 2d/3d graphics
// All pure functions are defined inline
// Stick to column-major ordering
// Stick to left-handed system (like not default opengl)
// Right handed is easy, just flip the z sign at ndc transformation. But also change vert ordering
#ifndef ALPHABETA_PI_H
#define ALPHABETA_PI_H

#include "alpha.h"

#ifndef M_PI
#define M_PI    3.1415926535897932384626433832795 
#endif

#ifndef M_2PI
#define M_2PI   6.283185307179586476925286766559 
#endif

#ifndef M_PI_2
#define M_PI_2  1.5707963267948966192313216916398 
#endif

#ifndef M_PI_4
#define M_PI_4  0.78539816339744830961566084581988 
#endif

#define SQRT2           1.41421356237309504880f
#define SQRT2_2         0.70710678118654752440f

#define DEG2RAD(phi) (0.01745329f * phi)
#define RAD2DEG(phi) (57.2957795f * phi)

typedef float32 vec_float_t;

typedef int vi2[2];
#define VI2TOV2(v) (v2){(v)[0], (v)[1]}
typedef vec_float_t v2[2];
typedef vec_float_t v3[3];

typedef v2 m2[2];
typedef v3 m3[3];

static void pi_v2_mul_m2(const m2 mat, const v2 vec, v2 res); // res can be vec as well
static void pi_m2_rotation(m2 mat, vec_float_t ang_in_rad);

/*
 *  Vec2 stuff
 */

#if DEBUG
#define debug_v2(v) debug("( %f %f )", v[0], v[1])
#else
#define debug_v2(v)
#endif

static void pi_v2_zeros(v2 vec)
{
    vec[0] = 0; vec[1] = 0;
}

static void pi_v2_ones(v2 vec)
{
    vec[0] = 1; vec[1] = 1;
}

static void pi_v2(v2 vec, const vec_float_t x, const vec_float_t y)
{
    vec[0] = x; vec[1] = y;
}

static void pi_v2_copy(const v2 src, v2 dst)
{
    dst[0] = src[0]; dst[1] = src[1];
}

static void pi_v2_add(const v2 vec0, const v2 vec1, v2 res)
{
    res[0] = vec0[0] + vec1[0];
    res[1] = vec0[1] + vec1[1];
}

static void pi_v2_adds(const v2 vec, const vec_float_t scalar, v2 res)
{
    res[0] = vec[0] + scalar;
    res[1] = vec[1] + scalar;
}

static void pi_v2_sub(const v2 vec0, const v2 vec1, v2 res)
{
    res[0] = vec0[0] - vec1[0];
    res[1] = vec0[1] - vec1[1];
}

static void pi_v2_subs(const v2 vec, const vec_float_t scalar, v2 res)
{
    res[0] = vec[0] - scalar;
    res[1] = vec[1] - scalar;
}

static void pi_v2_mul(const v2 vec0, const v2 vec1, v2 res)
{
    res[0] = vec0[0] * vec1[0];
    res[1] = vec0[1] * vec1[1];
}

static void pi_v2_muls(const v2 vec, const vec_float_t scalar, v2 res)
{
    res[0] = vec[0] * scalar;
    res[1] = vec[1] * scalar;
}

static void pi_v2_div(const v2 vec0, const v2 vec1, v2 res)
{
    res[0] = vec0[0] / vec1[0];
    res[1] = vec0[1] / vec1[1];
}

static void pi_v2_divs(const v2 vec, const vec_float_t scalar, v2 res)
{
    res[0] = vec[0] / scalar;
    res[1] = vec[1] / scalar;
}

static vec_float_t pi_v2_dot(const v2 vec0, const v2 vec1)
{
    return vec0[0] * vec1[0] + vec0[1] * vec1[1];
}

static vec_float_t pi_v2_cross(const v2 vec0, const v2 vec1)
{
    return vec0[0] * vec1[1] - vec0[1] * vec1[0];
}

static void pi_v2_orthogonal_right(const v2 vec, v2 res)
{
    vec_float_t tmp = vec[0];
    res[0] = vec[1]; res[1] = -tmp;
}

static void pi_v2_orthogonal_left(const v2 vec, v2 res)
{
    vec_float_t tmp = vec[0];
    res[0] = -vec[1]; res[1] = tmp;
}

static vec_float_t pi_v2_len_sq(const v2 vec)
{
    return pi_v2_dot(vec, vec);
}

static vec_float_t pi_v2_len(const v2 vec)
{
    return sqrtf(pi_v2_len_sq(vec));
}

static void pi_v2_normalize(const v2 vec, v2 res)
{
    const vec_float_t len = pi_v2_len(vec);
    pi_v2(res, vec[0] / len, vec[1] / len);
}

static void pi_v2_rotate(const v2 vec, const vec_float_t ang_in_rad, v2 res)
{
    m2 mat;
    pi_m2_rotation(mat, ang_in_rad);
    pi_v2_mul_m2(mat, vec, res);
}

#define pi_v2_rotate_

/*
 *  Vec3 stuff
 */

#if DEBUG
#define debug_v3(v) debug("( %f %f %f )", v[0], v[1], v[2])
#else
#define debug_v2(v)
#endif

static void pi_v3_zeros(v3 vec)
{
    vec[0] = 0; vec[1] = 0; vec[2] = 0;
}

static void pi_v3_ones(v3 vec)
{
    vec[0] = 1; vec[1] = 1; vec[2] = 1;
}

static void pi_v3(v3 vec, const vec_float_t x, const vec_float_t y, const vec_float_t z)
{
    vec[0] = x; vec[1] = y; vec[2] = z;
}

static void pi_v3_copy(const v3 src, v3 dst)
{
    dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2];
}

static void pi_v3_add(const v3 vec0, const v3 vec1, v3 res)
{
    res[0] = vec0[0] + vec1[0];
    res[1] = vec0[1] + vec1[1];
    res[2] = vec0[2] + vec1[2];
}

static void pi_v3_adds(const v3 vec, const vec_float_t scalar, v3 res)
{
    res[0] = vec[0] + scalar;
    res[1] = vec[1] + scalar;
    res[2] = vec[2] + scalar;
}

static void pi_v3_sub(const v3 vec0, const v3 vec1, v3 res)
{
    res[0] = vec0[0] - vec1[0];
    res[1] = vec0[1] - vec1[1];
    res[2] = vec0[2] - vec1[2];
}

static void pi_v3_subs(const v3 vec, const vec_float_t scalar, v3 res)
{
    res[0] = vec[0] - scalar;
    res[1] = vec[1] - scalar;
    res[2] = vec[2] - scalar;
}

static void pi_v3_mul(const v3 vec0, const v3 vec1, v3 res)
{
    res[0] = vec0[0] * vec1[0];
    res[1] = vec0[1] * vec1[1];
    res[2] = vec0[2] * vec1[2];
}

static void pi_v3_muls(const v3 vec, const vec_float_t scalar, v3 res)
{
    res[0] = vec[0] * scalar;
    res[1] = vec[1] * scalar;
    res[2] = vec[2] * scalar;
}

static void pi_v3_div(const v3 vec0, const v3 vec1, v3 res)
{
    res[0] = vec0[0] / vec1[0];
    res[1] = vec0[1] / vec1[1];
    res[2] = vec0[2] / vec1[2];
}

static void pi_v3_divs(const v3 vec, const vec_float_t scalar, v3 res)
{
    res[0] = vec[0] / scalar;
    res[1] = vec[1] / scalar;
    res[2] = vec[2] / scalar;
}

static vec_float_t pi_v3_dot(const v3 vec0, const v3 vec1)
{
    return vec0[0] * vec1[0] + vec0[1] * vec1[1] + vec0[2] * vec1[2];
}

static void pi_v3_cross(const v3 vec1, const v3 vec0, v3 res)
{
    pi_v3(res, vec0[1] * vec1[2] - vec0[2] * vec1[1],
               vec0[2] * vec1[0] - vec0[0] * vec1[2],
               vec0[0] * vec1[1] - vec0[1] * vec1[0]);
}

static vec_float_t pi_v3_len_sq(const v3 vec)
{
    return pi_v3_dot(vec, vec);
}

static vec_float_t pi_v3_len(const v3 vec)
{
    return sqrtf(pi_v3_len_sq(vec));
}

static void pi_v3_normalize(const v3 vec, v3 res)
{
    const vec_float_t len = pi_v3_len(vec);
    pi_v3(res, vec[0] / len, vec[1] / len, vec[2] / len);
}

/*
 *  Mat2 stuff
 */

#if DEBUG
#define debug_m2(m) debug("\n| %11.4f %11.4f |\n| %11.4f %11.4f |", m[0][0], m[0][1], m[1][0], m[1][1])
#else
#define debug_m2(m)
#endif

static void pi_m2_zeros(m2 mat)
{
    mat[0][0] = 0; mat[1][0] = 0;
    mat[0][1] = 0; mat[1][1] = 0;
}

static void pi_m2_identity(m2 mat)
{
    mat[0][0] = 1; mat[1][0] = 0;
    mat[0][1] = 0; mat[1][1] = 1;
}

static void pi_m2_copy(m2 src, m2 dst)
{
    memcpy(dst, src, sizeof(*src) * 2);
}

static void pi_m2(m2 mat, const vec_float_t a00, const vec_float_t a01, const vec_float_t a10, const vec_float_t a11)
{
    mat[0][0] = a00; mat[1][0] = a10;
    mat[0][1] = a01; mat[1][1] = a11;
}

static void pi_m2_rotation(m2 mat, vec_float_t ang_in_rad)
{
    const vec_float_t c = cosf(ang_in_rad);
    const vec_float_t s = sinf(ang_in_rad);
    pi_m2(mat, c, -s, s, c);
}

static void pi_m2_scaling(m2 mat, vec_float_t scale_factor)
{
    pi_m2(mat, scale_factor, 0, 0, scale_factor);
}

static void pi_m2_from_v2(m2 mat, const v2 vec0, const v2 vec1)
{
    mat[0][0] = vec0[0];
    mat[0][1] = vec0[1];
    mat[1][0] = vec1[0];
    mat[1][1] = vec1[1];
}

static void pi_v2_mul_m2(const m2 mat, const v2 vec, v2 res) // res can be vec as well
{
    pi_v2(res, vec[0] * mat[0][0] + vec[1] * mat[1][0],
               vec[0] * mat[0][1] + vec[1] * mat[1][1]);
}

static void pi_m2_mul_m2(const m2 mat1, const m2 mat0, m2 res) // res can be mat0 as well
{
    pi_m2(res, mat0[0][0] * mat1[0][0] + mat0[0][1] * mat1[0][1],
               mat0[0][0] * mat1[1][0] + mat0[0][1] * mat1[1][1],
               mat0[1][0] * mat1[0][0] + mat0[1][1] * mat1[0][1],
               mat0[1][0] * mat1[1][0] + mat0[1][1] * mat1[1][1]);
}

static vec_float_t pi_m2_det(const m2 mat)
{
    return mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
}

/*
 *  Mat3 stuff
 */

#if DEBUG
#define debug_m3(m) debug("\n| %11.4f %11.4f %11.4f |\n| %11.4f %11.4f %11.4f |\n| %11.4f %11.4f %11.4f |", \
                    m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2])
#else
#define debug_m2(m)
#endif

static void pi_m3_zeros(m3 mat)
{
    mat[0][0] = 0; mat[1][0] = 0; mat[2][0] = 0;
    mat[0][1] = 0; mat[1][1] = 0; mat[2][1] = 0;
    mat[0][2] = 0; mat[1][2] = 0; mat[2][2] = 0;
}

static void pi_m3_identity(m3 mat)
{
    mat[0][0] = 1; mat[1][0] = 0; mat[2][0] = 0;
    mat[0][1] = 0; mat[1][1] = 1; mat[2][1] = 0;
    mat[0][2] = 0; mat[1][2] = 0; mat[2][2] = 1;
}

static void pi_m3_copy(m3 src, m3 dst)
{
    memcpy(dst, src, sizeof(*src) * 3);
}

static void pi_m3(m3 mat, const vec_float_t a00, const vec_float_t a01, const vec_float_t a02,
                          const vec_float_t a10, const vec_float_t a11, const vec_float_t a12,
                          const vec_float_t a20, const vec_float_t a21, const vec_float_t a22)
{
    mat[0][0] = a00; mat[1][0] = a10; mat[2][0] = a20;
    mat[0][1] = a01; mat[1][1] = a11; mat[2][1] = a21;
    mat[0][2] = a02; mat[1][2] = a12; mat[2][2] = a22;
}

static void pi_m3_scaling(m3 mat, vec_float_t scale_factor)
{
    mat[0][0] = scale_factor; mat[1][0] = 0; mat[2][0] = 0;
    mat[0][1] = 0; mat[1][1] = scale_factor; mat[2][1] = 0;
    mat[0][2] = 0; mat[1][2] = 0; mat[2][2] = scale_factor;
}

static void pi_m3_from_v3(m3 mat, const v3 vec0, const v3 vec1, const v3 vec2)
{
    mat[0][0] = vec0[0]; mat[1][0] = vec1[0]; mat[2][0] = vec2[0];
    mat[0][1] = vec0[1]; mat[1][1] = vec1[1]; mat[2][1] = vec2[1];
    mat[0][2] = vec0[2]; mat[1][2] = vec1[2]; mat[2][2] = vec2[2];
}

static void pi_v3_mul_m3(const m3 mat, const v3 vec, v3 res) // res can be vec as well
{
    v3 tmp_vec;
    memcpy(tmp_vec, vec, sizeof(v3));
    res[0] = tmp_vec[0] * mat[0][0] + tmp_vec[1] * mat[1][0] + tmp_vec[2] * mat[2][0];
    res[1] = tmp_vec[0] * mat[0][1] + tmp_vec[1] * mat[1][1] + tmp_vec[2] * mat[2][1];
    res[2] = tmp_vec[0] * mat[0][2] + tmp_vec[1] * mat[1][2] + tmp_vec[2] * mat[2][2];
}

static void pi_m3_mul_m3(const m3 mat1, const m3 mat0, m3 res) // res can be mat0 as well
{
    m3 tmp_mat;
    memcpy(tmp_mat, mat0, sizeof(m3));
    res[0][0] = tmp_mat[0][0] * mat1[0][0] + tmp_mat[0][1] * mat1[1][0] + tmp_mat[0][2] * mat1[2][0];
    res[1][0] = tmp_mat[1][0] * mat1[0][0] + tmp_mat[1][1] * mat1[1][0] + tmp_mat[1][2] * mat1[2][0];
    res[2][0] = tmp_mat[2][0] * mat1[0][0] + tmp_mat[2][1] * mat1[1][0] + tmp_mat[2][2] * mat1[2][0];
    res[0][1] = tmp_mat[0][0] * mat1[0][1] + tmp_mat[0][1] * mat1[1][1] + tmp_mat[0][2] * mat1[2][1];
    res[1][1] = tmp_mat[1][0] * mat1[0][1] + tmp_mat[1][1] * mat1[1][1] + tmp_mat[1][2] * mat1[2][1];
    res[2][1] = tmp_mat[2][0] * mat1[0][1] + tmp_mat[2][1] * mat1[1][1] + tmp_mat[2][2] * mat1[2][1];
    res[0][2] = tmp_mat[0][0] * mat1[0][2] + tmp_mat[0][1] * mat1[1][2] + tmp_mat[0][2] * mat1[2][2];
    res[1][2] = tmp_mat[1][0] * mat1[0][2] + tmp_mat[1][1] * mat1[1][2] + tmp_mat[1][2] * mat1[2][2];
    res[2][2] = tmp_mat[2][0] * mat1[0][2] + tmp_mat[2][1] * mat1[1][2] + tmp_mat[2][2] * mat1[2][2];
}

static vec_float_t pi_m3_det(const m3 mat)
{
    return mat[0][0] * mat[1][1] * mat[2][2] + mat[1][0] * mat[2][1] * mat[0][2] + mat[2][0] * mat[0][1] * mat[1][2] -
           (mat[0][2] * mat[1][1] * mat[2][0] + mat[1][2] * mat[2][1] * mat[0][0] + mat[0][1] * mat[1][0] * mat[2][2]);
}

static void pi_m3_rotationX(m3 mat, vec_float_t ang_in_rad)
{
    const vec_float_t c = cosf(ang_in_rad);
    const vec_float_t s = sinf(ang_in_rad);
    pi_m3(mat, 1, 0, 0, 0, c, s, 0, -s, c);
}

static void pi_m3_rotationY(m3 mat, vec_float_t ang_in_rad)
{
    const vec_float_t c = cosf(ang_in_rad);
    const vec_float_t s = sinf(ang_in_rad);
    pi_m3(mat, c, 0, -s, 0, 1, 0, s, 0, c);
}

static void pi_m3_rotationZ(m3 mat, vec_float_t ang_in_rad)
{
    const vec_float_t c = cosf(ang_in_rad);
    const vec_float_t s = sinf(ang_in_rad);
    pi_m3(mat, c, s, 0, -s, c, 0, 0, 0, 1);
}

/*
 *  General purpose math
 */

static float32 wrap_angle(const float32 angle)
{
    const float32 mod = fmod(angle, 2 * M_PI);
    return mod > M_PI ? mod - 2 * M_PI : mod;
}

static bool pi_aabb_box_x_point(vi2 p, vi2 box[2])
{
    return ((p[0] >= box[0][0]) && (p[1] >= box[0][1])) && ((p[0] < box[1][0]) && (p[1] < box[1][1]));
}

static bool pi_aabb_box_x_point_by_size(int x, int y, int box_x, int box_y, int w, int h)
{
    return pi_aabb_box_x_point((vi2) {x, y}, (vi2[2]) {box_x, box_y, box_x + w, box_y + h});
}

static uint pi_log10(uint number)
{
    uint counter = 0;
    while (number /= 10) counter++;
    return counter;
}

static uint pi_log2(uint number)
{
    uint counter = 0;
    while (number >>= 1) counter++;
    return counter;
}

#define pi_lerp(start, end, amount) ((float32)(start) + (float32)((end) - (start)) * (float32)(amount))

#define pi_clamp(x, min, max) (MIN(MAX(x, min), max))

#endif //ALPHABETA_PI_H
