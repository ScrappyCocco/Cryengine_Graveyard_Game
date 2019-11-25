#include "StdAfx.h"
#include "ExpandedPointLightComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryCore/StaticInstanceList.h>

void CExpandedPointLightComponent::SetLightRadius(const float radius)
{
	if (radius < 0)
	{
		m_radius = 0.0f;
	}
	else if (radius > 32768)
	{
		m_radius = 32768.f;
	}
	else
	{
		m_radius = radius;
	}
}

float CExpandedPointLightComponent::GetLightRadius() const
{
	return m_radius;
}

void CExpandedPointLightComponent::SetLightViewDistance(const int distance)
{
	if (distance < 0)
	{
		m_viewDistance = 0;
	}
	else if (distance > 100)
	{
		m_viewDistance = 100;
	}
	else
	{
		m_viewDistance = distance;
	}
}

int CExpandedPointLightComponent::GetLightViewDistance() const
{
	return m_viewDistance;
}

void CExpandedPointLightComponent::SetDiffuseIntensity(const float intensity)
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

void CExpandedPointLightComponent::SetSpecularIntensity(const float intensity)
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

void CExpandedPointLightComponent::SetDiffuseAndSpecularIntensity(const float intensity)
{
	SetDiffuseIntensity(intensity);
	SetSpecularIntensity(intensity);
}

float CExpandedPointLightComponent::GetSpecularIntensity() const
{
	return m_color.m_specularMultiplier;
}

float CExpandedPointLightComponent::GetDiffuseIntensity() const
{
	return m_color.m_diffuseMultiplier;
}

void CExpandedPointLightComponent::SetLightColor(const ColorF& color)
{
	m_color.m_color = color;
}

static void RegisterExpandedPointLightComponent(Schematyc::IEnvRegistrar& registrar)
{
	Schematyc::CEnvRegistrationScope scope = registrar.Scope(IEntity::GetEntityScopeGUID());
	{
		Schematyc::CEnvRegistrationScope componentScope = scope.Register(
			SCHEMATYC_MAKE_ENV_COMPONENT(CExpandedPointLightComponent));
		// Functions
		{
		}
	}
}

CRY_STATIC_AUTO_REGISTER_FUNCTION(&RegisterExpandedPointLightComponent)
