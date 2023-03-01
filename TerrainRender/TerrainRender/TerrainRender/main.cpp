
#include "App.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pScmdline, int iCmdshow)
{
	App		app;
	bool	result;
	
	result = app.Initialize( hInstance );

	if (result)
	{
		app.Run();
	}
	app.Shutdown();

	return 0;

}