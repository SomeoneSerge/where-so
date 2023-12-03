// Enables Dl_serinfo
#define _GNU_SOURCE

#include <dlfcn.h>
#include <limits.h>
#include <link.h>
#include <memory>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
  char testSoname[] = "libstdc++.so";

  void *handle = dlopen(testSoname, RTLD_LAZY | RTLD_LOCAL);
  if (!handle) {
    fprintf(stderr, "Couldn't dlopen %s. Reason: %s\n", testSoname, dlerror());
    return EXIT_FAILURE;
  }

  char path[PATH_MAX + 1];
  if (dlinfo(handle, RTLD_DI_ORIGIN, &path) == -1) {
    return EXIT_FAILURE;
  }
  printf("%s\n", path);

  Dl_serinfo peek;
  if (dlinfo(handle, RTLD_DI_SERINFOSIZE, &peek) == -1) {
    fprintf(stderr, "RTLD_DI_SERINFOSIZE failed for %s. Reason: %s\n",
            testSoname, dlerror());
  }

  std::unique_ptr<uint8_t[]> storage =
      std::make_unique<uint8_t[]>(peek.dls_size);
  Dl_serinfo *info = (Dl_serinfo *)(void *)storage.get();

  if (dlinfo(handle, RTLD_DI_SERINFO, &peek) == -1) {
    fprintf(stderr, "RTLD_DI_SERINFOSIZE failed for %s. Reason: %s\n",
            testSoname, dlerror());
    return EXIT_FAILURE;
  }

  for (auto j = 0; j < info->dls_cnt; ++j) {
    printf("%s\n", info->dls_serpath[j].dls_name);
  }
}
