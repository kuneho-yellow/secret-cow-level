#pragma once

#include <glm/glm.hpp>

#include "scl_tansform.h"

struct SCLSprite;

class SCLCamera
{
public:
	SCLCamera();
	~SCLCamera();

	void Initialize(int ScreenWidth, int ScreenHeight, float CameraSizeX);
	void Update();

	glm::mat4 GetVPMatix() const;
	glm::vec2 GetPosition() const;
	void SetPosition(glm::vec2 NewPosition);
	void Translate(glm::vec2 Displacement);
	glm::vec2 GetCameraSize() const;
	glm::vec2 ConvertScreenToWorld(glm::vec2 ScreenCoord) const;
	glm::vec2 ConvertWorldToViewport(glm::vec2 WorldPos) const;
	bool IsSpriteSeen(const SCLSprite* pSprite) const;

private:
	glm::mat4 m_ProjMatrix;
	int m_ScreenWidth;
	int m_ScreenHeight;
	glm::vec2 m_CameraSize;
	SCLTransform m_Transform;
};

