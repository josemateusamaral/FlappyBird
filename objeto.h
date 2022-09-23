#include <SDL.h>

class Objeto
{
    public:
        Objeto();
        SDL_Rect coordenadasObjeto;
		SDL_Rect coordenadasTextura;
		SDL_Texture* textura;
};