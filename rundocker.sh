#!/usr/bin/env bash

SIMBA_DOCKER_VERSION=0.5.0
SIMBA_ROOT=$(pwd)

docker run \
       --rm \
       --interactive \
       --tty \
       -v /home/$USER:/home/$USER \
       -w /home/$USER \
       -v /etc/passwd:/etc/passwd:ro \
       -v /etc/group:/etc/group:ro \
       -v /dev:/dev \
       --user $(id -u):$(id -g) \
       --group-add dialout \
       --privileged \
       --network host \
       eerimoq/simba:$SIMBA_DOCKER_VERSION \
       /bin/bash -c \
       "cd $SIMBA_ROOT \
        && source setup.sh \
        && printf \"\nWelcome to the Simba development environment!\n\n\" \
        && /bin/bash"
