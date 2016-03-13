#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "gamemechanics.h"

Object MonsterProto = {
	.init = Monster_init,
	.attack = Monster_attack
};

Object RoomProto = {
	.move = Room_move,
	.attack = Room_attack
};

Object MapProto = {
	.init = Map_init,
	.move = Map_move,
	.attack = Map_attack
};

int Map_init(void *self)
{
	assert(self != NULL);

	Map *map = self;

	// Make some rooms for a small map
	Room *hall = NEW(Room, "The great Hall");
	Room *throne = NEW(Room, "The throne room");
	Room *arena = NEW(Room, "The arena, with the minotaur");
	Room *kitchen = NEW(Room, "Kitchen, you have the knife now");
	Room *dungeon = NEW(Room, "The dungeon, with another minotaur");
	Room *outside = NEW(Room, "A room that leads you to the outside.\n\nCongratulate you have finished the game");

	// Indicate that we have reached the end of the game.
	outside->done = 1;

	// setup the bad guys
	arena->bad_guy = NEW(Monster, "The evil minotaur");
	dungeon->bad_guy = NEW(Monster, "The brother of the evil minotaur");

	// setup the map rooms
	hall->north = throne;

	throne->west = arena;
	throne->east = kitchen;
	throne->south = hall;

	kitchen->west = throne;

	arena->east = throne;
	arena->west = dungeon;

	dungeon->east = arena;
	dungeon->north = outside;

	outside->south = dungeon;

	// start the map and the character off in the hall
	map->start = hall;
	map->location = hall;

	return 1;
}

int main(int argc, char *argv[])
{
	// simple way to setup the randomness
	srand(time(NULL));

	// make our map to work with
	Map *game = NEW(Map, "The Hall of the Minotaur.");

	printf("You enter the ");
	game->location->_(describe)(game->location);

	while(process_input(game)) {
		if(game->location->done) {
			break;
		}
	}

	return 0;
}
