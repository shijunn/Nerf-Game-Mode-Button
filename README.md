# Nerf Game Mode Button
<img src="https://github.com/user-attachments/assets/2708c357-ee3d-4856-8f36-ef8ba9cf3960" width=600 height=250>

This program is developed for game mode buttons used in NerfSG games.

Designed for ESP32 C6.

Game modes:

- capture the flag     -      3 mins, press to stop
- domination           -      3 mins, least number of clicks wins, start from 0
- king of the hill     -      5 mins, press and hold, start from 0
- clicker domination   -      3 mins, start from 0, most clicks wins
- chess clock          -      start from time limit, press once, timer goes down

## Features

- Adjustable game duration and countdown time
- Settings are saved for user convenience
- Teams are assigned using one button
- Audio announcements using mp3 library from https://github.com/bluejunimo/YX5300_ESP32

Work in progress:

- Chess clock game mode
- Issue with mp3 files playing in the wrong order
