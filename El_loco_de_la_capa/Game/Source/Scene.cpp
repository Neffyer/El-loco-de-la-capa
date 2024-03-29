#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Item.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "FadeToBlack .h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
	enemy->parameters = config.child("enemy");

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	app->map->Load();
	
	BKG1 = app->tex->Load("Assets/Textures/BKG1.png");
	BKG2 = app->tex->Load("Assets/Textures/BKG2.png");
	BKG3 = app->tex->Load("Assets/Textures/BKG3.png");
	

	
	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	MusicOn = true;

	playerUI = app->tex->Load("Assets/UI/PlayerUI.png");
	coinsUI = app->tex->Load("Assets/UI/coinsNum.png");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	if (MusicOn) {

		app->audio->PlayMusic("Assets/Audio/Music/GreenPath.ogg", 0);
		MusicOn = false;

	}
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (!app->entityManager->IsEnabled()) {
		app->entityManager->Enable();
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {

		if (!app->physics->debug) app->physics->debug = true;
		else app->physics->debug = false;

	}
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (!app->scene->player->godMode) app->scene->player->godMode = true;
		else app->scene->player->godMode = false;

	}

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	//Cuando cree la escena final
	
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {

		app->fadeToBlack->Fade(this, (Module*)app->sceneDeath, dt);

	}

	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {

		app->fadeToBlack->Fade(this, (Module*)app->sceneTitle, dt);

	}

	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	app->render->DrawTexture(BKG1, -200, 0, NULL, 1.3f);
	app->render->DrawTexture(BKG2, -200, 0, NULL, 1.2f);
	app->render->DrawTexture(BKG3, -200, 0, NULL, 1.1f);

	// Draw map

	app->map->Draw();
	app->render->DrawTexture(blackScreen, app->win->screenSurface->w, app->win->screenSurface->h);

	SDL_Rect rect = { 0, 0 + 54 * player->hits, 153, 54 };
	app->render->DrawTexture(playerUI, 25, 25, &rect);

	SDL_Rect nums = { 0, 0 + 17 * player->coins, 12, 17 };
	app->render->DrawTexture(coinsUI, 100, 60, &nums);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
