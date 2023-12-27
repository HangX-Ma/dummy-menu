#include "menu/scope.h"

namespace menu
{

void scope::Render::update(const MenuItem *menu_item, Size_t outside_box, uint32_t new_time,
                           bool target_changed)
{
    if (target_changed) {
        /* Keep target inside [x, x + width], [y, y + height] */
        auto [x, y] = menu_item->getPosition();
        auto [width, height] = menu_item->getSize();
        auto [box_width, box_height] = outside_box;

        anim.x.setPathCallback(anim_type.x);
        anim.x.setDuration(anim_duration.x);
        anim.y.setPathCallback(anim_type.y);
        anim.y.setDuration(anim_duration.y);

        if (x < anim.x.getValue(new_time)) {
            anim.x.setValues(anim.x.getValue(new_time), x);
        }
        else if (x + width > (anim.x.getValue(new_time) + box_width)) {
            anim.x.setValues(anim.x.getValue(new_time), x); // Align Left
            // anim.x.setValues(anim.x.getValue(new_time), x - box_width + width); // Align Right
        }
        anim.x.setCurrentValue(current_time);

        if (y < anim.y.getValue(new_time)) {
            anim.y.setValues(anim.y.getValue(new_time), y);
        }
        else if (y + height > (anim.y.getValue(new_time) + box_height)) {
            // anim.x.setValues(anim.x.getValue(new_time), y); // Align Up
            anim.y.setValues(anim.y.getValue(new_time), y - box_height + height); // Align Down
        }
        anim.y.setCurrentValue(current_time);
    }
    current_time = new_time;
}

void scope::Render::render(Size_t scope_size)
{
    if (cb != nullptr) {
        cb(anim.x.getValue(current_time), anim.y.getValue(current_time), scope_size.width,
           scope_size.height);
    }
}

} // namespace menu
