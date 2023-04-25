#ifndef GOOGLE_PCH_H
#define GOOGLE_PCH_H

// add headers that you want to pre-compile here
#include "gtest/gtest.h"
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