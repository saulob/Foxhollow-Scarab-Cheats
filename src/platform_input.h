#ifndef PLATFORM_INPUT_H_
#define PLATFORM_INPUT_H_

#include "foxhollow_mod_api.h"

int platformInputInitialize(FhMod* mod, const FhModHost* host);
void platformInputShutdown(void);
int platformInputActive(void);
int platformAddScarabsKeyDown(void);

#endif
