# Engine_Bakers

#### Version Gold

## Main features

### Editor
- **Console:** Display Warning, Error and Messages logs with filter and selectable options like clear, auto-scroll or clear on play.
- **FileBrowser:** Handle Drag and Drop, scene loading, material and shader selection. Behaves like the Windows file browser.
- **Hierarchy:** Display Scene graph, can handle Drag and Drop and allow to create and rename game objects.
- **Inspector:** Display components and allow components edition (delete, reset, add components).
- **Material:** Edit the bmaterial selected with FileBrowser.
- **Shader:** Edit the bshader selected with FileBrowser.
- **Profiler:** Display FPS.
- **Scene:** Navigation in Scene with holding right click. Allow to chose whether to display colliders and navMesh or not. Give access to translation, rotation and scale gizmos.
- **Viewport:** Game view. When several camera components are available, allow to chose which one to use inside a given window.
- **Game handling:** Playmode, Pause, Step by step.
- **Compiling:** Compile button to re-load lua scripts, shaders and nav mesh.

### Audio
- **Audio Listener:** Receive and play sound depending on 3D position of Game object
- **Audio Source:** Broadcast sound a 2D or 3D sound. Access to different parameters like loop, volume, mute, etc.

### Physics
- **Collider (Sphere, Cube, Capsule):** Handle collision and triggers. Have access to 4 groups of filtering used for collision and raycasts.
- **Rigidbody:** Handle physic movements on the scene with linear and angular velocity. Needs a collider to be usable.

### Resources
- **ResourcesManager:** Load of resources in multithread.

### Navigation
- **Nav mesh:** Navigation zone build based on static meshes in the scene. Can be rebaked with the Compile button.
- **Path follower:** Component allowing a gameobject to automatically reach a given global position through baked navigation.

### Rendering
- **Mesh:** Display model that can be set with an obj or a fbx. Uses a material that can be modified through editor.
- **Skeletal Mesh:** Mesh handling animations.
- **Light:** Add light to scene with transform of parent. Can set type (Direction, spot, point), color and specific parameters like range, angle, etc. Allow to cast shadows, but not used in gameplay scene.
- **Camera:** Display Game object POV inside a viewport.
- **Camera with Secondary View:** Specific camera allowing to display a second pov inside a same viewport. On play mode, also allows to zoom in or out by mouse scroll. Used for gameplay demo.
- **Postprocess:** Add different effects on top of the framebuffer rendered by a camera. Must be in the same object as the camera it is being applied to.
- **Environnement:** Handle skybox generation. Can be set as different types of skyboxes (sphere, cube, quads) each allowing to set textures independently.
- **ParticleSystem:** Add particles that can handle alpha blending with any given texture and can be modified through different parameters to create lots of possible effects (speed, time of respawn, color evolutions, gravity, etc.)
- **Text Renderer:** Allow to display text in the 3D scene.

### Scripting
- **Scripted component:** Receive a single lua script that will, if valid, call its Start and Update function as any other component. The movements of the player in the gameplay demo are handled by the InputMovement.lua script.
- **Compiling scripts:** The scripts can be recompiled at the runtime of the editor through the compile button.
