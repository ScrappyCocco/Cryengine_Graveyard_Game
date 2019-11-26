#include "StdAfx.h"
#include "DefaultPointLightComponent.h"

#include <ILevelSystem.h>
#include <CrySystem/IProjectManager.h>
#include <Cry3DEngine/IRenderNode.h>
#include <CryRenderer/IRenderAuxGeom.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>

namespace Cry
{
	namespace DefaultComponents
	{
		void CDefaultPointLightComponent::Register(Schematyc::CEnvRegistrationScope& componentScope)
		{
			// Functions
			{
				auto pFunction = SCHEMATYC_MAKE_ENV_FUNCTION(&CDefaultPointLightComponent::Enable, "{415ECD3F-880C-4647-92CB-2A8F2321709D}"_cry_guid, "Enable");
				pFunction->SetDescription("Enables or disables the light component");
				pFunction->SetFlags({ Schematyc::EEnvFunctionFlags::Member, Schematyc::EEnvFunctionFlags::Construction });
				pFunction->BindInput(1, 'ena', "Enable");
				componentScope.Register(pFunction);
			}
		}

		void CDefaultPointLightComponent::Initialize()
		{
			if (!m_bActive)
			{
				FreeEntitySlot();

				return;
			}

			SRenderLight light;

			light.m_nLightStyle = m_animations.m_style;
			light.SetAnimSpeed(m_animations.m_speed);

			light.SetPosition(ZERO);
			light.m_Flags = DLF_DEFERRED_LIGHT | DLF_POINT;

			light.SetLightColor(m_color.m_color * m_color.m_diffuseMultiplier);
			light.SetSpecularMult(m_color.m_specularMultiplier);

			light.m_nAreaShape = (uint8)m_shape.m_areaShape;
			light.m_bAreaTwoSided = m_shape.m_twoSided;
			light.m_fAreaWidth = m_shape.m_width;
			light.m_fAreaHeight = m_shape.m_height;
			light.m_bAreaTextured = true;

			light.m_fHDRDynamic = 0.f;

			// Area Light
			if (m_shape.m_areaShape != ELightShape::Point)
			{
				const char* szProjectorTexturePath = m_shape.m_texturePath.value;
				if (szProjectorTexturePath[0] == '\0')
				{
					light.m_bAreaTextured = false;

					if (m_shape.m_areaShape == ELightShape::Rectangle)
						szProjectorTexturePath = "%ENGINE%/EngineAssets/Textures/Lights/white_rectangle_areatex.dds";
					else if (m_shape.m_areaShape == ELightShape::Disk)
						szProjectorTexturePath = "%ENGINE%/EngineAssets/Textures/Lights/white_disk_areatex.dds";
				}

				light.m_pLightImage = gEnv->pRenderer->EF_LoadTexture(szProjectorTexturePath, 0);

				if ((light.m_pLightImage == nullptr || !light.m_pLightImage->IsTextureLoaded()) && light.m_pLightDynTexSource == nullptr)
				{
					CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Light projector texture %s not found, disabling projector component for entity %s", szProjectorTexturePath, m_pEntity->GetName());
					FreeEntitySlot();
					return;
				}
				light.m_Flags |= DLF_AREA;
			}

			if (m_options.m_bAffectsOnlyThisArea)
				light.m_Flags |= DLF_THIS_AREA_ONLY;

			if (m_options.m_bIgnoreVisAreas)
				light.m_Flags |= DLF_IGNORES_VISAREAS;

			if (m_options.m_bVolumetricFogOnly)
				light.m_Flags |= DLF_VOLUMETRIC_FOG_ONLY;

			if (m_options.m_bAffectsVolumetricFog)
				light.m_Flags |= DLF_VOLUMETRIC_FOG;

			if (m_options.m_bLinkToSkyColor)
				light.m_Flags |= DLF_LINK_TO_SKY_COLOR;

			if (m_options.m_bAmbient)
				light.m_Flags |= DLF_AMBIENT;

			bool shouldCastShadows = false;
			if (m_shadows.m_castShadowSpec != EMiniumSystemSpec::Disabled)
			{
				const int sysSpec = gEnv->pSystem->GetConfigSpec();
				if (sysSpec != CONFIG_CUSTOM)
				{
					shouldCastShadows = sysSpec >= static_cast<int>(m_shadows.m_castShadowSpec);
				}
				else
				{
					if (ICVar* const pSysSpecShadow = gEnv->pConsole->GetCVar("sys_spec_shadow"))
						shouldCastShadows = pSysSpecShadow->GetIVal() >= static_cast<int>(m_shadows.m_castShadowSpec);
				}
			}

			if (shouldCastShadows)
			{
				light.m_Flags |= DLF_CASTSHADOW_MAPS;

				light.SetShadowBiasParams(m_shadows.m_shadowBias, m_shadows.m_shadowSlopeBias);
				light.m_fShadowUpdateMinRadius = m_shadows.m_shadowUpdateMinRadius;
				light.m_fShadowResolutionScale = m_shadows.m_shadowResolutionScale;
				light.m_nShadowMinResolution = m_shadows.m_shadowMinResolution;

				light.m_nShadowUpdateRatio = max((uint16)1, (uint16)(m_shadows.m_shadowUpdateRatio * (1 << DL_SHADOW_UPDATE_SHIFT)));
			}
			else
				light.m_Flags &= ~DLF_CASTSHADOW_MAPS;

			light.SetRadius(m_radius, m_options.m_attenuationBulbSize);

			light.m_fFogRadialLobe = m_options.m_fogRadialLobe;

			m_pEntity->UpdateLightClipBounds(light);

			if (!m_optics.m_lensFlareName.empty() && m_optics.m_flareEnable)
			{
				int32 opticsIndex = 0;
				if (gEnv->pOpticsManager->Load(m_optics.m_lensFlareName.c_str(), opticsIndex))
				{
					IOpticsElementBase* pOpticsElement = gEnv->pOpticsManager->GetOptics(opticsIndex);
					light.SetLensOpticsElement(pOpticsElement);

					const int32 modularAngle = m_optics.m_flareFOV % 360;
					if (modularAngle == 0)
						light.m_LensOpticsFrustumAngle = 255;
					else
						light.m_LensOpticsFrustumAngle = (uint8)(m_optics.m_flareFOV * (255.0f / 360.0f));

					if (m_optics.m_attachToSun)
					{
						light.m_Flags |= DLF_ATTACH_TO_SUN | DLF_FAKE | DLF_IGNORES_VISAREAS;
						light.m_Flags &= ~DLF_THIS_AREA_ONLY;
					}
				}
			}

			// Load the light source into the entity
			const int slot = m_pEntity->LoadLight(GetOrMakeEntitySlotId(), &light);

			uint32 slotFlags = m_pEntity->GetSlotFlags(GetEntitySlotId());
			UpdateGIModeEntitySlotFlags((uint8)m_options.m_giMode, slotFlags);
			m_pEntity->SetSlotFlags(GetEntitySlotId(), slotFlags);

			if (IRenderNode* pRenderNode = m_pEntity->GetSlotRenderNode(slot))
			{
				int viewDistance = static_cast<int>((m_viewDistance / 100.0f) * 255.0f);
				pRenderNode->SetViewDistRatio(viewDistance);
			}

			IMaterial* pMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial(g_szDefaultLensFlareMaterialName);
			if (pMaterial && m_optics.m_flareEnable)
				m_pEntity->SetSlotMaterial(slot, pMaterial);
		}

		void CDefaultPointLightComponent::ProcessEvent(const SEntityEvent& event)
		{
			if (event.event == ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED || event.event == ENTITY_EVENT_LINK || event.event == ENTITY_EVENT_DELINK)
			{
				Initialize();
			}
		}

		Cry::Entity::EventFlags CDefaultPointLightComponent::GetEventMask() const
		{
			return ENTITY_EVENT_COMPONENT_PROPERTY_CHANGED | ENTITY_EVENT_LINK | ENTITY_EVENT_DELINK;
		}

#ifndef RELEASE
		void CDefaultPointLightComponent::Render(const IEntity& entity, const IEntityComponent& component, SEntityPreviewContext &context) const
		{
			if (context.bSelected)
			{
				Matrix34 slotTransform = GetWorldTransformMatrix();

				Vec3 pos = slotTransform.GetTranslation();

				SRenderLight light;
				light.SetLightColor(m_color.m_color * m_color.m_diffuseMultiplier);
				light.SetRadius(m_radius, m_options.m_attenuationBulbSize);
				float radius = light.m_fRadius;

				Vec3 p0, p1;
				float step = 10.0f / 180 * gf_PI;
				float angle;

				// Z Axis
				p0 = pos;
				p1 = pos;
				p0.x += radius * sin(0.0f);
				p0.y += radius * cos(0.0f);
				for (angle = step; angle < 360.0f / 180 * gf_PI + step; angle += step)
				{
					p1.x = pos.x + radius * sin(angle);
					p1.y = pos.y + radius * cos(angle);
					p1.z = pos.z;
					gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(p0, context.debugDrawInfo.color, p1, context.debugDrawInfo.color);
					p0 = p1;
				}

				// X Axis
				p0 = pos;
				p1 = pos;
				p0.y += radius * sin(0.0f);
				p0.z += radius * cos(0.0f);
				for (angle = step; angle < 360.0f / 180 * gf_PI + step; angle += step)
				{
					p1.x = pos.x;
					p1.y = pos.y + radius * sin(angle);
					p1.z = pos.z + radius * cos(angle);
					gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(p0, context.debugDrawInfo.color, p1, context.debugDrawInfo.color);
					p0 = p1;
				}

				// Y Axis
				p0 = pos;
				p1 = pos;
				p0.x += radius * sin(0.0f);
				p0.z += radius * cos(0.0f);
				for (angle = step; angle < 360.0f / 180 * gf_PI + step; angle += step)
				{
					p1.x = pos.x + radius * sin(angle);
					p1.y = pos.y;
					p1.z = pos.z + radius * cos(angle);
					gEnv->pRenderer->GetIRenderAuxGeom()->DrawLine(p0, context.debugDrawInfo.color, p1, context.debugDrawInfo.color);
					p0 = p1;
				}
			}
		}
#endif

		void CDefaultPointLightComponent::Enable(bool bEnable)
		{
			m_bActive = bEnable;

			Initialize();
		}
	}
}
