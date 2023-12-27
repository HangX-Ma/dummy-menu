#ifndef MENU_SCOPE_H
#define MENU_SCOPE_H

#include "menu/selector.h"

namespace menu
{

namespace scope
{

using RenderAttribute = selector::LVAnimProperty<int32_t>;

class Render : public selector::Render
{
public:
    void update(const MenuItem *menu_item, Size_t outside_box, uint32_t new_time,
                bool target_changed);
    void render(Size_t scope_size);
};

} // namespace scope

class MenuScope : public MenuSelector
{
public:
    void setSize(Size_t new_size) { size_ = new_size; }
    void updateAnimValue(uint32_t new_time, bool render_immediately = true)
    {
        render_.update(menu_container, size_, new_time, status.changed);

        if (status.changed) {
            status.changed = false;
        }
        if (render_immediately) {
            doRender();
        }
    }

    void doRender(bool render_menu = true)
    {
        render_.render(size_);
        if (render_menu) {
            menu_container->doRender();
        }
    }

    scope::RenderAttribute getRenderAttribute()
    {
        auto property = render_.getProperty();
        return {property.x, property.y, size_.width, size_.height};
    }

private:
    Size_t size_;
    scope::Render render_;
};

} // namespace menu

#endif