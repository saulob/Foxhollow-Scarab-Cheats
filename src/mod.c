#include "scarab_cheat.h"

#include <stdarg.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/* Number-row 0. The numpad 0 key is a different virtual key and is not read. */
#define ADD_SCARABS_KEY '0'

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

static int game_window_focused(void) {
  HWND window = GetForegroundWindow();
  DWORD processId = 0;

  if (window == NULL) return 0;
  GetWindowThreadProcessId(window, &processId);
  return processId == GetCurrentProcessId();
}

static int key_down(int key) {
  return (GetAsyncKeyState(key) & 0x8000) != 0;
}

FH_MOD_EXPORT int fh_mod_initialize(FhMod* mod, const FhModHost* host) {
  if (!host || host->abiVersion != FH_MOD_ABI_VERSION || host->structSize < sizeof(FhModHost)) return FH_MOD_ERROR;
  if (!host->log || !host->symbolAddress) return FH_MOD_ERROR;
  H = host;
  M = mod;
  if (!scarabCheatInit(mod, host)) {
    modLog(FH_LOG_ERROR, "disabled: required game symbols are unavailable");
    return FH_MOD_ERROR;
  }
  /* A key already held while the game starts is not a press. */
  sKeyDown = key_down(ADD_SCARABS_KEY);
  modLog(FH_LOG_INFO, "v1.0.0 loaded (0 Add 10 Scarabs)");
  return FH_MOD_OK;
}

FH_MOD_EXPORT void fh_mod_update(FhMod* mod) {
  int focused = game_window_focused();
  int down = key_down(ADD_SCARABS_KEY);
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
  sKeyDown = 0;
  H = 0;
  M = 0;
}
