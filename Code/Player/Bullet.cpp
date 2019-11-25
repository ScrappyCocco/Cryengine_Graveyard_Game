#include "StdAfx.h"
#include "Bullet.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CryEntitySystem/IEntitySystem.h>
#include <CryNetwork/Rmi.h>
#include <CryPhysics/physinterface.h>
#include <Components/ExpandedPointLight/ExpandedPointLightComponent.h>
#include <DefaultComponents/Lights/ILightComponent.h>

void CBulletComponent::Initialize()
{
	// Set the model
	const int geometrySlot = 0;
	m_pEntity->LoadGeometry(geometrySlot, "%ENGINE%/EngineAssets/Objects/primitive_sphere.cgf");

	// Load the custom bullet material.
	// This material has the 'mat_bullet' surface type applied, which is set up to play sounds on collision with 'mat_default' objects in Libs/MaterialEffects
	IMaterial* pBulletMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/bullet_emissive");
	m_pEntity->SetMaterial(pBulletMaterial);

	// Add light to bullet and set its proprieties
	CExpandedPointLightComponent* pBulletLight = m_pEntity->GetOrCreateComponent<CExpandedPointLightComponent>();
	pBulletLight->Enable(false); //Disable the light to set its new values
	
	pBulletLight->GetOptions().m_giMode = Cry::DefaultComponents::ELightGIMode::DynamicLight;
	//Get material emission color
	const ColorF colorValue = pBulletMaterial->GetShaderItem().m_pShaderResources->GetColorValue(EFTT_EMITTANCE);
	pBulletLight->SetLightColor(colorValue);
	//Set intensity and details
	pBulletLight->SetDiffuseAndSpecularIntensity(1.f);
	pBulletLight->SetLightViewDistance(100);
	pBulletLight->SetLightRadius(0.3f);
	pBulletLight->Enable(true); //Re-enable the light to render it with new values

	// Now create the physical representation of the entity
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_RIGID;
	physParams.mass = 20000.f;
	m_pEntity->Physicalize(physParams);

	// Make sure that bullets are always rendered regardless of distance
	// Ratio is 0 - 255, 255 being 100% visibility
	GetEntity()->SetViewDistRatio(255);

	// Apply an impulse so that the bullet flies forward
	if (auto* pPhysics = GetEntity()->GetPhysics())
	{
		pe_action_impulse impulseAction;

		const float initialVelocity = 1000.f;

		// Set the actual impulse, in this cause the value of the initial velocity CVar in bullet's forward direction
		impulseAction.impulse = GetEntity()->GetWorldRotation().GetColumn1() * initialVelocity;

		// Send to the physical entity
		pPhysics->Action(&impulseAction);
	}
}

Cry::Entity::EventFlags CBulletComponent::GetEventMask() const
{
	return ENTITY_EVENT_COLLISION;
}

void CBulletComponent::ProcessEvent(const SEntityEvent& event)
{
	// Handle the OnCollision event, in order to have the entity removed on collision
	if (event.event == ENTITY_EVENT_COLLISION)
	{
		// Collision info can be retrieved using the event pointer
		//EventPhysCollision *physCollision = reinterpret_cast<EventPhysCollision *>(event.ptr);

		// Queue removal of this entity, unless it has already been done
		gEnv->pEntitySystem->RemoveEntity(GetEntityId());
	}
}
