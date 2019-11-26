#pragma once

#include <DefaultComponents/Lights/ILightComponent.h>

class CPlugin_CryDefaultEntities;

namespace Cry
{
	namespace DefaultComponents
	{
		class CDefaultProjectorLightComponent
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
			CDefaultProjectorLightComponent() {}
			virtual ~CDefaultProjectorLightComponent() {}

			virtual void SetOptics(const char* szOpticsFullName) override
			{
				m_optics.m_lensFlareName = szOpticsFullName;

				Initialize(); 
			}

			static void ReflectType(Schematyc::CTypeDesc<CDefaultProjectorLightComponent>& desc)
			{
				desc.SetGUID("{45649625-EA96-4D22-B79E-420F691AF097}"_cry_guid);
				desc.SetEditorCategory("Lights");
				desc.SetLabel("Projector Light");
				desc.SetDescription("Emits light from its position in a general direction, constrained to a specified angle");
				desc.SetIcon("icons:ObjectTypes/light.ico");
				desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach, IEntityComponent::EFlags::ClientOnly, IEntityComponent::EFlags::NoCreationOffset });

				desc.AddMember(&CDefaultProjectorLightComponent::m_bActive, 'actv', "Active", "Active", "Determines whether the light is enabled", true);
				desc.AddMember(&CDefaultProjectorLightComponent::m_radius, 'radi', "Radius", "Range", "Determines whether the range of the light", 10.f);
				desc.AddMember(&CDefaultProjectorLightComponent::m_angle, 'angl', "Angle", "Angle", "Maximum angle to emit light to, from the light's forward axis.", 45.0_degrees);

				desc.AddMember(&CDefaultProjectorLightComponent::m_projectorOptions, 'popt', "ProjectorOptions", "Projector Options", nullptr, CDefaultProjectorLightComponent::SProjectorOptions());

				desc.AddMember(&CDefaultProjectorLightComponent::m_optics, 'opti', "Optics", "Optics", "Specific Optic Options", SOptics());
				desc.AddMember(&CDefaultProjectorLightComponent::m_color, 'colo', "Color", "Color", "Color emission information", SColor());
				desc.AddMember(&CDefaultProjectorLightComponent::m_shadows, 'shad', "Shadows", "Shadows", "Shadow casting settings", SShadows());
				desc.AddMember(&CDefaultProjectorLightComponent::m_options, 'opt', "Options", "Options", "Specific Light Options", SOptions());
				desc.AddMember(&CDefaultProjectorLightComponent::m_animations, 'anim', "Animations", "Animations", "Light style / animation properties", SAnimations());
			}

			struct SProjectorOptions
			{
				inline bool operator==(const SProjectorOptions &rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }

				void SetTexturePath(const char* szPath);
				const char* GetTexturePath() const { return m_texturePath.value.c_str(); }
				void SetMaterialPath(const char* szPath);
				const char* GetMaterialPath() const { return m_materialPath.value.c_str(); }
				bool HasMaterialPath() const { return m_materialPath.value.size() > 0; }

				static void ReflectType(Schematyc::CTypeDesc<SProjectorOptions>& desc)
				{
					desc.SetGUID("{C5009BCF-BEF3-4B8E-9DAD-069C0723613A}"_cry_guid);
					desc.AddMember(&CDefaultProjectorLightComponent::SProjectorOptions::m_nearPlane, 'near', "NearPlane", "Near Plane", nullptr, 0.f);
					desc.AddMember(&CDefaultProjectorLightComponent::SProjectorOptions::m_texturePath, 'tex', "Texture", "Projected Texture", "Path to a texture we want to emit", "");
					desc.AddMember(&CDefaultProjectorLightComponent::SProjectorOptions::m_materialPath, 'mat', "Material", "Material", "Path to a material we want to apply to the projector", "");
				}

				Schematyc::Range<0, 10000> m_nearPlane = 0.f;

				Schematyc::TextureFileName m_texturePath;
				Schematyc::MaterialFileName m_materialPath;
			};

			struct SFlare
			{
				inline bool operator==(const SFlare &rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }

				void SetTexturePath(const char* szPath);
				const char* GetTexturePath() const { return m_texturePath.value.c_str(); }
				bool HasTexturePath() const { return m_texturePath.value.size() > 0; }

				static void ReflectType(Schematyc::CTypeDesc<SFlare>& desc)
				{
					desc.SetGUID("{E3347C12-9FA4-4847-94FC-0F604F38EEC0}"_cry_guid);
					desc.AddMember(&CDefaultProjectorLightComponent::SFlare::m_angle, 'angl', "Angle", "Angle", nullptr, 360.0_degrees);
					desc.AddMember(&CDefaultProjectorLightComponent::SFlare::m_texturePath, 'tex', "Texture", "Flare Texture", "Path to the flare texture we want to use", "");
				}

				CryTransform::CClampedAngle<5, 360> m_angle = 360.0_degrees;

				Schematyc::TextureFileName m_texturePath;
			};

			virtual void Enable(bool bEnable) override;

			virtual void SetRadius(float radius) { m_radius = radius; }
			float GetRadius() const { return m_radius; }

			virtual void SetFieldOfView(CryTransform::CAngle angle) { m_angle = angle; }
			CryTransform::CAngle GetFieldOfView() { return m_angle; }

			virtual void SetNearPlane(float nearPlane) { m_projectorOptions.m_nearPlane = nearPlane; }
			float GetNearPlane() const { return m_projectorOptions.m_nearPlane; }

			virtual void SetFlareAngle(CryTransform::CAngle angle) { m_flare.m_angle = angle; }
			CryTransform:: CAngle GetFlareAngle() const { return m_flare.m_angle; }

		protected:
			bool m_bActive = true;
			Schematyc::Range<0, std::numeric_limits<int>::max()> m_radius = 10.f;
			CryTransform::CClampedAngle<0, 180> m_angle = 45.0_degrees;

			SProjectorOptions m_projectorOptions;

			SFlare m_flare;
		};
	}
}