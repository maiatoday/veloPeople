#! /bin/bash
#autostart script which will work with lubuntu install
cd /usr/share/forkmerge/boodler
boodler --output pulse --listen --extern theagents com.example.theagents/Example &
cd /usr/share/forkmerge/bin
./forkmerge &
