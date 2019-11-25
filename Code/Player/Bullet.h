#pragma once

////////////////////////////////////////////////////////
// Physicalized bullet shot from weaponry, expires on collision with another object
////////////////////////////////////////////////////////
class CBulletComponent final : public IEntityComponent
{
public:
	virtual ~CBulletComponent() = default;

	// IEntityComponent
	virtual void Initialize() override;

	// Reflect type to set a unique identifier for this component
	static void ReflectType(Schematyc::CTypeDesc<CBulletComponent>& desc)
	{
		desc.SetGUID("{B53A9A5F-F27A-42CB-82C7-B1E379C41A2A}"_cry_guid);
	}

	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
};