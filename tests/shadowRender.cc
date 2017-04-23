#include "shadowRender.h"

using namespace std;

//Ctor
ShadowRenderer::ShadowRenderer(const int h, const int w): WINDOW_HEIGHT(h), WINDOW_WIDTH(w), backgroundTexture(0), playerTexture(0), platformTexture(0){	}

//Dtor
ShadowRenderer::~ShadowRenderer(){
	glDeleteTextures(1, &backgroundTexture);
	glDeleteTextures(1, &playerTexture);
	glDeleteTextures(1, &platformTexture);
	SDL_GL_DeleteContext(gameContext);
	IMG_Quit();
}

//Initialize GL components, SDL_Image, and load textures
bool ShadowRenderer::initializeGL(SDL_Window *window){
	//Setting some SDL_GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	
	//Create OpenGL context
	gameContext = SDL_GL_CreateContext(window);
	if(gameContext == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to create OpenGL context. Error: %s\n", SDL_GetError());
        return 1;
    }
	
	//Initialize SDL_image
	int image = IMG_Init(IMG_INIT_PNG);
    if((image & IMG_INIT_PNG) != IMG_INIT_PNG){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL_image! Error: %s\n", IMG_GetError());
		return 1;
    }
	
    //Setting up the orthographic coordinate system
	glOrtho(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0, 1.0, -1.0);
	
    //These flags must be enabled for transparency of PNGs to work
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
    //Loading the textures we're using
    if(!loadTexture(string("../images/marblePng.png"), backgroundTexture)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load texture from string! Error: %s\n", SDL_GetError());
        return false;
    }
    if(!loadTexture(string("../images/cam.png"), playerTexture)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load texture from string! Error: %s\n", SDL_GetError());
        return false;
    }
    if(!loadTexture(string("../images/Platform.png"), platformTexture)){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load platform texture from string! Error: %s\n", SDL_GetError());
		return false;
	}
    return true;
}

void ShadowRenderer::glRender(Player player, bool platformsPresent, vector<SDL_Rect> &platforms){
	//Clear the context first
    glClear(GL_COLOR_BUFFER_BIT);
    //Need this to be enabled for this portion
    glEnable(GL_TEXTURE_2D);
    //Render background texture onto target
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);                           //Top left
        glTexCoord2f(1, 0); glVertex2f(WINDOW_WIDTH, 0);                //Top right
        glTexCoord2f(1, 1); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);    //Bottom right
        glTexCoord2f(0, 1); glVertex2f(0, WINDOW_HEIGHT);               //Bottom left
    glEnd();
    //Render player
    glBindTexture(GL_TEXTURE_2D, playerTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(player.x,player.y);
        glTexCoord2f(1,0); glVertex2f((player.x + player.width),player.y);
        glTexCoord2f(1,1); glVertex2f((player.x + player.width),(player.y + player.height));
        glTexCoord2f(0,1); glVertex2f(player.x,(player.y + player.height));
    glEnd();
	//Render platform(s) if they're present
	if(platformsPresent){
		for(size_t i = 0; i < platforms.size(); i++){
			glBindTexture(GL_TEXTURE_2D, platformTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0); glVertex2f(platforms[i].x,platforms[i].y);
				glTexCoord2f(1,0); glVertex2f((platforms[i].x + platforms[i].w),platforms[i].y);
				glTexCoord2f(1,1); glVertex2f((platforms[i].x + platforms[i].w),(platforms[i].y + platforms[i].h));
				glTexCoord2f(0,1); glVertex2f(platforms[i].x,(platforms[i].y + platforms[i].h));
			glEnd();
		}
	}
    //Done with rendering, disable this flags
    glDisable(GL_TEXTURE_2D);
}

//Load a texture from an image
bool ShadowRenderer::loadTexture(string image, GLuint &textureID){
	//Loading an SDL surface from png via SDL_image
	SDL_Surface *surface = IMG_Load(image.data());
	if(!surface){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load surface from image: %s Error: %s\n", image.data(), SDL_GetError());
        return false;
	}
	glGenTextures(1, &textureID);
	//Binding to the texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	//Setting up the texture's settings
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Actually creating the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    //Freeing the SDL surface, since we don't need it anymore
    SDL_FreeSurface(surface);
    return true;
}