
# VMeter Python demos
# VMeter.net
# ver 1. 1/26/13

import pypm
import array
import time
from collections import deque

INPUT=0
OUTPUT=1

def PrintDevices(InOrOut):
    for loop in range(pypm.CountDevices()):
        interf,name,inp,outp,opened = pypm.GetDeviceInfo(loop)
        if ((InOrOut == INPUT) & (inp == 1) |
            (InOrOut == OUTPUT) & (outp ==1)):
            print loop, name," ",
            if (inp == 1): print "(input) ",
            else: print "(output) ",
            if (opened == 1): print "(opened)"
            else: print "(unopened)"
    print
    

# Using the psutil library, displays current activity. A top sided
# envelope follower is used to smooth out the display.
# The envelope follower will immediately jump to a higher level,
# but falls slowly.
def MonitorCpu(MidiOut):
    import psutil # http://code.google.com/p/psutil/
    cpu_percent = 0.0
    while True:
        new_cpu_percent = psutil.cpu_percent(interval=.05)
        if new_cpu_percent > cpu_percent:
            cpu_percent = new_cpu_percent
        else:
            cpu_percent = cpu_percent * .8;
        output = int(cpu_percent * 1.27)
        SendColumn(MidiOut,output)



def SendArray(array, MidiOut):
    # assuming 38 length array
    # need to split array into (6) 7bit chunks
    # Individual LED control is sent to the aftertouch MIDI command and channels 14, 15 and 16.
    # Each of the data bytes transmit 7 LED states.
    bytes = [0,0,0,0,0,0]
    bytes[0] = array[0] | array[1]<<1 | array[2]<<2 | array[3]<<3 | array[4]<<4 | array[5]<<5 | array[6]<<6
    bytes[1] = array[7] | array[8]<<1 | array[9]<<2 | array[10]<<3 | array[11]<<4 | array[12]<<5 | array[13]<<6
    bytes[2] = array[14] | array[15]<<1 | array[16]<<2 | array[17]<<3 | array[18]<<4 | array[19]<<5 | array[20]<<6
    bytes[3] = array[21] | array[22]<<1 | array[23]<<2 | array[24]<<3 | array[25]<<4 | array[26]<<5 | array[27]<<6
    bytes[4] = array[28] | array[29]<<1 | array[30]<<2 | array[31]<<3 | array[32]<<4 | array[33]<<5 | array[34]<<6
    bytes[5] = array[35] | array[36]<<1 | array[37]<<2
    MidiOut.WriteShort(0xAD,bytes[0],bytes[1])
    MidiOut.WriteShort(0xAE,bytes[2],bytes[3])
    MidiOut.WriteShort(0xAF,bytes[4],bytes[5])
    
def SetLEDsIgnoreTouch(MidiOut):
    MidiOut.WriteShort(0xB0,119,107) # this causes the LEDs to no respond to touch, only MIDI input.

def EnableOnOffOutput(MidiOut):
    MidiOut.WriteShort(0xB0,119,120) # now the VMeter will send 127 via ctrl #17 when touched, and 0 when released. 119 disables.

def SendColumn(MidiOut,height):
    MidiOut.WriteShort(0xB0,20,height)

def EnablePressureOutput(MidiOut):
    MidiOut.WriteShort(0xB0,119,122)



led_array = [1,0,1,0,1,0,1,0,1,0,
             1,0,1,0,1,0,1,0,1,0,
             1,0,1,0,1,0,1,0,1,0,
             1,0,1,0,1,0,1,0]
led_array_deque = deque(led_array)



# Binary clock display.
# Each digit is displayed over 4 LEDs.
# Marker LEDs blink every half second to indicate the position of the digits.
# It displays hours, minutes and seconds, where hours are 24 hour format.
def BinaryClock(MidiOut):
    from datetime import datetime
    last_cycle_time = 0
    led_array = [0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0]
    update_time = 0
    while True:
        if pypm.Time() - last_cycle_time > 500:
            last_cycle_time = pypm.Time()
            led_array[11] = update_time # marker for minutes, just blinks with seconds
            led_array[16] = update_time # marker for minutes, just blinks with seconds
            led_array[26] = update_time # marker for hours, just blinks with seconds
            led_array[31] = update_time # marker for hours, just blinks with seconds
            
            if update_time == 0:
                update_time = 1
            
            else:
                update_time = 0
                ##            print "cycle"
                seconds = datetime.now().strftime('%S')
                seconds_first_digit = int(seconds[0])
                seconds_second_digit = int(seconds[1])
                
                minutes = datetime.now().strftime('%M')
                minutes_first_digit = int(minutes[0])
                minutes_second_digit = int(minutes[1])
                
                hours = datetime.now().strftime('%H')
                hours_first_digit = int(hours[0])
                hours_seconds_digit = int(hours[1])
                
             
                temp_counter = seconds_second_digit
                for i in range(4):
                    led_array[i] = 0x01 & temp_counter
                    temp_counter = temp_counter >> 1
                
                temp_counter = seconds_first_digit
                for i in range(4):
                    led_array[i+4] = 0x01 & temp_counter
                    temp_counter = temp_counter >> 1
                
                        
                temp_counter = minutes_second_digit
                for i in range(4):
                    led_array[i+12] = 0x01 & temp_counter
                    temp_counter = temp_counter >> 1
                
                temp_counter = minutes_first_digit
                for i in range(4):
                    led_array[i+17] = 0x01 & temp_counter
                    temp_counter = temp_counter >> 1
                
                
                temp_counter = hours_seconds_digit
                for i in range(4):
                    led_array[i+27] = 0x01 & temp_counter
                    temp_counter = temp_counter >> 1
                
                temp_counter = hours_first_digit
                for i in range(4):
                    led_array[i+32] = 0x01 & temp_counter
                    temp_counter = temp_counter >> 1

            
            
                print hours, minutes, seconds
            SendArray(led_array, MidiOut)


# A simple binary counter display.
def BinaryCounter(MidiOut):
    last_cycle_time = 0
    counter = 0
    led_array = [0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0,0,0,
                 0,0,0,0,0,0,0,0]
    
    while True:
        if pypm.Time() - last_cycle_time > 30:
            ##            print "cycle"
            last_cycle_time = pypm.Time()
            temp_counter = counter
            counter = counter + 1
            for i in range(20):
                led_array[i] = 0x01 & temp_counter
                temp_counter = temp_counter >> 1
                
            SendArray(led_array, MidiOut)



# A Game of Life simulation is usually performed on a 2D matrix, but here
# we apply similar rules to the 1D VMeter array of LEDs.
# Each cycle, a given LED is turned on or off based on how many of its neighbors
# are on or off.
# Different starting configurations will result in different patterns,
# some die off, some enter into a repeating cycle, and others continue to
# evolve.
# Touching the VMeter will cause the LEDs touched to switch states, which can restart
# a simulation that has died off.
def GameOfLife(MidiOut, MidiIn):
    led_array = [1,1,1,1,1,1,1,0,0,0,
                 0,0,0,0,0,0,1,1,1,1,
                 0,1,1,0,0,1,1,1,0,0,
                 0,0,0,1,0,0,0,0]
#    led_array = [1,0,1,1,1,1,1,0,0,0,
#                 0,0,0,0,0,0,1,1,1,1,
#                 0,1,1,0,0,1,1,1,0,0,
#                 0,0,0,1,0,0,1,0]
#    led_array = [1,0,0,0,0,0,0,0,0,0,
#                 0,0,0,0,0,0,1,0,1,0,
#                 0,1,0,0,0,1,0,0,0,0,
#                 0,0,0,1,0,0,0,0]
    last_cycle_time = 0
    i = 0
    while True:
        while MidiIn.Poll(): # invert LEDs where touched
            MidiData = MidiIn.Read(1)
            if MidiData[0][0][0] == 0xB0:
                if MidiData[0][0][1] == 20:
                    pos = MidiData[0][0][2]
                    index_pos = int(float(pos) / 127.0 * 37.0)
#                    print "index pos: ", index_pos
                    if led_array[index_pos] == 1:
                        led_array[index_pos] = 0
                    else:
                        led_array[index_pos] = 1
        if pypm.Time() - last_cycle_time > 100:
            last_cycle_time = pypm.Time()
            index_array = range(2,35)
            new_array = list(led_array)
            # copy over 4 edge LEDs since they don't have 4 neighbors.
            new_array[0] = led_array[0]
            new_array[1] = led_array[1]
            new_array[36] = led_array[36]
            new_array[37] = led_array[37]
            for i in index_array:
                sum =led_array[i-2]+led_array[i-1]+led_array[i+1]+led_array[i+2]
                if led_array[i] == 1: # live cell
                    if sum < 1:
                        new_array[i] = 0 # under population
                    elif sum < 3:
                        new_array[i] = 1 # just right
                    else:
                        new_array[i] = 0 # overcrowding
                else: # dead cell
                    if sum == 2 or sum == 3:
                        new_array[i] = 1
                    else:
                        new_array[i] = 0
                
            led_array = list(new_array)            
            SendArray(led_array, MidiOut)


def adjust_speed(new_speed,speed):
# here average the new_speed with the old speed
    speed = new_speed * .2 + speed * .8
    return speed


# this causes the LEDs to act like a scrolled page on a tablet.
# Simulated acceleration provides a smooth start and stop effect.
def ChaseDemoWithSpeedInput(MidiOut, MidiIn):
    x = 1
    speed = 500
    last_time = 0
    last_speed_calc_time = 0
    prev_pos = 0
    pos = 0
    prev_last_input_time = 0
    last_input_time = 0
    speed = 0.0
    new_speed = 0.0
    pos_array = [0, 0, 0, 0, 0]
    pos_array = deque(pos_array)
    time_array = deque([0, 0, 0, 0, 0])
    print_time = 0
    led_shift_time = 0
    touch_state = 0
    brake_time = 0
    
    led_deque = deque([1,1,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,1,1,
                       0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0])
    SendArray(led_deque, MidiOut)
    EnableOnOffOutput(MidiOut)
    
    while True:
        while MidiIn.Poll(): # throw out all but the latest input
            MidiData = MidiIn.Read(1)            
            if MidiData[0][0][0] == 0xB0:
                if MidiData[0][0][1] == 20:
                    pos = MidiData[0][0][2]
                    pos_array.appendleft(pos)
#                    pos_array.pop()
                    last_input_time = MidiData[0][1]
                    time_array.appendleft(last_input_time)
#                    time_array.pop()
#                    print(last_input_time)
                elif MidiData[0][0][1] == 17: # on / off output. 127 is touch, 0 is release
                    if MidiData[0][0][2] == 127:
#                        print "touch"
                        touch_state = 1
                    else:
#                        print "release"
                        touch_state = 0
    
        if last_input_time > last_speed_calc_time: # calc speed
            last_speed_calc_time = pypm.Time()
            pos_delta = pos_array[0] - pos_array[4]
            time_delta = time_array[0] - time_array[4]
            if time_delta > 0:
                new_speed = float(pos_delta) / float(time_delta)

            speed = adjust_speed(new_speed,speed)


        # handle case where VMeter is being touched, but position isn't moving
        if touch_state == 1 and pypm.Time() - last_input_time > 100:
            # reduce speed to 0
            if pypm.Time() - brake_time > 17:
                brake_time = pypm.Time()
#                print "braking"
                speed = adjust_speed(0.0,speed)

        if pypm.Time() - print_time > 150:
            print_time = pypm.Time()
#            if abs(speed) > .01:
#                print "speed: ", speed, ", per: ", 1.0 / speed
            if pypm.Time() - last_input_time > 100:
                # friction braking
                speed = adjust_speed(0.0,speed)

                    
        if abs(speed) > .001 and pypm.Time() - led_shift_time > int(2.5/abs(speed)):
            led_shift_time = pypm.Time()
            if speed > 0.0:
                led_deque.rotate(1)
            else:
                led_deque.rotate(-1)
            SendArray(led_deque, MidiOut)


# uses the rotate method of a deque to cause the LEDs to chase and wrap around.
def ChaseDemo2():
#    led_array = [1,0,1,0,1,0,1,0,1,0,
#                 1,0,1,0,1,0,1,0,1,0,
#                 1,0,1,0,1,0,1,0,1,0,
#                 1,0,1,0,1,0,1,0]
#    led_array_deque = deque(led_array)
    print(led_array_deque)
    SendArray(led_array_deque, MidiOut)
    timer_marker = pypm.Time()
    while True:
        timer_marker = pypm.Time()
        while pypm.Time() < timer_marker + 500:
            pass
    
        SendArray(led_array_deque, MidiOut)
        led_array_deque.rotate(1)


# another LED chasing demo utilizing bit shifting through 2x multiplication
def ChaseDemo(MidiOut):
    x = 1;
    while True:
        MidiTime = pypm.Time()
        MidiOut.WriteShort(0xAD,x,x)
        MidiOut.WriteShort(0xAE,x,x)
        MidiOut.WriteShort(0xAF,x,x)
        x = x * 2;
        if x == 128:
            x = 1;
        while pypm.Time() < MidiTime + 100:
            pass


# Draws a single LED cursor under the finder position.
def DrawCursor(MidiOut,height): # uses global led_deque
    # clear the deque - set all LEDs to off
    for i in range(38):
        led_array_deque[i] = 0
    cursor_pos = int(float(height) / 127.0 * 38.0)
    if cursor_pos > 37:
        cursor_pos = 37
    led_array_deque[cursor_pos] = 1 # turn on one LED
    SendArray(led_array_deque, MidiOut)


# draws a bar centered at height position with a given size.
# Kinda like a fat cursor.
def DrawBar(MidiOut,height,size):
    # clear the deque - set all LEDs to off
    for i in range(38):
        led_array_deque[i] = 0
    cursor_pos = int(float(height) / 127.0 * 37.0)
    lower_limit = cursor_pos - size / 2
    if lower_limit < 0:
        lower_limit = 0
    upper_limit = cursor_pos + size / 2
    if upper_limit > 37:
        upper_limit = 37
    i = lower_limit
    while i <= upper_limit:
        led_array_deque[i] = 1
        i = i + 1
    SendArray(led_array_deque, MidiOut)

# this draws a bar where touched instead of a cursor or column.
def DrawBarDemo(size):
    # size == 0 --> pressure adj
    bar_size = 1
    input_pos = 64
    while True:
        if MidiIn.Poll():
            MidiData = MidiIn.Read(1)
            #print  MidiData[0][0][0]," ",MidiData[0][0][1]," ",MidiData[0][0][2]
            if MidiData[0][0][0] == 0xB0:
                if MidiData[0][0][1] == 20:
                    input_pos = MidiData[0][0][2]
                #                SendColumn(MidiOut,input_pos)
                #                DrawCursor(MidiOut,input_pos)
                if size == 0:
                    if MidiData[0][0][1] == 18:
                        bar_size = MidiData[0][0][2] / 6
                else:
                    bar_size = size

                DrawBar(MidiOut,input_pos,bar_size)

# main code begins here
pypm.Initialize() # always call this first, or OS may crash when you try to open a stream

PrintDevices(OUTPUT)
dev = int(raw_input("Type output number: "))
MidiOut = pypm.Output(dev, 0)
PrintDevices(INPUT)
dev = int(raw_input("Type input number: "))
MidiIn = pypm.Input(dev)
# turn off internal LED finger tracking
SetLEDsIgnoreTouch(MidiOut)
# set initial column
SendColumn(MidiOut,45)
# turn on pressure output
EnablePressureOutput(MidiOut)

demo_choice = int(raw_input("""
Choose a demo:
1) Cursor tracks finger position
2) Cursor size adjusts based on pressure
3) Monitor CPU level
4) Binary Counter
5) Binary Clock
6) Chase
7) Scrollable treadmill
8) Game of Life
"""))

if demo_choice == 1:
    DrawBarDemo(1)
elif demo_choice == 2:
    DrawBarDemo(0) # input 0 to adjust cursor size with pressure
elif demo_choice == 3:
    MonitorCpu(MidiOut)
elif demo_choice == 4:
    BinaryCounter(MidiOut)
elif demo_choice == 5:
    BinaryClock(MidiOut)
elif demo_choice == 6:
    ChaseDemo(MidiOut)
elif demo_choice == 7:
    ChaseDemoWithSpeedInput(MidiOut,MidiIn)
elif demo_choice == 8:
    GameOfLife(MidiOut, MidiIn)
# be sure to try out different starting patterns



dummy = raw_input("ready to close and terminate... (type RETURN):")
del MidiOut
del MidiIn
pypm.Terminate()
