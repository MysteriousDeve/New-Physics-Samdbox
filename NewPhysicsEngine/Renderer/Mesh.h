#pragma once

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>

struct MeshInfo
{
	GLfloat* vertices = nullptr;
	GLsizeiptr verticesSize = 0;
	GLuint* indices = nullptr;
	GLsizeiptr indicesSize = 0;
	GLsizei indicesCount = 0;

	MeshInfo(GLfloat vertices[], GLsizeiptr verticesSize, GLuint indices[], GLsizeiptr indicesSize)
	{
		this->vertices = vertices;
		this->verticesSize = verticesSize;
		this->indices = indices;
		this->indicesSize = indicesSize;
		this->indicesCount = indicesSize / sizeof(float);
	}

	MeshInfo(){}
};

class Mesh
{
private:
	MeshInfo info;
public:
	GLuint VAO, VBO, EBO;

	Mesh(MeshInfo info)
	{
		this->info = info;
	}

	void Initialize()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, info.verticesSize, info.vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.indicesSize, info.indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Bind()
	{
		glBindVertexArray(VAO);
	}

	void Draw()
	{
		glDrawElements(GL_TRIANGLES, info.indicesCount, GL_UNSIGNED_INT, 0);
	}

	void Terminate()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}
};