Arduino oxymeter
=========
![Alt text](/img/screen.png "screen")


Theory

This works, because of the fact, that the pressure in arteries actually changes after each heart beat, while the rest (tissue, veins, bone etc.) stay stats quo. Since the blood is red (or at least the oxidized hemoglobin is) using red light is most suitable for this. So if we send red light trough a finger (or nose, or lip etc.), and have something that can detect the amount of light at the other side of it, we would have a base, but see 1-3% change in absorption. With some "clever" (or as you will see in my code, pretty simple) algorithms you can detect the heart beats from those changes.

I've found this old simple pulse-meter, which were using a IR led and a photo-diode. All with a nice jack connection.
I had to rebuild it, since I need two different LEDs to be able to measure the oxygen saturation (feature that I will hopefully add later). So for now it is still only a simple pulsemeter.


To stream the data to the browser I'm doing something like this: arduino->serial port->pyton websocket server->javascript websocket client

Schematics:


![Alt text](/img/schematic.png "schm")

