#include "Object.h"
#include <iostream>

Objects::GameObjMainHero::GameObjMainHero(std::shared_ptr<Objects::MainHero> objPtr, std::vector<float> &&arr,
                                          std::vector<unsigned int> &&ids, Render::VertexBuffArr &&vb,
                                          Render::IndexBuff &&em, Render::VertexArr &&va)
    : GameObj(std::move(arr), std::move(ids), std::move(vb), std::move(em), std::move(va))
{
    object = objPtr;
}

void Objects::GameObjMainHero::init()
{
    vbo.init(ver.data(), sizeof(float) * ver.size());
    vao.init();
    vao.add_buffer(vbo, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    vao.add_buffer(vbo, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    emo.init(indices.data(), sizeof(unsigned int) * indices.size());
    vao.unbind();
    object->get_shader_program()->usage();
    object->get_shader_program()->setInt("tex", 0);
}

void Objects::GameObjMainHero::update(float deltaTime, float &worldWidth, float &worldHeight, glm::mat4 &proj,
                                      glm::mat4 &view)
{
    auto shProg = object->get_shader_program();
    object->update(deltaTime, worldWidth, worldHeight);
    glm::vec2 pos_hero = object->get_position();
    glm::vec2 diff_pos = object->get_old_pos();

    shProg->usage();

    shProg->setMat4("projection", proj);
    shProg->setMat4("view", view);

    /*if (pos_hero.x >= worldWidth - (worldWidth * 0.3f))
    {
        worldWidth *= 2;
    }*/

    if (object->get_attack_flag())
    {
        object->bind_state(std::string("attack_hero"), ver);
    }
    else if (object->get_hitWall() || pos_hero != diff_pos)
    {
        object->bind_state(std::string("run_hero"), ver);
    }
    else if (pos_hero == diff_pos)
    {
        object->bind_state(std::string("idle_hero"), ver);
    }
    diff_pos = pos_hero;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos_hero.x, pos_hero.y, 0.0f));
    shProg->setMat4("model", model);
    vbo.update_data(ver.data(), ver.size() * sizeof(float));
}

void Objects::GameObjMainHero::render()
{
    auto shProg = object->get_shader_program();
    shProg->usage();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Objects::GameObjBackground::swap(GameObjBackground &obj) noexcept
{
    std::swap(ver, obj.ver);
    std::swap(indices, obj.indices);
    vbo = std::move(obj.vbo);
    emo = std::move(obj.emo);
    vao = std::move(obj.vao);
    std::swap(shProg, obj.shProg);
    std::swap(cam, obj.cam);
    std::swap(bg_map, obj.bg_map);
    std::swap(repeatBg, obj.repeatBg);
}

Objects::GameObjBackground::GameObjBackground(std::shared_ptr<Camera::Camera2D> cm,
                                              std::shared_ptr<Render::ProgramShader> ptr, std::vector<float> &&arr,
                                              std::vector<unsigned int> &&ids, Render::VertexBuffArr &&vb,
                                              Render::IndexBuff &&em, Render::VertexArr &&va)
    : GameObj(std::move(arr), std::move(ids), std::move(vb), std::move(em), std::move(va))
{
    shProg = ptr;
    cam = cm;
}

Objects::GameObjBackground::GameObjBackground(const GameObjBackground &right)
{
    if (this == &right)
    {
        return;
    }
    ver = right.ver;
    indices = right.indices;
    vbo = std::move(const_cast<GameObjBackground&>(right).vbo);
    emo = std::move(const_cast<GameObjBackground&>(right).emo);
    vao = (std::move(const_cast<GameObjBackground&>(right).vao));
    shProg = right.shProg;
    cam = right.cam;
    bg_map = right.bg_map;
    repeatBg = right.repeatBg;
}

Objects::GameObjBackground &Objects::GameObjBackground::operator=(const GameObjBackground &right)
{
    GameObjBackground copy{right};
    swap(copy);
    return *this;
}

Objects::GameObjBackground::GameObjBackground(GameObjBackground &&right) noexcept
{
    if (this == &right)
    {
        return;
    }
    ver = right.ver;
    indices = right.indices;
    vbo = std::move(right.vbo);
    emo = std::move(right.emo);
    vao = std::move(right.vao);
    shProg = right.shProg;
    cam = right.cam;
    bg_map = std::move(right.bg_map);
    repeatBg = right.repeatBg;
    right.shProg = nullptr;
    right.cam = nullptr;
    right.bg_map.clear();
}

Objects::GameObjBackground &Objects::GameObjBackground::operator=(GameObjBackground &&right) noexcept
{
    GameObjBackground copy{std::move(right)};
    swap(copy);
    return *this;
}

void Objects::GameObjBackground::add_layer(const std::string &name, BackroundLayer &lay,
                                           std::shared_ptr<Render::Texture2D> tex)
{
    if (bg_map.count(name) == 0)
    {
        bg_map[name] = {lay, tex};
    }
}

void Objects::GameObjBackground::init()
{
    vbo.init(ver.data(), sizeof(float) * ver.size());
    vao.init();
    vao.add_buffer(vbo, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    vao.add_buffer(vbo, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    emo.init(indices.data(), sizeof(unsigned int) * indices.size());
    vao.unbind();
    shProg->usage();
    shProg->setInt("tex", 0);
}

void Objects::GameObjBackground::update_bg(const std::string &name)
{
    if (bg_map.count(name) == 0)
    {
        return;
    }
    shProg->usage();
    auto pair = bg_map[name];
    auto layer = pair.first;
    auto tex = pair.second;

    tex->bind();
    layer.update(cam->get_cam_pos());
    shProg->setMat4("projection", cam->get_proj_matrix());
    shProg->setMat4("view", cam->get_view_matrix());
    shProg->setMat4("model", layer.get_model_bg());
    vbo.update_data(ver.data(), sizeof(float) * ver.size());
}

void Objects::GameObjBackground::render()
{
    shProg->usage();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Objects::GameObjBackground::multi_coord(float& curWorldWidht)
{
    curWorldWidht *= 2.0f;

    ver[0] = ver[5];
    ver[15] = ver[5];
    /*ver[1] = ver[6];*/
    ver[5] *= repeatBg;
    ver[8] = repeatBg;
    ver[13] = repeatBg;
    ver[10] *= repeatBg;
    std::cout << "VerBg: ";
}
