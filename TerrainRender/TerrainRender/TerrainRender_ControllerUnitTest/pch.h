//
// pch.h
//

#pragma once
#define _GTEST
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../TerrainRender/Controller/IController.h"
#include "../TerrainRender/Controller/Controller3DExplore.h"
#include "../TerrainRender/Controller/ControllerFlythrough.h"
#include "../TerrainRender/Controller/ControllerRouter.h"
#include "../TerrainRender/Controller/ControllerGui.h"
#include "../TerrainRender/Controller/MessageSystem.h"

#include "../TerrainRender/resource.h"
#include "../TerrainRender/App.h"
#include "../TerrainRender/COMException.h"
#include "../TerrainRender/ErrorHandler.h"
#include "../TerrainRender/RenderWindow.h"
#include "../TerrainRender/StringConverter.h"