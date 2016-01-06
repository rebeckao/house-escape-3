#ifndef VIEWEVENTMANAGER_H
#define VIEWEVENTMANAGER_H

#include "ViewEvent.h"
#include <iostream>
#include "unordered_map"

class ViewEventManager{
public:
	std::unordered_map<std::size_t, void(ViewEventManager::*)(ViewEvent*)> event_map;
	ViewEventManager();
	void exploreTile(ViewEvent* tile_explored_event);
	void megaExplosion(ViewEvent* mega_explosion_event);
};

#endif