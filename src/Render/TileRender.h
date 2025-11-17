#pragma once

#include <glad/glad.h>

#include "../Camera/Camera2D.h"
#include "../Resources/Resources.h"
#include "IndexBuff.h"
#include "Shaders.h"
#include "Texture2D.h"
#include "VertexArr.h"
#include "VertexBuffArr.h"

#include <memory>
#include <vector>

namespace Render
{
class TileRender
{
    std::shared_ptr<Camera::Camera2D> cam = nullptr;
    std::shared_ptr<Render::ProgramShader> sh_prog = nullptr;
    std::shared_ptr<Render::Texture2D> tex = nullptr;
    std::vector<TilesCord> crds;
    static std::vector<unsigned int> ids;
    std::vector<std::vector<float>> verticies;
    std::vector<VertexBuffArr> vbos;
    std::vector<IndexBuff> emos;
    std::vector<VertexArr> vaos;
    void swap(TileRender &copy) noexcept;

  public:
    TileRender(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> prog,
               std::shared_ptr<Render::Texture2D> tx, std::vector<TilesCord> arr);
    TileRender() = default;
    ~TileRender() = default;
    TileRender(const TileRender &) = delete;
    TileRender &operator=(const TileRender &) = delete;
    TileRender(TileRender &&right) noexcept;
    TileRender &operator=(TileRender &&right) noexcept;
    void init();
    void update();
    void draw();
};
} // namespace Render