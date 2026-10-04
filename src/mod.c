#include "scarab_cheat.h"
#include "platform_input.h"

#include <stdarg.h>
#include <stdio.h>

static const FhModHost* H;
static FhMod* M;
static int sKeyDown;

void modLog(FhLogLevel level, const char* format, ...) {
  char message[256];
  int prefix;
  va_list args;

  if (!H || !H->log || !M) return;
  prefix = snprintf(message, sizeof(message), "[Scarab Cheat] ");
  va_start(args, format);
  vsnprintf(message + prefix, sizeof(message) - (size_t)prefix, format, args);
  va_end(args);
  H->log(M, level, message);
}

FH_MOD_EXPORT int fh_mod_initialize(FhMod* mod, const FhModHost* host) {
  if (!host || host->abiVersion != FH_MOD_ABI_VERSION || host->structSize < sizeof(FhModHost)) return FH_MOD_ERROR;
  if (!host->log || !host->symbolAddress) return FH_MOD_ERROR;
  H = host;
  M = mod;
  if (!platformInputInitialize(mod, host)) {
    modLog(FH_LOG_ERROR, "disabled: keyboard input is unavailable");
    return FH_MOD_ERROR;
  }
  if (!scarabCheatInit(mod, host)) {
    platformInputShutdown();
    modLog(FH_LOG_ERROR, "disabled: required game symbols are unavailable");
    return FH_MOD_ERROR;
  }
  /* A key already held while the game starts is not a press. */
  sKeyDown = platformAddScarabsKeyDown();
  modLog(FH_LOG_INFO, "v1.1.0 loaded (+ Add 10 Scarabs)");
  return FH_MOD_OK;
}

FH_MOD_EXPORT void fh_mod_update(FhMod* mod) {
  int focused = platformInputActive();
  int down = platformAddScarabsKeyDown();
  int pressed = focused && down && !sKeyDown;
  (void)mod;

  /* The key is tracked every frame, focused or not and in any game state, so
     a key already held when the game regains focus or gameplay starts is not
     seen as a new press, and a press outside gameplay is dropped, not queued. */
  sKeyDown = down;
  if (pressed) scarabCheatAddScarabs();
}

FH_MOD_EXPORT void fh_mod_shutdown(FhMod* mod) {
  (void)mod;
  scarabCheatShutdown();
  platformInputShutdown();
  sKeyDown = 0;
  H = 0;
  M = 0;
}
