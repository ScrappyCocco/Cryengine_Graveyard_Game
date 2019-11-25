#pragma once

#include <DefaultComponents/Lights/ILightComponent.h>

class CPlugin_CryDefaultEntities;

namespace Cry
{
	namespace DefaultComponents
	{
		class DefaultPointLightComponent
			: public ILightComponent
#ifndef RELEASE
			, public IEntityComponentPreviewer
#endif
		{
		protected:
			friend CPlugin_CryDefaultEntities;
			static void Register(Schematyc::CEnvRegistrationScope& componentScope);

			// IEntityComponent
			virtual void Initialize() final;

			virtual void ProcessEvent(const SEntityEvent& event) final;
			virtual Cry::Entity::EventFlags GetEventMask() const final;

#ifndef RELEASE
			virtual IEntityComponentPreviewer* GetPreviewer() final { return this; }
#endif
			// ~IEntityComponent

#ifndef RELEASE
			// IEntityComponentPreviewer
			virtual void SerializeProperties(Serialization::IArchive& archive) final {}

			virtual void Render(const IEntity& entity, const IEntityComponent& component, SEntityPreviewContext &context) const final;
			// ~IEntityComponentPreviewer
#endif

		public:
			DefaultPointLightComponent() = default;
			virtual ~DefaultPointLightComponent() = default;

			virtual void SetOptics(const char* szOpticsFullName) override
			{
				m_optics.m_lensFlareName = szOpticsFullName;

				Initialize();
			}

			virtual void Enable(bool enable) override;

			static void ReflectType(Schematyc::CTypeDesc<DefaultPointLightComponent>& desc)
			{
				desc.SetGUID("{19BAAD5C-3B76-45B6-8EAA-C92F2EE0D996}"_cry_guid);
				desc.SetEditorCategory("Lights");
				desc.SetLabel("Point Light");
				desc.SetDescription("Emits light from its origin into all directions");
				desc.SetIcon("icons:ObjectTypes/light.ico");
				desc.SetComponentFlags({ IEntityComponent::EFlags::NoCreationOffset, IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach, IEntityComponent::EFlags::ClientOnly });

				desc.AddMember(&DefaultPointLightComponent::m_bActive, 'actv', "Active", "Active", "Determines whether the light is enabled", true);
				desc.AddMember(&DefaultPointLightComponent::m_radius, 'radi', "Radius", "Radius", "Determines the range of the point light", 10.f);
				desc.AddMember(&DefaultPointLightComponent::m_viewDistance, 'view', "ViewDistance", "View Distance", "Determines the distance in which this light will be active", 50);

				desc.AddMember(&DefaultPointLightComponent::m_optics, 'opti', "Optics", "Optics", "Specific Optic Options", DefaultPointLightComponent::SOptics());
				desc.AddMember(&DefaultPointLightComponent::m_color, 'colo', "Color", "Color", "Color emission information", DefaultPointLightComponent::SColor());
				desc.AddMember(&DefaultPointLightComponent::m_shadows, 'shad', "Shadows", "Shadows", "Shadow casting settings", DefaultPointLightComponent::SShadows());
				desc.AddMember(&DefaultPointLightComponent::m_options, 'opt', "Options", "Options", "Specific Light Options", DefaultPointLightComponent::SOptions());
				desc.AddMember(&DefaultPointLightComponent::m_animations, 'anim', "Animations", "Animations", "Light style / animation properties", DefaultPointLightComponent::SAnimations());
				desc.AddMember(&DefaultPointLightComponent::m_shape, 'shap', "Shape", "Shape", "Area light shape properties", DefaultPointLightComponent::SShape());
			}

		protected:
			Schematyc::Range<0, 32768> m_radius = 10.f;
			Schematyc::Range<0, 100, 0, 100, int> m_viewDistance = 50;
		};
	}
}