#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "VertexSpecification.h"

class Mesh
{
public:
    Mesh (float* vertices, int szVertices, unsigned int* indices, int szIndices, const VertexBufferLayout& vbl)
        : m_vb (vertices, szVertices, indices, szIndices)
    {
        m_va.AddBuffer (m_vb, vbl);
    }

    static Mesh Primitive_Plane (const glm::vec2 &size)
    {
		VertexBufferLayout vbl;
		vbl.AddAttribute<float> (3);
		vbl.AddAttribute<float> (2);

		std::vector<float> vertices;
		std::vector<unsigned> indices;

		int idxs = 0;
		for (int j = 0; j < size.x; j++)
			for (int i = 0; i < size.y; i++)
			{
                vertices.push_back (i - 0.5f);
                vertices.push_back (0.0);
                vertices.push_back (j - 0.5f);
                vertices.push_back (0);
                vertices.push_back (0);

                vertices.push_back (i - 0.5f);
                vertices.push_back (0.0);
                vertices.push_back (j + 0.5f);
                vertices.push_back (0);
                vertices.push_back (1);

                vertices.push_back (i + 0.5f);
                vertices.push_back (0.0);
                vertices.push_back (j - 0.5f);
                vertices.push_back (1);
                vertices.push_back (0);

                vertices.push_back (i + 0.5f);
                vertices.push_back (0.0);
                vertices.push_back (j + 0.5f);
                vertices.push_back (1);
				vertices.push_back (1);

				indices.push_back (idxs + 0);
                indices.push_back (idxs + 2);
                indices.push_back (idxs + 3);
                indices.push_back (idxs + 3);
                indices.push_back (idxs + 1);
                indices.push_back (idxs + 0);

				idxs += 4;
			}


		return Mesh (&vertices[0], sizeof (float) * vertices.size (), &indices[0], sizeof(unsigned) * indices.size (), vbl);
    }

	static Mesh Primitive_Cube ()
	{
		VertexBufferLayout vbl;
		vbl.AddAttribute<float> (3);
		vbl.AddAttribute<float> (2);

		std::vector<float> pvertices;
		std::vector<unsigned> pindices;

        GLfloat vertices[5 * 36] =
        {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        GLuint indices[36] =
        {
            0,1,2,3,4,
            5,6,7,8,9,
            10,11,12,13,14,
            15,16,17,18,19,
            20,21,22,23,24,
            25,26,27,28,29,
            30,31,32,33,34,
            35
        };


		return Mesh (vertices, sizeof (vertices), indices, sizeof (indices), vbl);
	}

    void Bind ()
    {
        m_va.Bind ();
        m_vb.Bind ();
    }

    int GetIndexCount () const { return m_vb.GetIndexCount (); }
    int GetVertexCount () const { return m_vb.GetVertexCount (); }

private:
    VertexArray m_va;
    VertexBuffer m_vb;
};