#pragma once

#define W_SCENE "Scene"
#define W_HIERARCHY "Hierarchy"
#define W_INSPECTOR "Inspector"
#define W_FILEBROWSER "File Browser"
#define W_CONSOLE "Console"

class Editor
{
public:
    Editor();
    ~Editor();
    void SetViewport();
    void BuildDock();
    void Update();
};

