Interface with a wireless doorbell and trigger a smartphone notification when someone rings it.
This requires a ESP8266 with a FTDI adapter for serial communication, a FM transmitter, a button, and some wires.
You will need to decode the signal of the doorbell's original FM transmitter, so you can mimic that fm signal and trigger the doorbell from the ESP8266. You can check out this guide http://nrocy.com/2014/08/02/mimicking-rf-remote-light-signals-with-arduino/ or view Samy Kamkar's videos on youtube for more information how to do this.
Furthermore, this application triggers notifications using the pushingbox API.
The particular doorbell I used is the MP3 doorbell from Clas Ohlson: http://www.clasohlson.com/uk/Wireless-MP3-Doorbell/36-3998 article number 36-3998.
