// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "../TerrainRender/win.h"
#include "../TerrainRender/Model/Persistence/DataAccess.h"
#include "../TerrainRender/Model/Persistence/ModelStructs.h"
#include "../TerrainRender/Model/Persistence/EpochTime.h"
#include "../TerrainRender/Model/IModel.h"
#include "../TerrainRender/Model/IModelSubscriber.h"

#include "../TerrainRender/Model/IRenderable.h"
#include "../TerrainRender/Model/LineList.h"
#include "../TerrainRender/Model/PolygonMesh.h"
#include "../TerrainRender/Model/PolyLine.h"

#include "../TerrainRender/Model/IPixelShader.h"
#include "../TerrainRender/Model/IVertexShader.h"

#include "../TerrainRender/Model/CameraPositioner.h"
#include "../TerrainRender/Model/CameraTrajectory.h"
#include "../TerrainRender/Model/Camera.h"


#include "../TerrainRender/App.h"
#include "../TerrainRender/COMException.h"
#include "../TerrainRender/ErrorHandler.h"
#include "../TerrainRender/RenderWindow.h"
#include "../TerrainRender/StringConverter.h"

#endif //PCH_H