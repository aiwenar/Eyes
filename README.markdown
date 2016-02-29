The Eyes Project
================

Ever wanted to have a hamster, but all of them died because you were too busy maintaining your Gentoo to feed them for a week? We just got something for you! 

Eyes are visual pet-like system monitor. Their mood indicate current (or recent) system status which can be somehow interpreted as mood of your PC right? So every time your CPU is bloated with tons of computing, running out of memory or battery - eyes are sad :c or even mad at you! Perfect pet-replacement if you feel a little itty bitty lonely in your server room surrounded by tons of racks and networking hardware 24/7 with no friends. As eyes are relatively interactive - they react to your mouse gestures and support camera for real world interaction, you won't be alone ever again.

![Eyes screenshot](https://dl.dropboxusercontent.com/u/44131220/undeletable/z152.png "Eyes")

If you totally won't care about them and just leave them running, occasionally slap with mouse accidentally, plus relatively often push your machine to the limits they will hate you. But at least they won't die, it's some improvement comparing to hamster right? Also they're free and don't eat so you can save money for I dunno new *mouse* or something HA HA ._.

Feautures
---------

Omg, glad you asked - they can do **everything**

- Custom, advanced, scalable skins
- CPU, RAM, temperature, battery, uptime, time monitoring (anger, sweating, being sleepy)
- Simple model of relationship with user affecting mood changes (linear friendship scale)
- Automatic typical machine usage adaptation (Eyes detect "typical" machine state and react only to relative differences from this state so they're also suitable for use in environments with high typical machine load eg. workstations)
- mouse interaction
- Huge temperature fluctuations detection (resulting in illness)
- Perv detection (battery remove trigger lol)
- Advanced camera support
- Dynamic reflection texture using camera
- Season/time or camera based pupils size
- Looking at objects in motion and human faces
- Screensaver management (xscreensaver, kscreensaver, gnome-screensaver) basing on face and motion detection
- Camera idle detection to reduce CPU usage
- Various optimization settings (gfx settings, camera settings)
- Automatic CPU usage adaptation (configured max CPU usage for active and idle modes - note: too harsh limits may result in disabling camera)
- auto emergency suspend / poweroff in case of critical battery / overheating (requires root)
- linear screen brightness adjustment

Installation on Linux
---------------------

Eyes require Qt4, OpenCV 4.2 and stackonfigure library found here: https://github.com/aiwenar/stackonfigure
Optionally they can utilize lm-sensors. Whereas camera itself is optional, OpenCV library is required.

    qmake
    make

To launch eyes enter their directory and type:

    export LD_LIBRARY_PATH=./libeyes
    eyes 1>./core_log.log 2>./gui_log.log

Alternatively if you have large enough console you can display their CLI panel, by changing last line to:

    eyes 2>./gui_log.log

This mode is also suitable for headless mode (eg. for various servers or other headless machines)

Installation on Microsoft™ Windows®
-----------------------------------

¯\\_(ツ)_/¯

FAQ
---

**Q:** It doesn't look like my waifu ;__;

**A:** Well, no worries they're fully skinnable, you can always add your custom skin - just grab some M$ Paint running under WINE or other pro-grade graphics editor and use it. Please share if you don't mind c:

**Q:** OMG this config file has like milion options, what if I don't know what are they for? :O

**A:** Don't worry buddy, i don't know either... Those which are self-explanatory are self-explanatory. Rest is probably not important.

**Q:** My Eyes look like they're high after launch!

**A:** No, they're just sleepy. It's normal, they just woke up after all.

**Q:** My Eyes are permanently tired and sweating, what to do?

**A:** They're probably ill. Just pet them and be nice, they'll be fine in few days

**Q:** Why Qt4/OpenCV 2.4.2/this code looks so lame
**A:** Well... we were coding it in secondary school it's like... MANY years ago. OpenCV 2.4.2 was then bleeding edge compiled from source, KDE4 was still unstable (just kidding, it still is) and we didn't have much experience back then.
