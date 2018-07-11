# SmartThingsSertaBedControl
Control Serta adjustable bed from SmartThings and Alexa


Configuration:


Setup your Photon. Add 3 files to your Particle project Code in the code editor:
CC2500_val.h
CC2500_REG.h
Particle_BedControl.ino
https://build.particle.io/build



This should get your device to the point where you can send it commands and manipulate the bed :)


SmartThings:
Create a new Device Handler in the SmartThings IDE 
https://graph.api.smartthings.com/ide/devices

see sample code in PhtoBedControl_SmartThingsDeviceHandler.txt

Create new Device in SmartThings that uses the new device handler.

Assuming you've gotten your smartThings setup with Alexa, you can now add this new device to alexa.
The smartThings device handler is defined as a switch. Alexa will allow you to use "ON" and "OFF" commands. 

The configuration for on and off, the switch states that alexa understands are defined as:
def on: Lounge
def off: Flat
You can change the default "on" state by changing the def on: state. These are the commands sent to the Particle device.

You'll need to setup your particle devices ID and your access key. These could probably be setup as configuration items for a device, instead of hard coded in the Device handler. In this case, only one particle can be controlled by the device handler. See SmartThings examples on how to setup preferences for each device, so that the device handler could be re-used to control many Particles.

Once setup in Smart Things, you'll see your device and all of the bed remote control buttons. In this example, The 'vibration' option buttons were not used. The CC2500_val.h file has the button codes defined, but are not configured in the particle ExecBedCmd function.

Once connected to Alexa/Amazon Echo, the following commands (and probably some variation of) are available:
"Alexa, turn on the bed"
"Alexa, turn off the bed"

Amazon Alexa sees this device as a normal switch. Any switch commands should work.


I believe there are ways to expand this device so that alexa can support any code word for the particle device. If you find this code useful and add those features (or other features) please let me know.
