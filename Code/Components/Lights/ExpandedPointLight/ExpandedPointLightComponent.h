// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Components/Lights/ExpandedPointLight/DefaultPointLightComponent/DefaultPointLightComponent.h>

////////////////////////////////////////////////////////
// Expanded Point Light
////////////////////////////////////////////////////////
class CExpandedPointLightComponent final : public Cry::DefaultComponents::CDefaultPointLightComponent
{
public:
	CExpandedPointLightComponent() = default;
	virtual ~CExpandedPointLightComponent() = default;

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CExpandedPointLightComponent>& desc)
	{
		desc.SetGUID("{56C25616-4922-495B-BD9E-B50B74D7733D}"_cry_guid);
		desc.SetEditorCategory("Lights");
		desc.SetLabel("Expanded Point Light");
		desc.SetDescription("Expand the basic Point Light adding more functions to edit protected fields");
	}

	void SetLightRadius(float radius = 10.f);
	float GetLightRadius() const;

	void SetLightViewDistance(int distance = 50);
	int GetLightViewDistance() const;

	void SetDiffuseIntensity(float intensity = 1.f);
	void SetSpecularIntensity(float intensity = 1.f);
	void SetDiffuseAndSpecularIntensity(float intensity = 1.f);
	float GetSpecularIntensity() const;
	float GetDiffuseIntensity() const;

	void SetLightColor(const ColorF& color = ColorF(1.f));
	
	//This call will enable shadows for the light
	//Changing the min graphics from None to Low, enabling them
	void EnableShadows();
};
