
# Welcome 

This project is meant to be a guided take on reverse-engineering a cheap laser rangefinder from lidl, the PLEM 50 C3, and later trying to repurpose it. By trying to reverse it, we can gain some knowledge about how this all works, and if it goes well, we can apply this know-how in taking a shot at bigger and more complex products.

![The PLEM 50 C3 rangefinder](https://github.com/AndreiVladescu/Reverse-Engineering-Laser-Rangefinder/blob/main/images/rangefinder.jpg)

## Gathering the toys

If trying to recreate this reversing or just repurposing the rangefinder to use in your side-projects, these will be usefull to have:

- Multimeter
- Logic analyzer
- Soldering iron
- Soldering wick
- Soldering wire
- 3V power supply / 2x1.5V batteries
- Microscope (optional)
- Tweezers
- Screwdriver
- Prototyping wire
- Nitril gloves
- Spare time


These tools are about everything you would want to use in reversing lots of devices, from taking them apart to probing around the PCB.

### Safety precautions

Be warned, every device you take apart, you will do it on your own, and you will most likely void any warranty left on it. If taking apart AC-connected devices or high-voltage DC devices, be extra-careful you don't electrocute yourself.

## How a rangefinder works

The principle that a typical rangefinder uses is time-of-flight. A beam of particles (in our case, invisible light) are thrown in a direction. When they reflect from a surface, some of then come back and will hit a receiver on the rangefinder. The distance is calculated by the formula $d = \frac{t}{2}$

![tof-principle](https://github.com/AndreiVladescu/Reverse-Engineering-Laser-Rangefinder/blob/main/images/tof_principle.png)
Knowing how these devices work, means that while using them, you **should not** stare directly into the emitter, and try to point it in a safe direction while experimenting with it.

[Let's try and recon it >>](https://github.com/AndreiVladescu/Reverse-Engineering-Laser-Rangefinder/blob/main/recon.md)
