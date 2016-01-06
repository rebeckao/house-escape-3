#include "ViewEventManager.h"

void ViewEventManager::exploreTile(ViewEvent* tile_explored_event){
	TileExploredEvent* current_event = dynamic_cast<TileExploredEvent*>(tile_explored_event);

	std::cout << "IT'S A NEW WORLD RECORD!" << std::endl;
	std::cout << current_event->i << " " << current_event->j << std::endl;
}

void ViewEventManager::megaExplosion(ViewEvent* mega_explosion_event){
	MegaExplosionEvent* current_event = dynamic_cast<MegaExplosionEvent*>(mega_explosion_event);
	std::cout << "IT'S A BOMB!" << std::endl;
	std::cout << current_event->i << " " <<current_event->j << std::endl;
}

ViewEventManager::ViewEventManager(){
	void(ViewEventManager::* explore_tile_pointer)(ViewEvent*);
	explore_tile_pointer = &ViewEventManager::exploreTile;
	event_map[VIEW_EVENT_EXPLORE_TILE] = explore_tile_pointer;

	void(ViewEventManager::* mega_explosion_pointer)(ViewEvent*);
	mega_explosion_pointer = &ViewEventManager::megaExplosion;
	event_map[VIEW_EVENT_MEGA_EXPLOSION] = mega_explosion_pointer;
}
