#ifndef TERRAIN_RENDER_RESOURCE_H
#define TERRAIN_RENDER_RESOURCE_H


// File
//#define IDMENU_FIlE_TERRAIN_SHARP			0
//#define IDMENU_FIlE_TERRAIN_PROJECT_SHARP	1
//#define IDMENU_FIlE_TERRAIN_SOFT			2
//#define IDMENU_FIlE_TERRAIN_PROJECT_SOFT	3
//#define IDMENU_FIlE_CAMERA_TRAJECTORY		4
//#define IDMENU_FIlE_CONFIGURATION			5
//#define IDMENU_FILE_OUTPUT_DIRECTORY		6
//#define IDMENU_HELP							7
//#define IDMENU_WINDOWS_GENERAL				8
//#define IDMENU_WINDOWS_FLYTHROUGH			9
//#define IDMENU_WINDOWS_EXPLORE3D			10


// -------------------------------------
//	
//	Message ID for Terrain Model (IDM)
//  100-999
//
// -------------------------------------

enum IModelMessageIDs
{
	IDM_LOAD_TERRAIN_SHARP			,
	IDM_LOAD_PROJECT_SHARP			,
	IDM_LOAD_TERRAIN_SOFT			,
	IDM_LOAD_PROJECT_SOFT			,
	IDM_LOAD_CAMERA_TRAJECTORY		,
	IDM_LOAD_CONFIGURATION			,

	IDM_E3D_MOVE_FORWARD		,
	IDM_E3D_MOVE_BACK			,
	IDM_E3D_MOVE_LEFT			,
	IDM_E3D_MOVE_RIGHT			,
	IDM_E3D_MOVE_UP				,
	IDM_E3D_MOVE_DOWN			,
	IDM_E3D_ROTATE				,
	IDM_E3D_SET_SPEED			,
	IDM_E3D_SET_ROTATION_SPEED	,
	IDM_E3D_CAMERA_RESET		,

	IDM_SET_CAMERA_FIELD_OF_VIEW		,
	IDM_SET_CAMERA_ASPECT_NEAR_SCREEN	,
	IDM_SET_CAMERA_ASPECT_FAR_SCREEN	,

	IDM_FLYTHROUGH_NEXT_FRAME			,
	IDM_FLYTHROUGH_START_POSITION		,
	IDM_FLYTHROUGH_STOP					,
	IDM_FLYTHROUGH_SET_FRAME			,
	IDM_FLYTHROUGH_SET_SPEED			,

	IDM_SET_TIME_E3D					,
	IDM_SET_START_TIME_TRAJECTORY		,
	
	IDM_MESHES_SCALE				,
	IDM_MESHES_ROTATION				,
	IDM_MESHES_TRANSLATION			,

	IDM_TRAJECTORY_ROTATION			,
	IDM_TRAJECTORY_TRANSLATION		,

	IDM_IRENDERABLE_COLOR			,
	IDM_IRENDERABLE_ISSEEN			,

	IDM_CLEAR_TRAJECTORY			,
	IDM_CLEAR_MESHES
};

// -------------------------------------
//	
//	Message ID for Terrain Controller (IDC) 
//  1000-9999
//
// -------------------------------------
//TODO: GOOD NAME FOR TJIS

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
IDC_ACTIVATE_FLYTHROUGH_MODE			,
IDC_ACTIVATE_3DEXPLORE_MODE			,

// 3DExpolore Mode					
IDC_E3D_CAMERA_RESET				,
IDC_E3D_CAMERA_SPEED				,
IDC_E3D_ROTATION_SPEED	,
IDC_SLIDER_PROJECTION_FIELD_OF_VIEW	,
IDC_SLIDER_PROJECTION_NEAR_SCREEN	,
IDC_SLIDER_PROJECTION_FAR_SCREEN	,
IDC_INPUT_3DE_UNIXTIME				,

// Flythrough mode					
IDC_FLYTHROUGH_START			,
IDC_FLYTHROUGH_PAUSE			,
IDC_FLYTHROUGH_STOP			,
IDC_FLYTHROUGH_RECORD_START		,
IDC_FLYTHROUGH_RECORD_STOP	,
IDC_FLYTHROUGH_SET_SPEED			,
IDC_FLYTHROUGH_SET_FRAME			,
IDC_INPUT_FLYTHROUGH_UNIXTIME		,

// IRenderable 						

IDC_SLIDER_IRENDERABLE_SCALE		,
IDC_SLIDER_IRENDERABLE_ROTATION		,
IDC_SLIDER_IRENDERABLE_TRANSLATION	,
IDC_CHECKBOX_IRENDERABLE_SEEN		,
IDC_BUTTON_CLEAR_MESHES				,
IDC_BUTTON_CLEAR_TRAJECTORY			,
IDC_SLIDER_IRENDERABLE_COLOR		,
IDC_CHECKBOX_IRENDERABLE_ISSEEN		,

FIRST = IDC_TIME_ELAPSED,
LAST  = IDC_CHECKBOX_IRENDERABLE_ISSEEN
};
//#define IDC_TIME_ELAPSED						1000
//
//// File
//#define IDC_BUTTON_FIlE_TERRAIN_SHARP			2000
//#define IDC_BUTTON_FIlE_CAMERA_TRAJECTORY		2001
//#define IDC_BUTTON_FIlE_TERRAIN_PROJECT			2003
//#define IDC_BUTTON_FIlE_PARAMETERS				2004
//#define IDC_BUTTON_FIlE_TERRAIN_SOFT			2005
//
//// Mode
//#define IDC_ACTIVATE_FLYTHROUGH_MODE				2100
//#define IDC_ACTIVATE_3DEXPLORE_MODE				2101
//
//// 3DExpolore Mode
//#define IDC_E3D_CAMERA_RESET					3000
//#define IDC_E3D_CAMERA_SPEED					3101
//#define IDC_E3D_ROTATION_SPEED		3102
//#define IDC_SLIDER_PROJECTION_FIELD_OF_VIEW		3103
//#define IDC_SLIDER_PROJECTION_NEAR_SCREEN		3104
//#define IDC_SLIDER_PROJECTION_FAR_SCREEN		3105
//#define IDC_INPUT_3DE_UNIXTIME					3106
//
//// Flythrough mode
//#define IDC_FLYTHROUGH_START				4210
//#define IDC_FLYTHROUGH_PAUSE				4211
//#define IDC_FLYTHROUGH_STOP				4212
//#define IDC_FLYTHROUGH_RECORD_START			4213
//#define IDC_FLYTHROUGH_RECORD_STOP			4214
//#define IDC_FLYTHROUGH_SET_SPEED				4220
//#define IDC_FLYTHROUGH_SET_FRAME				4221
//#define IDC_INPUT_FLYTHROUGH_UNIXTIME			4222
//
//// IRenderable 
//
//#define IDC_SLIDER_IRENDERABLE_SCALE			5000
//#define IDC_SLIDER_IRENDERABLE_ROTATION			5001
//#define IDC_SLIDER_IRENDERABLE_TRANSLATION		5002
//#define IDC_CHECKBOX_IRENDERABLE_SEEN			5003
//#define IDC_BUTTON_CLEAR_MESHES					5004
//#define IDC_BUTTON_CLEAR_TRAJECTORY				5005
//#define IDC_SLIDER_IRENDERABLE_COLOR			5006
//#define IDC_CHECKBOX_IRENDERABLE_ISSEEN			5007



// -------------------------------------
//	
//	Message ID for Terrain Controller from Terrain Controller (IDCC) 
//  10000-
//
// -------------------------------------

//#define	IDCC_ACTIVATE_FLYTHROUGH				10000
//#define	IDCC_ACTIVATE_3DEXPLORE					10001
//#define	IDCC_PLAY_FLYTHROUGH					10002
//#define	IDCC_PAUSE_FLYTHROUGH					10003
//#define	IDCC_STOP_FLYTHROUGH					10004
//#define	IDCC_SPEED_FLYTHROUGH					10005
//#define	IDCC_SET_FRAME_FLYTHROUGH				10006
//#define	IDCC_RECORD_FLYTHROUGH					10007
//#define	IDCC_STOP_RECORD_FLYTHROUGH					10008

#endif