# Bubble Bobble

### Project Overview
![Overview](./final_project/bubble-full.gif)

The game is developed following the event-driven design pattern. The challenges in this project are basically as follows:

Map: Load map from bits, detect map edges, and determine how map interacts with other objects.
Moving Mechanism: Hybridize "time state" concept for the backbone of the game. Time stamp is used as a criterion to define the trigger points of certain events. In addition, the movement and animation of objects are optimized to be as smooth as possible.
Music: Background music is played as streaming segments.

### How To Start
- `mkdir build` to create a `build` environment folder
- `cd build` and run `cmake ../final_project` ot start off the compiling process
- `cmake --build . --target bubble_bobble --config Release` tp build the project
- (for Windows) run the released `.exe` file by `bubble_bobble\Release\bubble_bobble.exe`