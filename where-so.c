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

    Dl_serinfo peek;
    if (dlinfo(handle, RTLD_DI_SERINFOSIZE, &peek) == -1) {
      status = EXIT_FAILURE;
      fprintf(stderr, "RTLD_DI_SERINFOSIZE failed for %s. Reason: %s\n",
              argv[i], dlerror());
      continue;
    }

    {
      Dl_serinfo *info = (Dl_serinfo *)malloc(peek.dls_size);
      if (!info) {
        perror("malloc");
        return EXIT_FAILURE;
      }

      if (dlinfo(handle, RTLD_DI_SERINFOSIZE, info) == -1) {
        free(info);
        status = EXIT_FAILURE;
        fprintf(stderr, "RTLD_DI_SERINFOSIZE (2nd) failed: %s\n", dlerror());
        continue;
      }
      if (dlinfo(handle, RTLD_DI_SERINFO, info) == -1) {
        free(info);
        status = EXIT_FAILURE;
        fprintf(stderr, "RTLD_DI_SERINFO failed for %s. Reason: %s\n", argv[i],
                dlerror());
        continue;
      }
      for (int j = 0; j < info->dls_cnt; ++j) {
        printf("%s\n", info->dls_serpath[j].dls_name);
      }
      free(info);
    }
  }
  return status;
}
