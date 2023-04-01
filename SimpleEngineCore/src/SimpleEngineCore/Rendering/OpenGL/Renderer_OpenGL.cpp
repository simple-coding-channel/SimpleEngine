#include "Renderer_OpenGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "SimpleEngineCore/Log.hpp"


namespace SimpleEngine {

    const char* gl_source_to_string(const GLenum source)
    {
        switch (source)
        {
            case GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "GL_DEBUG_SOURCE_SHADER_COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";

            default: return "UNKNOWN_DEBUG_SOURCE";
        }
    }

    const char* gl_type_to_string(const GLenum type)
    {
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
            case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
            case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
            case GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";

            default: return "UNKNOWN_DEBUG_TYPE";
        }
    }

    bool Renderer_OpenGL::init(GLFWwindow* pWindow)
    {
        glfwMakeContextCurrent(pWindow);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            LOG_CRITICAL("Failed to initialize GLAD");
            return false;
        }

        LOG_INFO("OpenGL context initialized:");
        LOG_INFO("  OpenGL Vendor: {0}", get_vendor_str());
        LOG_INFO("  OpenGL Renderer: {0}", get_renderer_str());
        LOG_INFO("  OpenGL Version: {0}", get_version_str());

        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        glDebugMessageCallback([](GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar* message,
                                  const void* userParam)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
                LOG_ERROR("OpenGL Error: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                LOG_WARN("OpenGL Warning: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                LOG_INFO("OpenGL Info: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                LOG_INFO("OpenGL Notificaton: [{0}:{1}]({2}): {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
                break;
            default:
                LOG_ERROR("OpenGL Error: [{0}:{1}] ({2}) : {3}",
                    gl_source_to_string(source),
                    gl_type_to_string(type),
                    id,
                    message);
            }
        }, nullptr);

        return true;
    }

    void Renderer_OpenGL::draw(const VertexArray& vertex_array)
    {
        vertex_array.bind();
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertex_array.get_indices_count()), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer_OpenGL::set_clear_color(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void Renderer_OpenGL::clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer_OpenGL::set_viewport(const unsigned int width, const unsigned int height, const unsigned int left_offset, const unsigned int bottom_offset)
    {
        glViewport(left_offset, bottom_offset, width, height);
    }

    void Renderer_OpenGL::enable_depth_test()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer_OpenGL::disable_depth_test()
    {
        glDisable(GL_DEPTH_TEST);
    }

    const char* Renderer_OpenGL::get_vendor_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    }

    const char* Renderer_OpenGL::get_renderer_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    }

    const char* Renderer_OpenGL::get_version_str()
    {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }

}
