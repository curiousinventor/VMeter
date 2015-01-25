Warning: this code is based on an earlier version of the Arduino USB Host shield, might require some tweaks to work with a recent shield!

<a href="http://vmeter.net/using-vmeter-and-arduino-to-control-fan-and-servos/">Original instructions</a>

<iframe width="420" height="315" src="//www.youtube.com/embed/fYatmbv_9Qo" frameborder="0" allowfullscreen></iframe>

<h1>Using VMeter and Arduino to control a Fan, Servo Motors, and Read Analog Input</h1>

[![Python VMeter MIDI Demo Video](http://img.youtube.com/vi/fYatmbv_9Qo/0.jpg)](http://www.youtube.com/watch?v=fYatmbv_9Qo)

Python VMeter MIDI Demo Video

<p>Using an Arduino and USB Host shield, you can use the VMeter to visualize and control physical knobs and motors.&nbsp;</p>

<h2>Things you'll need</h2>

<ul>
<li>Arduino</li>
<li><a href="http://www.circuitsathome.com/products-page/arduino-shields/usb-host-shield-2-0-for-arduino">USB Host Shield</a></li>
<li>The USB Host shield libraries (available at above link)</li>
<li><a href="https://bitbucket.org/curiousinventor/vmeter_arduino_demo">Arduino code</a>&nbsp;(note, this code works on an original arduino and USB Host Shield, you might need to tweak things if you're using a new model of either. Email us if you need any help).</li>
<li>Servos</li>
<li>DC Motor</li>
<li>Potentiometer (to test analog input)</li>
</ul>
<div>&nbsp;</div>
<h2>What the code does</h2>
<div>&nbsp;</div>
<div>Depending on what parts of the code you leave commented or uncommented, it can do the following:</div>
<div>&nbsp;</div>
<div>
<ul>
<li>(arduino --&gt; VMeter) read analog input (voltage) and display the output on the VMeter as a column</li>
<li>(VMeter --&gt; arduino) read the position and or pressure, and use this to control either a DC motor (analogWrite()) or Servo Motors.</li>
</ul>
<div>&nbsp;</div>
<h2>Note(s)</h2>
</div>
<div>
<ul>
<li>&nbsp;When you test out your USB Host shield, first run the <a href="https://www.circuitsathome.com/mcu/arduino-usb-host-part-3-descriptors">descriptors </a>test code (Arduino --&gt; examples --&gt; USB) to make sure you can initialize the VMeter.</li>
</ul>
