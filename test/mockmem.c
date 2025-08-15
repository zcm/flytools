#include <inttypes.h>

#include "mockmem.h"

#if __STDC_VERSION__ >= 201112L && !defined(_MSC_VER)
#if __STDC_VERSION__ < 202311L
#include <threads.h>
#endif  // __STDC_VERSION__ < 202311L
#else
#define thread_local
#endif  // __STDC_VERSION__ > 201112L

#ifdef _MSC_VER
static void *(*__real_malloc)(size_t) = NULL;
#else
extern void *__real_malloc(size_t size);
#endif

typedef struct mock_alloc {
  struct mock_alloc *next;
  void *ptr;
} mock_alloc;

static thread_local mock_alloc *playlist = NULL;
static thread_local mock_alloc *playlist_end = NULL;

void mockmem_queue(void *ptr) {
  mock_alloc *mock = (mock_alloc *) __real_malloc(sizeof (mock_alloc));
  mock->next = NULL;
  mock->ptr = ptr;

  if (!playlist) {
    playlist = mock;
  }

  if (playlist_end) {
    playlist_end->next = mock;
  }

  playlist_end = mock;
}

void *mockmem_peek() {
  return (void *) playlist;
}

void *__wrap_malloc(size_t size) {
  mock_alloc *mock;
  void *ret;

  if (playlist) {
    mock = playlist;
    playlist = mock->next;
    ret = mock->ptr;
    free(mock);
    return ret;
  }

  return __real_malloc(size);
}

#ifdef _MSC_VER
DWORD hook_malloc(HMODULE mod) {
  ULONG size;

  PIMAGE_IMPORT_DESCRIPTOR import = (PIMAGE_IMPORT_DESCRIPTOR)
    ImageDirectoryEntryToDataEx(
        mod, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &size, NULL);

  if (!import) {
    return GetLastError();
  }

  for (; import->Name; import++) {
    LPCSTR dll = (LPCSTR) ((BYTE *) mod + import->Name);

#ifdef _DEBUG
    int found = _stricmp(dll, "ucrtbased.dll") == 0;
#else
    int found = _stricmp(dll, "ucrtbase.dll") == 0;
#endif

    if (found) {
      PIMAGE_THUNK_DATA thunk =
        (PIMAGE_THUNK_DATA) ((BYTE*) mod + import->FirstThunk);
      PIMAGE_THUNK_DATA orig =
        (PIMAGE_THUNK_DATA) ((BYTE*) mod + import->OriginalFirstThunk);

      for (; orig->u1.AddressOfData; thunk++, orig++) {
        PIMAGE_IMPORT_BY_NAME import_name =
          (PIMAGE_IMPORT_BY_NAME) ((BYTE*) mod + orig->u1.AddressOfData);

        if (strcmp((char *) import_name->Name, "malloc") == 0) {
          DWORD protect;  // Stores original protect to restore after patching

          VirtualProtect(
              &thunk->u1.Function, sizeof (LPVOID), PAGE_READWRITE, &protect);

          __real_malloc = (void *(__cdecl *)(size_t)) thunk->u1.Function;
          thunk->u1.Function = (ULONG_PTR) __wrap_malloc;

          VirtualProtect(
              &thunk->u1.Function, sizeof (LPVOID), protect, &protect);

          break;
        }
      }
    }
  }

  return 0;
}
#endif  // _MSC_VER
