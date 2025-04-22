#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#define WINDOW_WIDTH 841
#define WINDOW_HEIGHT 650
#define HERO_SPEED 5

typedef struct {
    SDL_Surface *map;
    SDL_Surface *minimap;
    SDL_Surface *minijoueur;
    SDL_Rect positionmap;
    SDL_Rect positionminijoueur;
} Minimap;

//ytfytfkfuyyugggggyguyggygyyyyyyyyyyggygyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
void afficherMinimap(Minimap m, SDL_Surface *screen) {
    SDL_BlitSurface(m.map, NULL, screen, &m.positionmap);
    SDL_BlitSurface(m.minijoueur, NULL, screen, &m.positionminijoueur);
    SDL_Rect border = {m.positionmap.x - 2, m.positionmap.y - 2, m.map->w + 4, m.map->h + 4};
    SDL_FillRect(screen, &border, SDL_MapRGB(screen->format, 255, 255, 255)); // white border
    SDL_BlitSurface(m.map, NULL, screen, &m.positionmap);
    SDL_BlitSurface(m.minijoueur, NULL, screen, &m.positionminijoueur);
}

int main(int argc, char** argv) {
    SDL_Surface *screen, *map, *map2, *hero;
    SDL_Surface *minimap2;
    SDL_Rect posHero, posHeroOnScreen;
    SDL_Event event;
    SDL_Rect camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    Mix_Music *musique;
    int continuer = 1;
    int currentLevel = 1;

    Minimap m;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Erreur creation fenetre: %s\n", SDL_GetError());
        return 1;
    }

    map = IMG_Load("outils/map.png");
    m.map = IMG_Load("outils/minimap.png");
    map2 = IMG_Load("outils/map2.jpg");
    minimap2 = IMG_Load("outils/minimap2.jpg");
    hero = IMG_Load("outils/hero.png");
    m.minijoueur = IMG_Load("outils/joueurmini.png");

    if (!map || !m.map || !map2 || !minimap2 || !hero || !m.minijoueur) {
        fprintf(stderr, "Erreur chargement images: %s\n", SDL_GetError());
        return 1;
    }

    musique = Mix_LoadMUS("outils/musique.mp3");
    if (!musique) {
        fprintf(stderr, "Erreur chargement musique: %s\n", Mix_GetError());
        return 1;
    }

    Mix_PlayMusic(musique, -1);

    posHero.x = 0;
    posHero.y = 530;

    m.positionmap.x = 10;
    m.positionmap.y = 10;
    m.positionmap.w = m.map->w;
    m.positionmap.h = m.map->h;

    SDL_EnableKeyRepeat(100, 10);

    while (continuer) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                continuer = 0;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        continuer = 0;
                        break;
                    case SDLK_LEFT:
                        if (posHero.x - HERO_SPEED >= 0)
                            posHero.x -= HERO_SPEED;
                        break;
                    case SDLK_RIGHT:
                        if (!(currentLevel == 2 && posHero.x + HERO_SPEED > map->w - hero->w)) {
                            posHero.x += HERO_SPEED;
                        }
                        break;
                    case SDLK_UP:
                        if (posHero.y - HERO_SPEED >= 0)
                            posHero.y -= HERO_SPEED;
                        break;
                    case SDLK_DOWN:
                        if (posHero.y + HERO_SPEED < map->h - hero->h)
                            posHero.y += HERO_SPEED;
                        break;
                }
            }
        }

        // yetfa9ed est ce t3ada lel level li ba3dou
        if (currentLevel == 1 && posHero.x > map->w - 50) {
            currentLevel = 2;
            posHero.x = 0;
            posHero.y = 530;

            // yet3ada l next level
            map = map2;
            m.map = minimap2;

            m.positionmap.w = m.map->w;
            m.positionmap.h = m.map->h;
        }

        //  camera tescroli
         camera.x = posHero.x - WINDOW_WIDTH / 2;
        camera.y = posHero.y - WINDOW_HEIGHT / 2;

        if (camera.x < 0) camera.x = 0;
        if (camera.y < 0) camera.y = 0;
        if (camera.x > map->w - camera.w) camera.x = map->w - camera.w;
        if (camera.y > map->h - camera.h) camera.y = map->h - camera.h;

        // mise a jouer lel position ta3 minijoueur fel mini map
        m.positionminijoueur.x = m.positionmap.x + (posHero.x * m.map->w / map->w);
        m.positionminijoueur.y = m.positionmap.y + (posHero.y * m.map->h / map->h);

        // display
        SDL_BlitSurface(map, &camera, screen, NULL);

        posHeroOnScreen.x = posHero.x - camera.x;
        posHeroOnScreen.y = posHero.y - camera.y;
        SDL_BlitSurface(hero, NULL, screen, &posHeroOnScreen);

        afficherMinimap(m, screen);

        SDL_Flip(screen);
    }

    SDL_FreeSurface(map);
    SDL_FreeSurface(map2);
    SDL_FreeSurface(m.map);
    SDL_FreeSurface(minimap2);
    SDL_FreeSurface(hero);
    SDL_FreeSurface(m.minijoueur);
    Mix_FreeMusic(musique);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

