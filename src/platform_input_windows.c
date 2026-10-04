#include "platform_input.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int platformInputInitialize(FhMod* mod, const FhModHost* host) {
  (void)mod;
  (void)host;
  return 1;
}

void platformInputShutdown(void) {
}

int platformInputActive(void) {
  HWND window = GetForegroundWindow();
  DWORD processId = 0;

  if (window == NULL) return 0;
  GetWindowThreadProcessId(window, &processId);
  return processId == GetCurrentProcessId();
}

static int key_down(int key) {
  return (GetAsyncKeyState(key) & 0x8000) != 0;
}

/* The + control: VK_OEM_PLUS is the main-keyboard key that types "=", or "+"
   with Shift. Shift is deliberately not read, so the key works either way and
   pressing or releasing Shift while it is held is not a new press. VK_ADD is
   numpad +, which Num Lock does not affect. Both keys are one control: it is
   down while either key is down. */
int platformAddScarabsKeyDown(void) {
  return key_down(VK_OEM_PLUS) || key_down(VK_ADD);
}
