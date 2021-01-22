#include "geometry.h"

// Vx returned in CCW direction
// 0--3
// | /|
// |/ |
// 1--2
// void generate_quat(v_GLfloat_t *vertices, v_GLuint_t *indices, ChunkVx vx[4], GLuint vx_stride)
// {
//     GLuint start_index = vertices->size / vx_stride;

//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[0].p.v[i]);
//     for (int i = 0; i < 2; i++)
//         vec_push(*vertices, vx[0].t.v[i]);
//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[0].n.v[i]);

//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[1].p.v[i]);
//     for (int i = 0; i < 2; i++)
//         vec_push(*vertices, vx[1].t.v[i]);
//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[1].n.v[i]);

//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[2].p.v[i]);
//     for (int i = 0; i < 2; i++)
//         vec_push(*vertices, vx[2].t.v[i]);
//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[2].n.v[i]);

//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[3].p.v[i]);
//     for (int i = 0; i < 2; i++)
//         vec_push(*vertices, vx[3].t.v[i]);
//     for (int i = 0; i < 3; i++)
//         vec_push(*vertices, vx[3].n.v[i]);

//     vec_push(*indices, start_index + 0);
//     vec_push(*indices, start_index + 1);
//     vec_push(*indices, start_index + 3);
//     vec_push(*indices, start_index + 3);
//     vec_push(*indices, start_index + 1);
//     vec_push(*indices, start_index + 2);
// }

void generate_pube(v_GLfloat_t *vertices, v_GLuint_t *indices, bool adj_check[4], int x, int y, v2_t t, GLuint cur_pube_idx)
{
    const GLfloat s = 0.5f;
    // GLfloat vx[] = {
    //     -s, -s, -s, 4, 1.f, 0.f, 0.f, 0.f,
    //     -s, -s, +s, 1, 1.f, 0.f, 0.f, 0.f,
    //     -s, +s, -s, 4, 0.f, 0.f, 0.f, 0.f,
    //     -s, +s, +s, 1, 0.f, 0.f, 0.f, 0.f,
    //     +s, -s, -s, 3, 1.f, 0.f, 0.f, 0.f,
    //     +s, -s, +s, 2, 1.f, 0.f, 0.f, 0.f,
    //     +s, +s, -s, 3, 0.f, 0.f, 0.f, 0.f,
    //     +s, +s, +s, 2, 0.f, 0.f, 0.f, 0.f,
    //     -s, -s, -s, 0, 1.f, 0.f, 0.f, 0.f,
    //     -s, +s, -s, 0, 0.f, 0.f, 0.f, 0.f};
    GLfloat vx[] = {
        -s, -s, -s, 0, 1.f, -1.f, 0.f, 0.f, //lower side       0
        -s, -s, +s, 1, 1.f, -1.f, 0.f, 0.f, // 1
        -s, -s, +s, 1, 1.f, 0.f, 0.f, 1.f,  // 2
        +s, -s, +s, 2, 1.f, 0.f, 0.f, 1.f,  // 3
        +s, -s, +s, 2, 1.f, 1.f, 0.f, 0.f,  // 4
        +s, -s, -s, 3, 1.f, 1.f, 0.f, 0.f,  // 5
        +s, -s, -s, 3, 1.f, 0.f, 0.f, -1.f, // 6
        -s, -s, -s, 4, 1.f, 0.f, 0.f, -1.f, // 7

        -s, +s, -s, 0, 0.f, -1.f, 0.f, 0.f, //upper side       8
        -s, +s, +s, 1, 0.f, -1.f, 0.f, 0.f, // 9
        -s, +s, +s, 1, 0.f, 0.f, 0.f, 1.f,  // 10
        +s, +s, +s, 2, 0.f, 0.f, 0.f, 1.f,  // 11
        +s, +s, +s, 2, 0.f, 1.f, 0.f, 0.f,  // 12
        +s, +s, -s, 3, 0.f, 1.f, 0.f, 0.f,  // 13
        +s, +s, -s, 3, 0.f, 0.f, 0.f, -1.f, // 14
        -s, +s, -s, 4, 0.f, 0.f, 0.f, -1.f, // 15
    };
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i += 8)
    {
        vx[i + 0] += x;
        vx[i + 1] += 0;
        vx[i + 2] += y;
        vx[i + 3] += t.x;
        vx[i + 4] += t.y;
        vx[i + 3] *= (tileset_tile_w);
        vx[i + 4] *= (tileset_tile_h);
        vx[i + 3] /= tileset_tex_w;
        vx[i + 4] /= tileset_tex_h;
    }
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i++)
    {
        vec_push(*vertices, vx[i]);
    }
    GLuint cur_idx = cur_pube_idx * (ARR_ONE_DIM_LEN(vx) / 8);
    if (adj_check[0])
    {
        //GLuint ind[] = {0, 2, 4, 2, 6, 4};
        GLuint ind[] = {7, 15, 6, 15, 14, 6};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[1])
    {
        //GLuint ind[] = {6, 5, 4, 6, 7, 5};
        GLuint ind[] = {13, 4, 5, 13, 12, 4};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[2])
    {
        //GLuint ind[] = {7, 3, 5, 3, 1, 5};
        GLuint ind[] = {11, 10, 3, 10, 2, 3};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
    if (adj_check[3])
    {
        //GLuint ind[] = {3, 9, 1, 9, 8, 1};
        GLuint ind[] = {9, 8, 1, 8, 0, 1};
        for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
        {
            vec_push(*indices, ind[i] + cur_idx);
        }
    }
}

mapls_register(GLuint);
typedef struct mls_GLuint_t m_GLuint_t;

static void
obj_parse_face(char *face_name, m_GLuint_t table_faces, uint *cur_id,
               v_GLfloat_t *vertices, v_GLuint_t *indices, v_GLfloat_t positions, v_GLfloat_t tex_coords, v_GLfloat_t normals)
{
    GLuint *ptr = mapls_search_ptr(table_faces, face_name);
    if (ptr)
    {
        vec_push(*indices, *ptr);
    }
    else
    {
        mapls_add(table_faces, face_name, *cur_id);
        vec_push(*indices, *cur_id);
        (*cur_id)++;

        int face_indices[3] = {-1, -1, -1};
        char *ind = strtok(face_name, "/");
        for (uint i = 0; i < 3 && ind; i++)
        {
            face_indices[i] = atoi(ind) - 1;
            ind = strtok(NULL, "/");
        }
        //debug("%d %d %d", face_indices[0], face_indices[1], face_indices[2]);
        vec_push(*vertices, positions.data[face_indices[0] * 3]);
        vec_push(*vertices, positions.data[face_indices[0] * 3 + 1]);
        vec_push(*vertices, positions.data[face_indices[0] * 3 + 2]);
        if (face_indices[1] >= 0)
        {
            vec_push(*vertices, tex_coords.data[face_indices[1] * 2 + 0]);
            vec_push(*vertices, tex_coords.data[face_indices[1] * 2 + 1]);
        }
        else
        {
            vec_push(*vertices, 0);
            vec_push(*vertices, 0);
        }
        if (face_indices[2] >= 0)
        {
            vec_push(*vertices, normals.data[face_indices[2] * 3]);
            vec_push(*vertices, normals.data[face_indices[2] * 3 + 1]);
            vec_push(*vertices, normals.data[face_indices[2] * 3 + 2]);
        }
        else
        {
            vec_push(*vertices, 0);
            vec_push(*vertices, 0);
            vec_push(*vertices, 0);
        }
    }
}

#define obj_line_size 100
Mesh obj_parse(const char *path)
{
    Mesh mesh = {};
    FILE *obj_file = fopen(path, "r");
    if (!obj_file)
        error("Cannot open %s obj file", path);

    uint v_count = 0;
    uint vt_count = 0;
    uint vn_count = 0;
    uint ind_count = 0;
    char ch = 0;
    do
    {
        ch = fgetc(obj_file);

        switch (ch)
        {
        case EOF:
            break;

        case 'v':
            ch = fgetc(obj_file);
            switch (ch)
            {
            case ' ':
                v_count++;
                SKIP_LINE(ch, obj_file);
                break;

            case 't':
                vt_count++;
                SKIP_LINE(ch, obj_file);
                break;

            case 'n':
                vn_count++;
                SKIP_LINE(ch, obj_file);
                break;

            default:
                error("Unsupported vertex type, aborting");
            }
            break;

        case 'f':
            ind_count++;
            SKIP_LINE(ch, obj_file);
            break;

        default:
            SKIP_LINE(ch, obj_file);
            break;
        }

    } while (ch != EOF);

    m_GLuint_t table_faces;

    mapls_new(GLuint, table_faces, 101);

    v_GLfloat_t positions, tex_coords, normals;
    vec_new(GLfloat, positions, 3 * v_count);
    vec_new(GLfloat, tex_coords, 2 * vt_count);
    vec_new(GLfloat, normals, 3 * vn_count);
    vec_new(GLuint, mesh.indices, ind_count * 3);
    vec_new(GLfloat, mesh.vertices, 128 * 8);
    uint cur_id = 0;

    fseek(obj_file, 0, SEEK_SET);

    do
    {
        v3_t v;
        ch = fgetc(obj_file);

        switch (ch)
        {
        case EOF:
            break;

        case '#':
            SKIP_LINE(ch, obj_file);
            break;

        case 'v':
            ch = fgetc(obj_file);
            switch (ch)
            {
            case ' ':
                fscanf(obj_file, "%f %f %f\n", &v.x, &v.y, &v.z);
                vec_push(positions, v.x);
                vec_push(positions, v.y);
                vec_push(positions, v.z);
                break;

            case 't':
                fscanf(obj_file, " %f %f\n", &v.x, &v.y);
                v.y = 1 - v.y;
                //v.x = 1 - v.x;
                vec_push(tex_coords, v.x);
                vec_push(tex_coords, v.y);
                break;

            case 'n':
                fscanf(obj_file, " %f %f %f\n", &v.x, &v.y, &v.z);
                vec_push(normals, v.x);
                vec_push(normals, v.y);
                vec_push(normals, v.z);
                break;

            default:
                error("Unsupported vertex type, aborting");
            }
            break;

        case 'f':
        {
            char face1[16] = {0};
            char face2[16] = {0};
            char face3[16] = {0};
            fscanf(obj_file, "%15s", face1);
            fscanf(obj_file, "%15s", face2);
            fscanf(obj_file, "%15s", face3);
            obj_parse_face(face1, table_faces, &cur_id, &mesh.vertices, &mesh.indices, positions, tex_coords, normals);
            obj_parse_face(face2, table_faces, &cur_id, &mesh.vertices, &mesh.indices, positions, tex_coords, normals);
            obj_parse_face(face3, table_faces, &cur_id, &mesh.vertices, &mesh.indices, positions, tex_coords, normals);
        }
        break;

        default:
            SKIP_LINE(ch, obj_file);
            break;
        }

    } while (ch != EOF);

    // for (uint i = 0; i < vertices->size; i+=8)
    // {
    //    debug("p: %f %f %f t: %f %f n: %f %f %f", vertices->data[i], vertices->data[i + 1], vertices->data[i + 2], vertices->data[i + 3],
    //                                              vertices->data[i + 4], vertices->data[i + 5], vertices->data[i + 6], vertices->data[i + 7]);
    // }
    // for (uint i = 0; i < indices->size; i++)
    // {
    //    printf("%d ", indices->data[i]);
    // }
    // debug("\n%d %d", vertices->size, indices->size);

    fclose(obj_file);
    return mesh;
}

Mesh generate_sphere(const GLuint num_slices, const GLfloat radius)
{
    Mesh mesh = {};

    GLuint numberParallels = num_slices / 2;
    GLuint numberVertices = (numberParallels + 1u) * (num_slices + 1u);
    GLuint numberIndices = numberParallels * num_slices * 6u;

    vec_new(GLfloat, mesh.vertices, numberVertices);
    vec_new(GLuint, mesh.indices, numberIndices);

    GLfloat angleStep = M_PI * 2.f / num_slices;

    for (GLuint i = 0u; i < numberParallels + 1u; i++)
    {
        for (GLuint j = 0u; j < num_slices + 1u; j++)
        {
            GLuint index = (i * (num_slices + 1u) + j);
            GLfloat x, y, z;
            x = radius * sinf(angleStep * (GLfloat)i) * sinf(angleStep * (GLfloat)j);
            vec_push(mesh.vertices, x);
            y = radius * cosf(angleStep * (GLfloat)i);
            vec_push(mesh.vertices, y);
            z = radius * sinf(angleStep * (GLfloat)i) * cosf(angleStep * (GLfloat)j);
            vec_push(mesh.vertices, z);
            vec_push(mesh.vertices, (GLfloat)j / (GLfloat)num_slices); // tex coords
            vec_push(mesh.vertices, ((GLfloat)i / numberParallels));
            vec_push(mesh.vertices, x / radius); // normals
            vec_push(mesh.vertices, y / radius);
            vec_push(mesh.vertices, z / radius);
        }
    }

    GLuint indexIndices = 0u;
    for (GLuint i = 0u; i < numberParallels; i++)
    {
        for (GLuint j = 0u; j < num_slices; j++)
        {
            vec_push(mesh.indices, i * (num_slices + 1u) + j);
            vec_push(mesh.indices, (i + 1u) * (num_slices + 1u) + j);
            vec_push(mesh.indices, (i + 1u) * (num_slices + 1u) + (j + 1u));

            vec_push(mesh.indices, i * (num_slices + 1u) + j);
            vec_push(mesh.indices, (i + 1u) * (num_slices + 1u) + (j + 1u));
            vec_push(mesh.indices, i * (num_slices + 1u) + (j + 1u));
        }
    }

    return mesh;
}

static inline void fill_vertex(v_GLfloat_t *vertices, v3_t pos, v2_t tex, v3_t norm)
{
    vec_push(*vertices, pos.x);
    vec_push(*vertices, pos.y);
    vec_push(*vertices, pos.z);
    vec_push(*vertices, tex.x);
    vec_push(*vertices, tex.y);
    vec_push(*vertices, norm.x);
    vec_push(*vertices, norm.y);
    vec_push(*vertices, norm.z);
}

Mesh generate_pyramid(const GLfloat base, const GLfloat height)
{
    Mesh mesh = {};
    vec_new(GLfloat, mesh.vertices, 22);
    vec_new(GLuint, mesh.indices, 18);

    v3_t top = {0, height, 0};
    v3_t p0 = {-base / 2, 0, -base / 2};
    v3_t p1 = {base / 2, 0, -base / 2};
    v3_t p2 = {base / 2, 0, base / 2};
    v3_t p3 = {-base / 2, 0, base / 2};
    v3_t normSide0 = v3_norm(v3_cross(v3_sub(top, p3), v3_sub(p0, p3)));
    v3_t normSide1 = v3_norm(v3_cross(v3_sub(top, p0), v3_sub(p1, p0)));
    v3_t normSide2 = v3_norm(v3_cross(v3_sub(top, p1), v3_sub(p2, p1)));
    v3_t normSide3 = v3_norm(v3_cross(v3_sub(top, p2), v3_sub(p3, p2)));
    v3_t normBottom = v3_norm(v3_cross(v3_sub(p3, p0), v3_sub(p1, p0)));

    fill_vertex(&mesh.vertices, p0, (v2_t){1, 1}, normSide0);
    fill_vertex(&mesh.vertices, p3, (v2_t){0, 1}, normSide0);
    fill_vertex(&mesh.vertices, top, (v2_t){0.5, 0}, normSide0);

    fill_vertex(&mesh.vertices, p1, (v2_t){1, 1}, normSide1);
    fill_vertex(&mesh.vertices, p0, (v2_t){0, 1}, normSide1);
    fill_vertex(&mesh.vertices, top, (v2_t){0.5, 0}, normSide1);

    fill_vertex(&mesh.vertices, p2, (v2_t){1, 1}, normSide2);
    fill_vertex(&mesh.vertices, p1, (v2_t){0, 1}, normSide2);
    fill_vertex(&mesh.vertices, top, (v2_t){0.5, 0}, normSide2);

    fill_vertex(&mesh.vertices, p3, (v2_t){1, 1}, normSide3);
    fill_vertex(&mesh.vertices, p2, (v2_t){0, 1}, normSide3);
    fill_vertex(&mesh.vertices, top, (v2_t){0.5, 0}, normSide3);

    fill_vertex(&mesh.vertices, p0, (v2_t){1, 0}, normBottom);
    fill_vertex(&mesh.vertices, p1, (v2_t){0, 0}, normBottom);
    fill_vertex(&mesh.vertices, p2, (v2_t){0, 1}, normBottom);
    fill_vertex(&mesh.vertices, p0, (v2_t){1, 0}, normBottom);
    fill_vertex(&mesh.vertices, p2, (v2_t){0, 1}, normBottom);
    fill_vertex(&mesh.vertices, p3, (v2_t){1, 1}, normBottom);

    for (int i = 0; i < 18; i++)
        vec_push(mesh.indices, i);

    return mesh;
}

// CW order
Mesh generate_plane(v3_t p0, v3_t p1, v3_t p2, v3_t p3)
{
    Mesh mesh = {};
    vec_new(GLfloat, mesh.vertices, 4);
    vec_new(GLuint, mesh.indices, 6);

    fill_vertex(&mesh.vertices, p0, (v2_t){0, 0}, (v3_t){0});
    fill_vertex(&mesh.vertices, p1, (v2_t){p1.x, 0}, (v3_t){0});
    fill_vertex(&mesh.vertices, p2, (v2_t){p2.x, p2.z}, (v3_t){0});
    fill_vertex(&mesh.vertices, p3, (v2_t){0, p3.z}, (v3_t){0});
    vec_push(mesh.indices, 0);
    vec_push(mesh.indices, 3);
    vec_push(mesh.indices, 1);
    vec_push(mesh.indices, 1);
    vec_push(mesh.indices, 3);
    vec_push(mesh.indices, 2);

    return mesh;
}

Mesh generate_hyperboloid(float32 height, float32 a, float32 b, uint offset, uint num_vertices)
{
    Mesh mesh = {};
    vec_new(GLfloat, mesh.vertices, (num_vertices + 2) * 8);   // +2 centers of circles
    vec_new(GLuint, mesh.indices, 3 * (2 * num_vertices - 2)); // +2 center triangles of circles
    float32 zerof = 0;
    float32 phi = 0;
    float32 d_phi = 2 * M_PI / (num_vertices - 0);
    for (uint i = 0; i < 8; i++) // push center 1 coordinates
        vec_push(mesh.vertices, 0);
    for (uint i = 0; i < 8; i++) // push center 2 coordinates
        vec_push(mesh.vertices, 0);
    mesh.vertices.data[2] = height;
    mesh.vertices.data[8 + 2] = -height;
    for (int c = 0; c <= 3; c++)
    {
        phi = 0;
        for (uint i = 0; i <= num_vertices; phi += d_phi, i++)
        {
            float32 x = a * cosf(phi);
            vec_push(mesh.vertices, x);
            float32 y = b * sinf(phi);
            vec_push(mesh.vertices, y);
            vec_push(mesh.vertices, height);
            if (c > 1)
            {
                vec_push(mesh.vertices, 0);
                vec_push(mesh.vertices, 0);
                vec_push(mesh.vertices, 0);
                vec_push(mesh.vertices, 0);
                vec_push(mesh.vertices, 0);
            }
            else
            {
                float32 uv[2] = {};
                uint n = num_vertices;
                if (height > 0)
                {
                    uv[0] = 1 - (float32)((i + offset)) / n;
                    if (uv[0] < 0)
                        uv[0] += 1;
                    uv[1] = 1;
                }
                else
                {
                    uv[0] = 1 - (float32)(i) / n;
                    if (uv[0] < 0)
                        uv[0] += 1;
                    uv[1] = 0;
                }
                vec_push(mesh.vertices, uv[0]);
                vec_push(mesh.vertices, uv[1]);
                vec_push(mesh.vertices, 0);
                vec_push(mesh.vertices, 0);
                vec_push(mesh.vertices, 0);
            }
        }
        height *= -1.f;
    }
    num_vertices += 1;
    for (uint i = 2; i < num_vertices + 2; i++)
    {
        uint j = i + num_vertices + offset;
        if (j >= 2 * num_vertices + 2)
            j -= num_vertices;
        uint i_inc = i + 1;
        uint j_inc = j + 1;
        if (i_inc >= num_vertices + 2)
            i_inc = 2;
        if (j_inc >= 2 * num_vertices + 2)
            j_inc = num_vertices + 2;
        vec_push(mesh.indices, i);
        vec_push(mesh.indices, i_inc);
        vec_push(mesh.indices, j);
        vec_push(mesh.indices, j);
        vec_push(mesh.indices, i_inc);
        vec_push(mesh.indices, j_inc);
    }
    uint center = 0;
    for (uint i = 2 * num_vertices + 2; i < 3 * num_vertices + 2; i++)
    {
        vec_push(mesh.indices, i);
        uint tmp = i + 1;
        if (tmp >= 3 * num_vertices + 2)
            tmp = 2 * num_vertices + 2;
        vec_push(mesh.indices, tmp);
        vec_push(mesh.indices, center);
    }
    center = 1;
    for (uint i = 3 * num_vertices + 2; i < 4 * num_vertices + 2; i++)
    {
        vec_push(mesh.indices, i);
        vec_push(mesh.indices, center);
        uint tmp = i + 1;
        if (tmp >= 4 * num_vertices + 2)
            tmp = 3 * num_vertices + 2;
        vec_push(mesh.indices, tmp);
    }
    //for (uint i = 0; i < vec_get_size(mesh.vertices); i+=8)
    //{
    //    debug("%d. p: %f %f %f t: %f %f n: %f %f %f", i / 8, mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2], mesh.vertices[i + 3],
    //                                              mesh.vertices[i + 4], mesh.vertices[i + 5], mesh.vertices[i + 6], mesh.vertices[i + 7]);
    //}
    //
    //for (uint i = 0; i < vec_get_size(mesh.indices) / 3; i+=3)
    //{
    //    printf("%d. %d ", i / 3, mesh.indices[i]);
    //    printf("%d ", mesh.indices[i + 1]);
    //    printf("%d\n", mesh.indices[i + 2]);
    //}
    //debug("\n%d %d", vec_get_size(mesh.vertices), vec_get_size(mesh.indices));
    return mesh;
}