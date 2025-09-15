#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <map>

namespace Render {
	class ProgramShader;
}


class ResourceManager {
	typedef std::map<std::string, std::shared_ptr<Render::ProgramShader>> shaderProgMap;

	shaderProgMap sh_map;
	std::string e_path;
public:
	ResourceManager(const std::string& pathExeFile);
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;

	std::shared_ptr<Render::ProgramShader> loadShader(const std::string& shaderName, 
														const std::string& vertexPath, 
														const std::string& fragmentPath);
	std::shared_ptr<Render::ProgramShader> getShader(const std::string& nameShader);
};
