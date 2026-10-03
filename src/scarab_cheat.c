#include "scarab_cheat.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define GAME_STATE_RUNNING 1
#define UI_DLL_GAMEPLAY 1

/* Scarab Bag ownership bits, from gamebit_ids.h. */
#define GAMEBIT_ITEM_50ScarabBag_Got 0x919
#define GAMEBIT_ITEM_100ScarabBag_Got 0x91A
#define GAMEBIT_ITEM_200ScarabBag_Got 0x91B

/* Bag capacities, the same caps playerAddMoney applies. */
#define SCARAB_BAG_START 10
#define SCARAB_BAG_50 50
#define SCARAB_BAG_100 100
#define SCARAB_BAG_200 200
#define SCARABS_ADDED 10

typedef struct GameObject GameObject;

typedef struct ScarabCheatGame {
  int (*getGameState)(void);
  int (*getCurUiDll)(void);
  int (*getSaveGameLoadStatus)(void);
  GameObject* (*Obj_GetPlayerObject)(void);
  GameObject* (*getArwing)(void);
  int (*objIsCurModelNotZero)(GameObject* obj);
  int (*playerGetMoney)(GameObject* player);
  void (*playerAddMoney)(GameObject* obj, int amount);
  uint32_t (*mainGetBit)(int gameBit);
  void (*mainSetBits)(int gameBit, int value);
} ScarabCheatGame;

typedef struct Symbol {
  const char* name;
  void** address;
} Symbol;

static ScarabCheatGame game;

static const Symbol kSymbols[] = {
    {"getGameState", (void**)&game.getGameState},
    {"getCurUiDll", (void**)&game.getCurUiDll},
    {"getSaveGameLoadStatus", (void**)&game.getSaveGameLoadStatus},
    {"Obj_GetPlayerObject", (void**)&game.Obj_GetPlayerObject},
    {"getArwing", (void**)&game.getArwing},
    {"objIsCurModelNotZero", (void**)&game.objIsCurModelNotZero},
    {"playerGetMoney", (void**)&game.playerGetMoney},
    {"playerAddMoney", (void**)&game.playerAddMoney},
    {"mainGetBit", (void**)&game.mainGetBit},
    {"mainSetBits", (void**)&game.mainSetBits},
};

int scarabCheatInit(FhMod* mod, const FhModHost* host) {
  int ok = 1;
  size_t i;

  for (i = 0; i < sizeof(kSymbols) / sizeof(kSymbols[0]); i++) {
    *kSymbols[i].address = host->symbolAddress(mod, kSymbols[i].name);
    if (*kSymbols[i].address == NULL) {
      modLog(FH_LOG_ERROR, "could not resolve %s", kSymbols[i].name);
      ok = 0;
    }
  }
  if (!ok) {
    memset(&game, 0, sizeof(game));
  }
  return ok;
}

void scarabCheatShutdown(void) {
  memset(&game, 0, sizeof(game));
}

/* The same context the cheat menu offered Scarabs in: running gameplay with no
   save loading, on foot rather than in the Arwing, and playing as Fox. The
   player is looked up again on every press, never kept. */
static GameObject* scarab_player(void) {
  GameObject* player;

  if (game.getGameState() != GAME_STATE_RUNNING || game.getCurUiDll() != UI_DLL_GAMEPLAY ||
      game.getSaveGameLoadStatus() != 0 || game.getArwing() != NULL) {
    return NULL;
  }
  player = game.Obj_GetPlayerObject();
  if (player == NULL || game.objIsCurModelNotZero(player) == 0) {
    return NULL;
  }
  return player;
}

/* The largest bag owned wins, in the order playerAddMoney checks them. */
static int scarab_capacity(void) {
  if (game.mainGetBit(GAMEBIT_ITEM_200ScarabBag_Got) != 0) {
    return SCARAB_BAG_200;
  }
  if (game.mainGetBit(GAMEBIT_ITEM_100ScarabBag_Got) != 0) {
    return SCARAB_BAG_100;
  }
  if (game.mainGetBit(GAMEBIT_ITEM_50ScarabBag_Got) != 0) {
    return SCARAB_BAG_50;
  }
  return SCARAB_BAG_START;
}

void scarabCheatAddScarabs(void) {
  GameObject* player;
  int before;
  int after;
  int capacity;

  if (game.playerAddMoney == NULL) return;
  player = scarab_player();
  if (player == NULL) return;

  /* When the new total would not fit, only the next bag is granted. Bag bits
     are only ever set, so the bag never shrinks and never goes past 200. */
  before = game.playerGetMoney(player);
  capacity = scarab_capacity();
  if (before + SCARABS_ADDED > capacity) {
    if (capacity == SCARAB_BAG_START) {
      game.mainSetBits(GAMEBIT_ITEM_50ScarabBag_Got, 1);
    } else if (capacity == SCARAB_BAG_50) {
      game.mainSetBits(GAMEBIT_ITEM_100ScarabBag_Got, 1);
    } else if (capacity == SCARAB_BAG_100) {
      game.mainSetBits(GAMEBIT_ITEM_200ScarabBag_Got, 1);
    }
  }
  /* playerAddMoney caps the total at the current bag's capacity. */
  game.playerAddMoney(player, SCARABS_ADDED);

  after = game.playerGetMoney(player);
  capacity = scarab_capacity();
  if (after > before) {
    modLog(FH_LOG_INFO, "Added %d Scarabs (%d / %d)", after - before, after, capacity);
  } else {
    modLog(FH_LOG_INFO, "Scarab Bag is full (%d / %d)", after, capacity);
  }
}
