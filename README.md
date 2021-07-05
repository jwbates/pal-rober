# pal-rober
Palette finder from rober's class

Things used here:

1) Arduino Uno R3
2) Arduino Nano Sense 33 BLE
3) Arducam OV2640
4) Elegoo 2.8" TFT display

Code is split into two parts/sketches: computer, which does the clustering, and
controller, which does the UI and data acquisition. Shared code sits in the
C++ headers in src/include. All classes are header-only, because I'm lazy.

Honestly, this whole thing could have fit on the Nano Sense. It's just that big.
Unfortunately, its processor ]is a new model and the support libraries are spotty,
hence the Uno does the grunt work and the Nano does the sophisticated pieces.
