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

struct RenderAttribute
{
    int x;
    int y;
    int width;
    int height;
    int selected_idx;
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

    void update(const MenuItem *menu_item, Position_t prev_pos, uint32_t new_time,
                bool target_changed);
    void render();

    void reset(uint32_t new_time)
    {
        anim.x.setDuration(1);
        anim.x.setValues(0, 0);
        anim.y.setDuration(1);
        anim.y.setValues(0, 0);
        anim.width.setDuration(0);
        anim.width.setValues(0, 0);
        anim.height.setDuration(0);
        anim.height.setValues(0, 0);
        current_time = new_time;
        setAnimCurrentValue(new_time);
    }

    void setRenderCallback(CallbackPtr new_cb) { cb = new_cb; };
    void setAnimPathType(Type_t type) { anim_type = type; }
    void setAnimDuration(Value_t duration) { anim_duration = duration; }
    void setAnimCurrentValue(uint32_t new_time)
    {
        anim.x.setCurrentValue(new_time);
        anim.y.setCurrentValue(new_time);
        anim.width.setCurrentValue(new_time);
        anim.height.setCurrentValue(new_time);
    }

    Value_t getProperty()
    {
        return {anim.x.getValue(current_time), anim.y.getValue(current_time),
                anim.width.getValue(current_time), anim.height.getValue(current_time)};
    }

protected:
    bool isFinished(uint32_t new_time)
    {
        return anim.x.isFinished(new_time) && anim.y.isFinished(new_time)
               && anim.width.isFinished(new_time) && anim.height.isFinished(new_time);
    }

protected:
    LVAnim_t anim;
    Type_t anim_type{lvgl::LVAnimPathType::EASE_OUT, lvgl::LVAnimPathType::EASE_OUT,
                     lvgl::LVAnimPathType::OVERSHOOT, lvgl::LVAnimPathType::EASE_OUT};
    Value_t anim_duration{100, 100, 400, 200};
    uint32_t current_time{0};
    CallbackPtr cb{nullptr};
};

} // namespace selector

using Status_t = struct Status
{
    int prev_selected{0};
    int selected{0};
    bool changed{false};

    void reset()
    {
        prev_selected = 0;
        selected = 0;
        changed = true;
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

    void reset(uint32_t new_time)
    {
        render_.reset(new_time);
        updateAnimValue(0, true, true);
    }

    void last() { gotoItem(status.selected - 1); }
    void next() { gotoItem(status.selected + 1); }
    void front() { gotoItem(0); }
    void back() { gotoItem(menu_container->size() - 1); }

    void switchSelectorMode(bool is_loop) { loop_mode = is_loop; }
    void setRenderCallback(selector::CallbackPtr new_cb) { render_.setRenderCallback(new_cb); };
    void setAnimPathType(selector::Type_t type) { render_.setAnimPathType(type); }
    void setAnimDuration(selector::Value_t duration) { render_.setAnimDuration(duration); }
    void setAnimCurrentValue(uint32_t new_time) { render_.setAnimCurrentValue(new_time); }

    void pressed()
    {
        if (menu_container == nullptr) {
            return;
        }
        render_.pressed(menu_container->at(status.selected));
    }
    void released()
    {
        if (menu_container == nullptr) {
            return;
        }
        render_.release(menu_container->at(status.selected));
    }

    void updateAnimValue(uint32_t new_time, bool render_immediately = false,
                         bool render_menu = true)
    {
        if (menu_container == nullptr) {
            return;
        }

        render_.update(menu_container->at(status.selected),
                       menu_container->at(status.prev_selected)->getPosition(), new_time,
                       status.changed);

        // reset status if necessary
        if (status.changed) {
            status.changed = false;
        }

        if (render_immediately) {
            doRender(render_menu);
        }
    }

    void doRender(bool render_menu = true)
    {
        render_.render();
        if (render_menu && menu_container != nullptr) {
            menu_container->doRender();
        }
    }

    selector::RenderAttribute getRenderAttribute()
    {
        auto property = render_.getProperty();
        return {property.x, property.y, property.width, property.height, status.selected};
    }

protected:
    MenuContainer *menu_container{nullptr};
    bool loop_mode{false};
    Status_t status;

protected:
    void gotoItem(int item_id);
    int checkBound(int item_id);

private:
    selector::Render render_;
};

} // namespace menu

#endif