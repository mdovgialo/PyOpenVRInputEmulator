# PyOpenVRInputEmulator
Minimal "just works" wrapper for https://github.com/matzman666/OpenVR-InputEmulator

To use:

Drop the unpacked release .zip into your python package/script location.

Requires pyopenvr https://github.com/cmbruns/pyopenvr and https://github.com/matzman666/OpenVR-InputEmulator 1.3

run SteamVR then you can use this example script:
Position will be set relative to the main Lighthouse.
```
from pyopenvrinputemu import VRInputSystem

from math import sin, cos
import time

kinect_location = [0, 0, 0]
kinect_rotation = [0, 0, 0]

vrinput = VRInputSystem(global_offset=kinect_location, global_rotation=kinect_rotation)

hip_tracker = vrinput.add_tracker('hip')
while True:

  hip_tracker.update(x=sin(time.time()), y=cos(time.time()), z=1)
  time.sleep(1/60)

```
