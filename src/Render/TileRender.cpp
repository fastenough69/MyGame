#include "TileRender.h"

std::vector<unsigned int> Render::TileRender::ids = {0, 1, 2, 2, 3, 0};

void Render::TileRender::swap(TileRender &copy) noexcept
{
    std::swap(cam, copy.cam);
    std::swap(sh_prog, copy.sh_prog);
    std::swap(tex, copy.tex);
    std::swap(crds, copy.crds);
    vbos = std::move(copy.vbos);
    emos = std::move(copy.emos);
    vaos = std::move(copy.vaos);
}

Render::TileRender::TileRender(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> prog,
                               std::shared_ptr<Render::Texture2D> tx, std::vector<TileProc::TilesCord> arr)
    : cam{cm}, sh_prog{prog}, tex{tx}, crds{arr}
{
    unsigned int size = crds.size();
    for (int i{}; i < size; i++)
    {
        vbos.push_back(VertexBuffArr{});
        emos.push_back(IndexBuff{});
        vaos.push_back(VertexArr{});
    }
}

Render::TileRender::TileRender(TileRender &&right) noexcept
{
    if (this == &right)
    {
        return;
    }
    cam = right.cam;
    sh_prog = right.sh_prog;
    tex = right.tex;
    crds = right.crds;
    vbos = std::move(right.vbos);
    emos = std::move(right.emos);
    vaos = std::move(right.vaos);
    right.cam = nullptr;
    right.sh_prog = nullptr;
    right.tex = nullptr;
}

Render::TileRender &Render::TileRender::operator=(TileRender &&right) noexcept
{
    TileRender copy{std::move(right)};
    swap(copy);
    return *this;
}

void Render::TileRender::init()
{
    unsigned int size = crds.size();
    for (int i{}; i < size; i++)
    {
        std::vector<float> ver = {
            crds[i].point.x,     crds[i].point.y,     0.0f, crds[i].tex_cords.min_u, crds[i].tex_cords.min_v,
            crds[i].point.x + 1, crds[i].point.y,     0.0f, crds[i].tex_cords.max_u, crds[i].tex_cords.min_v,
            crds[i].point.x + 1, crds[i].point.y + 1, 0.0f, crds[i].tex_cords.max_u, crds[i].tex_cords.max_v,
            crds[i].point.x,     crds[i].point.y + 1, 0.0f, crds[i].tex_cords.min_u, crds[i].tex_cords.max_v};
        verticies.push_back(ver);
    }
    for (int i{}; i < size; i++)
    {
        vbos[i].init(verticies[i].data(), verticies[i].size() * sizeof(float));
        vaos[i].init();
        vaos[i].add_buffer(vbos[i], 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        vaos[i].add_buffer(vbos[i], 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        emos[i].init(ids.data(), sizeof(unsigned int) * ids.size());
        vaos[i].unbind();
        sh_prog->usage();
        sh_prog->setInt("tex", 0);
    }
}

void Render::TileRender::update()
{
    unsigned int size = crds.size();
    for (int i{}; i < size; i++)
    {
        sh_prog->usage();
        tex->bind();
        sh_prog->setMat4("projection", cam->get_proj_matrix());
        sh_prog->setMat4("view", cam->get_view_matrix());
        sh_prog->setMat4("model", glm::mat4(1.0f));
        vbos[i].update_data(verticies[i].data(), sizeof(float) * verticies[i].size());
    }
}

void Render::TileRender::draw()
{
    unsigned int size = crds.size();
    for (int i{}; i < size; i++)
    {
        sh_prog->usage();
        vaos[i].bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}