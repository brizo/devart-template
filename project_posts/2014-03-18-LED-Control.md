LED Control:

To control the LEDs, we chose to use the open standard [Art-Net](http://en.wikipedia.org/wiki/Art-Net "Art-Net") because there are many systems we could plug into or use our own.  It is based on the lighting control protocol [DMX512-A](http://en.wikipedia.org/wiki/DMX512-A "DMX") and each channel is 1 pixel, or 1 LED.  [Art-Net](http://en.wikipedia.org/wiki/Art-Net "Art-Net") is broken up into multiple universes, each universe being 512 pieces of information.  Since each LED consists of 3 bits of information (RGB), [Art-Net](http://en.wikipedia.org/wiki/Art-Net "Art-Net") supports 170 LEDs.  Spanning multiple universes we can control the total array of LEDs as we would like.  

By breaking up the 3D image into slices, we then pass on each slice to a 2D array of LEDs like a normal screen. 

![Example Image](project_images/cover.jpg?raw=true "Example Image")

We are using a modified version of the [OctoWS2811](https://www.pjrc.com/teensy/td_libs_OctoWS2811.html "OctoWs2811") library incorporating the [Art-Net](http://en.wikipedia.org/wiki/Art-Net "Art-Net") protocol on one [Teensy 3.1](https://www.pjrc.com/store/teensy31.html "Teensy") microcontroller board.  This enables us to have the ability to control the LEDs with not just our software, but expand to other Art-Net enabled software also.  

![Teensy3.1](project_images/teensy31.jpg?raw=true "Teensy")

We are planing on using WS2811 enabled LED balls with full RGB color capabilities.  They consist of 5050 LEDs and controlled by a WS2811 chip.

![LED Balls](project_images/ledball.jpg?raw=true "LED Ball")