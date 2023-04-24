#include "pch.h"
#include "CppUnitTest.h"
#include "../TerrainRender/Model/Persistence/FileDataAccessAsync.cpp"
#include "../TerrainRender/Model/Persistence/ReadSTLChunk.cpp"
#include "../TerrainRender/Model/Persistence/DataAccess.h"
#include "../TerrainRender/ErrorHandler.cpp"
#include "../TerrainRender/StringConverter.cpp"
#include "../TerrainRender/Model/Persistence/ModelStructs.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TerrainRenderModelUnitTest
{
	TEST_CLASS(FileDataAccessAsyncUnitTest)
	{
	public:
		TEST_METHOD(LoadConfigureFile)
		{
			IDataAccessPtr persistence = std::make_shared<BinaryFileDataAccessAsync>();
			ParameterFile file;
			auto func = [persistence, &file] { persistence->LoadConfigurationFile(L"invalid", file); };

			TCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);
			
			std::wstring path(buffer);
			path += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\testConfig.json";
			persistence->LoadConfigurationFile(path.c_str(), file);
			Assert::AreEqual(file.origo.latitude, 47.497913);
			Assert::AreEqual(file.origo.longitude, 19.040236);
			Assert::AreEqual(file.terrain.translation, {10,11,12});
			Assert::AreEqual(file.terrain.rotation, {20,21,22});

			Assert::IsFalse(file.terrain.colors.empty());
			Assert::AreEqual(file.terrain.colors.size(), size_t(3));
			Assert::AreEqual(file.trajectory.translation, {700,65,-250});
			Assert::AreEqual(file.trajectory.rotation, {0,0,0});
		}

		TEST_METHOD(LoadTrajectoryFile)
		{
			IDataAccessPtr persistence = std::make_shared<BinaryFileDataAccessAsync>();
			std::vector<CameraPose> cameraPoses;
			auto func = [persistence, &cameraPoses] { persistence->LoadCameraTrajectory(L"invalid", cameraPoses) ; };

			TCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);

			std::wstring path(buffer);
			path += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\FR_Video_409.csv";
			persistence->LoadCameraTrajectory(path.c_str(), cameraPoses);
			
			Assert::AreEqual(cameraPoses.size(), size_t(1670));		
			CameraPose test = { {1664534691, 679506806}, 1.37945, -0.0253926,0.00191395 , 188.429, -675.288, 0.000213623 };
			Assert::AreEqual(cameraPoses.at(9), test);
		}

		TEST_METHOD(LoadTerrain_withSoftEdges)
		{
			IDataAccessPtr persistence = std::make_shared<BinaryFileDataAccessAsync>();
			auto func = [persistence] { persistence->LoadTerrainSoftEdges(L"invalid"); };

			TCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);

			std::wstring spherePath(buffer);
			spherePath += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\3d_model_of_Sphere.stl";
			persistence->LoadTerrainSoftEdges(spherePath.c_str());

			Assert::AreEqual(persistence->GetVertices_Soft().size(), size_t(1538));
			Assert::AreEqual(persistence->GetIndices_Soft().size(), size_t(3072));

			std::wstring bunny70k_path(buffer);
			bunny70k_path += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\bunny70k.stl";
			persistence->LoadTerrainSoftEdges(bunny70k_path.c_str());

			Assert::AreEqual(persistence->GetVertices_Soft().size(), size_t(34834));
			Assert::AreEqual(persistence->GetIndices_Soft().size(), size_t(69451));

			std::wstring bunny10k_path(buffer);
			bunny10k_path += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\bunny10k.stl";
			persistence->LoadTerrainSoftEdges(bunny10k_path.c_str());

			Assert::AreEqual(persistence->GetVertices_Soft().size(), size_t(5051));
			Assert::AreEqual(persistence->GetIndices_Soft().size(), size_t(9999));
		}

		TEST_METHOD(LoadTerrain_withSharpEdges)
		{
			IDataAccessPtr persistence = std::make_shared<BinaryFileDataAccessAsync>();
			auto func = [persistence] { persistence->LoadTerrainSharpEdges(L"invalid"); };

			TCHAR buffer[MAX_PATH];
			GetCurrentDirectory(MAX_PATH, buffer);

			std::wstring spherePath(buffer);
			spherePath += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\3d_model_of_Sphere.stl";
			persistence->LoadTerrainSharpEdges(spherePath.c_str());

			Assert::AreEqual(persistence->GetFacets().size(), size_t(3072));

			std::wstring bunny70k_path(buffer);
			bunny70k_path += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\bunny70k.stl";
			persistence->LoadTerrainSharpEdges(bunny70k_path.c_str());

			Assert::AreEqual(persistence->GetFacets().size(), size_t(69451));

			std::wstring bunny10k_path(buffer);
			bunny10k_path += L"..\\..\\..\\TerrainRenderModelUnitTest\\ResourceFiles\\bunny10k.stl";
			persistence->LoadTerrainSharpEdges(bunny10k_path.c_str());

			Assert::AreEqual(persistence->GetFacets().size(), size_t(9999));
		}
	};
}