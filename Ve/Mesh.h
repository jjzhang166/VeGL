#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture2D.h"
#include "VertexSpecification.h"

#include <glm/gtx/normal.hpp>

class Mesh
{
public:
    Mesh (GLfloat* vertices, int szVertices, GLuint* indices, int nIndices, const VertexBufferLayout& vbl, GLenum draw_mode = GL_TRIANGLES)
        : m_vb (vertices, szVertices, indices, nIndices), draw_mode(draw_mode)
    {
        m_va.AddBuffer (m_vb, vbl);
    }

    static Mesh Primitive_Plane (const float resolution = 500)
    {
		VertexBufferLayout vbl;
		vbl.AddAttribute<GLfloat> (3);
		vbl.AddAttribute<GLfloat> (2);

		std::vector<GLfloat> vertices;
		std::vector<GLuint> indices;

		float step = 1.0f / resolution;

		int idxs = 0;
		for (float j = 0; j < 1; j += step)
			for (float i = 0; i < 1; i += step)
			{
                vertices.push_back (i);
                vertices.push_back (0.0);
                vertices.push_back (j);
                vertices.push_back (0);
                vertices.push_back (0);

                vertices.push_back (i);
                vertices.push_back (0.0);
                vertices.push_back (j + step);
                vertices.push_back (0);
                vertices.push_back (1);

                vertices.push_back (i + step);
                vertices.push_back (0.0);
                vertices.push_back (j);
                vertices.push_back (1);
                vertices.push_back (0);

                vertices.push_back (i + step);
                vertices.push_back (0.0);
                vertices.push_back (j + step);
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


		return Mesh (&vertices[0], sizeof (float) * vertices.size (), &indices[0], indices.size (), vbl);
    }

	static Mesh Primitive_Cube ()
	{
        GLfloat pos_uvs[5 * 36] =
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

		std::vector<GLfloat> vertices;
		std::vector<GLuint> indices;

		for (int i = 0; i < 5*36;)
		{
			auto p1_x = pos_uvs[i++];
			auto p1_y = pos_uvs[i++];
			auto p1_z = pos_uvs[i++];
			auto u1 = pos_uvs[i++];
			auto v1 = pos_uvs[i++];

			auto p2_x = pos_uvs[i++];
			auto p2_y = pos_uvs[i++];
			auto p2_z = pos_uvs[i++];
			auto u2 = pos_uvs[i++];
			auto v2 = pos_uvs[i++];

			auto p3_x = pos_uvs[i++];
			auto p3_y = pos_uvs[i++];
			auto p3_z = pos_uvs[i++];
			auto u3 = pos_uvs[i++];
			auto v3 = pos_uvs[i++];

			glm::vec3 pos1 (p1_x, p1_y, p1_z);
			glm::vec3 pos2 (p2_x, p2_y, p2_z);
			glm::vec3 pos3 (p3_x, p3_y, p3_z);


			auto N = glm::triangleNormal (pos1, pos2, pos3);

			/*glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1; 
			glm::vec2 deltaUV1 = glm::vec2(u2,v2) - glm::vec2 (u1,v1);
			glm::vec2 deltaUV2 = glm::vec2 (u3,v3) - glm::vec2 (u1,v1);

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			glm::vec3 tangent, bitangent;
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent = glm::normalize (tangent);

			bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			bitangent = glm::normalize (bitangent);

			auto triangle = 
			{ 
				p1_x, p1_y, p1_z, N.x, N.y, N.z, u1, v1, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
				p2_x, p2_y, p2_z, N.x, N.y, N.z, u2, v2, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z,
				p3_x, p3_y, p3_z, N.x, N.y, N.z, u3, v3, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z
			};*/

			auto triangle =
			{
				p1_x, p1_y, p1_z, u1, v1, N.x, N.y, N.z,
				p2_x, p2_y, p2_z, u2, v2, N.x, N.y, N.z,
				p3_x, p3_y, p3_z, u3, v3, N.x, N.y, N.z
			};

			vertices.insert (vertices.end (), triangle.begin (), triangle.end());

			while(indices.size() < 36)
				indices.push_back (indices.size());
		}

		VertexBufferLayout vbl;
		vbl.AddAttribute<float> (3);
		vbl.AddAttribute<float> (2);
		vbl.AddAttribute<float> (3);
		//vbl.AddAttribute<float> (3);
		//vbl.AddAttribute<float> (3);

		return Mesh (&vertices[0], vertices.size() * sizeof (GLfloat), &indices[0], indices.size (), vbl);
	}

	static Mesh Primitive_Sphere ()
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<GLuint> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;

		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos (xSegment * 2.0f * PI) * std::sin (ySegment * PI);
				float yPos = std::cos (ySegment * PI);
				float zPos = std::sin (xSegment * 2.0f * PI) * std::sin (ySegment * PI);

				positions.push_back (glm::vec3 (xPos, yPos, zPos));
				uv.push_back (glm::vec2 (xSegment, ySegment));
				normals.push_back (glm::vec3 (xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) 
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back (y * (X_SEGMENTS + 1) + x);
					indices.push_back ((y + 1) * (X_SEGMENTS + 1) + x);
				}
			else
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back ((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back (y * (X_SEGMENTS + 1) + x);
				}
			oddRow = !oddRow;
		}

		std::vector<float> vertices;
		for (int i = 0; i < positions.size (); ++i)
		{
			vertices.push_back (positions[i].x);
			vertices.push_back (positions[i].y);
			vertices.push_back (positions[i].z);
			vertices.push_back (uv[i].x);
			vertices.push_back (uv[i].y);
			vertices.push_back (normals[i].x);
			vertices.push_back (normals[i].y);
			vertices.push_back (normals[i].z);
		}

		VertexBufferLayout vbl;
		vbl.AddAttribute<float> (3);
		vbl.AddAttribute<float> (2);
		vbl.AddAttribute<float> (3);

		return Mesh (&vertices[0], vertices.size () * sizeof (GLfloat), &indices[0], indices.size (), vbl, GL_TRIANGLE_STRIP);
	}

    void Bind ()
    {
        m_va.Bind ();
        m_vb.Bind ();
    }

    int GetIndexCount () const { return m_vb.GetIndexCount (); }
    int GetVertexSize () const { return m_vb.GetVertexSize (); }
	GLenum GetDrawMode () const { return draw_mode; }

private:
    VertexArray m_va;
    VertexBuffer m_vb;
	GLenum draw_mode;
};