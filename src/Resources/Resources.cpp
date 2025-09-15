#include "Resources.h"
#include "../Render/Shaders.h"


ResourceManager::ResourceManager(const std::string& pathExeFile)
{
	size_t fn = pathExeFile.find_last_of("/\\");
	e_path = pathExeFile.substr(0, fn);

}
