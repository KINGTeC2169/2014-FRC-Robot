
@echo off
set "var=%cd%"

start cmd /c "C:\Program Files (x86)\FRC Driver Station\Driver Station.exe"
start cmd /c "C:\Program Files (x86)\FRC Dashboard\Dashboard.exe"
start cmd /c "C:\Program Files (x86)\RoboRealm\RoboRealm.exe" %var%\Vision\vision.robo
start cmd /c "http://10.21.69.11"
exit