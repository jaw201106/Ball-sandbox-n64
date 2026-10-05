# Ball Sandbox N64

A tiny fun physics sandbox for the Nintendo 64. Move a cursor around the screen, hold **A**, and spawn bouncing balls. Watch them pile up.

Originally ported from a Windows XP version I made — now running on real N64 hardware (or an emulator) via libdragon.

## Controls

- **Analog stick** — move cursor
- **Hold A** — spawn balls

Up to 900 balls at 320x240 with simple gravity and collision physics.

## Requirements

- libdragon toolchain: https://github.com/DragonMinded/libdragon
- `N64_INST` environment variable pointing at your libdragon install

## Build

```bash
# if you haven't set these yet
export N64_INST=/opt/libdragon
export PATH=$PATH:$N64_INST/bin

cd N64
make
```

This produces `ball_physics.z64`, which you can run on an emulator or flash cart.

## Notes

See libdragon's repo for full toolchain setup instructions if you're starting from scratch.
