#include "StdAfx.h"
#include "ExpandedProjectorLightComponent.h"

#include <CrySchematyc/Env/Elements/EnvComponent.h>
#include <CrySchematyc/Env/Elements/EnvFunction.h>
#include <CryCore/StaticInstanceList.h>

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
