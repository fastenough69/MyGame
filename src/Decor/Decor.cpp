#include "Decor.h"

Objects::uvCoords Objects::get_uv_coords(const SizeTexture &size)
{
    uvCoords result{};
    result.min_u = size.x / size.fullWidht;
    result.min_v = size.y / size.fullHeight;
    result.max_u = (size.x + size.widht) / size.fullWidht;
    result.max_v = (size.y + size.height) / size.fullHeight;
    return result;
}

std::vector<std::vector<float>> Objects::new_coords(std::vector<float> first, unsigned int count)
{
    std::vector<std::vector<float>> res{count, first};
    float widht = res[0][10];
    for (int i = 1; i < count; i++)
    {

        res[i][0] = res[i - 1][10];
        res[i][5] = res[i - 1][10] + widht;
        res[i][10] = res[i - 1][10] + widht;
        res[i][15] = res[i - 1][10];
    }

    /*for (int i{}; i < count; i++)
    {
        for (int j{}; j < 20; j++)
        {
            std::cout << res[i][j] << ' ';
        }
        std::cout << std::endl;
    }*/
    return res;
}

void Objects::DecorObj::swap(DecorObj copy)
{
    std::swap(cam, copy.cam);
    std::swap(shProg, copy.shProg);
    std::swap(tex, copy.tex);
    std::swap(ver, copy.ver);
    std::swap(ids, copy.ids);
    std::swap(cord, copy.cord);
    vbo = std::move(copy.vbo);
    emo = std::move(copy.emo);
    vao = std::move(copy.vao);
}

Objects::DecorObj::DecorObj(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> prog,
                            std::shared_ptr<Render::Texture2D> tx, SizeTexture &&size, std::vector<float> &&vr,
                            std::vector<unsigned int> &&indices)
    : cam{cm}, shProg{prog}, tex{tx}, ver{vr}, ids{indices}
{
    cord = get_uv_coords(size);
    if (ver.size() == 20)
    {
        ver[3] = cord.min_u;
        ver[4] = cord.min_v;
        ver[8] = cord.max_u;
        ver[9] = cord.min_v;
        ver[13] = cord.max_u;
        ver[14] = cord.max_v;
        ver[18] = cord.min_u;
        ver[19] = cord.max_v;
    }
    vbo = Render::VertexBuffArr{};
    emo = Render::IndexBuff{};
    vao = Render::VertexArr{};
}

Objects::DecorObj::DecorObj(const DecorObj& right)
{
    if(this == &right)
    {
        return;
    }
    cam = right.cam;
    shProg = right.shProg;
    tex = right.tex;
    cord = right.cord;
    ver = right.ver;
    ids = right.ids;
    vbo = Render::VertexBuffArr{};
    emo = Render::IndexBuff{};
    vao = Render::VertexArr{};
}

Objects::DecorObj& Objects::DecorObj::operator=(const DecorObj& right)
{
    DecorObj copy{ right };
    swap(copy);
    return *this;
}

Objects::DecorObj::DecorObj(DecorObj&& right) noexcept
{
    if (this == &right)
    {
        return;
    }
    cam = right.cam;
    shProg = right.shProg;
    tex = right.tex;
    cord = right.cord;
    ver = right.ver;
    ids = right.ids;
    vbo = std::move(right.vbo);
    emo = std::move(right.emo);
    vao = std::move(right.vao);
    right.cam = nullptr;
    right.shProg = nullptr;
    right.tex = nullptr;
}

Objects::DecorObj& Objects::DecorObj::operator=(DecorObj&& right) noexcept
{
    DecorObj copy{ std::move(right) };
    swap(copy);
    return *this;
}

void Objects::DecorObj::init()
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

void Objects::DecorObj::update()
{
    shProg->usage();
    tex->bind();
    shProg->setMat4("projection", cam->get_proj_matrix());
    shProg->setMat4("view", cam->get_view_matrix());
    shProg->setMat4("model", glm::mat4(1.0f));
    vbo.update_data(ver.data(), sizeof(float) * ver.size());
}

void Objects::DecorObj::render()
{
    shProg->usage();
    vao.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Objects::DecorObj::set_vertecies(std::vector<float> &new_ver)
{
    ver = new_ver;
}

std::vector<float> Objects::DecorObj::get_vertecies() const
{
    return ver;
}
