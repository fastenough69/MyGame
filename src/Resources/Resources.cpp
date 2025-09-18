#include "Resources.h"
#include "../Render/Shaders.h"

#include <fstream>
#include <sstream>

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager(const std::string& pathExeFile)
{
	size_t fn = pathExeFile.find_last_of("/\\");
	e_path = pathExeFile.substr(0, fn);

}

std::string ResourceManager::getFileStr(const std::string filePath) const
{
	std::ifstream f{};
	f.open(e_path + "/" + filePath, std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "Failed to open file" << filePath << std::endl;
		return std::string();
	}

	std::stringstream buff;
	buff << f.rdbuf();
	return buff.str();
}

ResourceManager* ResourceManager::getInstance(const std::string& pathExFile)
{
	if (ResourceManager::instance == nullptr)
	{
		ResourceManager::instance = new ResourceManager(pathExFile);
	}
	return ResourceManager::instance;
}

std::shared_ptr<Render::ProgramShader> ResourceManager::loadShaderPr(const std::string& shaderName, 
																	const std::string& vertexPath, 
																	const std::string& fragmentPath)
{
	const std::string vShader = getFileStr(vertexPath), frShader = getFileStr(fragmentPath);
	if (vShader.empty())
	{
		std::cerr << "No vertex shader" << std::endl;
		return nullptr;
	}

	if (frShader.empty())
	{
		std::cerr << "No fragment shader" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Render::ProgramShader> psh_ptr = std::make_shared<Render::ProgramShader>(vShader, frShader);
	if (psh_ptr->get_compile())
	{
		sh_map.emplace(shaderName, psh_ptr);
	}
	return psh_ptr;
}

std::shared_ptr<Render::ProgramShader> ResourceManager::getShaderPr(const std::string& nameShader)
{
	if (!sh_map.count(nameShader))
	{
		std::cerr << "Incorrect name shader" << std::endl;
		return nullptr;
	}
	return sh_map[nameShader];
}
