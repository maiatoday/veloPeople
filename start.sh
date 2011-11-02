#! /bin/bash
#autostart script which will work with lubuntu install
cd /home/maia/oF/openFrameworks/apps/myApps/lumoPeople/boodler
boodler --output pulse --listen --extern theagents com.example.theagents/Example &
cd /home/maia/oF/openFrameworks/apps/myApps/lumoPeople/bin
./lumoPeople 
