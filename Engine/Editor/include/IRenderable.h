#pragma once

/**
 * Interface to make a GUI (widgets, panels, canvas) that is drawn.
 */
class IRenderable
{
public:
    /*
     * Function called by the engine to draw the object
     */
	virtual void Draw() = 0;
};

