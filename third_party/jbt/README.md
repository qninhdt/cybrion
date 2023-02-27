# jbt
The JSON-like Binary Tag (JBT) file format - A simple file format developed for store binary data: game save, world, ...

## Example
```cpp
#include <iostream>
#include <jbt/jbt.hpp>

int main() {

    jbt::init(); // required
    
    // build jbt tag
    jbt::tag player(jbt::tag_type::OBJECT);

    player.set_string("name", "qninhdt");
    player.set_bool("is_living", true);

    player.set_ushort("health", 100);

    player.set_float("pos_x", 10.0f / 7);
    player.set_float("pos_y", 4.8f / 7);
    player.set_float("pos_z", -94.0f / 7);

    jbt::tag axe(jbt::tag_type::OBJECT);
    axe.set_string("type", "tool");
    axe.set_byte("power", 12);

    jbt::tag sword(jbt::tag_type::OBJECT);
    sword.set_string("type", "weapon");
    sword.set_byte("power", 90);

    jbt::tag inventory(jbt::tag_type::LIST);
    inventory.add_tag(axe);
    inventory.add_tag(sword);

    player.set_tag("inventory", inventory);

    // save tag
    jbt::save_tag(player, "player.jbt");

    // read player data
    jbt::tag xplayer;
    jbt::open_tag(xplayer, "player.jbt");

    // print tag to terminal
    std::cout << xplayer;
}
```

## Output
```js
{
    health: 100us,
    inventory: [
        {
            power: 12b,
            type: "tool"
        },
        {
            power: 90b,
            type: "weapon"
        }
    ],
    is_living: true,
    name: "qninhdt",
    pos_x: 1.428571f,
    pos_y: 0.6857143f,
    pos_z: -13.42857f
}
```
