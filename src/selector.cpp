#include "menu/selector.h"

namespace menu
{

namespace selector
{
void Render::pressed(const MenuItem *menu_item)
{
    auto [width, height] = menu_item->getSize();

    /* squeeze selector  */
    anim.width.setPathCallback(anim_type.width);
    anim.width.setDuration(anim_duration.width);
    anim.width.setValues(anim.width.getValue(current_time), width / 4 * 5);
    anim.width.setCurrentValue(current_time);

    anim.height.setPathCallback(anim_type.height);
    anim.height.setDuration(anim_duration.height);
    anim.height.setValues(anim.height.getValue(current_time), height / 4 * 5);
    anim.height.setCurrentValue(current_time);
}

void Render::release(const MenuItem *menu_item)
{
    auto [width, height] = menu_item->getSize();

    /* expand selector  */
    anim.width.setPathCallback(anim_type.width);
    anim.width.setDuration(anim_duration.width);
    anim.width.setValues(anim.width.getValue(current_time), width);
    anim.width.setCurrentValue(current_time);

    anim.height.setPathCallback(anim_type.height);
    anim.height.setDuration(anim_duration.height);
    anim.height.setValues(anim.height.getValue(current_time), height);
    anim.height.setCurrentValue(current_time);
}

void Render::update(const MenuItem *menu_item, Position_t prev_pos, uint32_t new_time,
                    bool target_changed)
{
    if (target_changed) {
        auto [width, height] = menu_item->getSize();
        anim.width.setPathCallback(anim_type.width);
        anim.width.setDuration(anim_duration.width);
        anim.width.setValues(anim.width.getValue(new_time), width);
        anim.width.setCurrentValue(current_time);

        anim.height.setPathCallback(anim_type.height);
        anim.height.setDuration(anim_duration.height);
        anim.height.setValues(anim.height.getValue(new_time), height);
        anim.height.setCurrentValue(current_time);

        auto [x, y] = menu_item->getPosition();
        anim.x.setPathCallback(anim_type.x);
        anim.x.setDuration(anim_duration.x);
        anim.x.setValues(prev_pos.x + anim.x.getValue(new_time), x);
        anim.x.setCurrentValue(current_time);

        anim.y.setPathCallback(anim_type.y);
        anim.y.setDuration(anim_duration.y);
        anim.y.setValues(prev_pos.y + anim.height.getValue(new_time), y);
        anim.y.setCurrentValue(current_time);
    }
    current_time = new_time;
}

void Render::render()
{
    if (cb != nullptr) {
        cb(anim.x.getValue(current_time), anim.y.getValue(current_time),
           anim.width.getValue(current_time), anim.height.getValue(current_time));
    }
}

} // namespace selector

void MenuSelector::gotoItem(int item_id)
{
    item_id = checkBound(item_id);
    if (item_id < 0) {
        return;
    }
    status.prev_selected = status.selected;
    status.selected = item_id;
    status.changed = true;
}

int MenuSelector::checkBound(int item_id)
{
    int ret = item_id;
    if (menu_container == nullptr) {
        return -1;
    }

    if (item_id > menu_container->size() - 1) {
        ret = loop_mode ? 0 : menu_container->size() - 1;
    }

    if (item_id < 0) {
        ret = loop_mode ? menu_container->size() - 1 : 0;
    }

    return ret;
}

} // namespace menu