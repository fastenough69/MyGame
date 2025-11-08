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

        res[i][0] = res[i - 1][10] - 5.0f;
        res[i][5] = res[i - 1][10] + widht;
        res[i][10] = res[i - 1][10] + widht;
        res[i][15] = res[i - 1][10];
    }
    return res;
}

void Objects::set_new_coord(const std::vector<float> &prev, std::vector<float> &curr, float widht)
{
    curr[0] = prev[10] - 5.0f;
    curr[5] = prev[10] + widht;
    curr[10] = prev[10] + widht;
    curr[15] = prev[10];
}

void Objects::DecorObj::swap(DecorObj copy)
{
    std::swap(cam, copy.cam);
    std::swap(shProg, copy.shProg);
    std::swap(tex, copy.tex);
    std::swap(ver, copy.ver);
    std::swap(ids, copy.ids);
    std::swap(cord, copy.cord);
    std::swap(point, copy.point);
    vbo = std::move(copy.vbo);
    emo = std::move(copy.emo);
    vao = std::move(copy.vao);
}

void Objects::DecorObj::set_new_point(glm::vec2 mousePos, GLFWwindow *window)
{
    if ((mousePos.x >= point.x || mousePos.x <= point.x + sizeTx.widht) &&
        (mousePos.y >= point.y || mousePos.y <= point.y + sizeTx.height))
    {
        /* std::cout << "Mouse in object\n";
         if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
         {
             std::cout << "Move object with mouse\n";
             point = mousePos;
         }*/
    }
}

Objects::DecorObj::DecorObj(std::shared_ptr<Camera::Camera2D> cm, std::shared_ptr<Render::ProgramShader> prog,
                            std::shared_ptr<Render::Texture2D> tx, SizeTexture &&size, glm::vec2 &&pt,
                            std::vector<unsigned int> &&indices, glm::vec2 size_draw)
    : cam{cm}, shProg{prog}, tex{tx}, ids{indices}, sizeTx{size}, point{pt}
{
    cord = get_uv_coords(sizeTx);
    ver = std::vector<float>(20, 0.0f);
    float widht = 0, height = 0;
    if (size_draw.x != 0 && size_draw.y != 0)
    {
        widht = size_draw.x;
        height = size_draw.y;
    }
    else
    {
        widht = sizeTx.widht;
        height = sizeTx.height;
    }

    if (ver.size() == 20)
    {
        // vertecies
        ver[0] = point.x;
        ver[1] = point.y;
        ver[5] = point.x + widht;
        ver[6] = point.y;
        ver[10] = point.x + widht;
        ver[11] = point.y + height;
        ver[15] = point.x;
        ver[16] = point.y + height;
        // tex coord
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

Objects::DecorObj::DecorObj(const DecorObj &right)
{
    if (this == &right)
    {
        return;
    }
    cam = right.cam;
    shProg = right.shProg;
    tex = right.tex;
    sizeTx = right.sizeTx;
    cord = right.cord;
    point = right.point;
    ver = right.ver;
    ids = right.ids;
    vbo = Render::VertexBuffArr{};
    emo = Render::IndexBuff{};
    vao = Render::VertexArr{};
}

Objects::DecorObj &Objects::DecorObj::operator=(const DecorObj &right)
{
    DecorObj copy{right};
    swap(copy);
    return *this;
}

Objects::DecorObj::DecorObj(DecorObj &&right) noexcept
{
    if (this == &right)
    {
        return;
    }
    cam = right.cam;
    shProg = right.shProg;
    tex = right.tex;
    cord = right.cord;
    sizeTx = right.sizeTx;
    point = right.point;
    ver = right.ver;
    ids = right.ids;
    vbo = std::move(right.vbo);
    emo = std::move(right.emo);
    vao = std::move(right.vao);
    // right.cam = nullptr;
    right.cam.reset();
    right.shProg.reset();
    right.tex.reset();
}

Objects::DecorObj &Objects::DecorObj::operator=(DecorObj &&right) noexcept
{
    DecorObj copy{std::move(right)};
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
    // set_new_point(mousePos, window);
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

void Objects::DecorObj::set_uvCord(SizeTexture &&size)
{
    cord = get_uv_coords(size);
}

Objects::SizeTexture Objects::DecorObj::get_szTexture() const
{
    return sizeTx;
}