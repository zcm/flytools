#include <inttypes.h>

#include "mockmem.h"

#if __STDC_VERSION__ >= 201112L
#if __STDC_VERSION__ < 202311L
#include <threads.h>
#endif  // __STDC_VERSION__ < 202311L
#else
#define thread_local
#endif  // __STDC_VERSION__ > 201112L

extern void *__real_malloc(size_t size);

typedef struct mock_alloc {
  struct mock_alloc *next;
  void *ptr;
} mock_alloc;

static thread_local mock_alloc *playlist = NULL;
static thread_local mock_alloc *playlist_end = NULL;

void mockmem_playlist_queue(void *ptr) {
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
