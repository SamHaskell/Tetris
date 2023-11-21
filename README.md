## Tetris!

Hi! This is a small Tetris clone that I decided to put together over the course of a couple of weekends. I use SDL and emscripten to target the web, and the game is now live on [itch](https://samhaskell.itch.io/tetris) if you fancy giving it a play.

To build for the web, simply clone the repo, cd into the root directory and run:

```
premake5 gmake 
cd build
emmake gmake config=release_web
```

For desktop, I currently only target MacOS for my own development builds, although the premake script should be very easy to modify in order to target Windows or Linux as all dependencies are cross platform.

You will also need SDL2 to dynamically link to. On MacOS this comes as standard, but you may need to brew / macports the latest stable version of SDL2 to get it to work. You will also need SDL2_ttf. On Windows, you will need to supply those dynamic libraries yourself and place them in the relevant directories so that they can be found by the executable.