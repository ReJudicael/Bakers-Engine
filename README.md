# Engine_Bakers

#### Version Gold (1.0)

## How to launch project
- Enter/Use the commands
    -  git pull
    -  git submodule init
    -  git submodule update

- Open Engine/Engine.sln
- Select Debug x64 | Release x64
- Set Editor project as start-up project in visual studio solution
- Build and Run

## How to launch executable
- Set Executable project as start-up project in visual studio solution
- Build and Run

## How to use Tracy profiler (Leak detection)
- Go to Engine/Dependencies/Tracy/profiler/build/win32
- Open Tracy.sln
- Change config to Release
- Build and run

### To use Tracy:
- Compile and run engine in Tracy
- Start profiler and click on the connect button
- Do desired operations in editor or exe
- Close editor or exe and press "enter" to properly finish the session
- Engine leak can be found in the memory tab in the up-left corner

![Alt text](https://github.com/ReJudicael/Bakers-Engine/blob/main/Showcase/Tracy/MenuBar.png)

RTTR create lots of “fake” leak, which is needed for it to work and must not be cleaned. Those useful leaks are marked down by comments when the mouse cursor is hovering the leak text.

![Alt text](https://github.com/ReJudicael/Bakers-Engine/blob/main/Showcase/Tracy/RTTRLeak.png)

## Documentation wih Doxygen
- Doxygen: Create documentation
- Go to Engine/Doxygen
- Launch MakeDoc.bat or MakeDoc.sh depending on linux or windows (linux version needs doxygen - package to be installed)
- Go to Doc
- Open index.html
- This opens the doxygen documentation of engine and editor

## Used libraries
- Assimp (Loading resources)
- fmod (Sound)
- freetype (Text)
- OpenGL with glad (Graphics API)
- GLFW (Windows and input)
- ImGui (Editor)
- ImGuizmo (Gizmo handling)
- json (Save)
- Lua with Sol (Scripting)
- PhysX SDK (Physic)
- RecastNavigation (Nav mesh)
- RTTR (Reflection)
- stb_image (Images)
- Tracy (Profiling)


# Game demo
This engine comes with a main scene featuring a RPG game demo. In this game, the player controls both a human character, directly through WASD inputs, and a dragon ally, through holding shift and clicking on the screen. The dragon will move to the asked direction, or follow the player when Z is pressed. If the dragon gets lost, pressing the P key will warp it back next to the player. 
These features allow puzzle-type mechanics and a more action-based battle system in which the player and their ally can both attack enemies.

![Alt text](https://github.com/ReJudicael/Bakers-Engine/blob/main/Showcase/Game/Boss.gif)

![Alt text](https://github.com/ReJudicael/Bakers-Engine/blob/main/Showcase/Game/Fight.gif)

![Alt text](https://github.com/ReJudicael/Bakers-Engine/blob/main/Showcase/Game/Boss.gif)

#### See also
The ReleaseNotes giving more informations about most features available in the Bakers Engine.
