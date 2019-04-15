#include "scl_tansform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

SCLTransform::SCLTransform()
	: m_Position(0.0f, 0.0f)
	//m_WorldMatrix(1.0f)
{
}

SCLTransform::~SCLTransform()
{

}

glm::vec2 SCLTransform::GetPosition() const
{
	return m_Position;
}

void SCLTransform::SetPosition(glm::vec2 Pos)
{
	//glm::vec3 Translation(Pos.x, Pos.y, 0.0f);
	//m_WorldMatrix = glm::translate(glm::mat4(1.0f), Translation);
	m_Position = Pos;
}

void SCLTransform::Translate(glm::vec2 DeltaPos)
{
	//glm::vec3 Translation(DeltaPos.x, DeltaPos.y, 0.0f);
	//m_WorldMatrix = glm::translate(m_WorldMatrix, Translation);
	m_Position += DeltaPos;
}