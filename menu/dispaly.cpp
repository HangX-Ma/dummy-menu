#include "menu/lv_anim.h"
#include <random>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

static LGFX lcd(240, 240);

static LGFX_Sprite *canvas;

// clang-format off
#define BTN_LEFT  39
#define BTN_DOWN  38
#define BTN_RIGHT 37
#define BTN_UP    36
// clang-format on

std::random_device rd;
std::mt19937 gen(rd());
int randomGenerator(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

#include "menu/menu.h"

uint32_t updateTimeStamp() { return SDL_GetTicks(); }

/* ================ CALLBACKS ================ */
void menuContainerTestCallback(const menu::MenuItem *menu_item)
{
    canvas->setFont(&fonts::efontCN_24);
    canvas->setTextColor(TFT_WHITE);

    canvas->setTextSize(menu_item->getSize().height / 24.0);
    canvas->drawString(menu_item->getTag().c_str(), menu_item->getPosition().x,
                       menu_item->getPosition().y);
}

void menuSelectorTestCallback(int x, int y, int width, int height)
{
    canvas->fillSmoothRoundRect(x, y, width, height, 5, TFT_SKYBLUE);
}

void menuCallback(const std::vector<menu::MenuItem *> &menu_item_list,
                  const menu::selector::RenderAttribute &selector,
                  const menu::scope::RenderAttribute &scope)
{
    canvas->clear();

    /* Draw selector */
    canvas->fillSmoothRoundRect(
        selector.x - scope.x,
        selector.y - scope.y
            + (menu_item_list[selector.selected_idx]->getSize().height - selector.height) / 2,
        selector.width, selector.height, 5, TFT_SKYBLUE);
    canvas->fillSmoothRoundRect(scope.width - 10 - scope.x, selector.y - scope.y, 10, 24, 2,
                                TFT_SKYBLUE);

    /* Draw menu */
    canvas->setFont(&fonts::efontCN_24);
    canvas->setTextColor(TFT_WHITE);

    for (const auto item : menu_item_list) {
        canvas->setTextSize(item->getSize().height / 24.0);
        canvas->drawString(item->getTag().c_str(), item->getPosition().x - scope.x,
                           item->getPosition().y - scope.y);
    }

    /* Draw scope */
    canvas->drawRect(0, 0, scope.width, scope.height, TFT_GREENYELLOW);

    canvas->pushSprite(0, 0);
}

/* ================ TESTS ================ */
void menuContainerTestNormal()
{
    auto *container = new menu::MenuContainer;
    container->setRenderCallback(menuContainerTestCallback);

    int text_width = 12;
    int text_height = 24;
    int text_size = 1;
    std::string tag_list[]
        = {"LCD test", "RTC time", "Brightness", "Buzzer test", "SD test", "Button test"};

    for (int i = 0; i < 6; i += 1) {
        container->addItem(tag_list[i],
                           {static_cast<int>(text_width * text_size * tag_list[i].size()),
                            text_height * text_size},
                           {10, 10 + (text_height + 2) * text_size * i});
    }

    while (true) {
        container->updateAnimValue(updateTimeStamp());

        canvas->clear();
        container->doRender();
        canvas->pushSprite(0, 0);
    }

    // no entry
    canvas->clear();
    container->clear(true);
    delete container;
}

void menuContainerTestVertical()
{
    auto *container = new menu::MenuContainer;
    container->setRenderCallback(menuContainerTestCallback);

    int text_width = 12;
    int text_height = 24;
    int text_size = 1;
    std::string tag_list[]
        = {"LCD test", "RTC time", "Brightness", "Buzzer test", "SD test", "Button test"};

    for (int i = 0; i < 6; i += 1) {
        container->addVItem(tag_list[i],
                            {static_cast<int>(text_width * text_size * tag_list[i].size()),
                             text_height * text_size},
                            {0, text_height});
    }

    while (true) {
        container->updateAnimValue(updateTimeStamp());

        canvas->clear();
        container->doRender();
        canvas->pushSprite(0, 0);
    }

    // no entry
    canvas->clear();
    container->clear(true);
    delete container;
}

void menuContainerTestHorizontal()
{
    auto *container = new menu::MenuContainer;
    container->setRenderCallback(menuContainerTestCallback);

    int text_width = 12;
    int text_height = 24;
    int text_size = 1;
    std::string tag_list[]
        = {"LCD test", "RTC time", "Brightness", "Buzzer test", "SD test", "Button test"};

    for (int i = 0; i < 6; i += 1) {
        container->addHItem(tag_list[i],
                            {static_cast<int>(text_width * text_size * tag_list[i].size()),
                             text_height * text_size},
                            {text_width, 0});
    }

    while (true) {
        container->updateAnimValue(updateTimeStamp());

        canvas->clear();
        container->doRender();
        canvas->pushSprite(0, 0);
    }

    // no entry
    canvas->clear();
    container->clear(true);
    delete container;
}

void menuSelectorTest()
{
    auto *container = new menu::MenuContainer;
    container->setRenderCallback(menuContainerTestCallback);

    int text_width = 12;
    int text_height = 24;
    int text_size = 1;
    std::string tag_list[]
        = {"LCD test", "RTC time", "Brightness", "Buzzer test", "SD test", "Button test"};

    for (int i = 0; i < 6; i += 1) {
        container->addItem(tag_list[i],
                           {static_cast<int>(text_width * text_size * tag_list[i].size()),
                            text_height * text_size},
                           {10, 10 + (text_height + 2) * text_size * i});
    }

    auto *selector = new menu::MenuSelector;
    selector->setMenuContainer(container);
    selector->setRenderCallback(menuSelectorTestCallback);
    selector->switchSelectorMode(true);
    selector->reset(updateTimeStamp());
    selector->setAnimPathType({lvgl::LVAnimPathType::OVERSHOOT, lvgl::LVAnimPathType::OVERSHOOT,
                               lvgl::LVAnimPathType::OVERSHOOT, lvgl::LVAnimPathType::EASE_OUT});
    while (true) {
        selector->updateAnimValue(updateTimeStamp());
        canvas->clear();
        selector->doRender(true);
        canvas->pushSprite(0, 0);

        int time_count = 0;
        if (!lgfx::gpio_in(BTN_DOWN)) {
            selector->next();
            while (!lgfx::gpio_in(BTN_DOWN)) {

                selector->updateAnimValue(updateTimeStamp());
                canvas->clear();
                selector->doRender(true);
                canvas->pushSprite(0, 0);

                time_count++;
                if (time_count > 2000) {
                    break;
                }
            }
        }

        if (!lgfx::gpio_in(BTN_UP)) {
            selector->last();
            while (!lgfx::gpio_in(BTN_UP)) {

                selector->updateAnimValue(updateTimeStamp());
                canvas->clear();
                selector->doRender(true);
                canvas->pushSprite(0, 0);

                time_count++;
                if (time_count > 2000) {
                    break;
                }
            }
        }

        if (!lgfx::gpio_in(BTN_RIGHT)) {
            selector->pressed();
            while (!lgfx::gpio_in(BTN_RIGHT)) {
                selector->updateAnimValue(updateTimeStamp());
                canvas->clear();
                selector->doRender(true);
                canvas->pushSprite(0, 0);
            }
            selector->released();
        }
    }
}

void menuTest()
{
    menu::Menu menu(240, 100);

    menu.getScope()->setAnimPathType(
        {lvgl::LVAnimPathType::OVERSHOOT, lvgl::LVAnimPathType::OVERSHOOT});
    menu.getScope()->setAnimDuration({400, 400});
    menu.setRenderCallback(menuCallback);
    menu.switchLoopMode(true);

    int text_width = 12;
    int text_height = 24;
    int text_size = 1;
    std::string tag_list[]
        = {"LCD test", "RTC time", "Brightness", "Buzzer test", "SD test", "Button test"};

    for (int i = 0; i < 6; i += 1) {
        menu.getMenu()->addItem(tag_list[i],
                                {static_cast<int>(text_width * text_size * tag_list[i].size()),
                                 text_height * text_size},
                                {10, 10 + (text_height + 2) * text_size * i});
    }

    while (true) {
        menu.updateAnimValue(updateTimeStamp());

        int time_count = 0;
        if (!lgfx::gpio_in(BTN_DOWN)) {
            menu.next();
            while (!lgfx::gpio_in(BTN_DOWN)) {
                menu.updateAnimValue(updateTimeStamp());
                time_count += 1;
                if (time_count > 2000) {
                    break;
                }
            }
        }

        time_count = 0;
        if (!lgfx::gpio_in(BTN_UP)) {
            menu.last();
            while (!lgfx::gpio_in(BTN_UP)) {
                menu.updateAnimValue(updateTimeStamp());
                time_count += 1;
                if (time_count > 2000) {
                    break;
                }
            }
        }

        if (!lgfx::gpio_in(BTN_RIGHT)) {
            menu.getSelector()->pressed();
            while (!lgfx::gpio_in(BTN_RIGHT)) {
                menu.updateAnimValue(updateTimeStamp());
            }
            menu.getSelector()->released();
        }
    }
}

void setup()
{
    lcd.init();
    canvas = new LGFX_Sprite(&lcd);
    canvas->createSprite(lcd.width(), lcd.height());

    canvas->fillScreen(TFT_BLUE);
    canvas->pushSprite(0, 0);

    canvas->setFont(&fonts::efontCN_24);
}

// clang-format off
void loop() {
    // menuContainerTestNormal();
    // menuContainerTestVertical();
    // menuContainerTestHorizontal();
    // menuSelectorTest();
    menuTest();
}
