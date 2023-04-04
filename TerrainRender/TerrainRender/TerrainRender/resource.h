#ifndef TERRAIN_RENDER_RESOURCE_H
#define TERRAIN_RENDER_RESOURCE_H


// -------------------------------------
//	
//	Message ID for Terrain Model (IDM)
//  100-999
//
// -------------------------------------
#define IDM_CAMERA_MOVE_FORWARD			100
#define IDM_CAMERA_MOVE_BACK			101
#define IDM_CAMERA_MOVE_LEFT			102
#define IDM_CAMERA_MOVE_RIGHT			103
#define IDM_CAMERA_MOVE_UP				104
#define IDM_CAMERA_MOVE_DOWN			105
#define IDM_CAMERA_ROTATE				106

#define IDM_SET_CAMERA_SPEED					110
#define IDM_SET_CAMERA_ROTATION_SPEED			111
#define IDM_SET_CAMERA_FIELD_OF_VIEW			112
#define IDM_SET_CAMERA_ASPECT_RATIO				113
#define IDM_SET_CAMERA_ASPECT_NEAR_SCREEN		114
#define IDM_SET_CAMERA_ASPECT_FAR_SCREEN		115

#define IDM_CAMERA_TRAJECTORY_NEXT_FRAME		120
#define IDM_CAMERA_TRAJECTORY_START				121
#define IDM_CAMERA_TRAJECTORY_STOP				123
#define IDM_CAMERA_TRAJECTORY_FRAME				124

#define IDM_TRANSFORMATION_IRENDERABLE_SCALE			200
#define IDM_TRANSFORMATION_IRENDERABLE_ROTATION			201
#define IDM_TRANSFORMATION_IRENDERABLE_TRANSLATION		202
#define IDM_CLEAR_MESHES								203
#define IDM_TRANSFORMATION_IRENDERABLE_COLOR			204



// -------------------------------------
//	
//	Message ID for Terrain Controller (IDC) 
//  1000-9999
//
// -------------------------------------
//TODO: GOOD NAME FOR TJIS
#define IDC_TIME_ELAPSED						1000

// File
#define IDC_BUTTON_FIlE_TERRAIN					2000
#define IDC_BUTTON_FIlE_CAMERA_TRAJECTORY		2001
#define IDC_BUTTON_FIlE_CAMERA_PROPERTIES		2002
#define IDC_BUTTON_FIlE_TERRAIN_PROJECT			2003

// Mode
#define IDC_BUTTON_FLYTHROUGH_MODE				2100
#define IDC_BUTTON_3DEXPLORE_MODE				2101

// 3DExpolore Mode
#define IDC_BUTTON_CAMERA_RESET					3000
#define IDC_SLIDER_CAMERA_SPEED					3101
#define IDC_SLIDER_CAMERA_ROTATION_SPEED		3102
#define IDC_SLIDER_PROJECTION_FIELD_OF_VIEW		3103
#define IDC_SLIDER_PROJECTION_NEAR_SCREEN		3104
#define IDC_SLIDER_PROJECTION_FAR_SCREEN		3105

// Flythrough mode
#define IDC_BUTTON_FLYTHROUGH_START				4210
#define IDC_BUTTON_FLYTHROUGH_PAUSE				4211
#define IDC_BUTTON_FLYTHROUGH_STOP				4212
#define IDC_BUTTON_FLYTHROUGH_RECORD			4213
#define IDC_SLIDER_FLYTHROUGH_SPEED				4220
#define IDC_SLIDER_FLYTHROUGH_FRAME				4221

// IRenderable 

#define IDC_SLIDER_IRENDERABLE_SCALE			5000
#define IDC_SLIDER_IRENDERABLE_ROTATION			5001
#define IDC_SLIDER_IRENDERABLE_TRANSLATION		5002
#define IDC_CHECKBOX_IRENDERABLE_SEEN			5003
#define IDC_BUTTON_CLEAR_MESHES					5004
#define IDC_SLIDER_IRENDERABLE_COLOR			5005



// -------------------------------------
//	
//	Message ID for Terrain Controller from Terrain Controller (IDCC) 
//  10000-
//
// -------------------------------------

#define	IDCC_ACTIVATE_FLYTHROUGH				10000
#define	IDCC_ACTIVATE_3DEXPLORE					10001
#define	IDCC_START_FLYTHROUGH					10002
#define	IDCC_PAUSE_FLYTHROUGH					10003
#define	IDCC_STOP_FLYTHROUGH					10004
#define	IDCC_SPEED_FLYTHROUGH					10005
#define	IDCC_SET_FRAME_FLYTHROUGH				10006

#endif