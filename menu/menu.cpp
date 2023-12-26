#include "menu/menu.h"
#include "menu/container.h"
#include "menu/selector.h"

namespace menu
{

Menu::Menu(int scope_width, int scope_height)
{
    container_ = new MenuContainer;

    selector_ = new MenuSelector;
    selector_->setMenuContainer(container_);

    scope_ = new MenuScope;
    scope_->setMenuContainer(container_);
    scope_->setSize({scope_width, scope_height});
}

Menu::~Menu()
{
    delete container_;
    delete selector_;
    delete scope_;
}

void Menu::updateAnimValue(uint32_t new_time, bool render_immediately)
{
    if (container_ == nullptr) {
        return;
    }
    if (container_->empty()) {
        return;
    }

    if (!trigger_) {
        trigger_ = true;
        selector_->setAnimCurrentValue(new_time);
        selector_->front();
        scope_->front();
        selector_->updateAnimValue(0);
    }
    container_->updateAnimValue(new_time);
    selector_->updateAnimValue(new_time, false);
    scope_->updateAnimValue(new_time, false);

    if (render_immediately) {
        doRender();
    }
}
void Menu::doRender()
{
    if (render_cb_ == nullptr) {
        return;
    }
    render_cb_(container_->getMenuItemList(), selector_->getRenderAttribute(),
               scope_->getRenderAttribute());
}

} // namespace menu