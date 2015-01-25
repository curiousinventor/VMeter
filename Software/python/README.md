Instructions copied from <a href="http://vmeter.net/python-control-of-vmeter-usb-midi-device-via-pyportmidi-and-portmidi/">VMeter Python MIDI Control</a>.

<iframe width="560" height="315" src="//www.youtube.com/embed/rwfxXF12d7I?list=UUOGrxFj_j7PZRQM63OFCwmA" frameborder="0" allowfullscreen></iframe>

<h1>Python Control of VMeter USB MIDI Device via PyPortMIDI and MidiPort</h1>
<p>&nbsp;</p>
<p>This tutorial will show how to setup pyMidiPort on OS X to control the VMeter USB MIDI device with python scripts. Using the new sysex commands in the latest VMeter firmware (1.8, only on hardware rev B+), you can control every single LED directly (not just columns as with MIDI Controller messages).</p>
<p>&nbsp;</p>
<p>&nbsp;Check out the video demo here: <a href="http://vmeter.net/controlling-individual-vmeter-leds-via-midi-binary-clock-game-of-life-demos/">Binary Clock, Chase, Treadmill, Game of Life via Individual LED control of VMeter.</a></p>
<p>&nbsp;</p>
<p><span style="color: #ff0000;">Warning</span>: installing pyPortMidi may require a bit of installation judo and trial &amp; error. It may take 5 min., but it took us 2 days! If you know other software for MIDI and python let us know! <a href="https://groups.google.com/forum/#!topic/pygame-mirror-on-google-groups/sf3I8Q-wYQA">PyGame was a deadend</a> for us.&nbsp;</p>
<p>&nbsp;</p>
<p>Instructions are based on <strong>OS X 10.8.2 Mountain Lion</strong>. Most of these instructions were gleaned from&nbsp;<a href="http://cratel.wichita.edu/cratel/cratel%20pyportmidi#build_on_OS_X">http://cratel.wichita.edu/cratel/cratel%20pyportmidi#build_on_OS_X</a>.</p>
<p>&nbsp;</p>
<p>You may also need to install <a href="http://kennethreitz.com/xcode-gcc-and-homebrew.html">XCode command line tools</a>.</p>
<p>&nbsp;</p>
<ol>
<li>Install <a href="http://mxcl.github.com/homebrew/">homebrew</a> and run "<em>brew doctor</em>" to make sure everything is ok.</li>
<li>Install portmidi (installing portmidi from source failed for us).</li>
<ol>
<li>Use homebrew to install portmidi:<em> brew install portmidi</em></li>
<li>We got the error "Warning: Could not link portmidi. Unlinking...", and had to then run "<em>brew link --overwrite portmidi</em>"</li>
</ol>
<li>Download <a href="http://www.cosc.canterbury.ac.nz/greg.ewing/python/Pyrex/oldtar/">PyRex ver 0.9.6.4</a>. Other versions may work, but they're untested.</li>
<li>Change line 21 of Pyrex/Mac/DarwinSystem.py to:&nbsp;os.environ["MACOSX_DEPLOYMENT_TARGET"] = "10.6"</li>
<li>Build and install Pyrex: <em>sudo python setup.py install</em></li>
<li>Download <a href="http://alumni.media.mit.edu/~harrison/code.html">PyPortMidi</a>&nbsp;(ver 0.0.3)</li>
<li>replace portmidi.h, porttime.h with the files from the installed portmidi. In my case, these were located in /usr/local/Cellar/portmidi/200/include/</li>
<li>Copy&nbsp;libportmidi.dylib from the installed portmidi to the OSX directory in pyPortMidi-0.0.3. In my case, this was located in /usr/local/Cellar/portmidi/200/lib/</li>
<li>Change line 357 of pypm.pyx to&nbsp;<br />&nbsp;&nbsp;&nbsp;<em>while(Pm_Poll(self.midi) != pmNoError):</em></li>
<li>Run the pyPortMidi build: <em>sudo python setup.py install</em></li>
<li>Plug in a USB Midi device and run the test: <em>python test.py</em></li>
</ol>
