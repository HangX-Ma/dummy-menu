#include "menu/container.h"

#include <utility>

namespace menu
{

namespace container
{
void Render::update(std::vector<MenuItem *> &menu_item_list, uint32_t new_time)
{
    if (!triggered) {
        triggered = true;
        anim.open.setCurrentValue(new_time);
    }

    if (isFinished(new_time)) {
        return;
    }

    anim.open.setPathCallback(anim_type.open);
    anim.open.setDuration(anim_duration.open);

    int x;
    int y;
    for (auto item : menu_item_list) {
        auto [dsx, dsy] = item->getDestination();

        anim.open.setValues(0, dsx);
        x = anim.open.getValue(new_time);
        anim.open.setValues(0, dsy);
        y = anim.open.getValue(new_time);

        item->setPosition({x, y});
    }
    current_time = new_time;
}

void Render::render(const MenuItem *item)
{
    if (cb == nullptr) {
        return;
    }
    cb(item);
}

bool Render::isFinished(uint32_t new_time) { return anim.open.isFinished(new_time); }

} // namespace container

int MenuContainer::addItem(MenuItem *item)
{
    if (item == nullptr) {
        return -1;
    }
    item->setId(menu_item_list_.size());
    menu_item_list_.push_back(item);
    return item->getId();
}

int MenuContainer::addItem(std::string tag, Position_t pos, Size_t size, void *user_data)
{
    auto *item = new MenuItem;

    item->setTag(std::move(tag));
    item->setPosition(this->pos + pos);
    item->setSize(size);
    item->setUserData(user_data);
    item->setDestination(pos);

    return addItem(item);
}

void MenuContainer::clear(bool free_mem)
{
    if (free_mem) {
        for (auto item : menu_item_list_) {
            delete item;
        }
    }
    return menu_item_list_.clear();
}

void MenuContainer::updateAnimValue(uint32_t new_time)
{
    render_.update(menu_item_list_, new_time);
}

void MenuContainer::doRender()
{
    for (const auto item : menu_item_list_) {
        render_.render(item);
    }
}
} // namespace menu
