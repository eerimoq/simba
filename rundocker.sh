#!/usr/bin/env bash

SIMBA_DOCKER_VERSION=0.4.0
SIMBA_ROOT=$(pwd)

docker run \
       --rm \
       --interactive \
       --tty \
       -v /home/$USER:/home/$USER \
       -w /home/$USER \
       -v /etc/passwd:/etc/passwd:ro \
       -v /etc/group:/etc/group:ro \
       -u $(id -u):$(id -g) \
       eerimoq/simba:$SIMBA_DOCKER_VERSION \
       /bin/bash -c \
       "cd $SIMBA_ROOT \
        && source setup.sh \
        && printf \"\nWelcome to the Simba development environment!\n\n\" \
        && /bin/bash"
