#include "Touch.h"

#include "DxLib.h"

int Touch::getID(size_t index)
{
    if (index >= ids.size())
    {
        return -1;
    }
    return ids[index];
}

int Touch::getX(int id)
{
    return infos[id].x;
}

int Touch::getY(int id)
{
    return infos[id].y;
}

int Touch::getDrevice(int id)
{
    return infos[id].device;
}

int Touch::getPressingCount(int id)
{
    return infos[id].pressing;
}

int Touch::getReleasingCount(int id)
{
    return infos[id].releasing;
}

size_t Touch::size()
{
    return infos.size();
}

bool Touch::update()
{
    for (size_t i = 0, size = infos.size(); i < size; ++i)
    {
        infos[getID(i)].flag = true;
    }
    for (size_t i = 0, size = GetTouchInputNum(); i < size; ++i)
    {
        int x;
        int y;
        int id;
        int device;
        GetTouchInput(i, &x, &y, &id, &device);
        infos[id].x = x;
        infos[id].y = y;
        infos[id].device = device;
        infos[id].flag = false;

        ++infos[id].pressing;
        infos[id].releasing = 0;

        bool flag = true;
        for (size_t j = 0, ssize = ids.size(); j < ssize; ++j)
        {
            if (ids[j] == id)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            ids.push_back(id);
        }
    }
    for (size_t i = 0, size = infos.size(); i < size; ++i)
    {
        if (infos[getID(i)].flag)
        {
            ++infos[getID(i)].releasing;
            infos[getID(i)].pressing = 0;
        }
    }
    return false;
}
