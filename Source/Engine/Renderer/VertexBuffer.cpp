#include "VertexBuffer.h"

namespace neu
{
	VertexBuffer::VertexBuffer()
	{
		// Make Vertex Array & Bind
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_vbo) glDeleteBuffers(1, &m_vbo);
		if (m_ibo) glDeleteBuffers(1, &m_ibo);
		if (m_vao) glDeleteVertexArrays(1, &m_vao);
	}

	//void VertexBuffer::Draw(GLenum primitiveType)
	//{
	//	if (m_vao) glBindVertexArray(m_vao);
	//	else Logger::Error("VertexBuffer.cpp", 21, "m_vao doesn't exist during draw");
	//	if (m_ibo) glDrawElements(primitiveType, m_indexCount, m_indexType, 0);
	//	else if (m_vbo) glDrawArrays(primitiveType, 0, m_vertexCount);
	//}

	//void VertexBuffer::CreateVertexBuffer(GLsizei size, GLsizei count, GLvoid* data)
	//{
	//	m_vertexCount = count;
	//	glGenBuffers(1, &m_vbo);

	//	// Points
	//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	//}

	//void VertexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei count, GLvoid* data)
	//{
	//	m_indexType = indexType;
	//	m_indexCount = count;
	//	
	//	glBindVertexArray(m_vao);

	//	glGenBuffers(1, &m_ibo);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	//	GLsizei size = 0;
	//	switch (indexType)
	//	{
	//	case GL_UNSIGNED_BYTE: size = count * sizeof(GLubyte); break;
	//	case GL_UNSIGNED_SHORT: size = count * sizeof(GLushort); break;
	//	case GL_UNSIGNED_INT: size = count * sizeof(GLuint); break;
	//	}

	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	//}

	//void VertexBuffer::SetAttribute(GLuint index, GLint size, GLsizei stride, GLuint offset)
	//{
	//	glBindVertexArray(m_vao);
	//	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//	glEnableVertexAttribArray(index);
	//	glVertexAttribPointer(index, size, m_indexType, GL_FALSE, stride, (void*)offset);
	//}

	// Members you should have:
	GLuint m_vao = 0, m_vbo = 0, m_ebo = 0;
	GLsizei m_vertexCount = 0, m_indexCount = 0;
	GLenum  m_indexType = GL_UNSIGNED_INT;

	// Create VAO+VBO and upload vertex data
	void VertexBuffer::CreateVertexBuffer(GLsizei byteSize, GLsizei vertexCount, GLvoid* data) {
		m_vertexCount = vertexCount;
		if (!m_vao) glGenVertexArrays(1, &m_vao);
		if (!m_vbo) glGenBuffers(1, &m_vbo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

		// leave VAO bound for SetAttribute to run next
	}

	// Bind EBO while VAO is bound so the association is captured by the VAO
	void VertexBuffer::CreateIndexBuffer(GLenum indexType, GLsizei indexCount, GLvoid* data) {
		m_indexType = indexType;
		m_indexCount = indexCount;

		if (!m_vao) glGenVertexArrays(1, &m_vao);
		if (!m_ebo) glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		GLsizeiptr idxBytes = (indexType == GL_UNSIGNED_SHORT ? 2 :
			indexType == GL_UNSIGNED_BYTE ? 1 : 4) * (GLsizeiptr)indexCount;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxBytes, data, GL_STATIC_DRAW);

		// keep VAO bound or unbind if you want; the EBO stays associated with VAO
	}

	// The critical part: use the passed size/stride/offset, and ensure VBO is bound
	void VertexBuffer::SetAttribute(GLuint index, GLint size, GLsizei stride, GLsizei offset) {
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);            // MUST be bound here
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, (void*)(uintptr_t)offset);
	}

	// Draw using whichever path is available
	void VertexBuffer::Draw(GLenum primitiveType) {
		glBindVertexArray(m_vao);
		if (m_indexCount > 0) {
			glDrawElements(primitiveType, m_indexCount, m_indexType, (void*)0);
		}
		else {
			glDrawArrays(primitiveType, 0, m_vertexCount);
		}
	}

}
