#include "VertexBuffer.hpp"

#include "SimpleEngineCore/Log.hpp"

#include <glad/glad.h>

namespace SimpleEngine {

    constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage)
    {
        switch (usage)
        {
            case VertexBuffer::EUsage::Static:  return GL_STATIC_DRAW;
            case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
            case VertexBuffer::EUsage::Stream:  return GL_STREAM_DRAW;
        }

        LOG_ERROR("Unknown VertexBuffer usage");
        return GL_STREAM_DRAW;
    }

    VertexBuffer::VertexBuffer(const void* data, const size_t size, const EUsage usage)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept
    {
        m_id = vertexBuffer.m_id;
        vertexBuffer.m_id = 0;
        return *this;
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
    {
        m_id = vertexBuffer.m_id;
        vertexBuffer.m_id = 0;
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
