cmake_minimum_required (VERSION 3.14)
set(CRYENGINE_DIR "C:/BrokenPathSoftware/Folder With Spaces/Crytek/CRYENGINE_5.6" CACHE STRING "CRYENGINE root directory.")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/InitialSetup.cmake")
include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Root"
		"StdAfx.cpp"
		"StdAfx.h"
)
add_sources("Components_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Components"
)
add_sources("Lights_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Components\\\\Lights"
)
add_sources("ExpandedPointLight_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Components\\\\Lights\\\\ExpandedPointLight"
		"Components/Lights/ExpandedPointLight/ExpandedPointLightComponent.cpp"
		"Components/Lights/ExpandedPointLight/ExpandedPointLightComponent.h"
)
add_sources("DefaultPointLightComponent_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Components\\\\Lights\\\\ExpandedPointLight\\\\DefaultPointLightComponent"
		"Components/Lights/ExpandedPointLight/DefaultPointLightComponent/DefaultPointLightComponent.cpp"
		"Components/Lights/ExpandedPointLight/DefaultPointLightComponent/DefaultPointLightComponent.h"
)
add_sources("ExpandedProjectorLight_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Components\\\\Lights\\\\ExpandedProjectorLight"
		"Components/Lights/ExpandedProjectorLight/ExpandedProjectorLightComponent.cpp"
		"Components/Lights/ExpandedProjectorLight/ExpandedProjectorLightComponent.h"
)
add_sources("DefaultProjectorLightComponent_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Components\\\\Lights\\\\ExpandedProjectorLight\\\\DefaultProjectorLightComponent"
		"Components/Lights/ExpandedProjectorLight/DefaultProjectorLightComponent/DefaultProjectorLightComponent.cpp"
		"Components/Lights/ExpandedProjectorLight/DefaultProjectorLightComponent/DefaultProjectorLightComponent.h"
)
add_sources("SpawnPoint_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Components\\\\SpawnPoint"
		"Components/SpawnPoint/SpawnPoint.cpp"
		"Components/SpawnPoint/SpawnPoint.h"
)
add_sources("GamePlugin_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "GamePlugin"
		"GamePlugin/GamePlugin.cpp"
		"GamePlugin/GamePlugin.h"
)
add_sources("Player_uber.cpp"
    PROJECTS GraveyardGame
    SOURCE_GROUP "Player"
		"Player/Bullet.cpp"
		"Player/Player.cpp"
		"Player/Bullet.h"
		"Player/Player.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryGameModule(GraveyardGame FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-variable
        -Wno-reorder
        -Wno-unknown-pragmas
        -Wno-parentheses
        -Wno-switch
        -Wno-format
        -Wno-dynamic-class-memaccess
        -Wno-unused-private-field
        -Wno-unused-value
        -Wno-invalid-offsetof
        -Wno-multichar
        -Wno-char-subscripts
        -Wno-null-conversion
        -Wno-empty-body
        -Wno-unused-lambda-capture
        -Wno-unused-function
        -Wno-tautological-constant-out-of-range-compare
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-aligned-new
        -Wno-int-in-bool-context
        -Wno-invalid-offsetof
        -Wno-comment
        -Wno-sign-compare
        -Wno-unused-but-set-variable
        -Wno-maybe-uninitialized
        -Wno-misleading-indentation
        -Wno-unused-result
        -Wno-unknown-pragmas
        -Wno-multichar
        -Wno-strict-aliasing
        -Wno-char-subscripts
        -Wno-conversion-null
        -Wno-reorder
        -Wno-parentheses
        -Wno-format
        -Wno-switch
    )
endif()


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
        add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
        set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
        if (WIN32)
            set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/GraveyardGame.cryproject\"")
        endif()
    endif()
else()
    add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/GraveyardGame.cryproject\"")
    endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}/GraveyardGame.cryproject\"")
    endif()

    add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}/GraveyardGame.cryproject\"")
    endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}/GraveyardGame.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
#END-CUSTOM