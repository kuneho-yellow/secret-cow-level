#pragma once

#include <GL/glew.h>
#include <string>

class SCLShaderProgram
{
public:
	enum Attributes
	{
		ATTRB_VERTEX_POSITION,
		ATTRB_VERTEX_COLOR,
		ATTRB_VERTEX_UV,
		ATTRB_SIZE
	};

	enum Uniforms
	{
		UNI_TEXTURE,
		UNI_VP_MAT,
		UNI_SIZE
	};

	SCLShaderProgram();
	~SCLShaderProgram();

	bool Initialize();
	void Start();
	void End();

	void SetCameraMatrix(const GLfloat* CameraMatrix);

private:
	bool CompileShader(const std::string& FilePath, GLuint &Id);
	GLint GetUniformLocation(const std::string& UniformName);

	static const GLchar* m_AttributeNames[ATTRB_SIZE];
	static const GLchar* m_UniformNames[UNI_SIZE];

	GLuint m_ProgramId;
	GLuint m_VertShaderId;
	GLuint m_FragShaderId;
	GLint m_UniformLoc[UNI_SIZE];
};

