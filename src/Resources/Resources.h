#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <map>

namespace Render {
	class ProgramShader;
}


class ResourceManager {
	using shaderProgMap = std::map<std::string, std::shared_ptr<Render::ProgramShader>>;

	shaderProgMap sh_map;
	std::string e_path;
	static ResourceManager* instance;
	ResourceManager(const std::string& pathExeFile);
	std::string getFileStr(const std::string filePath) const;
public:
	static ResourceManager* getInstance(const std::string& pathExFile);
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	std::shared_ptr<Render::ProgramShader> loadShaderPr(const std::string& shaderName, 
														const std::string& vertexPath, 
														const std::string& fragmentPath);
	std::shared_ptr<Render::ProgramShader> getShaderPr(const std::string& nameShader);
};
