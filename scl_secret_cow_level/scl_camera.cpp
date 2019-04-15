#include "scl_camera.h"
#include "scl_sprite.h"

// For creating the view and projection matrices
#include <glm/gtc/matrix_transform.hpp>

SCLCamera::SCLCamera()
	: m_ProjMatrix(1.0f),
	m_ScreenWidth(0),
	m_ScreenHeight(0),
	m_CameraSize(0.0f),
	m_Transform()
{
}

SCLCamera::~SCLCamera()
{
}

void SCLCamera::Initialize(int ScreenWidth, int ScreenHeight, float CameraSizeX)
{
	m_ScreenWidth = ScreenWidth;
	m_ScreenHeight = ScreenHeight;
	float ScreenRatio = (float)m_ScreenHeight / m_ScreenWidth;
	m_CameraSize.x = CameraSizeX;
	m_CameraSize.y = CameraSizeX * ScreenRatio;

	// Screen width in world units == CameraSizeX
	float ScreenEdgeX = m_CameraSize.x * 0.5f;
	float ScreenEdgeY = m_CameraSize.y * 0.5f;
	m_ProjMatrix = glm::ortho(-ScreenEdgeX, ScreenEdgeX, -ScreenEdgeY, ScreenEdgeY);

	// If I want 1 pixel == 1 world unit
	/* m_ProjMatrix = glm::ortho(-m_ScreenWidth * 0.5f,
	m_ScreenWidth * 0.5f,
	-m_ScreenHeight * 0.5f,
	m_ScreenHeight * 0.5f); */
}

void SCLCamera::Update()
{

}

glm::mat4 SCLCamera::GetVPMatix() const
{
	// Create the View matrix
	glm::vec3 Eye(m_Transform.GetPosition().x, m_Transform.GetPosition().y, 1.0f);
	glm::vec3 Target(Eye.x, Eye.y, 0.0f);
	glm::vec3 Up(0.0f, 1.0f, 0.0f);
	glm::mat4 ViewMatrix = glm::lookAt(Eye, Target, Up);

	// And multiply with the orthographic projection matrix
	return m_ProjMatrix * ViewMatrix;
}

glm::vec2 SCLCamera::GetPosition() const
{
	return m_Transform.GetPosition();
}

void SCLCamera::SetPosition(glm::vec2 NewPosition)
{
	m_Transform.SetPosition(glm::vec2(NewPosition.x, NewPosition.y));
}

void SCLCamera::Translate(glm::vec2 Displacement)
{
	m_Transform.Translate(Displacement);
}

glm::vec2 SCLCamera::GetCameraSize() const
{
	return m_CameraSize;
}

glm::vec2 SCLCamera::ConvertScreenToWorld(glm::vec2 ScreenCoord) const
{
	// Compute world x
	// convert (0 ~ m_ScreenWidth-1) to (-m_CameraSize.x/2 ~ m_CameraSize.x/2)
	glm::vec2 WorldPos(0.0f);
	WorldPos.x = ((ScreenCoord.x) * m_CameraSize.x) / m_ScreenWidth;
	WorldPos.x += -m_CameraSize.x * 0.5f;

	// Compute world y
	// convert (m_ScreenWidth-1 ~ 0) to (-m_CameraSize.y/2 ~ m_CameraSize.y/2)
	float ScreenRatio = (float)m_ScreenHeight / m_ScreenWidth;
	// Invert y axis
	ScreenCoord.y = m_ScreenHeight - ScreenCoord.y;
	WorldPos.y = ((ScreenCoord.y) * m_CameraSize.y) / m_ScreenHeight;
	WorldPos.y += -m_CameraSize.y * 0.5f;

	// Add the camera's world position
	return WorldPos + m_Transform.GetPosition();
}

glm::vec2 SCLCamera::ConvertWorldToViewport(glm::vec2 WorldPos) const
{
	// Viewport coordinate is from 0.0 to 1.0 within the camera's view

	glm::vec2 CameraPos = m_Transform.GetPosition();
	float ScreenRatio = (float)m_ScreenHeight / m_ScreenWidth;
	float XMin = (-m_CameraSize.x * 0.5f) + CameraPos.x;
	float YMin = (-m_CameraSize.y * 0.5f) + CameraPos.y;

	glm::vec2 ViewportPos(0.0f, 0.0f);
	ViewportPos.x = (WorldPos.x - XMin) / m_CameraSize.x;
	ViewportPos.y = (WorldPos.y - YMin) / m_CameraSize.y;

	return ViewportPos;
}

bool SCLCamera::IsSpriteSeen(const SCLSprite* pSprite) const
{
	glm::vec2 CameraPos = m_Transform.GetPosition();
	float XMin = (-m_CameraSize.x * 0.5f) + CameraPos.x;
	float XMax = XMin + m_CameraSize.x;
	float YMin = (-m_CameraSize.y * 0.5f) + CameraPos.y;
	float YMax = YMin + m_CameraSize.y;

	if (pSprite->VertBotLeft.Position.x >= XMax)
		return false;
	if (pSprite->VertTopRight.Position.x <= XMin)
		return false;
	if (pSprite->VertBotLeft.Position.y >= YMax)
		return false;
	if (pSprite->VertTopRight.Position.y <= YMin)
		return false;

	return true;
}