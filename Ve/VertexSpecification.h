#pragma once
#include <vector>
#include <GL/glew.h>
#include "Debug.h"

struct VertexAttribute
{
	GLuint type;
	GLuint count;
	GLuint normalized;
	GLuint size;
};

class VertexBufferLayout
{
public:
	VertexBufferLayout ()
		: m_stride (0)
	{}

	template<typename T>
	void AddAttribute (GLuint count)
	{
		fprintf (stderr, "[ERROR][OPENGL] Invalid attribute type");
		std::exit (EXIT_FAILURE);
	}

	template<>
	void AddAttribute<GLfloat> (GLuint count)
	{
		m_attributes.push_back (VertexAttribute{ GL_FLOAT, count, GL_FALSE, sizeof (GLfloat) });
		m_stride += sizeof (GLfloat) * count;
	}

	template<>
	void AddAttribute<GLint> (GLuint count)
	{
		m_attributes.push_back (VertexAttribute{ GL_INT, count, GL_FALSE, sizeof (GLint) });
		m_stride += sizeof (GLint) * count;
	}

	template<>
	void AddAttribute<GLuint> (GLuint count)
	{
		m_attributes.push_back (VertexAttribute{ GL_UNSIGNED_INT, count, GL_FALSE, sizeof (GLuint) });
		m_stride += sizeof (GLuint) * count;
	}

	const std::vector<VertexAttribute>& GetAttributes () const { return m_attributes; };
	GLuint GetStride () const { return m_stride; };

private:
	std::vector<VertexAttribute> m_attributes;
	GLuint m_stride = 0;
};

class VertexBuffer
{
public:
	VertexBuffer (const GLfloat* p_vertices, GLsizei p_szVertices,
		const GLuint* p_indices, GLsizei p_nIndices)
		: m_nIndex (p_nIndices), m_szVertex (p_szVertices)
	{
		GLCall (glGenBuffers (1, &m_vboID));
		GLCall (glBindBuffer (GL_ARRAY_BUFFER, m_vboID));
		GLCall (glBufferData (GL_ARRAY_BUFFER, p_szVertices, p_vertices, GL_STATIC_DRAW));

		GLCall (glGenBuffers (1, &m_iboID));
		GLCall (glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_iboID));
		GLCall (glBufferData (GL_ELEMENT_ARRAY_BUFFER, p_nIndices*sizeof(GLuint), p_indices, GL_STATIC_DRAW));
	}

	~VertexBuffer ()
	{
		GLCall (glDeleteBuffers (1, &m_vboID));
		GLCall (glDeleteBuffers (1, &m_iboID));
	}

	void Bind () const
	{
		GLCall (glBindBuffer (GL_ARRAY_BUFFER, m_vboID));
		GLCall (glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, m_iboID));
	}

	void Unbind () const
	{
		GLCall (glBindBuffer (GL_ARRAY_BUFFER, 0));
		GLCall (glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	GLsizei GetIndexCount () const { return m_nIndex; }
	GLsizei GetVertexSize () const { return m_szVertex; }

private:
	GLuint m_iboID;
	GLuint m_vboID;
	GLsizei m_nIndex, m_szVertex;
};


class VertexArray
{
public:
	VertexArray ()
	{
		GLCall (glGenVertexArrays (1, &m_id));
	}

	~VertexArray ()
	{
		GLCall (glDeleteVertexArrays (1, &m_id));
	}

	void AddBuffer (VertexBuffer& buffer, const VertexBufferLayout& layout)
	{
		Bind ();
		buffer.Bind ();
		const auto m_attributes = layout.GetAttributes ();
		GLuint offset = 0;

		for (GLuint i = 0; i < m_attributes.size (); i++)
		{
			const auto& attribute = m_attributes[i];
			GLCall (glVertexAttribPointer (i, attribute.count, attribute.type, GL_FALSE, layout.GetStride (), (void*)offset));
			GLCall (glEnableVertexAttribArray (i));
			offset += attribute.count * attribute.size;
		}
	}

	void Bind ()
	{
		GLCall (glBindVertexArray (m_id));
	}

	void Unbind ()
	{
		GLCall (glBindVertexArray (0));
	}

private:
	GLuint m_id = 0;
};