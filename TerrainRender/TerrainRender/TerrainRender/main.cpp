#include "App.h"
#include "ErrorHandler.h"

int CALLBACK wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR     lpCmdLine,
	int       nCmdShow)
{
	try
	{
		App		app;
		bool	result;

		result = app.Initialize(hInstance, 1200, 700);

		if (result)
		{
			app.Run();
		}
		app.Shutdown();

		return 0;
	}
	catch (const COMException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const TRException& e)
	{
		ErrorHandler::Log(e);
	}
	catch (const std::exception& e)
	{
		ErrorHandler::Log(e);
	}
	catch (...)
	{
		ErrorHandler::Log("Unknown Exception: No details available");
	}
	return -1;
}