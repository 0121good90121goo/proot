#ifndef FAKE_ID0_CHROOT_H
#define FAKE_ID0_CHROOT_H

#include "tracee/tracee.h"
#include "extension/fake_id0/config.h"

int handle_chroot_exit_end(Tracee *tracee, Config *config);
int handle_chroot_sigsys(Tracee *tracee, Config *config);

#endif /* FAKE_ID0_CHROOT_H */
