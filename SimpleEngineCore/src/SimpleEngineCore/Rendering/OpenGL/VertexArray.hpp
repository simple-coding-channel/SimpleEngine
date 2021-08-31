#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace SimpleEngine {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertex_buffer) noexcept;
        VertexArray(VertexArray&& vertex_buffer) noexcept;

        void add_vertex_buffer(const VertexBuffer& vertex_buffer);
        void set_index_buffer(const IndexBuffer& index_buffer);
        void bind() const;
        static void unbind();
        size_t get_indices_count() const { return m_indices_count; }

    private:
        unsigned int m_id = 0;
        unsigned int m_elements_count = 0;
        size_t m_indices_count = 0;
    };

}
