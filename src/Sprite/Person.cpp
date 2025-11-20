#include "Person.h"

void PersonProcesing::Person::loadTexForSprite(std::string &templateNameTexFile)
{
    std::vector<Render::Texture2D> res;

    size_t pos = templateNameTexFile.find_last_of('.');
    templateNameTexFile = templateNameTexFile.substr(0, pos);

    for (int i = 1; i <= countFrames; i++)
    {
        char buff[20] = {};
        std::string tempNameFile(templateNameTexFile + itoa(i, buff, 10) + ".png");
    }
}
// сделать ее в ResManager