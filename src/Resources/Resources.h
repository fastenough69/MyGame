#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <algorithm>

#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

namespace Render
{
class ProgramShader;
class Texture2D;
} // namespace Render

namespace TileProc
{
struct uvCoords
{
    float min_u, max_u;
    float min_v, max_v;
};

struct TileSize
{
    float x, y;
    float widht, height;
    float fullWidht, fullHeight;
};
struct TilesCord
{
    tmx::Vector2f point;
    TileProc::uvCoords tex_cords;
    std::vector<tmx::Rectangle<float>> AABB;
};
uvCoords get_uv_coords(const TileSize &size);
} // namespace TileProc

class ResourceManager
{
    using shaderProgMap = std::map<std::string, std::shared_ptr<Render::ProgramShader>>;
    using textureMap = std::map<std::string, std::shared_ptr<Render::Texture2D>>;
    using TileInfo = std::map<unsigned int, std::pair<TileProc::TileSize, std::vector<tmx::Rectangle<float>>>>;
    using Tailsets = std::map<std::string, std::pair<TileInfo, std::shared_ptr<Render::Texture2D>>>;
    using Levels =
        std::map<std::string, std::pair<std::vector<TileProc::TilesCord>, std::shared_ptr<Render::Texture2D>>>;

    shaderProgMap sh_map;
    textureMap t_map;
    Tailsets ts_map;
    Levels level_map;

    std::string e_path;
    static ResourceManager *instance;
    ResourceManager(const std::string &pathExeFile);
    std::string getFileStr(const std::string filePath) const;
    std::vector<tmx::Rectangle<float>> getAABB(std::vector<tmx::Object> &objs);

  public:
    static ResourceManager *getInstance(const std::string &pathExFile);
    ResourceManager() = delete;
    ~ResourceManager();
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;
    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    std::shared_ptr<Render::ProgramShader> loadShaderPr(const std::string &shaderName, const std::string &vertexPath,
                                                        const std::string &fragmentPath);
    std::shared_ptr<Render::ProgramShader> getShaderPr(const std::string &nameShader);
    std::shared_ptr<Render::Texture2D> loadTexture(const std::string &texName, const std::string &texPath);
    std::shared_ptr<Render::Texture2D> getTexture(const std::string &texName);

    std::pair<TileInfo, std::shared_ptr<Render::Texture2D>> loadTileset(tmx::Map &map, const std::string &name);

    std::pair<std::vector<TileProc::TilesCord>, std::shared_ptr<Render::Texture2D>> tileMapProcessing(
        const std::string &tmxPath, const std::string &nameLvl);
};