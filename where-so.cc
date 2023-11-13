// Enables Dl_serinfo
#define _GNU_SOURCE

#include <dlfcn.h>
#include <limits.h>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s SONAME[...]\n", argv[0]);
    return EXIT_FAILURE;
  }

  int status = EXIT_SUCCESS;

  for (int i = 1; i < argc; ++i) {
    void *handle = dlopen(argv[i], RTLD_LAZY | RTLD_LOCAL);
    if (!handle) {
      status = EXIT_FAILURE;
      fprintf(stderr, "Couldn't dlopen %s. Reason: %s\n", argv[i], dlerror());
      continue;
    }

    char path[PATH_MAX + 1];
    if (dlinfo(handle, RTLD_DI_ORIGIN, &path) == -1) {
      status = EXIT_FAILURE;
      continue;
    }
    printf("%s\n", path);
  }
  return status;
}
