#include "SimpleEngineCore/Application.hpp"
#include "SimpleEngineCore/Log.hpp"
#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Event.hpp"
#include "SimpleEngineCore/Input.hpp"

#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Texture2D.hpp"
#include "SimpleEngineCore/Camera.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "SimpleEngineCore/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

namespace SimpleEngine {

    GLfloat pos_norm_uv[] = {
        //    position             normal            UV                  index

        // FRONT
        -1.0f, -1.f, -1.f,    -1.f,  0.f,  0.f,     0.f, 0.f,              // 0
        -1.0f,  1.f, -1.f,    -1.f,  0.f,  0.f,     1.f, 0.f,              // 1
        -1.0f,  1.f,  1.f,    -1.f,  0.f,  0.f,     1.f, 1.f,              // 2
        -1.0f, -1.f,  1.f,    -1.f,  0.f,  0.f,     0.f, 1.f,              // 3

        // BACK                                  
         1.0f, -1.f, -1.f,     1.f,  0.f,  0.f,     1.f, 0.f,              // 4
         1.0f,  1.f, -1.f,     1.f,  0.f,  0.f,     0.f, 0.f,              // 5
         1.0f,  1.f,  1.f,     1.f,  0.f,  0.f,     0.f, 1.f,              // 6
         1.0f, -1.f,  1.f,     1.f,  0.f,  0.f,     1.f, 1.f,              // 7

        // RIGHT
        -1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     0.f, 0.f,              // 8
         1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     1.f, 0.f,              // 9
         1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,              // 10
        -1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     0.f, 1.f,              // 11

        // LEFT
        -1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     1.f, 0.f,              // 12
         1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     0.f, 0.f,              // 13
         1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     0.f, 1.f,              // 14
        -1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     1.f, 1.f,              // 15

        // TOP
        -1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 0.f,              // 16
        -1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 0.f,              // 17
         1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 1.f,              // 18
         1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 1.f,              // 19

         // BOTTOM
         -1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 1.f,              // 20
         -1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 1.f,              // 21
          1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 0.f,              // 22
          1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 0.f,              // 23
    };


    GLuint indices[] = {
        0,   1,  2,  2,  3,  0, // front
        4,   5,  6,  6,  7,  4, // back
        8,   9, 10, 10, 11,  8, // right
        12, 13, 14, 14, 15, 12, // left
        16, 17, 18, 18, 19, 16, // top
        20, 21, 22, 22, 23, 20  // bottom
    };

    void generate_circle(unsigned char* data,
                         const unsigned int width,
                         const unsigned int height,
                         const unsigned int center_x,
                         const unsigned int center_y,
                         const unsigned int radius,
                         const unsigned char color_r,
                         const unsigned char color_g,
                         const unsigned char color_b)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
                {
                    data[3 * (x + width * y) + 0] = color_r;
                    data[3 * (x + width * y) + 1] = color_g;
                    data[3 * (x + width * y) + 2] = color_b;
                }
            }
        }
    }

    void generate_smile_texture(unsigned char* data,
                                const unsigned int width,
                                const unsigned int height)
    {
        // background
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                data[3 * (x + width * y) + 0] = 200;
                data[3 * (x + width * y) + 1] = 191;
                data[3 * (x + width * y) + 2] = 231;
            }
        }

        // face
        generate_circle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

        // smile
        generate_circle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
        generate_circle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

        // eyes
        generate_circle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
        generate_circle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
    }

    void generate_quads_texture(unsigned char* data,
                                const unsigned int width,
                                const unsigned int height)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            for (unsigned int y = 0; y < height; ++y)
            {
                if ((x < width / 2 && y < height / 2) || x >= width / 2 && y >= height / 2)
                {
                    data[3 * (x + width * y) + 0] = 0;
                    data[3 * (x + width * y) + 1] = 0;
                    data[3 * (x + width * y) + 2] = 0;
                }
                else
                {
                    data[3 * (x + width * y) + 0] = 255;
                    data[3 * (x + width * y) + 1] = 255;
                    data[3 * (x + width * y) + 2] = 255;
                }
            }
        }
    }

    const char* vertex_shader =
        R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_normal;
           layout(location = 2) in vec2 texture_coord;

           uniform mat4 model_view_matrix;
           uniform mat4 mvp_matrix;
           uniform mat3 normal_matrix;
           uniform int current_frame; 

           out vec2 tex_coord_smile;
           out vec2 tex_coord_quads;
           out vec3 frag_position_eye;
           out vec3 frag_normal_eye;

           void main() {
              tex_coord_smile = texture_coord;
              tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);
              frag_normal_eye = normal_matrix * vertex_normal;
              frag_position_eye = vec3(model_view_matrix * vec4(vertex_position, 1.0));
              gl_Position = mvp_matrix * vec4(vertex_position, 1.0);
           }
        )";

    const char* fragment_shader =
        R"(#version 460
           in vec2 tex_coord_smile;
           in vec2 tex_coord_quads;
           in vec3 frag_position_eye;
           in vec3 frag_normal_eye;

           layout (binding = 0) uniform sampler2D InTexture_Smile;
           layout (binding = 1) uniform sampler2D InTexture_Quads;

           uniform vec3 light_position_eye;
           uniform vec3 light_color;
           uniform float ambient_factor;
           uniform float diffuse_factor;
           uniform float specular_factor;
           uniform float shininess;

           out vec4 frag_color;

           void main() {

              // ambient
              vec3 ambient = ambient_factor * light_color;

              // diffuse
              vec3 normal = normalize(frag_normal_eye);
              vec3 light_dir = normalize(light_position_eye - frag_position_eye);
              vec3 diffuse = diffuse_factor * light_color * max(dot(normal, light_dir), 0.0);

              // specular
              vec3 view_dir = normalize(-frag_position_eye);
              vec3 reflect_dir = reflect(-light_dir, normal);
              float specular_value = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
              vec3 specular = specular_factor * specular_value * light_color;

              //frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);
              frag_color = texture(InTexture_Smile, tex_coord_smile) * vec4(ambient + diffuse + specular, 1.f);
           }
        )";

    const char* light_source_vertex_shader =
        R"(#version 460
           layout(location = 0) in vec3 vertex_position;
           layout(location = 1) in vec3 vertex_normal;
           layout(location = 2) in vec2 texture_coord;

           uniform mat4 mvp_matrix;

           void main() {
              gl_Position = mvp_matrix * vec4(vertex_position * 0.1f, 1.0);
           }
        )";

    const char* light_source_fragment_shader =
        R"(#version 460
           out vec4 frag_color;

           uniform vec3 light_color;

           void main() {
              frag_color = vec4(light_color, 1.f);
           }
        )";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<ShaderProgram> p_light_source_shader_program;
    std::unique_ptr<VertexBuffer> p_cube_positions_vbo;
    std::unique_ptr<IndexBuffer> p_cube_index_buffer;
    std::unique_ptr<Texture2D> p_texture_smile;
    std::unique_ptr<Texture2D> p_texture_quads;
    std::unique_ptr<VertexArray> p_cube_vao;
    float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };

    std::array<glm::vec3, 5> positions = {
            glm::vec3(-2.f, -2.f, -4.f),
            glm::vec3(-5.f,  0.f,  3.f),
            glm::vec3( 2.f,  1.f, -2.f),
            glm::vec3( 4.f, -3.f,  3.f),
            glm::vec3( 1.f, -7.f,  1.f)
    };

    Application::Application()
    {
        LOG_INFO("Starting Application");
    }


    Application::~Application()
    {
        LOG_INFO("Closing Application");
    }

    void Application::draw()
    {
        Renderer_OpenGL::set_clear_color(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        Renderer_OpenGL::clear();

        p_shader_program->bind();

        //glm::mat4 scale_matrix(scale[0], 0, 0, 0,
        //    0, scale[1], 0, 0,
        //    0, 0, scale[2], 0,
        //    0, 0, 0, 1);

        //float rotate_in_radians = glm::radians(rotate);
        //glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
        //    -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
        //    0, 0, 1, 0,
        //    0, 0, 0, 1);

        //glm::mat4 translate_matrix(1, 0, 0, 0,
        //    0, 1, 0, 0,
        //    0, 0, 1, 0,
        //    translate[0], translate[1], translate[2], 1);

        //glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
        //p_shader_program->set_matrix4("model_matrix", model_matrix);


        static int current_frame = 0;
        p_shader_program->set_int("current_frame", current_frame++);
        p_shader_program->set_vec3("light_position_eye", glm::vec3(camera.get_view_matrix() * glm::vec4(light_source_position[0], light_source_position[1], light_source_position[2], 1.f)));
        p_shader_program->set_vec3("light_color", glm::vec3(light_source_color[0], light_source_color[1], light_source_color[2]));
        p_shader_program->set_float("ambient_factor", ambient_factor);
        p_shader_program->set_float("diffuse_factor", diffuse_factor);
        p_shader_program->set_float("specular_factor", specular_factor);
        p_shader_program->set_float("shininess", shininess);

        // cubes
        for (const glm::vec3& current_position : positions)
        {
            glm::mat4 translate_matrix(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                current_position[0], current_position[1], current_position[2], 1);
            const glm::mat4 model_view_matrix = camera.get_view_matrix()* translate_matrix;
            p_shader_program->set_matrix4("model_view_matrix", model_view_matrix);
            p_shader_program->set_matrix4("mvp_matrix", camera.get_projection_matrix() * model_view_matrix);
            p_shader_program->set_matrix3("normal_matrix", glm::transpose(glm::inverse(glm::mat3(model_view_matrix))));
            Renderer_OpenGL::draw(*p_cube_vao);
        }

        // light source
        {
            p_light_source_shader_program->bind();
            glm::mat4 translate_matrix(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                light_source_position[0], light_source_position[1], light_source_position[2], 1);
            p_light_source_shader_program->set_matrix4("mvp_matrix", camera.get_projection_matrix() * camera.get_view_matrix() * translate_matrix);
            p_light_source_shader_program->set_vec3("light_color", glm::vec3(light_source_color[0], light_source_color[1], light_source_color[2]));
            Renderer_OpenGL::draw(*p_cube_vao);
        }

        UIModule::on_ui_draw_begin();
        on_ui_draw();
        UIModule::on_ui_draw_end();

        m_pWindow->on_update();
        on_update();
    }

    int Application::start(unsigned int window_width, unsigned int window_height, const char* title)
    {
        m_pWindow = std::make_unique<Window>(title, window_width, window_height);
        camera.set_viewport_size(static_cast<float>(window_width), static_cast<float>(window_height));

        m_event_dispatcher.add_event_listener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            });

        m_event_dispatcher.add_event_listener<EventWindowResize>(
            [&](EventWindowResize& event)
            {
                LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
                camera.set_viewport_size(static_cast<float>(event.width), static_cast<float>(event.height));
                draw();
            });

        m_event_dispatcher.add_event_listener<EventWindowClose>(
            [&](EventWindowClose& event)
            {
                LOG_INFO("[WindowClose]");
                close();
            });

        m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
            [&](EventMouseButtonPressed& event)
            {
                LOG_INFO("[Mouse button pressed: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::PressMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
            });

        m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
            [&](EventMouseButtonReleased& event)
            {
                LOG_INFO("[Mouse button released: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::ReleaseMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
            });

        m_event_dispatcher.add_event_listener<EventKeyPressed>(
            [&](EventKeyPressed& event)
            {
                if (event.key_code <= KeyCode::KEY_Z)
                {
                    if (event.repeated)
                    {
                        LOG_INFO("[Key pressed: {0}, repeated", static_cast<char>(event.key_code));
                    }
                    else
                    {
                        LOG_INFO("[Key pressed: {0}", static_cast<char>(event.key_code));
                    }
                }
                Input::PressKey(event.key_code);
            });

        m_event_dispatcher.add_event_listener<EventKeyReleased>(
            [&](EventKeyReleased& event)
            {
                if (event.key_code <= KeyCode::KEY_Z)
                {
                    LOG_INFO("[Key released: {0}", static_cast<char>(event.key_code));
                }
                Input::ReleaseKey(event.key_code);
            });

        m_pWindow->set_event_callback(
            [&](BaseEvent& event)
            {
                m_event_dispatcher.dispatch(event);
            }
        );
        
        const unsigned int width = 1000;
        const unsigned int height = 1000;
        const unsigned int channels = 3;
        auto* data = new unsigned char[width * height * channels];

        generate_smile_texture(data, width, height);
        p_texture_smile = std::make_unique<Texture2D>(data, width, height);
        p_texture_smile->bind(0);

        generate_quads_texture(data, width, height);
        p_texture_quads = std::make_unique<Texture2D>(data, width, height);
        p_texture_quads->bind(1);

        delete[] data;

        //---------------------------------------//
        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->is_compiled())
        {
            return false;
        }

        BufferLayout buffer_layout_1vec3
        {
            ShaderDataType::Float3
        };


        BufferLayout buffer_layout_vec3_vec3_vec2
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };

        p_cube_vao = std::make_unique<VertexArray>();
        p_cube_positions_vbo = std::make_unique<VertexBuffer>(pos_norm_uv, sizeof(pos_norm_uv), buffer_layout_vec3_vec3_vec2);
        p_cube_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        p_cube_vao->add_vertex_buffer(*p_cube_positions_vbo);
        p_cube_vao->set_index_buffer(*p_cube_index_buffer);
        //---------------------------------------//

        p_light_source_shader_program = std::make_unique<ShaderProgram>(light_source_vertex_shader, light_source_fragment_shader);
        if (!p_light_source_shader_program->is_compiled())
        {
            return false;
        }

        Renderer_OpenGL::enable_depth_test();
        while (!m_bCloseWindow)
        {
            draw();
        }

        m_pWindow = nullptr;

        return 0;
    }

    glm::vec2 Application::get_current_cursor_position() const
    {
        return m_pWindow->get_current_cursor_position();
    }

    void Application::close()
    {
        m_bCloseWindow = true;
    }
}