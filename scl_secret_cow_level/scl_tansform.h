#pragma once

#include <glm/glm.hpp>

// TODO: Currently just used to hold and process vec2 position

class SCLTransform
{
public:
	SCLTransform();
	~SCLTransform();

	glm::vec2 GetPosition() const;
	void SetPosition(glm::vec2 Pos);
	void Translate(glm::vec2 DeltaPos);
//	glm::mat4 GetWorldMatrix() const;

private:
	glm::vec2 m_Position;
//	glm::mat4 m_WorldMatrix; // Nope, so slow, not needed >_<
};

