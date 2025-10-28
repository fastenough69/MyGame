#include "Object.h"

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

void Objects::GameObjMainHero::update(float deltaTime, float worldWidth, float worldHeight, glm::vec2 &diff_coord,
                                      glm::mat4 &proj, glm::mat4 &view)
{
    auto shProg = object->get_shader_program();
    object->update(deltaTime, worldWidth, worldHeight);
    glm::vec2 pos_hero = object->get_position();

    shProg->usage();

    shProg->setMat4("projection", proj);
    shProg->setMat4("view", view);

    if (object->get_attack_flag())
    {
        object->bind_state(std::string("attack_hero"), ver);
    }
    else if (object->get_hitWall() || pos_hero != diff_coord)
    {
        object->bind_state(std::string("run_hero"), ver);
    }
    else if (pos_hero == diff_coord)
    {
        object->bind_state(std::string("idle_hero"), ver);
    }
    diff_coord = pos_hero;

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

Objects::GameObjBackground::GameObjBackground(std::shared_ptr<Camera::Camera2D> cm,
                                              std::shared_ptr<Render::ProgramShader> ptr, std::vector<float> &&arr,
                                              std::vector<unsigned int> &&ids, Render::VertexBuffArr &&vb,
                                              Render::IndexBuff &&em, Render::VertexArr &&va)
    : GameObj(std::move(arr), std::move(ids), std::move(vb), std::move(em), std::move(va))
{
    shProg = ptr;
    cam = cm;
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

void Objects::GameObjBackground::update_bg(const std::string& name)
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
}

void Objects::GameObjBackground::render()
{
    shProg->usage();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
