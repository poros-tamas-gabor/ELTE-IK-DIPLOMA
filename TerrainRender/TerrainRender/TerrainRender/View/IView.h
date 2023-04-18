#ifndef I_VIEW_H
#define I_VIEW_H

#include <memory>
#include <vector>
#include "../Model/Persistence/ModelStructs.h"
#include "../Model/IModelSubscriber.h"
#include <string>

class IView : public IModelSubscriber
{

public:
	virtual ~IView() {}
	virtual	bool Resize(unsigned screenWidth, unsigned screenHeight) = 0;
	virtual	bool CaptureScreen(unsigned frameNum) = 0;
	virtual void SetOutputDirectory(const std::wstring& m_outputDirectoryPath) = 0;
	virtual void Help() = 0;
};

typedef std::shared_ptr<IView> IViewPtr;
#endif