//g++ main.cpp -o main.exe `sdl2-config --cflags --libs`
//g++ main.cpp -o main `sdl2-config --cflags --libs`

#include <iostream>
#include <SDL.h>
#include "objeto.h"

#define REFRESH_SPEED 60

using namespace std;	

bool verificar_colisao( SDL_Rect A, SDL_Rect B)
{		
	//collide box A
	int direita_A =  A.x + A.w;
	int esquerda_A =  A.x;
	int cima_A =  A.y;
	int baixo_A =  A.y + A.h;
	
	//collide box B
	int direita_B =  B.x + B.w;
	int esquerda_B =  B.x;
	int cima_B =  B.y;
	int baixo_B =  B.y + B.h;
			
	//verificar colisao
	if ( ( direita_A >= esquerda_B ) && ( esquerda_A <= direita_B ) && ( cima_A <= baixo_B ) && ( baixo_A >= cima_B ) ){
		return true;
	}
	else{
		return false;
	}		
	
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_AUDIO);

	// constantes do ambiente
	const int WINDOW_HEIGHT = 480;
	const int WINDOW_WIDTH = 720;
	const int PERSONAGEM_HEIGHT = WINDOW_HEIGHT / 10;
	const int PERSONAGEM_WIDTH = WINDOW_HEIGHT / 7;
	
	// variaveis do ambiente
	SDL_Window* Main_Window;
	SDL_Renderer* Main_Renderer;
	SDL_Surface* Loading_Surf;
	SDL_Texture* background;
	SDL_Texture* textura;
	SDL_Event event;
	bool gameLoop = true;
	int fx = 0;
	int fy = 0;
	int gravidade = -1;
	int inercia = 1;
	Objeto objetos[10];
	
	// configurar janela do jogo
	Main_Window = SDL_CreateWindow("Trabalho POO",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT,0);
	Main_Renderer = SDL_CreateRenderer(Main_Window, -1, SDL_RENDERER_ACCELERATED);
	
	// imagem de fundo 
	Loading_Surf = SDL_LoadBMP("assets/textures/fundo.bmp");
	background = SDL_CreateTextureFromSurface(Main_Renderer, Loading_Surf);
	SDL_FreeSurface(Loading_Surf);		
	
	//adicionar caixas
	int quantidadeDeObjetos = 8;
	for( int i = 0 ; i < quantidadeDeObjetos ; i++ ){
		Objeto obj;
		Loading_Surf = SDL_LoadBMP("assets/textures/caixa.bmp");
		textura = SDL_CreateTextureFromSurface(Main_Renderer, Loading_Surf);
		SDL_FreeSurface(Loading_Surf);
		obj.textura = textura;
		obj.coordenadasTextura.x = 0;
		obj.coordenadasTextura.y = 0;
		obj.coordenadasTextura.w = 102;
		obj.coordenadasTextura.h = 102;
		obj.coordenadasObjeto.x = 50 * (i + 1);
		obj.coordenadasObjeto.y = 50 * (i + 1);
		obj.coordenadasObjeto.w = 50;
		obj.coordenadasObjeto.h = 50;
		objetos[i] = obj;
	}

	// adicionar personagem
	Objeto personagem;
	Loading_Surf = SDL_LoadBMP("assets/textures/personagem.bmp");
	textura = SDL_CreateTextureFromSurface(Main_Renderer, Loading_Surf);
	SDL_FreeSurface(Loading_Surf);
	personagem.textura = textura;
	personagem.coordenadasTextura.x = 0;
	personagem.coordenadasTextura.y = 0;
	personagem.coordenadasTextura.w = 43;
	personagem.coordenadasTextura.h = 30;
	personagem.coordenadasObjeto.x = 0;
	personagem.coordenadasObjeto.y = 0;
	personagem.coordenadasObjeto.w = PERSONAGEM_WIDTH;
	personagem.coordenadasObjeto.h = PERSONAGEM_HEIGHT;

	//	Game Loop
	while(gameLoop){
	
		// ###### [ TECLADO ] #######################
		//	ler teclado
		while( SDL_PollEvent( &event ) ){
			//system("clear");
		    switch( event.type ){
		    	
		    	//checar teclas sendo pressionadas
		        case SDL_KEYDOWN:
		            switch( event.key.keysym.sym ){
		                case SDLK_LEFT:
		                    fx = -10;
		                    break;
		                case SDLK_RIGHT:
		                    fx = 10;
		                    break;
		                case SDLK_UP:
		                    fy = 20;
		                    break;
		                case SDLK_ESCAPE:
		                    gameLoop = false;
		                    break;
		                default:
		                    break;
		            }
				
		    }
	  	}



		// ####### [ FISICA ] ####################
		// atualizar as forcas fisicas
		fy = fy + gravidade;
		if( fx != 0 ){
			if( fx > 0 ){
				fx = fx - inercia;
			}
			else{
				fx = fx + inercia;
			}
		}
		
		// atualizar posicao do personagem
		int oldPosition = personagem.coordenadasObjeto.x;
		personagem.coordenadasObjeto.x = personagem.coordenadasObjeto.x + fx;
		for( int i = 0 ; i < quantidadeDeObjetos ; i++){
			if (verificar_colisao( personagem.coordenadasObjeto, objetos[i].coordenadasObjeto)){
				personagem.coordenadasObjeto.x = oldPosition;
				fx = 0;
				break;
			}
		}
		oldPosition = personagem.coordenadasObjeto.y;
		personagem.coordenadasObjeto.y = personagem.coordenadasObjeto.y - fy;
		for( int i = 0 ; i < quantidadeDeObjetos ; i++){
			if (verificar_colisao( personagem.coordenadasObjeto, objetos[i].coordenadasObjeto)){
				personagem.coordenadasObjeto.y = oldPosition;
				fy = 0;
				break;
			}
		}
	 	
	 	//nao deixar o personagem sair da tela
	 	if ( personagem.coordenadasObjeto.x > ( WINDOW_WIDTH - PERSONAGEM_WIDTH ) ){
	 			personagem.coordenadasObjeto.x = WINDOW_WIDTH - PERSONAGEM_WIDTH;
		    fx = 0;
		}
	 	else if ( personagem.coordenadasObjeto.x < 0 ){
	 			personagem.coordenadasObjeto.x = 0;
		    fx = 0;
	 	}
		if ( personagem.coordenadasObjeto.y > ( WINDOW_HEIGHT - PERSONAGEM_HEIGHT ) ){
	 			personagem.coordenadasObjeto.y = WINDOW_HEIGHT - PERSONAGEM_HEIGHT;
		    fy = 0;
	 	}
	 	else if ( personagem.coordenadasObjeto.y < 0 ){
	 			personagem.coordenadasObjeto.y = 0;
		    fy = 0;
	 	}
	 
	 
	 
	 
	 
	 	// ########## [ RENDERIZAR ] ####################
		// zerar render
		SDL_RenderClear(Main_Renderer);
		
		//	desenhar render
		SDL_RenderCopy(Main_Renderer, background, NULL, NULL);
		for( int i = 0 ; i < quantidadeDeObjetos ; i++ ){
			SDL_RenderCopy(Main_Renderer, objetos[i].textura, &objetos[i].coordenadasTextura, &objetos[i].coordenadasObjeto);
		}
		if( fx > 0 ){
			personagem.coordenadasTextura.x = 0; 	
		}
		else if( fx < 0 ){
			personagem.coordenadasTextura.x = 43;
		}
		SDL_RenderCopy(Main_Renderer, personagem.textura, &personagem.coordenadasTextura, &personagem.coordenadasObjeto);
		
		// mostrar render
		SDL_RenderPresent(Main_Renderer);
		SDL_Delay(1000/REFRESH_SPEED);
	
	}
	
	
	
	
	// ###### [ FECHAR JOGO ] ################################
	for( int i = 0 ; i < quantidadeDeObjetos ; i++ ){
			SDL_DestroyTexture(objetos[0].textura);
	}
	SDL_DestroyTexture(personagem.textura);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(Main_Renderer);
	SDL_DestroyWindow(Main_Window);
	SDL_Quit();
	return 0;
	
}

