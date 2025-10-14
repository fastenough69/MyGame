#include "Resources.h"
#include "../Render/Shaders.h"
#include "../Render/Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define  STBI_ONLY_PNG

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

ResourceManager::~ResourceManager()
{
	delete instance;
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

std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture(const std::string& texName, const std::string& texPath)
{
	int chanels = 0, widht = 0, height = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data =  stbi_load(std::string(e_path + "/" + texPath).c_str(), &widht, &height, &chanels, 0);
	if (!data)
	{
		std::cerr << "Cant load image" << std::endl;
		return  nullptr;
	}

	auto currTexture = std::make_shared<Render::Texture2D>(widht, height, data, chanels, GL_NEAREST);
	t_map.emplace(texName, currTexture);
	
	stbi_image_free(data);
	return currTexture;
}

std::shared_ptr<Render::Texture2D> ResourceManager::getTexture(const std::string& texName)
{
	if (!t_map.count(texName))
	{
		std::cerr << "Incorrect texture name" << std::endl;
		return nullptr;
	}
	return t_map[texName];
}