Simple Amiga SDL Test
=====================

This is intended to be a simple SDL test for Amiga OS3 on Vampire4 SA hardware. It just opens a screen and draws some random boxes, in theory measuring the FPS.

I have tried two SDL libraries. 1) SDL_040_RTG port by Novacoder on Aminet and 2) the one from AmigaPorts on GitHub which I may be wrong, but my understanding is that this is the one bundled with
the gcc 6 build by Bebbo?

Anyway, the point of the test was to demonstrate the following problems I'm having.

1. When I try to open on V4SA using AmigaPorts it crashes the display and although the system doesn't entirely crash, SDL doesn't respond to input so you need to reboot.
2. When I try to open under emulation (FS-UAE) the code does not find any viable screens to run at despite Picasso modes being defined
3. Using the Novacoder port it seems to cap out at 5FPS which I'm guessing can't really be correct for an 68080 that reports 153mips? Maybe it is? So I'm not sure if I'm maybe doing something wrong in the FPS calculation? I'm aware it's not an accurate calculation by any means!
