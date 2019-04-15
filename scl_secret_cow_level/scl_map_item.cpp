#include "scl_map_item.h"
#include "scl_resource_manager.h"
#include "scl_random.h"
#include "scl_game_constants.h"

SCLMapItem::SCLMapItem()
	: m_Type((SCLMapItemType)0),
	m_Index(0),
	m_pPlayerGO(NULL)
{

}

SCLMapItem::~SCLMapItem()
{
}

void SCLMapItem::Initialize(SCLMapItemType Type, unsigned int Index, glm::vec2 InitPos)
{
	m_Type = Type;
	m_Index = Index;
	float Size = GetSizeFromType(m_Type);
	SetSprite(GetTextureFromType(m_Type), Size);
	SetCollisionLayer(GetLayerFromType(m_Type));
	SetPosition(InitPos);
	SetColliderRadius(Size * 0.5f);
	m_IsActive = true;
}

void SCLMapItem::Update(float DeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}
}
void SCLMapItem::FixedUpdate(float FixedDeltaTime)
{
	if (!m_IsActive)
	{
		return;
	}

	// Process pickups type only
	if (m_Type >= MAP_ITEM_PICKUP_1 && m_Type <= MAP_ITEM_PICKUP_4)
	{
		if (m_pPlayerGO != NULL)
		{
			CheckCollision(*m_pPlayerGO);
		}
	}
}
void SCLMapItem::NotifyCollision(SCLGameObject& OtherGO)
{
	// Process pickups type only
	if (m_Type < MAP_ITEM_PICKUP_1 || m_Type > MAP_ITEM_PICKUP_4)
	{
		return;
	}

	switch (OtherGO.GetCollisionLayer())
	{
	case LAYER_PLAYER:
		m_IsActive = false;
		OtherGO.NotifyCollision(*this);
		break;
	}
}

SCLMapItemType SCLMapItem::GetType() const
{
	return m_Type;
}

void SCLMapItem::SetPlayerGameObject(SCLGameObject* pPlayerGO)
{
	m_pPlayerGO = pPlayerGO;
}

// TODO: Put these somewhere else

SCLTexture SCLMapItem::GetTextureFromType(SCLMapItemType Type)
{
	switch (Type)
	{
	case MAP_ITEM_BG:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_BG);
	case MAP_ITEM_DECOR:
		return GetRandomDecorTexture();
	case MAP_ITEM_STATIC:
		return GetRandomStaticItemTexture();
	case MAP_ITEM_PICKUP_START:
	case MAP_ITEM_PICKUP_1:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Pickup1);
	case MAP_ITEM_PICKUP_2:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Pickup2);
	case MAP_ITEM_PICKUP_3:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Pickup3);
	case MAP_ITEM_PICKUP_4:
	case MAP_ITEM_PICKUP_END:
		return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_Pickup4);
	}
	return SCLResourceManager::GetLoadedTexture(SCLResources::TextureType_BG);
}

float SCLMapItem::GetSizeFromType(SCLMapItemType Type)
{
	switch (Type)
	{
	case MAP_ITEM_BG:
		return SCLConstants::MAP_BG_SIZE;
	case MAP_ITEM_DECOR:
		return SCLConstants::MAP_DECOR_SIZE;
	case MAP_ITEM_STATIC:
		return SCLConstants::MAP_STATIC_ITEM_SIZE;
	case MAP_ITEM_PICKUP_START:
	case MAP_ITEM_PICKUP_1:
	case MAP_ITEM_PICKUP_2:
	case MAP_ITEM_PICKUP_3:
	case MAP_ITEM_PICKUP_4:
	case MAP_ITEM_PICKUP_END:
		return SCLConstants::PICKUP_SIZE;
	}
	return 1.0f;
}

SCLCollisionLayer SCLMapItem::GetLayerFromType(SCLMapItemType Type)
{
	switch (Type)
	{
	case MAP_ITEM_BG:
	case MAP_ITEM_DECOR:
		return LAYER_DEFAULT;
	case MAP_ITEM_STATIC:
		return LAYER_MAP;
	case MAP_ITEM_PICKUP_1:
	case MAP_ITEM_PICKUP_2:
	case MAP_ITEM_PICKUP_3:
	case MAP_ITEM_PICKUP_4:
		return LAYER_PICKUP;
	}
	return LAYER_DEFAULT;
}

SCLTexture SCLMapItem::GetRandomDecorTexture()
{
	int Rand = SCLRandom::GetRandomInt(SCLResources::TextureType_Grass1,
										SCLResources::TextureType_Grass3);
	return SCLResourceManager::GetLoadedTexture((SCLResources::TextureType)Rand);
}

SCLTexture SCLMapItem::GetRandomStaticItemTexture()
{
	int Rand = SCLRandom::GetRandomInt(SCLResources::TextureType_Bush1,
										SCLResources::TextureType_Rock1);
	return SCLResourceManager::GetLoadedTexture((SCLResources::TextureType)Rand);
}