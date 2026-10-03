#ifndef SCARAB_CHEAT_H_
#define SCARAB_CHEAT_H_

#include "foxhollow_mod_api.h"

void modLog(FhLogLevel level, const char* format, ...);

int scarabCheatInit(FhMod* mod, const FhModHost* host);
void scarabCheatShutdown(void);
void scarabCheatAddScarabs(void);

#endif
