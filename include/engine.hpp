
#pragma once

/*
1. run function that initializes the window, and glfw
*/
// REVIEW: Engine is unused — all initialization and the game loop live in main().
// Either move the main loop logic into Engine (with a run() method), or remove this class.
// Also, 'Window window' has no default constructor, so Engine can't be default-constructed.
class Engine {
    public:



    private:
    Window window;
};