#ifndef MENU_SELECTOR_H
#define MENU_SELECTOR_H

#include "menu/lv_anim.h"
#include "menu/container.h"

namespace menu
{

namespace selector
{

template <typename T>
struct LVAnimProperty
{
    T x;
    T y;
    T width;
    T height;
};

using Type_t = LVAnimProperty<lvgl::LVAnimPathType>;
using Value_t = LVAnimProperty<int32_t>;
using LVAnim_t = LVAnimProperty<lvgl::LVAnim>;
using CallbackPtr = void (*)(int x, int y, int width, int height);

class Render
{
public:
    void pressed(const MenuItem *menu_item);
    void release(const MenuItem *menu_item);
    void update(const MenuItem *menu_item, uint32_t new_time, bool target_changed);
    void render();

    void setRenderCallback(CallbackPtr new_cb) { cb = new_cb; };

protected:
    bool isFinished(uint32_t new_time)
    {

        return anim.x.isFinished(new_time) && anim.y.isFinished(new_time)
               && anim.width.isFinished(new_time) && anim.height.isFinished(new_time);
    }

protected:
    LVAnim_t anim;
    Type_t anim_type;
    Value_t anim_duration;
    uint32_t current_time{0};
    CallbackPtr cb{nullptr};
};

} // namespace selector

using Status_t = struct Status
{
    int selected{0};
    bool changed{false};

    void reset()
    {
        selected = 0;
        changed = false;
    }
};

class MenuSelector
{
public:
    void setMenuContainer(MenuContainer *menu_container)
    {
        this->menu_container = menu_container;
        status.reset();
    }

    void last() { gotoItem(status.selected - 1); }
    void next() { gotoItem(status.selected + 1); }
    void front() { gotoItem(0); }
    void back() { gotoItem(menu_container->size() - 1); }

    void updateAnimValue(uint32_t new_time, bool render_immediately = true)
    {
        render_.update(menu_container->at(status.selected), new_time, status.changed);

        // reset status if necessary
        if (status.changed) {
            status.changed = false;
        }

        if (render_immediately) {
            doRender();
        }
    }

    void doRender(bool render_menu = true)
    {
        render_.render();
        if (render_menu) {
            menu_container->doRender();
        }
    }

protected:
    MenuContainer *menu_container;
    bool loop_mode;
    Status_t status;

protected:
    void gotoItem(int item_id);
    int checkBound(int item_id);

private:
    selector::Render render_;
};

} // namespace menu

#endif