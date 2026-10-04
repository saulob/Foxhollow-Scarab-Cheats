#define _GNU_SOURCE

#include "platform_input.h"
#include "scarab_cheat.h"

#include <dlfcn.h>
#include <stdbool.h>
#include <stddef.h>

/* SDL_Scancode values from Foxhollow's SDL3 (SDL_scancode.h). */
#define SDL3_SCANCODE_EQUALS 46
#define SDL3_SCANCODE_KP_PLUS 87

typedef const bool* (*SdlGetKeyboardStateFn)(int* numkeys);
typedef void* (*SdlGetKeyboardFocusFn)(void);

static SdlGetKeyboardStateFn sGetKeyboardState;
static SdlGetKeyboardFocusFn sGetKeyboardFocus;

static void* resolve_sdl_symbol(FhMod* mod, const FhModHost* host, const char* name) {
  void* address = dlsym(RTLD_DEFAULT, name);

  if (address == NULL) {
    address = host->symbolAddress(mod, name);
  }
  if (address == NULL) {
    modLog(FH_LOG_ERROR, "could not resolve %s", name);
  }
  return address;
}

int platformInputInitialize(FhMod* mod, const FhModHost* host) {
  const bool* keys;
  int count = 0;

  sGetKeyboardState = (SdlGetKeyboardStateFn)resolve_sdl_symbol(mod, host, "SDL_GetKeyboardState");
  sGetKeyboardFocus = (SdlGetKeyboardFocusFn)resolve_sdl_symbol(mod, host, "SDL_GetKeyboardFocus");
  if (sGetKeyboardState == NULL || sGetKeyboardFocus == NULL) {
    platformInputShutdown();
    return 0;
  }
  keys = sGetKeyboardState(&count);
  if (keys == NULL || count <= SDL3_SCANCODE_KP_PLUS) {
    modLog(FH_LOG_ERROR, "SDL keyboard state is unavailable");
    platformInputShutdown();
    return 0;
  }
  return 1;
}

void platformInputShutdown(void) {
  sGetKeyboardState = NULL;
  sGetKeyboardFocus = NULL;
}

int platformInputActive(void) {
  return sGetKeyboardFocus != NULL && sGetKeyboardFocus() != NULL;
}

static int key_down(const bool* keys, int count, int scancode) {
  return keys != NULL && scancode < count && keys[scancode];
}

/* The + control, by physical key: SDL_SCANCODE_EQUALS is the main-keyboard key
   left of Backspace, "=" or "+" with Shift on a US layout, and
   SDL_SCANCODE_KP_PLUS is numpad +. Scancodes ignore Shift, so pressing or
   releasing Shift while the key is held is not a new press. Both keys are one
   control: it is down while either key is down. SDL clears this state when the
   window loses focus and restores only modifier keys when it regains it, so a
   key held across a focus change reads as up until it is pressed again. */
int platformAddScarabsKeyDown(void) {
  const bool* keys;
  int count = 0;

  if (sGetKeyboardState == NULL) return 0;
  keys = sGetKeyboardState(&count);
  return key_down(keys, count, SDL3_SCANCODE_EQUALS) || key_down(keys, count, SDL3_SCANCODE_KP_PLUS);
}
