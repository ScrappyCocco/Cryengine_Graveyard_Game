// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <Components/Lights/ExpandedProjectorLight/DefaultProjectorLightComponent/DefaultProjectorLightComponent.h>

////////////////////////////////////////////////////////
// Expanded Point Light
////////////////////////////////////////////////////////
class CExpandedProjectorLightComponent final : public Cry::DefaultComponents::CDefaultProjectorLightComponent
{
public:
	CExpandedProjectorLightComponent() = default;
	virtual ~CExpandedProjectorLightComponent() = default;

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CExpandedProjectorLightComponent>& desc)
	{
		desc.SetGUID("{E45AEF30-8CFE-4F88-BF70-BCE7296AB254}"_cry_guid);
		desc.SetEditorCategory("Lights");
		desc.SetLabel("Expanded Projector Light");
		desc.SetDescription("Expand the basic Projector Light adding more functions to edit protected fields");
	}
	
	float GetLightAngle() const;
	void SetLightAngle(float angle = 45.0);

	void SetDiffuseIntensity(float intensity = 1.f);
	void SetSpecularIntensity(float intensity = 1.f);
	void SetDiffuseAndSpecularIntensity(float intensity = 1.f);
	float GetSpecularIntensity() const;
	float GetDiffuseIntensity() const;

	void SetLightColor(const ColorF& color = ColorF(1.f));

	void ToggleTorch();

	//This call will enable shadows for the light
	//Changing the min graphics from None to Low, enabling them
	void EnableShadows();
};
