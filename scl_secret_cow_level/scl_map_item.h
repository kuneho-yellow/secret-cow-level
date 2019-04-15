#pragma once

#include "scl_game_object.h"

enum SCLMapItemType
{
	MAP_ITEM_BG,
	MAP_ITEM_DECOR,
	MAP_ITEM_STATIC,
	MAP_ITEM_PICKUP_START,
	MAP_ITEM_PICKUP_1,
	MAP_ITEM_PICKUP_2,
	MAP_ITEM_PICKUP_3,
	MAP_ITEM_PICKUP_4,
	MAP_ITEM_PICKUP_END
};

class SCLMapItem : public SCLGameObject
{
public:
	SCLMapItem();
	~SCLMapItem();

	void Initialize(SCLMapItemType Type, unsigned int Index, glm::vec2 InitPos);
	void Update(float DeltaTime);
	void FixedUpdate(float FixedDeltaTime);
	void NotifyCollision(SCLGameObject& OtherGO);

	SCLMapItemType GetType() const;
	void SetPlayerGameObject(SCLGameObject* pPlayer);

private:
	// TODO: Put these somewhere else
	static SCLTexture GetTextureFromType(SCLMapItemType Type);
	static float GetSizeFromType(SCLMapItemType Type);
	static SCLCollisionLayer GetLayerFromType(SCLMapItemType Type);
	static SCLTexture GetRandomDecorTexture();
	static SCLTexture GetRandomStaticItemTexture();

	SCLMapItemType m_Type;
	unsigned int m_Index;
	SCLGameObject* m_pPlayerGO;
};

