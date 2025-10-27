#include "Object.h"

Objects::GameObjMainHero::GameObjMainHero(std::shared_ptr<Objects::MainHero> objPtr, std::vector<float> &&arr,
                                          std::vector<unsigned int> &&ids, Render::VertexBuffArr &&vb,
                                          Render::IndexBuff &&em, Render::VertexArr &&va)
    : object{objPtr}, ver{arr}, indices{ids}, vbo{std::move(vb)}, emo{std::move(em)}, vao{std::move(va)}
{
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

void Objects::GameObjMainHero::update(float deltaTime, float worldWidth, float worldHeight, glm::vec2 &diff_coord, glm::mat4& proj, glm::mat4& view)
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

    /*shProg->usage();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
}

void Objects::GameObjMainHero::render()
{
    auto shProg = object->get_shader_program();
    shProg->usage();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
