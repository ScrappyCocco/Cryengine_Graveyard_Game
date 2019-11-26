#include "StdAfx.h"
#include "ExpandedProjectorLightComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryCore/StaticInstanceList.h>

float CExpandedProjectorLightComponent::GetLightAngle() const
{
	return m_angle.ToDegrees();
}

void CExpandedProjectorLightComponent::SetLightAngle(float angle)
{
	m_angle = CryTransform::CAngle::FromDegrees(angle);
}

void CExpandedProjectorLightComponent::SetDiffuseIntensity(const float intensity)
{
	if (intensity < 0)
	{
		m_color.m_diffuseMultiplier = 0.0f;
	}
	else if (intensity > 10000)
	{
		m_color.m_diffuseMultiplier = 10000.0f;
	}
	else
	{
		m_color.m_diffuseMultiplier = intensity;
	}
}

void CExpandedProjectorLightComponent::SetSpecularIntensity(const float intensity)
{
	if (intensity < 0)
	{
		m_color.m_specularMultiplier = 0.0f;
	}
	else if (intensity > 10000)
	{
		m_color.m_specularMultiplier = 10000.f;
	}
	else
	{
		m_color.m_specularMultiplier = intensity;
	}
}

void CExpandedProjectorLightComponent::SetDiffuseAndSpecularIntensity(const float intensity)
{
	SetDiffuseIntensity(intensity);
	SetSpecularIntensity(intensity);
}

float CExpandedProjectorLightComponent::GetSpecularIntensity() const
{
	return m_color.m_specularMultiplier;
}

float CExpandedProjectorLightComponent::GetDiffuseIntensity() const
{
	return m_color.m_diffuseMultiplier;
}

void CExpandedProjectorLightComponent::SetLightColor(const ColorF& color)
{
	m_color.m_color = color;
}

void CExpandedProjectorLightComponent::ToggleTorch()
{
	Enable(!m_bActive);
}

static void RegisterExpandedPointLightComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(
			SCHEMATYC_MAKE_ENV_COMPONENT(CExpandedProjectorLightComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterExpandedPointLightComponent)
