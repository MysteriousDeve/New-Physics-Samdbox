#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <Windows.h>
#include <glm/glm.hpp>
using namespace std;

class Shader
{
private: 
	string parentPath = "Shaders/";

public:

	GLuint ID;
	string vert, frag;

	Shader(string vert, string frag)
	{
		this->vert = vert;
		this->frag = frag;

	}
	void Initialize()
	{
		string v = getFileContent(parentPath + vert + ".vert");
		const char* vertexShaderText = v.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderText, NULL);
		glCompileShader(vertexShader);
		compileErrors(vertexShader, "VERTEX");


		string f = getFileContent(parentPath + frag + ".frag");
		const char* fragmentShaderText = f.c_str();

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderText, NULL);
		glCompileShader(fragmentShader);
		compileErrors(fragmentShader, "FRAGMENT");


		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		compileErrors(ID, "PROGRAM");


		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	void DestroyShaderProgram()
	{
		glDeleteProgram(ID);
	}
	void Use()
	{
		glUseProgram(ID);
	}
	void SetUniformVec2(const GLchar* name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(ID, name), x, y);
	}
	void SetUniformVec4(const GLchar* name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
	}
	void SetUniformFloat(const GLchar* name, float x)
	{
		glUniform1f(glGetUniformLocation(ID, name), x);
	}
	void SetUniformMat4x4(const GLchar* name, glm::mat4x4 mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);
	}

private:
	string getFileContent(string filename)
	{
		std::ifstream in(filename, std::ios::binary);
		if (in)
		{
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}
	void compileErrors(unsigned int shader, const char* type)
	{
		GLint hasCompiled;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				MessageBoxA(NULL, infoLog, "Shader Compiling Error", MB_ICONERROR | MB_OK);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
			if (hasCompiled == GL_FALSE)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				MessageBoxA(NULL, infoLog, "Shader Linking Error", MB_ICONERROR | MB_OK);
			}
		}
	}
};