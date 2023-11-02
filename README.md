# PurdueFall2023ECE477Team2
Code and other files for a desktop Skee-Ball Machine. Our team website can be
found at [https://engineering.purdue.edu/477grp2/](https://engineering.purdue.edu/477grp2/).

# Purdue ECE47700 - Digital Systems Senior Project
This class involves designing a electronics project that could be sold as a 
product. Such a product must involve either a microcontroller or FPGA. More
information can be found at [https://engineering.purdue.edu/ece477/](https://engineering.purdue.edu/ece477/).

# Git Folders and Files
The files in our Git Repository can be broken into two sections: electrical and
code. Electrical design was done in KiCAD 7.0 and includes both schematics and
PCB layouts. Code was done using SystemWorkbench. There are also some other 
code and support files. Bellow is a description of each folder and what it
contains along with some soport files files.

## General Files
- Used Resources.txt: This contains what parts of the microcontroller are being
                      used for what.

## Electrical
- Audio AMP: Contains initial design for our audio amplifier.
- Schematic: Contains the overall schematic and PCB for our design.
- power control: Contains the initial design for our power circuitry.
- sensor: Contains the initial design for our laser break circuit.

## Microcontroller Code
- HUB75Lib: This contains a library and test code to run a 32x64 HUB75 display
            including a font library.
- WS2812BLib: This contains a library and test code for running a WS2812B LED strip.
- SoundLib: This contains a library and test code for generating and playing audio.

## Other Code
- HUB75Font\_Generate: This contains code for generating a font library for the
                       HUB75Lib.



# Project Description
Our project aims to rekindle the nostalgic joy of the traditional Skee-Ball game while
incorporating cutting-edge technology for an enhanced playing experience in the form of
game modes, normal, time attack, and streak. The normal game mode is the same 
as the traditional skate-ball game mode, such that a player would get nine balls and attempt
to score the highest score possible at their own pace, for which the scores per ball are
simply added to get the final score. After they run out of nine balls, any ball they try
to score after would not count, as the game would be over. In time attack mode, the
player sets a time (up to 5 minutes) with an unlimited ball count to score as high as
possible. For the streak mode, the player gets an "unlimited" number of balls as long as
as they score 20 points or higher on the current shot; Once they miss the shot, the score
is finalized. Finally, the timed streak mode combines the time attack with a score.
multiplier, which will go up by 0.1x from 1x every time they make the shot, maxing out at
1.5x and indicated by an LED light bar to the player. Once the player misses a 20+
point shot, the multiplier is reset. The Skee-Ball Machine" is similar to other arcade
machines that you can easily see in arcades; however, they are unaffordable to buy and
are huge in size. Therefore, we specifically designed it to be portable and affordable. 
To make it portable, we replaced the rolling rail into bouncing pad, so the player will 
score the ball by throwing the ball off of the bouncing pad. This device will use a microcontroller,
along with several sensors, buttons, displays, and a speaker to make an iterative game with
different scoring modes to keep the game fresh. In detail, sensors will detect the ball
once they are scored. Also, there will be a speaker, so it will have certain game effects.
sounds when scored through a user-defined amplifier. Also, there will be a display for user 
interfaces to show the score and how much time is left. Finally, we will create a power regulator
to regulate the power supply. We will use a commercial 12V power supply because our speaker 
requires 12V, then we will implement a voltage regulator (buck converter) from 12V commercial 
voltage input to 5V to supply our display, and another linear regulator from 5V to 3.3V to 
supply microprocessors and other components.

## Stretch Goal:
An ability to implement a WS2812B LED-strip to generate some flashing light as a game effect, 
using GPIO pins to operate and a PWM to adjust brightness and frequency to generate certain effect.

## Project Specific Design Requirements (PSDRs):
- PSDR #1 (Hardware): An ability to implement one 32-by-64 column-row
                      matrix display to interface with user, by communicating through GPIO
                      pins from microcontroller. 
- PSDR #2 (Hardware): An ability to implement one 4-by-4 column-row
                      matrix keypad to receive user's input, by communicating through GPIO
                      pins from microcontroller.
- PSDR #3 (Hardware): An ability to generate a soundwave using DAC 
                      from microcontroller and amplify the signal using a user designed
                      audio amplifier to play game effect sounds through a speaker.
- PSDR #4 (Hardware): An ability to implement a step down voltage
                      regulator(buck converter) from 12V commercial voltage input to 5V, and
                      another linear regulator from 5V to 3.3V as we need both 5V and 3.3V
                      to supply different components.
- PSDR #5 (Software): An ability to implement the game logic using system
                      workbench for STM32(eclipse), to properly implement all components, such
                      as optical sensor, display, keypad, and speaker.
