#ifndef GRAPHICS_CORE_HPP
#define GRAPHICS_CORE_HPP

//Returns 0 if no error
int initGraphics(int resX, int resY, const char *windowTitle);
int renderFrame();
//Call this every frame to check if the window needs to be closed
void checkCloseWindow();
//Destroys the window, as well as all other graphics components
void unloadGraphics();

#endif
