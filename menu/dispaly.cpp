#include <random>

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

static LGFX lcd(240, 120);

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

#include "menu/container.h"

uint32_t updateTimeStamp() { return SDL_GetTicks(); }

void menuTest1cb(const menu::MenuItem *menu_item)
{
    /* Draw menu */
    canvas->setFont(&fonts::efontCN_24);
    canvas->setTextColor(TFT_WHITE);

    canvas->setTextSize(menu_item->getSize().height / 24.0);
    canvas->drawString(menu_item->getTag().c_str(), menu_item->getPosition().x,
                       menu_item->getPosition().y);
}

void menuTest1()
{
    auto *container = new menu::MenuContainer;
    container->setRenderCallback(menuTest1cb);

    int text_width = 12;
    int text_height = 24;
    int text_size = 1;
    std::string tag_list[]
        = {"LCD test", "RTC time", "Brightness", "Buzzer test", "SD test", "Button test"};

    for (int i = 0; i < 6; i += 1) {
        container->addItem(tag_list[i], {10, 10 + (text_height + 2) * text_size * i},
                           {static_cast<int>(text_width * text_size * tag_list[i].size()),
                            text_height * text_size});
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

void setup()
{
    lcd.init();
    canvas = new LGFX_Sprite(&lcd);
    canvas->createSprite(lcd.width(), lcd.height());

    canvas->fillScreen(TFT_BLUE);
    canvas->pushSprite(0, 0);

    canvas->setFont(&fonts::efontCN_24);
}

void loop() { menuTest1(); }
