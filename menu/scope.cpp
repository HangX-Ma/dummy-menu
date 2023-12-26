#include "menu/scope.h"

namespace menu
{

void scope::Render::update(const MenuItem *menu_item, Size_t outside_box, uint32_t new_time,
                           bool target_changed)
{
    /* Keep target inside [x, x + width], [y, y + height] */
    auto [x, y] = menu_item->getPosition();
    auto [width, height] = menu_item->getSize();
    auto [box_width, box_height] = outside_box;

    anim.x.setPathCallback(anim_type.x);
    anim.x.setDuration(anim_duration.x);
    anim.x.setCurrentValue(current_time);

    if (x < anim.x.getValue(new_time)) {
        anim.x.setValues(anim.x.getValue(new_time), x);
    }
    if (x + width > (anim.x.getValue(new_time) + box_width)) {
        anim.x.setValues(anim.x.getValue(new_time), x); // Align Left
        // anim.x.setValues(anim.x.getValue(new_time), x - box_width + width); // Align Right
    }

    if (y < anim.y.getValue(new_time)) {
        anim.y.setValues(anim.y.getValue(new_time), y);
    }
    if (y + height > (anim.y.getValue(new_time) + box_height)) {
        // anim.x.setValues(anim.x.getValue(new_time), y); // Align Up
        anim.y.setValues(anim.y.getValue(new_time), y - box_height + height); // Align Down
    }

    current_time = new_time;
}

} // namespace menu
