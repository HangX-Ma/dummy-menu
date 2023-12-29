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

int MenuContainer::addItem(std::string tag, Size_t size, Position_t pos, void *user_data)
{
    auto *item = new MenuItem;

    item->setTag(std::move(tag));
    item->setPosition(this->pos + pos);
    item->setSize(size);
    item->setUserData(user_data);
    item->setDestination(pos);

    return addItem(item);
}

int MenuContainer::addVItem(std::string tag, Size_t size, Position_t padding, void *user_data)
{
    int y = 0;
    if (!menu_item_list_.empty()) {
        y = menu_item_list_.back()->getPosition().y + menu_item_list_.back()->getSize().height
            - this->pos.y + padding.y;
    }
    return addItem(std::move(tag), size, {padding.x, y}, user_data);
}

int MenuContainer::addHItem(std::string tag, Size_t size, Position_t padding, void *user_data)
{
    int x = 0;
    if (!menu_item_list_.empty()) {
        x = menu_item_list_.back()->getPosition().x + menu_item_list_.back()->getSize().width
            - this->pos.x + padding.x;
    }
    return addItem(std::move(tag), size, {x, padding.y}, user_data);
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
