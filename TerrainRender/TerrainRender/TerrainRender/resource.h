#ifndef TERRAIN_RENDER_RESOURCE_H
#define TERRAIN_RENDER_RESOURCE_H

// -------------------------------------
//	
//	Message ID for Terrain Model (IDM)
//
// -------------------------------------

enum IModelMessageIDs
{
	IDM_INVALID							,

	IDM_ACTIVATE_FLYTHROUGH_MODE		,
	IDM_ACTIVATE_3DEXPLORE_MODE			,

	IDM_LOAD_TERRAIN_SHARP				,
	IDM_LOAD_PROJECT_SHARP				,
	IDM_LOAD_TERRAIN_SOFT				,
	IDM_LOAD_PROJECT_SOFT				,
	IDM_LOAD_CAMERA_TRAJECTORY			,
	IDM_LOAD_CONFIGURATION				,

	IDM_E3D_MOVE_FORWARD				,
	IDM_E3D_MOVE_BACK					,
	IDM_E3D_MOVE_LEFT					,
	IDM_E3D_MOVE_RIGHT					,
	IDM_E3D_MOVE_UP						,
	IDM_E3D_MOVE_DOWN					,
	IDM_E3D_ROTATE						,
	IDM_E3D_SET_SPEED					,
	IDM_E3D_SET_ROTATION_SPEED			,
	IDM_E3D_CAMERA_RESET				,

	IDM_SET_CAMERA_FIELD_OF_VIEW		,
	IDM_SET_CAMERA_NEAR_SCREEN			,
	IDM_SET_CAMERA_FAR_SCREEN			,

	IDM_FLYTHROUGH_NEXT_FRAME			,
	IDM_FLYTHROUGH_START_POSITION		,
	IDM_FLYTHROUGH_STOP					,
	IDM_FLYTHROUGH_SET_FRAME			,
	IDM_FLYTHROUGH_SET_SPEED			,

	IDM_SET_TIME_E3D					,
	IDM_SET_START_TIME_TRAJECTORY		,

	IDM_MESH_GROUP_SCALE				,
	IDM_MESH_GROUP_ROTATION				,
	IDM_MESH_GROUP_TRANSLATION			,

	IDM_TRAJECTORY_SET_ISSEEN			,
	IDM_TRAJECTORY_ROTATION				,
	IDM_TRAJECTORY_TRANSLATION			,

	IDM_MESH_SET_COLOR					,
	IDM_MESH_SET_ISSEEN,

	IDM_CLEAR_TRAJECTORY				,
	IDM_CLEAR_MESHES
};

// -------------------------------------
//	
//	Message ID for Terrain Controller (IDC) 
//
// -------------------------------------

enum IControllerMessageIDs
{
	IDC_TIME_ELAPSED					,

// File							
IDMENU_FIlE_TERRAIN_SHARP			,
IDMENU_FIlE_TERRAIN_PROJECT_SHARP	,
IDMENU_FIlE_TERRAIN_SOFT			,
IDMENU_FIlE_TERRAIN_PROJECT_SOFT	,
IDMENU_FIlE_CAMERA_TRAJECTORY		,
IDMENU_FIlE_CONFIGURATION			,
IDMENU_FILE_OUTPUT_DIRECTORY		,
IDMENU_HELP							,
IDMENU_WINDOWS_GENERAL				,
IDMENU_WINDOWS_FLYTHROUGH			,
IDMENU_WINDOWS_EXPLORE3D			,

// Mode								
IDC_ACTIVATE_FLYTHROUGH_MODE		,
IDC_ACTIVATE_3DEXPLORE_MODE			,

// 3DExpolore Mode					
IDC_E3D_CAMERA_RESET				,
IDC_E3D_CAMERA_SPEED				,
IDC_E3D_ROTATION_SPEED				,
IDC_SET_CAMERA_FIELD_OF_VIEW		,
IDC_SET_CAMERA_NEAR_SCREEN			,
IDC_SET_CAMERA_FAR_SCREEN			,
IDC_SET_TIME_E3D					,

// Flythrough mode					
IDC_FLYTHROUGH_START				,
IDC_FLYTHROUGH_PAUSE				,
IDC_FLYTHROUGH_STOP					,
IDC_FLYTHROUGH_RECORD_START			,
IDC_FLYTHROUGH_RECORD_STOP			,
IDC_FLYTHROUGH_SET_SPEED			,
IDC_FLYTHROUGH_SET_FRAME			,
IDC_SET_START_TIME_TRAJECTORY		,

// IRenderable 						
IDC_MESH_SET_ISSEEN					,

IDC_MESH_SET_COLOR					,
IDC_MESH_GROUP_SCALE				,
IDC_MESH_GROUP_ROTATION				,
IDC_MESH_GROUP_TRANSLATION			,

IDC_TRAJECTORY_SET_ISSEEN			,
IDC_TRAJECTORY_ROTATION				,
IDC_TRAJECTORY_TRANSLATION			,

IDC_BUTTON_CLEAR_MESHES				,
IDC_BUTTON_CLEAR_TRAJECTORY			,


FIRST = IDC_TIME_ELAPSED,
LAST  = IDC_BUTTON_CLEAR_TRAJECTORY
};

inline	IModelMessageIDs IDC2IDM(IControllerMessageIDs message)
{
	switch (message)
	{
	case IDC_TIME_ELAPSED:
	case IDMENU_FILE_OUTPUT_DIRECTORY:
	case IDMENU_HELP:
	case IDMENU_WINDOWS_GENERAL:
	case IDMENU_WINDOWS_FLYTHROUGH:
	case IDMENU_WINDOWS_EXPLORE3D:
		return IDM_INVALID;

	case IDMENU_FIlE_TERRAIN_SHARP:
		return IDM_LOAD_TERRAIN_SHARP;
	case IDMENU_FIlE_TERRAIN_PROJECT_SHARP:
		return IDM_LOAD_PROJECT_SHARP;
	case IDMENU_FIlE_TERRAIN_SOFT:
		return IDM_LOAD_TERRAIN_SOFT;
	case IDMENU_FIlE_TERRAIN_PROJECT_SOFT:
		return IDM_LOAD_PROJECT_SOFT;
	case IDMENU_FIlE_CAMERA_TRAJECTORY:
		return IDM_LOAD_CAMERA_TRAJECTORY;
	case IDMENU_FIlE_CONFIGURATION:
		return IDM_LOAD_CONFIGURATION;

	case IDC_ACTIVATE_FLYTHROUGH_MODE:
		return IDM_ACTIVATE_FLYTHROUGH_MODE;
	case IDC_ACTIVATE_3DEXPLORE_MODE:
		return IDM_ACTIVATE_3DEXPLORE_MODE;

	case IDC_E3D_CAMERA_RESET:
		return IDM_E3D_CAMERA_RESET;
	case IDC_E3D_CAMERA_SPEED:
		return IDM_E3D_SET_SPEED;
	case IDC_E3D_ROTATION_SPEED:
		return IDM_E3D_SET_ROTATION_SPEED;

	case IDC_SET_CAMERA_FIELD_OF_VIEW:
		return IDM_SET_CAMERA_FIELD_OF_VIEW;
	case IDC_SET_CAMERA_NEAR_SCREEN:
		return IDM_SET_CAMERA_NEAR_SCREEN;

	case IDC_SET_CAMERA_FAR_SCREEN:
		return IDM_SET_CAMERA_FAR_SCREEN;

	case IDC_SET_TIME_E3D:
		return IDM_SET_TIME_E3D;

	case IDC_FLYTHROUGH_START:
	case IDC_FLYTHROUGH_PAUSE:
		return IDM_INVALID;

	case IDC_FLYTHROUGH_STOP:
		return IDM_FLYTHROUGH_START_POSITION;
		break;

	case IDC_FLYTHROUGH_RECORD_START:
	case IDC_FLYTHROUGH_RECORD_STOP:
		return IDM_INVALID;

	case IDC_FLYTHROUGH_SET_SPEED:
		return IDM_FLYTHROUGH_SET_SPEED;

	case IDC_FLYTHROUGH_SET_FRAME:
		return IDM_FLYTHROUGH_SET_FRAME;

	case IDC_SET_START_TIME_TRAJECTORY:
		return IDM_SET_START_TIME_TRAJECTORY;

	case IDC_MESH_SET_ISSEEN:
		return IDM_MESH_SET_ISSEEN;

	case IDC_MESH_SET_COLOR:
		return IDM_MESH_SET_COLOR;

	case IDC_MESH_GROUP_SCALE:
		return IDM_MESH_GROUP_SCALE;

	case IDC_MESH_GROUP_ROTATION:
		return IDM_MESH_GROUP_ROTATION;

	case IDC_MESH_GROUP_TRANSLATION:
		return IDM_MESH_GROUP_TRANSLATION;

	case IDC_TRAJECTORY_SET_ISSEEN:
		return IDM_TRAJECTORY_SET_ISSEEN;

	case IDC_TRAJECTORY_ROTATION:
		return IDM_TRAJECTORY_ROTATION;

	case IDC_TRAJECTORY_TRANSLATION:
		return IDM_TRAJECTORY_TRANSLATION;

	case IDC_BUTTON_CLEAR_MESHES:
		return IDM_CLEAR_MESHES;

	case IDC_BUTTON_CLEAR_TRAJECTORY:
		return IDM_CLEAR_TRAJECTORY;

	default:
		return IDM_INVALID;
	}
}

#endif