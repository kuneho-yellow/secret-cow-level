#include "scl_shader_program.h"

#include <fstream>

const GLchar* SCLShaderProgram::m_AttributeNames[ATTRB_SIZE] =
{
	"VertexPosition",
	"VertexColor",
	"VertexUV"
};

const GLchar* SCLShaderProgram::m_UniformNames[UNI_SIZE] =
{
	"TexSampler",
	"VPMat"
};

SCLShaderProgram::SCLShaderProgram()
	: m_ProgramId(0),
	m_VertShaderId(0),
	m_FragShaderId(0)
{
	
}


SCLShaderProgram::~SCLShaderProgram()
{
}

bool SCLShaderProgram::Initialize()
{
	// Create a program
	m_ProgramId = glCreateProgram();
	if (m_ProgramId == 0)
	{
		printf("ERROR: Failed to create glsl program\n");
		return false;
	}

	// Create and compile the vertex shader
	m_VertShaderId = glCreateShader(GL_VERTEX_SHADER);
	if (m_VertShaderId == 0)
	{
		glDeleteProgram(m_ProgramId);
		printf("ERROR: Failed to create vertex shader\n");
		return false;
	}
	if (!CompileShader("files/shaders/vertex_shader.vert", m_VertShaderId))
	{
		glDeleteProgram(m_ProgramId);
		printf("ERROR: Failed to compile vertex shader\n");
		return false;
	}

	// Create and compile the fragment shader
	m_FragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	if (m_FragShaderId == 0)
	{
		glDeleteProgram(m_ProgramId);
		glDeleteShader(m_VertShaderId);
		printf("ERROR: Failed to create fragment shader\n");
		return false;
	}
	if (!CompileShader("files/shaders/fragment_shader.frag", m_FragShaderId))
	{
		glDeleteProgram(m_ProgramId);
		glDeleteShader(m_VertShaderId);
		printf("ERROR: Failed to compile fragment shader\n");
		return false;
	}

	// Add Attributes
	for (unsigned int AttrI = 0; AttrI < ATTRB_SIZE; ++AttrI)
	{
		glBindAttribLocation(m_ProgramId, AttrI, m_AttributeNames[AttrI]);
	}

	// Attach shaders to the program
	glAttachShader(m_ProgramId, m_VertShaderId);
	glAttachShader(m_ProgramId, m_FragShaderId);

	// Link the program
	glLinkProgram(m_ProgramId);
	GLint Result = 0;
	glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		glDeleteProgram(m_ProgramId);
		glDeleteShader(m_VertShaderId);
		glDeleteShader(m_FragShaderId);

		std::printf("ERROR: Failed to link the shader program\n");
		return false;
	}

	// Detach and delete shaders after a successful link.
	glDetachShader(m_ProgramId, m_VertShaderId);
	glDetachShader(m_ProgramId, m_FragShaderId);
	glDeleteShader(m_VertShaderId);
	glDeleteShader(m_FragShaderId);

	// Find uniform locations
	for (unsigned int UniI = 0; UniI < UNI_SIZE; ++UniI)
	{
		m_UniformLoc[UniI] = GetUniformLocation(m_UniformNames[UniI]);
	}

	return true;
}

void SCLShaderProgram::Start()
{
	glUseProgram(m_ProgramId);
	for (unsigned int AttrI = 0; AttrI < ATTRB_SIZE; ++AttrI)
	{
		glEnableVertexAttribArray(AttrI);
	}

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(m_UniformLoc[UNI_TEXTURE], 0); // 0 - first texture bound
}

void SCLShaderProgram::End()
{
	glUseProgram(0);
	for (unsigned int AttrI = 0; AttrI < ATTRB_SIZE; ++AttrI)
	{
		glDisableVertexAttribArray(AttrI);
	}
}

void SCLShaderProgram::SetCameraMatrix(const GLfloat* pCameraMatrix)
{
	glUniformMatrix4fv(m_UniformLoc[UNI_VP_MAT], 1, GL_FALSE, pCameraMatrix);
}

bool SCLShaderProgram::CompileShader(const std::string& FilePath, GLuint &Id)
{
	std::ifstream FileStream(FilePath);
	if (FileStream.fail())
	{
		printf("ERROR: Failed to open %s", FilePath.c_str());
	}

	std::string FileContents = "";
	std::string line;
	while (std::getline(FileStream, line))
	{
		FileContents += line + "\n";
	}
	FileStream.close();

	const char* contentsPtr = FileContents.c_str();
	glShaderSource(Id, 1, &contentsPtr, NULL);
	glCompileShader(Id);
	GLint Result = 0;
	glGetShaderiv(Id, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE)
	{
		printf("ERROR: Failed to compile shader: %s\n", FilePath.c_str());
		return false;
	}
	return true;
}

GLint SCLShaderProgram::GetUniformLocation(const std::string& UniformName)
{
	GLint location = glGetUniformLocation(m_ProgramId, UniformName.c_str());
	if (location == GL_INVALID_INDEX)
	{
		printf("ERROR: Uniform not found in shader: %s\n", UniformName.c_str());
	}
	return location;
}