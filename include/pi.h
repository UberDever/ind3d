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
#define M_PI 3.1415926535897932384626433832795
#endif

#ifndef M_2PI
#define M_2PI 6.283185307179586476925286766559
#endif

#ifndef M_PI_2
#define M_PI_2 1.5707963267948966192313216916398
#endif

#ifndef M_PI_4
#define M_PI_4 0.78539816339744830961566084581988
#endif

#define SQRT2 1.41421356237309504880f
#define SQRT2_2 0.70710678118654752440f

#define DEG2RAD(phi) (0.01745329f * (phi))
#define RAD2DEG(phi) (57.2957795f * (phi))

#define pi_lerp(start, end, amount) ((float32)(start) + (float32)((end) - (start)) * (float32)(amount))
#define pi_clamp(x, min, max) (MIN(MAX(x, min), max))

/*
 *  Newly updated vector math
 */

typedef union v2_t
{
    float32 v[2];
    struct
    {
        float32 x, y;
    };
} v2_t;
#define debug_v2(v) debug("( %f %f )", (v).x, (v).y)

typedef union v3_t
{
    float32 v[3];

    struct
    {
        float32 x, y, z;
    };
} v3_t;
#define debug_v3(v) debug("( %f %f %f )", (v).x, (v).y, (v).z)

typedef union v4_t
{
    float32 v[4];
    struct
    {
        union
        {
            v3_t vec;
            struct
            {
                float32 x, y, z;
            };
        };
        float32 w;
    };
} v4_t;
#define debug_v4(v) debug("( %f %f %f %f)", (v).x, (v).y, (v).z, (v).w)
typedef v4_t quaternion_t;

typedef union m2_t
{
    float32 m[4];
    float32 mat[2][2];
    struct
    {
        float32 m00, m10;
        float32 m01, m11;
    };
} m2_t;

typedef union m3_t
{
    float32 m[9];
    float32 mat[3][3];
    struct
    {
        float32 m00, m10, m20;
        float32 m01, m11, m21;
        float32 m02, m12, m22;
    };
} m3_t;

typedef union m4_t
{
    float32 m[16];
    float32 mat[4][4];
    struct
    {
        float32 m00, m01, m02, m03;
        float32 m10, m11, m12, m13;
        float32 m20, m21, m22, m23;
        float32 m30, m31, m32, m33;
    };
} m4_t;

// v2
#if 1
static inline v2_t v2_ones()
{
    return (v2_t){1.f, 1.f};
}

static inline v2_t v2_(const v2_t v)
{
    return (v2_t){v.x, v.y};
}

static inline v2_t v2_add(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x + v1.x, v0.y + v1.y};
}

static inline v2_t v2_sub(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x - v1.x, v0.y - v1.y};
}

static inline v2_t v2_mul(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x * v1.x, v0.y * v1.y};
}

static inline v2_t v2_div(const v2_t v0, const v2_t v1)
{
    return (v2_t){v0.x / v1.x, v0.y / v1.y};
}

static inline v2_t v2_adds(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x + scalar, v.y + scalar};
}

static inline v2_t v2_subs(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x - scalar, v.y - scalar};
}

static inline v2_t v2_muls(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x * scalar, v.y * scalar};
}

static inline v2_t v2_divs(const v2_t v, const float32 scalar)
{
    return (v2_t){v.x / scalar, v.y / scalar};
}

static inline float32 v2_dot(const v2_t v0, const v2_t v1)
{
    return v0.x * v1.x + v0.y * v1.y;
}

static inline float32 v2_area(const v2_t v0, const v2_t v1)
{
    return v0.x * v1.y - v0.y * v1.x;
}

static inline float32 v2_len_sq(const v2_t v)
{
    return v2_dot(v, v);
}

static inline float32 v2_len(const v2_t v)
{
    return sqrtf(v2_len_sq(v));
}

static inline v2_t v2_norm(const v2_t v)
{
    float32 len = v2_len(v);
    return (v2_t){v.x / len, v.y / len};
}

// m2

static inline m2_t m2_identity()
{
    return (m2_t){1, 0, 0, 1};
}

// v3

static v3_t unit_x = (v3_t){.x = 1, .y = 0, .z = 0};
static v3_t unit_y = (v3_t){.x = 0, .y = 1, .z = 0};
static v3_t unit_z = (v3_t){.x = 0, .y = 0, .z = 1};

static inline v3_t v3_ones()
{
    return (v3_t){1.f, 1.f, 1.f};
}

static inline v3_t v3(float32 x, float32 y, float32 z)
{
    return (v3_t){x, y, z};
}

static inline v3_t v3_(const v3_t v)
{
    return (v3_t){v.x, v.y, v.z};
}

static inline v3_t v3_add(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x + v1.x, v0.y + v1.y, v0.z + v1.z};
}

static inline v3_t v3_sub(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x - v1.x, v0.y - v1.y, v0.z - v1.z};
}

static inline v3_t v3_mul(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x * v1.x, v0.y * v1.y, v0.z * v1.z};
}

static inline v3_t v3_div(const v3_t v0, const v3_t v1)
{
    return (v3_t){v0.x / v1.x, v0.y / v1.y, v0.z / v1.z};
}

static inline v3_t v3_adds(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x + scalar, v.y + scalar, v.z + scalar};
}

static inline v3_t v3_subs(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x - scalar, v.y - scalar, v.z - scalar};
}

static inline v3_t v3_muls(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x * scalar, v.y * scalar, v.z * scalar};
}

static inline v3_t v3_divs(const v3_t v, const float32 scalar)
{
    return (v3_t){v.x / scalar, v.y / scalar, v.z / scalar};
}

static inline float32 v3_dot(const v3_t v0, const v3_t v1)
{
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

static inline v3_t v3_cross(const v3_t v0, const v3_t v1)
{
    return (v3_t){
        v0.y * v1.z - v0.z * v1.y,
        v0.z * v1.x - v0.x * v1.z,
        v0.x * v1.y - v0.y * v1.x};
}

static inline float32 v3_len_sq(const v3_t v)
{
    return v3_dot(v, v);
}

static inline float32 v3_len(const v3_t v)
{
    return sqrtf(v3_len_sq(v));
}

static inline v3_t v3_norm(const v3_t v)
{
    float32 len = v3_len(v);
    return (v3_t){v.x / len, v.y / len, v.z / len};
}

static inline v3_t v3_lerp(const v3_t v0, const v3_t v1, const float32 amount)
{
    v3_t v = {};
    v.x = pi_lerp(v0.x, v1.x, amount);
    v.y = pi_lerp(v0.y, v1.y, amount);
    v.z = pi_lerp(v0.z, v1.z, amount);
    return v;
}

static inline v3_t v3_mul_m3(v3_t v, m3_t m)
{
    return (v3_t){m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
                  m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
                  m.m20 * v.x + m.m21 * v.y + m.m22 * v.z};
}

/*
    v4_t
*/

static inline v4_t v4_divs(const v4_t v, const float32 scalar)
{
    return (v4_t){v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar};
}

static inline float32 v4_dot(const v4_t v0, const v4_t v1)
{
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z + v0.w * v1.w;
}

static inline float32 v4_len_sq(const v4_t v)
{
    return v4_dot(v, v);
}

static inline float32 v4_len(const v4_t v)
{
    return sqrtf(v4_len_sq(v));
}

static inline v4_t v4_norm(const v4_t v)
{
    float32 len = v4_len(v);
    return (v4_t){v.x / len, v.y / len, v.z / len, v.w / len};
}

/*
    quaternion_t
*/

static inline quaternion_t quaternion_divs(quaternion_t q, float32 scalar)
{
    return v4_divs(q, scalar);
}

static inline quaternion_t quaternion_conjugate(quaternion_t q)
{
    return (quaternion_t){-q.x, -q.y, -q.z, q.w};
}

static inline float32 quaternion_len(quaternion_t q)
{
    return v4_len(q);
}

static inline quaternion_t quaternion_norm(quaternion_t q)
{
    return v4_norm(q);
}

// Assuming q is normalized
static inline quaternion_t quaternion_inverse(quaternion_t q)
{
    return quaternion_conjugate(q);
}

static inline quaternion_t quaternion_mul(quaternion_t q0, quaternion_t q1)
{
    return (quaternion_t){
        .x = q0.y * q1.z - q0.z * q1.y + q0.x * q1.w + q0.w * q1.x,
        .y = q0.z * q1.x - q0.x * q1.z + q0.y * q1.w + q0.w * q1.y,
        .z = q0.x * q1.y - q0.y * q1.x + q0.z * q1.w + q0.w * q1.z,
        .w = q0.w * q1.w - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z};
}

static quaternion_t quaternion_slerp(quaternion_t q0, quaternion_t q1, float32 t)
{
    // quaternion to return
    quaternion_t q = {};
    // Calculate angle between them.
    const float32 cosHalfTheta = q0.w * q1.w + q0.x * q1.x + q0.y * q1.y + q0.z * q1.z;
    // if q0=q1 or q0=-q1 then theta = 0 and we can return q0
    if (fabsf(cosHalfTheta) >= 1.0)
    {
        q.w = q0.w;
        q.x = q0.x;
        q.y = q0.y;
        q.z = q0.z;
        return q;
    }
    // Calculate temporary values.
    const float32 halfTheta = acos(cosHalfTheta);
    const float32 sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
    // if theta = 180 degrees then result is not fully defined
    // we could rotate around any axis normal to q0 or q1
    if (fabsf(sinHalfTheta) < FLT_EPSILON)
    {
        q.w = (q0.w * 0.5f + q1.w * 0.5f);
        q.x = (q0.x * 0.5f + q1.x * 0.5f);
        q.y = (q0.y * 0.5f + q1.y * 0.5f);
        q.z = (q0.z * 0.5f + q1.z * 0.5f);
        return q;
    }
    const float32 ratioA = sin((1.f - t) * halfTheta) / sinHalfTheta;
    const float32 ratioB = sin(t * halfTheta) / sinHalfTheta;
    //calculate Quaternion.
    q.w = (q0.w * ratioA + q1.w * ratioB);
    q.x = (q0.x * ratioA + q1.x * ratioB);
    q.y = (q0.y * ratioA + q1.y * ratioB);
    q.z = (q0.z * ratioA + q1.z * ratioB);
    return q;
}

static inline quaternion_t quaternion_axis(v3_t axis, float32 angle)
{
    quaternion_t q = {};
    const float32 s_2 = sinf(angle / 2);
    const float32 c_2 = cosf(angle / 2);

    q.x = axis.x * s_2;
    q.y = axis.y * s_2;
    q.z = axis.z * s_2;
    q.w = c_2;
    return q;
}

// Angles in radians,
static inline quaternion_t quaternion_from_euler(v3_t v)
{
    float32 c1 = cosf(v.z / 2.f);
    float32 c2 = cosf(v.y / 2.f);
    float32 c3 = cosf(v.x / 2.f);
    float32 s1 = sinf(v.z / 2.f);
    float32 s2 = sinf(v.y / 2.f);
    float32 s3 = sinf(v.x / 2.f);

    return (quaternion_t){
        .x = c1 * c2 * s3 - s1 * s2 * c3,
        .y = c1 * s2 * c3 + s1 * c2 * s3,
        .z = s1 * c2 * c3 - c1 * s2 * s3,
        .w = c1 * c2 * c3 + s1 * s2 * s3};
}

/*
    Matrices
*/

static inline m3_t
mat3_v(v3_t i, v3_t j, v3_t k)
{
    return (m3_t){i.x, i.y, i.z,
                  j.x, j.y, j.z,
                  k.x, k.y, k.z};
}

static inline m3_t mat3(
    float m00, float m10, float m20,
    float m01, float m11, float m21,
    float m02, float m12, float m22)
{
    return (m3_t){
        .mat[0][0] = m00, .mat[1][0] = m10, .mat[2][0] = m20, .mat[0][1] = m01, .mat[1][1] = m11, .mat[2][1] = m21, .mat[0][2] = m02, .mat[1][2] = m12, .mat[2][2] = m22};
}

static inline m4_t mat4(
    float32 m00, float32 m10, float32 m20, float32 m30,
    float32 m01, float32 m11, float32 m21, float32 m31,
    float32 m02, float32 m12, float32 m22, float32 m32,
    float32 m03, float32 m13, float32 m23, float32 m33)
{
    return (m4_t){
        .mat[0][0] = m00, .mat[1][0] = m10, .mat[2][0] = m20, .mat[3][0] = m30, .mat[0][1] = m01, .mat[1][1] = m11, .mat[2][1] = m21, .mat[3][1] = m31, .mat[0][2] = m02, .mat[1][2] = m12, .mat[2][2] = m22, .mat[3][2] = m32, .mat[0][3] = m03, .mat[1][3] = m13, .mat[2][3] = m23, .mat[3][3] = m33};
}

static inline m4_t m4_perspective(float vertical_field_of_view_in_deg, float aspect_ratio, float near_view_distance, float far_view_distance)
{
    float fovy_in_rad = vertical_field_of_view_in_deg / 180 * M_PI;
    float f = 1.0f / tanf(fovy_in_rad / 2.0f);
    float ar = aspect_ratio;
    float nd = near_view_distance, fd = far_view_distance;

    return mat4(
        f / ar, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (fd + nd) / (nd - fd), (2 * fd * nd) / (nd - fd),
        0, 0, -1, 0);
}

static inline m4_t m4_identity()
{
    return mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_translation(v3_t offset)
{
    return mat4(
        1, 0, 0, offset.x,
        0, 1, 0, offset.y,
        0, 0, 1, offset.z,
        0, 0, 0, 1);
}

static inline m4_t m4_scaling(v3_t scale)
{
    float x = scale.x, y = scale.y, z = scale.z;
    return mat4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1);
}

static m4_t m4_rotation(float angle_in_rad, v3_t axis)
{
    v3_t normalized_axis = v3_norm(axis);
    float x = normalized_axis.x, y = normalized_axis.y, z = normalized_axis.z;
    float c = cosf(angle_in_rad), s = sinf(angle_in_rad);

    return mat4(
        c + x * x * (1 - c), x * y * (1 - c) - z * s, x * z * (1 - c) + y * s, 0,
        y * x * (1 - c) + z * s, c + y * y * (1 - c), y * z * (1 - c) - x * s, 0,
        z * x * (1 - c) - y * s, z * y * (1 - c) + x * s, c + z * z * (1 - c), 0,
        0, 0, 0, 1);
}

static inline m4_t m4_rotation_x(float angle_in_rad)
{
    float s = sinf(angle_in_rad), c = cosf(angle_in_rad);
    return mat4(
        1, 0, 0, 0,
        0, c, -s, 0,
        0, s, c, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_rotation_y(float angle_in_rad)
{
    float s = sinf(angle_in_rad), c = cosf(angle_in_rad);
    return mat4(
        c, 0, s, 0,
        0, 1, 0, 0,
        -s, 0, c, 0,
        0, 0, 0, 1);
}

static inline m4_t m4_rotation_z(float angle_in_rad)
{
    float s = sinf(angle_in_rad), c = cosf(angle_in_rad);
    return mat4(
        c, -s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

static inline m4_t quaternion_rot_mat(quaternion_t q)
{
    const float32 x2 = q.x * q.x;
    const float32 y2 = q.y * q.y;
    const float32 z2 = q.z * q.z;
    const float32 xy = q.x * q.y;
    const float32 xz = q.x * q.z;
    const float32 yz = q.y * q.z;
    const float32 wx = q.w * q.x;
    const float32 wy = q.w * q.y;
    const float32 wz = q.w * q.z;
    return (m4_t)mat4(1.f - 2.f * (y2 + z2), 2.f * (xy - wz), 2.f * (xz + wy), 0,
                      2.f * (xy + wz), 1.f - 2.f * (x2 + z2), 2.f * (yz - wx), 0,
                      2.f * (xz - wy), 2.f * (yz + wx), 1.f - 2.f * (x2 + y2), 0,
                      0, 0, 0, 1.f);
}

static inline m4_t m4_transpose(m4_t matrix)
{
    return mat4(
        matrix.m00, matrix.m01, matrix.m02, matrix.m03,
        matrix.m10, matrix.m11, matrix.m12, matrix.m13,
        matrix.m20, matrix.m21, matrix.m22, matrix.m23,
        matrix.m30, matrix.m31, matrix.m32, matrix.m33);
}

static inline m4_t m4_mul(m4_t a, m4_t b)
{
    m4_t result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0;
            for (int k = 0; k < 4; k++)
            {
                sum += a.mat[k][j] * b.mat[i][k];
            }
            result.mat[i][j] = sum;
        }
    }

    return result;
}

static inline v3_t m4_mul_pos(m4_t matrix, v3_t position)
{
    v3_t result = v3(
        matrix.m00 * position.x + matrix.m10 * position.y + matrix.m20 * position.z + matrix.m30,
        matrix.m01 * position.x + matrix.m11 * position.y + matrix.m21 * position.z + matrix.m31,
        matrix.m02 * position.x + matrix.m12 * position.y + matrix.m22 * position.z + matrix.m32);

    float w = matrix.m03 * position.x + matrix.m13 * position.y + matrix.m23 * position.z + matrix.m33;
    if (w != 0 && w != 1)
        return v3(result.x / w, result.y / w, result.z / w);

    return result;
}

static inline v3_t m4_mul_dir(m4_t matrix, v3_t direction)
{
    v3_t result = v3(
        matrix.m00 * direction.x + matrix.m10 * direction.y + matrix.m20 * direction.z,
        matrix.m01 * direction.x + matrix.m11 * direction.y + matrix.m21 * direction.z,
        matrix.m02 * direction.x + matrix.m12 * direction.y + matrix.m22 * direction.z);

    float w = matrix.m03 * direction.x + matrix.m13 * direction.y + matrix.m23 * direction.z;
    if (w != 0 && w != 1)
        return v3(result.x / w, result.y / w, result.z / w);

    return result;
}

static inline m4_t m4_look_at(v3_t from, v3_t dir, v3_t up)
{
    v3_t z = v3_muls(dir, -1);
    v3_t x = v3_norm(v3_cross(up, z));
    v3_t y = v3_cross(z, x);

    return mat4(
        x.x, x.y, x.z, -v3_dot(from, x),
        y.x, y.y, y.z, -v3_dot(from, y),
        z.x, z.y, z.z, -v3_dot(from, z),
        0, 0, 0, 1);
}

static m4_t m4_invert_affine(m4_t matrix)
{
    // Create shorthands to access matrix members
    float m00 = matrix.m00, m10 = matrix.m10, m20 = matrix.m20, m30 = matrix.m30;
    float m01 = matrix.m01, m11 = matrix.m11, m21 = matrix.m21, m31 = matrix.m31;
    float m02 = matrix.m02, m12 = matrix.m12, m22 = matrix.m22, m32 = matrix.m32;

    // Invert 3x3 part of the 4x4 matrix that contains the rotation, etc.
    // That part is called R from here on.

    // Calculate cofactor matrix of R
    float c00 = m11 * m22 - m12 * m21, c10 = -(m01 * m22 - m02 * m21), c20 = m01 * m12 - m02 * m11;
    float c01 = -(m10 * m22 - m12 * m20), c11 = m00 * m22 - m02 * m20, c21 = -(m00 * m12 - m02 * m10);
    float c02 = m10 * m21 - m11 * m20, c12 = -(m00 * m21 - m01 * m20), c22 = m00 * m11 - m01 * m10;

    // Caclculate the determinant by using the already calculated determinants
    // in the cofactor matrix.
    // Second sign is already minus from the cofactor matrix.
    float det = m00 * c00 + m10 * c10 + m20 * c20;
    if (fabsf(det) < 0.00001f)
        return m4_identity();

    // Calcuate inverse of R by dividing the transposed cofactor matrix by the
    // determinant.
    float i00 = c00 / det, i10 = c01 / det, i20 = c02 / det;
    float i01 = c10 / det, i11 = c11 / det, i21 = c12 / det;
    float i02 = c20 / det, i12 = c21 / det, i22 = c22 / det;

    // Combine the inverted R with the inverted translation
    return mat4(
        i00, i10, i20, -(i00 * m30 + i10 * m31 + i20 * m32),
        i01, i11, i21, -(i01 * m30 + i11 * m31 + i21 * m32),
        i02, i12, i22, -(i02 * m30 + i12 * m31 + i22 * m32),
        0, 0, 0, 1);
}

static void m4_fprintp(FILE *stream, m4_t matrix, int width, int precision)
{
    m4_t m = matrix;
    int w = width, p = precision;
    for (int r = 0; r < 4; r++)
    {
        fprintf(stream, "| %*.*f %*.*f %*.*f %*.*f |\n",
                w, p, m.mat[0][r], w, p, m.mat[1][r], w, p, m.mat[2][r], w, p, m.mat[3][r]);
    }
}

static void m4_print(m4_t matrix)
{
    m4_fprintp(stdout, matrix, 6, 2);
}

static void m4_printp(m4_t matrix, int width, int precision)
{
    m4_fprintp(stdout, matrix, width, precision);
}

static void m4_fprint(FILE *stream, m4_t matrix)
{
    m4_fprintp(stream, matrix, 6, 2);
}

#endif
/*
 *  General purpose math
 */

static float32
wrap_angle(const float32 angle)
{
    const float32 mod = fmod(angle, 2 * M_PI);
    return mod > M_PI ? mod - 2 * M_PI : mod;
}

static bool pi_aabb_box_x_point(int p_x, int p_y, int b0_x, int b0_y, int b1_x, int b1_y)
{
    return ((p_x >= b0_x) && (p_y >= b0_y)) && ((p_x < b1_x) && (p_y < b1_y));
}

static inline bool pi_aabb_box_x_point_by_size(int x, int y, int box_x, int box_y, int w, int h)
{
    return pi_aabb_box_x_point(x, y, box_x, box_y, box_x + w, box_y + h);
}

static bool pi_raycast(float32 pos_x, float32 pos_y, float32 ray_x, float32 ray_y, float32 *intersect_x, float32 *intersect_y, bool (*f)(int, int))
{
    float32 side_x, side_y;
    float32 delta_x = fabsf(1.f / ray_x), delta_y = fabsf(1.f / ray_y);
    int step_x, step_y;
    bool hit = false;
    if (ray_x > 0)
    {
        step_x = 1;
        side_x = ((int)pos_x - pos_x + 1) * delta_x;
    }
    else
    {
        step_x = -1;
        side_x = (pos_x - (int)pos_x) * delta_x;
    }
    if (ray_y > 0)
    {
        step_y = 1;
        side_y = ((int)pos_y - pos_y + 1) * delta_y;
    }
    else
    {
        step_y = -1;
        side_y = (pos_y - (int)pos_y) * delta_y;
    }
    while (!hit)
    {
        if (side_x < side_y)
        {
            side_x += delta_x;
            pos_x += step_x;
        }
        else
        {
            side_y += delta_y;
            pos_y += step_y;
        }
        if (f(pos_x, pos_y))
            hit = true;
    }
    if (intersect_x && intersect_y)
    {
        *intersect_x = pos_x;
        *intersect_y = pos_y;
    }
    return hit;
}

static uint pi_log10(uint number)
{
    uint counter = 0;
    while (number /= 10)
        counter++;
    return counter;
}

static uint pi_log2(uint number)
{
    uint counter = 0;
    while (number >>= 1)
        counter++;
    return counter;
}

#endif //ALPHABETA_PI_H
