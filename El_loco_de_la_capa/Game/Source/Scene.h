#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	Enemy* enemy;

	SDL_Texture* blackScreen;
	SDL_Texture* BKG1;
	SDL_Texture* BKG2;
	SDL_Texture* BKG3;

	SDL_Texture* playerUI;
	SDL_Texture* coinsUI;

	bool MusicOn;

private:
	SDL_Texture* img;

};

#endif // __SCENE_H__