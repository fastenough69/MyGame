#pragma once

#include <glad/glad.h>

#include "../Camera/Camera2D.h"
#include "../Render/IndexBuff.h"
#include "../Render/Shaders.h"
#include "../Render/Texture2D.h"
#include "../Render/VertexArr.h"
#include "../Render/VertexBuffArr.h"
#include "SpriteAnim.h"

#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
//#include <format>

namespace PersonProcesing
{
class Person
{
    std::shared_ptr<Camera::Camera2D> cam = nullptr;
    std::shared_ptr<Render::ProgramShader> sh_prog = nullptr;
    std::map<std::string, std::vector<std::shared_ptr<Render::Texture2D>>> sprite_map;
    float lastUpTime = 0.0f;
    float animSpeed = 10.0f;
    float countFrames;
    tmx::Rectangle<float> AABB{};
    tmx::Vector2f point;
    float move_speed = 5.0f;
    std::vector<float> ver;
    std::vector<unsigned int> ids = {0, 1, 2, 2, 3, 0};
    Render::VertexBuffArr vbo;
    Render::IndexBuff emo;
    Render::VertexArr vao;
    void loadTexForSprite(std::string& templateNameTexFile);
    
  public:
    Person() = delete;
    Person(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> prog, tmx::Vector2f pt, unsigned int _countFrames);
    ~Person() = default;
    Person(const Person &) = delete;
    Person &operator=(const Person &) = delete;
    Person(Person &&) = delete;
    Person &operator=(Person &&) = delete;
};
} // namespace PersonProcesing