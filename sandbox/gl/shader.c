#include "shader.h"

// Graphics functions
static char *shader_load(const char *path)
{
    FILE *shader_file = NULL;
    char *shader_stream = NULL;
    shader_file = fopen(path, "r");

    if (shader_file)
    {
        fseek(shader_file, 0, SEEK_END);
        uint length = ftell(shader_file);
        fseek(shader_file, 0, SEEK_SET);
        shader_stream = malloc((length + 1) * sizeof(char));
        if (shader_stream)
        {
            fread(shader_stream, 1, length, shader_file);
        }
        shader_stream[length] = 0;
        fclose(shader_file);
        return shader_stream;
    }
    else
    {
        error("Impossible to open %s!\n", path);
        return 0;
    }
}

static void check_error(GLuint id)
{
    GLint result;
    int log_len;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_len);
    if (log_len > 0)
    {
        char error_message[log_len + 1];
        glGetShaderInfoLog(id, log_len, NULL, &error_message[0]);
        error("\nStatus: %u\nMessage: %s", result, error_message);
    }
}

static void shader_compile(const char *path, GLuint id, char const *stream)
{
    printf("Compiling shader : %s\n", path);
    glShaderSource(id, 1, &stream, NULL);
    glCompileShader(id);

    check_error(id);
}

Shader shader_init_str(const char *vertex_text, const char* fragment_text)
{
    Shader shader = {};
    // Create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    debug("Compiling shaders");
    shader_compile("VS", vertex_shader_id, vertex_text);
    shader_compile("FS", fragment_shader_id, fragment_text);

    // Link the program
    debug("Linking program");
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // Check the program
    check_error(program_id);

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    shader.program = program_id;
    vec_new(Uniform, shader.uniforms, 4);
    vec_new(GLuint, shader.samplers_location, 4);
    return shader;
}

Shader shader_init(const char *vertex_file_path, const char *fragment_file_path)
{
    debug("Loading shaders from file");
    char const *vertex_shader_stream = shader_load(vertex_file_path);
    char const *fragment_shader_stream = shader_load(fragment_file_path);

    return shader_init_str(vertex_shader_stream, fragment_shader_stream);
}