#include "Background.h"

Objects::BackroundLayer::BackroundLayer(float offset) : offsetLayer{offset}
{
}

void Objects::BackroundLayer::update(glm::vec2 &cameraPos)
{
    pos = cameraPos * offsetLayer;
}

glm::mat4 Objects::BackroundLayer::get_model_bg() const
{
    return glm::translate(glm::mat4(1.0f), glm::vec3(-pos, 0.0f));
}

Objects::BackgroundParalax::BackgroundParalax(std::shared_ptr<Camera::Camera2D> cm,
                                              std::shared_ptr<Render::ProgramShader> prog, std::vector<float> &&vr,
                                              std::vector<unsigned int> &&indices)
    : cam{cm}, shProg{prog}, ver{vr}, ids{indices}
{
    vbo = Render::VertexBuffArr{};
    emo = Render::IndexBuff{};
    vao = Render::VertexArr{};
}

void Objects::BackgroundParalax::add_layer(const std::string &name, float &&offsetParalax,
                                           std::shared_ptr<Render::Texture2D> tex)
{
    if (bg_map.count(name) == 0)
    {
        BackroundLayer lay{offsetParalax};
        bg_map[name] = {lay, tex};
    }
}

void Objects::BackgroundParalax::init()
{
    vbo.init(ver.data(), sizeof(float) * ver.size());
    vao.init();
    vao.add_buffer(vbo, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    vao.add_buffer(vbo, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    emo.init(ids.data(), sizeof(unsigned int) * ids.size());
    vao.unbind();
    shProg->usage();
    shProg->setInt("tex", 0);
}

void Objects::BackgroundParalax::update(const std::string &name)
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
    glm::mat4 model = layer.get_model_bg();
    shProg->setMat4("model", model);
    vbo.update_data(ver.data(), sizeof(float) * ver.size());
}

void Objects::BackgroundParalax::render()
{
    shProg->usage();
    vao.bind();
    glDrawElements(GL_TRIANGLES, ids.size(), GL_UNSIGNED_INT, 0);
}