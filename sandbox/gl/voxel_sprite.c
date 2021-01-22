#include "voxel_sprite.h"

typedef uint64_t depth_t;
#define DEPTH_LEVELS (sizeof(depth_t) * CHAR_BIT)

static void generate_voxel_side(v_GLfloat_t *vertices, v_GLuint_t *indices, v3_t p0, v3_t p1, v3_t p2, v3_t p3, v3_t normal, v2_t tex_coords, int cur_idx)
{
    GLfloat vx[] = {
        p0.x, p0.y, p0.z, tex_coords.x, tex_coords.y, normal.x, normal.y, normal.z,
        p1.x, p1.y, p1.z, tex_coords.x, tex_coords.y, normal.x, normal.y, normal.z,
        p2.x, p2.y, p2.z, tex_coords.x, tex_coords.y, normal.x, normal.y, normal.z,
        p3.x, p3.y, p3.z, tex_coords.x, tex_coords.y, normal.x, normal.y, normal.z};
    GLuint ind[] = {
        0, 1, 2, 2, 1, 3};
    for (uint i = 0; i < ARR_ONE_DIM_LEN(ind); i++)
    {
        vec_push(*indices, cur_idx + ind[i]);
    }
    for (uint i = 0; i < ARR_ONE_DIM_LEN(vx); i++)
    {
        vec_push(*vertices, vx[i]);
    }
}

static inline depth_t depth_correct(depth_t *depth, depth_t d_test,
                                    int base_x, int base_y, int base_w, int base_h,
                                    int test_x, int test_y)
{
    if ((test_x) >= 0 && (test_y) >= 0 && (test_x < base_w) && (test_y < base_h))
    {
        d_test &= ~depth[((test_y)*base_w) + (test_x)];
    }
    return d_test;
}

static inline void generate_voxel_face(v_GLfloat_t *vertices, v_GLuint_t *indices,
                                       int base_x, int base_y, int base_w, int base_h,
                                       int s_x, int s_y, int e_x, int e_y, int w, int h,
                                       v2_t tex_coords, depth_t depth,
                                       int *cur_idx, char direction)
{
    float32 p_x0, p_x1, p_y0, p_y1;
    v3_t normal;
    switch (direction)
    {
    case 'w':
        p_x0 = 0;
        p_x1 = 0;
        p_y0 = s_y;
        p_y1 = e_y;
        normal = (v3_t){-1, 0, 0};
        break;
    case 'n':
        p_x0 = e_x;
        p_x1 = s_x;
        p_y0 = 0;
        p_y1 = 0;
        normal = (v3_t){0, 1, 0};
        break;
    case 'e':
        p_x0 = w;
        p_x1 = w;
        p_y0 = e_y;
        p_y1 = s_y;
        normal = (v3_t){1, 0, 0};
        break;
    case 's':
        p_x0 = s_x;
        p_x1 = e_x;
        p_y0 = h;
        p_y1 = h;
        normal = (v3_t){0, -1, 0};
        break;
    }
    int z_value = -1;
    for (int j = 0; j < DEPTH_LEVELS; j++)
    {
        if (depth & (1ULL << (depth_t)j))
        {
            if (z_value == -1)
                z_value = j;
        }
        else
        {
            if (z_value != -1)
            {
                v3_t p0 = (v3_t){(base_x + p_x0), base_h - (base_y + p_y0), z_value};
                v3_t p1 = (v3_t){(base_x + p_x1), base_h - (base_y + p_y1), z_value};
                v3_t p2 = (v3_t){(base_x + p_x0), base_h - (base_y + p_y0), j};
                v3_t p3 = (v3_t){(base_x + p_x1), base_h - (base_y + p_y1), j};
                generate_voxel_side(vertices, indices, p0, p1, p2, p3, normal, tex_coords, *cur_idx);
                *cur_idx += 4;
                z_value = -1;
            }
        }
    }
}

static void voxel_sprite_generate(v_GLfloat_t *vertices, v_GLuint_t *indices, const char *sprite_path, const char *depthmap_path, bool has_back, v3_t voxel_dimensions)
{

    int sprite_w, sprite_h;
    int nrComponents;
    unsigned char *sprite = stbi_load(sprite_path, &sprite_w, &sprite_h, &nrComponents, 4);
    unsigned char *sprite_back = NULL;
    if (!sprite)
    {
        error("Cannot open sprite %s", sprite_path);
    }

    sprite_h = has_back == true ? sprite_h / 2 : sprite_h;
    const int sprite_size = sprite_w * sprite_h;
    bool is_conquered[sprite_size];
    memset(is_conquered, 0, sprite_size);
    depth_t depth[sprite_size];
    memset(depth, 0, sprite_size);
    color *pixels = (color *)(sprite);
    unsigned char r = 0, g = 0, b = 0, a = 0;
    depth_t cur_depth = 0;
    int cur_idx = 0;
    if (depthmap_path)
    {
        FILE *depthmap = fopen(depthmap_path, "rb");
        if (!depthmap)
            error("Cannot open depthmap %s", depthmap_path);
        fread(depth, sizeof(depth_t) * sprite_size, 1, depthmap);
        fclose(depthmap);
    }
    else
    {
        for (int i = 0; i < sprite_size; i++)
        {
            depth[i] |= (1ULL << 31ULL) | (1ULL << 32ULL);
        }
    }

    for (int i_y = 0; i_y < sprite_h; i_y++)
    {
        for (int i_x = 0; i_x < sprite_w; i_x++)
        {
            const int i = i_y * sprite_w + i_x;
            r = pixels[i].r;
            g = pixels[i].g;
            b = pixels[i].b;
            a = pixels[i].a;
            cur_depth = depth[i];
            if (is_conquered[i] || (a == 0))
                continue;
            bool pixels_left = true;
            int w = 1, h = 1;
            while (1)
            {
                bool is_width_grew = true;
                for (int j = 0; j < h; j++)
                {
                    is_width_grew &= (r == pixels[i + w + (j * sprite_w)].r) &&
                                     (g == pixels[i + w + (j * sprite_w)].g) &&
                                     (b == pixels[i + w + (j * sprite_w)].b) &&
                                     (a == pixels[i + w + (j * sprite_w)].a) &&
                                     (cur_depth == depth[i + w + (j * sprite_w)]);
                }
                int test_w = w + (is_width_grew == true);
                bool is_height_grew = true;
                for (int j = 0; j < test_w; j++)
                {
                    is_height_grew &= (r == pixels[i + j + ((h)*sprite_w)].r) &&
                                      (g == pixels[i + j + ((h)*sprite_w)].g) &&
                                      (b == pixels[i + j + ((h)*sprite_w)].b) &&
                                      (a == pixels[i + j + ((h)*sprite_w)].a) &&
                                      (cur_depth == depth[i + j + ((h)*sprite_w)]);
                }
                h += (is_height_grew == true);
                w = test_w;

                if (!is_width_grew && !is_height_grew)
                {
                    depth_t segment_depth[w * h];
                    v2_t segment_tex_coords = {.x = ((float32)i_x + 0.5) / (float32)(sprite_w) + .0f,
                                               .y = ((float32)i_y + 0.5) / (float32)(sprite_h * (has_back ? 2 : 1)) + .0f};
                    for (int k = 0; k < h; k++)
                    {
                        for (int l = 0; l < w; l++)
                        {
                            segment_depth[k * w + l] = depth[(i_y + k) * sprite_w + (i_x + l)];
                        }
                    }
                    //test frame adjacent pixels, opposite frame direction
                    int prev_x = 0, prev_y = 0;
                    depth_t d_test = segment_depth[prev_y * w + prev_x];
                    depth_t prev_depth = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x - 1, i_y);
                    for (int y = 1; (y <= h); y++) // west faces
                    {
                        depth_t d = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x - 1, i_y + y);
                        if (d != prev_depth || y == h)
                        {
                            generate_voxel_face(vertices, indices, i_x, i_y, sprite_w, sprite_h, prev_x, prev_y, 0, y, w, h, segment_tex_coords, prev_depth, &cur_idx, 'w');
                            prev_depth = d;
                            prev_y = y;
                        }
                    }
                    prev_x = 0, prev_y = 0;
                    d_test = segment_depth[prev_y * w + prev_x];
                    prev_depth = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x, i_y - 1);
                    for (int x = 1; (x <= w); x++) // north faces
                    {
                        depth_t d = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x + x, i_y - 1);
                        if (d != prev_depth || x == w)
                        {
                            generate_voxel_face(vertices, indices, i_x, i_y, sprite_w, sprite_h, prev_x, prev_y, x, 0, w, h, segment_tex_coords, prev_depth, &cur_idx, 'n');
                            prev_depth = d;
                            prev_x = x;
                        }
                    }
                    prev_x = w - 1, prev_y = 0;
                    prev_depth = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x + w, i_y);
                    for (int y = 1; (y <= h); y++) // east faces
                    {
                        depth_t d = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x + w, i_y + y);
                        if (d != prev_depth || y == h)
                        {
                            generate_voxel_face(vertices, indices, i_x, i_y, sprite_w, sprite_h, prev_x, prev_y, w, y, w, h, segment_tex_coords, prev_depth, &cur_idx, 'e');
                            prev_depth = d;
                            prev_y = y;
                        }
                    }
                    prev_x = 0, prev_y = h - 1;
                    prev_depth = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x, i_y + h);
                    for (int x = 1; (x <= w); x++) // south faces
                    {
                        depth_t d = depth_correct(depth, d_test, i_x, i_y, sprite_w, sprite_h, i_x + x, i_y + h);
                        if (d != prev_depth || x == w)
                        {
                            generate_voxel_face(vertices, indices, i_x, i_y, sprite_w, sprite_h, prev_x, prev_y, x, h, w, h, segment_tex_coords, prev_depth, &cur_idx, 's');
                            prev_depth = d;
                            prev_x = x;
                        }
                    }
                    // generate flip-flop faces (+- z faces)
                    depth_t d = segment_depth[0];
                    bool prev_bit = false;
                    for (int j = 0; j <= DEPTH_LEVELS; j++)
                    {
                        bool bit = d & (1ULL << (depth_t)j) ? true : false;
                        if (prev_bit == false && bit == true)
                        {
                            // -z
                            v2_t tex_coords = (v2_t){segment_tex_coords.x,
                                                     segment_tex_coords.y + 0.5f * (has_back ? 1 : 0)};
                            v3_t p0 = (v3_t){(i_x + 0), sprite_h - (i_y + 0), j};
                            v3_t p1 = (v3_t){(i_x + w), sprite_h - (i_y + 0), j};
                            v3_t p2 = (v3_t){(i_x + 0), sprite_h - (i_y + h), j};
                            v3_t p3 = (v3_t){(i_x + w), sprite_h - (i_y + h), j};
                            generate_voxel_side(vertices, indices, p0, p1, p2, p3, (v3_t){0, 0, -1.f}, tex_coords, cur_idx);
                            cur_idx += 4;
                        }
                        else if (prev_bit == true && (bit == false || j == DEPTH_LEVELS))
                        {
                            // +z
                            v3_t p0 = (v3_t){(i_x + 0), sprite_h - (i_y + h), j};
                            v3_t p1 = (v3_t){(i_x + w), sprite_h - (i_y + h), j};
                            v3_t p2 = (v3_t){(i_x + 0), sprite_h - (i_y + 0), j};
                            v3_t p3 = (v3_t){(i_x + w), sprite_h - (i_y + 0), j};
                            generate_voxel_side(vertices, indices, p0, p1, p2, p3, (v3_t){0, 0, 1.f}, segment_tex_coords, cur_idx);
                            cur_idx += 4;
                        }
                        prev_bit = bit;
                    }
                    for (int k = 0; k < h; k++)
                    {
                        for (int l = 0; l < w; l++)
                        {
                            is_conquered[(i_y + k) * sprite_w + (i_x + l)] = true;
                        }
                    }
                    is_conquered[i_y * sprite_w + i_x] = 7;
                    break;
                }
            }
        }
    }
    // for (int k = 0; k < sprite_h; k++)
    // {
    //     for (int l = 0; l < sprite_w; l++)
    //     {
    //         printf("%d ", is_conquered[(k * sprite_w) + l]);
    //     }
    //     printf("\n");
    // }
    // printf("Veritces count: %d Triangle count: %d\n", vertices->size / 8, cur_idx / 4);

    for (uint i = 0; i < vertices->size; i += 8)
    {
        vertices->data[i + 0] -= sprite_w / 2;
        vertices->data[i + 0] *= voxel_dimensions.x;
        vertices->data[i + 1] -= sprite_h / 2;
        vertices->data[i + 1] *= voxel_dimensions.y;
        vertices->data[i + 2] -= DEPTH_LEVELS / 2;
        vertices->data[i + 2] *= voxel_dimensions.z;
        //vertices->data[i + 3] /= w;
        //vertices->data[i + 4] /= h;
        //vertices->data[i + 5] /= 0xFF;
    }
    stbi_image_free(sprite);
}

Model voxel_sprite_construct(const char *sprite_path, const char *depthmap_path, bool has_back, v3_t voxel_dimensions)
{
    Model vs;
    v_GLfloat_t vertices;
    vec_new(GLfloat, vertices, 4096);
    v_GLuint_t indices;
    vec_new(GLuint, indices, 6144);
    voxel_sprite_generate(&vertices, &indices, sprite_path, depthmap_path, has_back, voxel_dimensions);
    Mesh mesh;
    mesh.vertices = vertices;
    mesh.indices = indices;
    vs = model_build(mesh);
    return vs;
}