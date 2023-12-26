#ifndef MENU_CONTAINER_H
#define MENU_CONTAINER_H

#include "menu/lv_anim.h"

#include <string>
#include <vector>

namespace menu
{

class MenuItem;
namespace container
{

template <typename T>
struct LVAnimProperty
{
    T open;
};

using Type_t = LVAnimProperty<lvgl::LVAnimPathType>;
using Value_t = LVAnimProperty<int32_t>;
using LVAnim_t = LVAnimProperty<lvgl::LVAnim>;
using CallbackPtr = void (*)(const MenuItem *menu_item);
class Render
{
public:
    void update(std::vector<MenuItem *> &menu_item_list, uint32_t new_time);
    void render(const MenuItem *item);

    void setRenderCallback(CallbackPtr new_cb) { cb = new_cb; };
    void setAnimPathType(Type_t type) { anim_type = type; }
    void setAnimDuration(Value_t duration) { anim_duration = duration; }

protected:
    bool isFinished(uint32_t new_time);

protected:
    LVAnim_t anim;
    Type_t anim_type{lvgl::LVAnimPathType::EASE_OUT};
    Value_t anim_duration{400};
    uint32_t current_time{0};
    CallbackPtr cb{nullptr};

    bool triggered{false};
};

} // namespace container

using Position_t = struct Position;
using Destination_t = struct Position;
struct Position
{
    int x;
    int y;

    Position operator+(Position &rhs) { return {this->x + rhs.x, this->y + rhs.y}; }
    Position operator-(Position &rhs) { return {this->x - rhs.x, this->y - rhs.y}; }
    bool operator==(Position &rhs) { return (this->x == rhs.x) && (this->y == rhs.y); }
    bool operator!=(Position &rhs) { return (this->x != rhs.x) || (this->y != rhs.y); }
};

using Size_t = struct Size
{
    int width;
    int height;
};

class MenuItem
{
public:
    void setUserData(void *new_data) { user_data = new_data; };
    void setSize(Size_t new_size) { size = new_size; }
    void setId(int new_id) { id = new_id; }
    void setTag(std::string new_tag) { tag = std::move(new_tag); }
    void setPosition(Position_t new_pos) { pos = new_pos; }
    void setDestination(Destination_t new_des) { des = new_des; }

    void *getUserData() { return user_data; }
    const std::string &getTag() const { return tag; }
    Position_t getPosition() const { return pos; }
    Destination_t getDestination() const { return des; }
    Size_t getSize() const { return size; }
    int getId() const { return id; }

protected:
    void *user_data{nullptr};
    std::string tag;
    Position_t pos;
    Destination_t des;
    Size_t size;
    int id;
};

class MenuContainer : public MenuItem
{
public:
    int addItem(MenuItem *item);
    int addItem(std::string tag, Size_t size, Position_t pos, void *user_data = nullptr);
    int addVItem(std::string tag, Size_t size, Position_t padding = {0, 0},
                 void *user_data = nullptr);
    int addHItem(std::string tag, Size_t size, Position padding = {0, 0},
                 void *user_data = nullptr);

    size_t size() const { return menu_item_list_.size(); }
    void clear(bool free_mem = false);
    const MenuItem *at(int idx) const { return menu_item_list_.at(idx); }

    void setRenderCallback(container::CallbackPtr new_cb) { render_.setRenderCallback(new_cb); };
    void setAnimPathType(container::Type_t type) { render_.setAnimPathType(type); }
    void setAnimDuration(container::Value_t duration) { render_.setAnimDuration(duration); }

    void updateAnimValue(uint32_t new_time);
    void doRender();

private:
    container::Render render_;
    std::vector<MenuItem *> menu_item_list_;
};

} // namespace menu

#endif