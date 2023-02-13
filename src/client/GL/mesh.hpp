#pragma once

#include "core/transform.hpp"

namespace cybrion::GL
{
    enum class Type
    {
        INT,
        UINT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4
    };

    struct MeshAttribute
    {
        Type type;
    };

    class Mesh : public Transform
    {
    public:
        Mesh(bool useGlobalIBO = false);

        Mesh(const Mesh&) = default;

        Mesh(Mesh&&) noexcept;

        Mesh& operator = (const Mesh&) {
            assert(false && "Cannot assign");
            return *this;
        }

        ~Mesh();

        void setVerticesData(void* data, u32 size);

        template <typename T>
        void setVertices(T* vertices, u32 count)
        {
            setVerticesData(vertices, count * sizeof(T));
        }

        void setIndices(u32* indices, u32 count);
        void setAttributes(std::initializer_list<MeshAttribute> attributes);
        void drawTriangles() const;
        void drawLines() const;
        void setDrawCount(u32 drawCount);

        //~Mesh();

        static void GenerateGlobalIBO();

    private:
        static GLuint ToGLType(Type type);
        static u32 GetElementCount(Type type);
        static u32 GetTypeSize(Type type);

        static GLuint s_globalIBO;

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ibo;
        u32 m_drawCount;
        u32 m_maxBufferSize;
        u32 m_maxIndicesCount;
        bool m_useGlobalIBO;
    };
}