#include "Resources.h"
#include "../Render/Shaders.h"
#include "../Render/Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STBI_ONLY_PNG

#include <fstream>
#include <sstream>

ResourceManager *ResourceManager::instance = nullptr;

ResourceManager::ResourceManager(const std::string &pathExeFile)
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
    f.close();
    return buff.str();
}

ResourceManager *ResourceManager::getInstance(const std::string &pathExFile)
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

std::shared_ptr<Render::ProgramShader> ResourceManager::loadShaderPr(const std::string &shaderName,
                                                                     const std::string &vertexPath,
                                                                     const std::string &fragmentPath)
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

std::shared_ptr<Render::ProgramShader> ResourceManager::getShaderPr(const std::string &nameShader)
{
    if (!sh_map.count(nameShader))
    {
        std::cerr << "Incorrect name shader" << std::endl;
        return nullptr;
    }
    return sh_map[nameShader];
}

std::shared_ptr<Render::Texture2D> ResourceManager::loadTexture(const std::string &texName, const std::string &texPath)
{
    int chanels = 0, widht = 0, height = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(std::string(e_path + "/" + texPath).c_str(), &widht, &height, &chanels, 0);
    if (!data)
    {
        std::cerr << "Cant load image" << std::endl;
        return nullptr;
    }

    auto currTexture = std::make_shared<Render::Texture2D>(widht, height, data, chanels, GL_NEAREST);
    if (t_map.count(texName) == 0)
    {
        t_map.emplace(texName, currTexture);
    }

    stbi_image_free(data);
    return currTexture;
}

std::shared_ptr<Render::Texture2D> ResourceManager::getTexture(const std::string &texName)
{
    if (!t_map.count(texName))
    {
        std::cerr << "Incorrect texture name" << std::endl;
        return nullptr;
    }
    return t_map[texName];
}

std::pair<ResourceManager::TileInfo, std::shared_ptr<Render::Texture2D>> ResourceManager::loadTileset(
    const std::string &tmxFilePath, const std::string &nameTs)
{
    tmx::Map map;
    if (!map.load(e_path + '/' + tmxFilePath))
    {
        return {};
    }
    const auto &tileset = map.getTilesets()[0];
    const auto &sizeTile = map.getTileSize().x;
    tmx::Vector2u sizeTex = tileset.getImageSize();
    TileInfo temp;
    const auto &tiles = tileset.getTiles();

    for (int y{}; y < (sizeTex.y / sizeTile); y++)
    {
        for (int x{}; x < (sizeTex.x / sizeTile); x++)
        {
            temp.emplace(tiles[y * (sizeTex.x / sizeTile) + x].ID + tileset.getFirstGID(),
                         Objects::TileSize{(float)(x * sizeTile), (sizeTex.y - sizeTile) - (float)(y * sizeTile),
                                           (float)sizeTile, (float)sizeTile, (float)(sizeTex.x), (float)(sizeTex.y)});
        }
    }

    std::string tex_path = tileset.getImagePath();
    size_t count = tex_path.find("res/");
    tex_path = tex_path.substr(count, tex_path.size());

    auto tex = loadTexture(nameTs, tex_path);
    std::pair<TileInfo, std::shared_ptr<Render::Texture2D>> pair = {temp, tex};
    if (ts_map.count(nameTs) == 0)
    {
        ts_map.emplace(nameTs, pair);
    }
    return pair;
}

void ResourceManager::tileMapProcessing(const std::string &tmxPath, const std::string &nameLvl)
{
    tmx::Map map;
    if (!map.load(e_path + '/' + tmxPath))
    {
        return;
    }
    if (level_map.count(nameLvl) != 0)
    {
        return;
    }
    const auto &layers = map.getLayers();
    tmx::Vector2u sizeMap = map.getTileCount();
    const auto &tailset = loadTileset(tmxPath, "ts1");
    auto tex = tailset.second;
    auto info = tailset.first;
    std::vector<TilesCord> temp;

    for (const auto &layer : layers)
    {
        const auto &tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
        for (int y{}; y < sizeMap.y; y++)
        {
            for (int x{}; x < sizeMap.x; x++)
            {
                auto tileId = tiles[y * sizeMap.x + x].ID;
                if (tileId != 0)
                {
                    Objects::uvCoords uvcrd = Objects::get_uv_coords(info[tileId]);
                    temp.push_back(TilesCord{tmx::Vector2f((float)x, (float)y), uvcrd, tex});

                    std::cout << uvcrd.min_u << ' ' << uvcrd.max_u << std::endl
                              << uvcrd.min_v << ' ' << uvcrd.max_v << std::endl;
                    std::cout << "Map coords: " << x << ' ' << y;
                    std::cout << std::endl;
                }
            }
        }
    }
    level_map.emplace(nameLvl, temp);
}

Objects::uvCoords Objects::get_uv_coords(const TileSize &size)
{
    uvCoords result{};
    result.min_u = size.x / size.fullWidht;
    result.min_v = size.y / size.fullHeight;
    result.max_u = (size.x + size.widht) / size.fullWidht;
    result.max_v = (size.y + size.height) / size.fullHeight;
    return result;
}
