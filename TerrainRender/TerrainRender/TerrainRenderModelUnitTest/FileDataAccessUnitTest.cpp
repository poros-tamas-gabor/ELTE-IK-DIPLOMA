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
		TEST_METHOD(LoadConfigFile)
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
	};
}