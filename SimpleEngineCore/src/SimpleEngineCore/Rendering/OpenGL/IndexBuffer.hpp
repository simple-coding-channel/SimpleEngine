#pragma once
#include "VertexBuffer.hpp"

namespace SimpleEngine {

    class IndexBuffer {
    public:

        IndexBuffer(const void* data, const size_t count, const VertexBuffer::EUsage usage = VertexBuffer::EUsage::Static);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer& operator=(IndexBuffer&& index_buffer) noexcept;
        IndexBuffer(IndexBuffer&& index_buffer) noexcept;

        void bind() const;
        static void unbind();
        size_t get_count() const { return m_count; }

    private:
        unsigned int m_id = 0;
        size_t m_count;
    };

}
