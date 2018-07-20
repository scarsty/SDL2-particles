#include "ParticleExample.h"
#include "SDL2/SDL.h"

int main(int, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    auto win = SDL_CreateWindow("SDL2 Particles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);
    auto ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    auto p = new ParticleExample();        // create a new particle system pointer
    p->setRenderer(ren);                   // set the renderer
    p->setPosition(512, 384);              // set the position
    p->setStyle(ParticleExample::FIRE);    // set the example effects
    p->setStartSpin(0);
    p->setStartSpinVar(90);
    p->setEndSpin(90);
    p->setStartSpinVar(90);

    while (1)
    {
        SDL_Event e;
        SDL_PollEvent(&e);
        if (e.type == SDL_KEYUP)
        {
            int s = (e.key.keysym.sym - SDLK_a + 1);
            p->setStyle(ParticleExample::PatticleStyle(s));    // switch the example effects
        }
        if (e.type == SDL_QUIT)
        {
            break;
        }

        SDL_RenderClear(ren);
        p->draw();    // you have to draw it in each loop
        SDL_RenderPresent(ren);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    delete p;    // destroy it

    return 0;
}