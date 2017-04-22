/*ShadowRenderer is a class that is responsible for the rendering portion of ShadowPuppet
Author: Andy Dolan*/
#ifndef RENDERER_INCLUDED
#define RENDERER_INCLUDED

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <string>
#include <vector>

class ShadowRenderer{
	public:
		//Ctor does initialization
		ShadowRenderer(const int WINDOW_HEIGHT, const int WINDOW_WIDTH);
		
		//Dtor destroys textures
		~ShadowRenderer();
		
		//Initialize the OpenGL components
		bool initializeGL(SDL_Window *window);
		
		//Render textures
		void glRender(SDL_Rect player, bool platformsPresent, const int numPlatforms, 
			std::vector<SDL_Rect> &platforms);
			
	private:
		
		//Load a texture from an image
		bool loadTexture(std::string image, GLuint &textureID);
		
		//Window dimensions
		const int WINDOW_HEIGHT;
		const int WINDOW_WIDTH;
		
		//Textures
		GLuint backgroundTexture;
		GLuint playerTexture;
		GLuint platformTexture;
		
		//OpenGL context
		SDL_GLContext gameContext;
		
};

#endif