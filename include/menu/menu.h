#ifndef MENU_MENU_H
#define MENU_MENU_H

#include "menu/container.h"
#include "menu/scope.h"
#include "menu/selector.h"

namespace menu
{

using CallbackPtr
    = void (*)(const std::vector<MenuItem *> &menu_item_list,
               const selector::RenderAttribute_t &selector, const scope::RenderAttribute &scope);

class Menu
{
public:
    explicit Menu(int scope_width = 100, int scope_height = 100);
    ~Menu();

    MenuContainer *getMenuContainer() { return container_; }
    MenuSelector *getSelector() { return selector_; }
    MenuScope *getScope() { return scope_; }

    void updateAnimValue(uint32_t new_time, bool render_immediately = true);
    void doRender();

    void last()
    {
        selector_->last();
        scope_->last();
    }

    void next()
    {
        selector_->next();
        scope_->next();
    }

    void front()
    {
        selector_->front();
        scope_->front();
    }

    void back()
    {
        selector_->back();
        scope_->back();
    }

    void switchLoopMode(bool is_loop)
    {
        selector_->switchSelectorMode(is_loop);
        scope_->switchSelectorMode(is_loop);
    }

    void setRenderCallback(CallbackPtr new_cb) { render_cb_ = new_cb; }

private:
    MenuContainer *container_{nullptr};
    MenuSelector *selector_{nullptr};
    MenuScope *scope_{nullptr};
    CallbackPtr render_cb_{nullptr};
    bool trigger_{false};
};

} // namespace menu

#endif