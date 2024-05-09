# 2D-firework


## Introduction

The project is a 2D firework simulator. The simulator provides users with the function to create a sequence of fireworks by the fireworks’ color, type, position and launch time. After finishing the sequence loading, the simulator will display the animation of the fireworks. 

There are 6 colors, and 7 types of fireworks can be chosen. 



## Usage

There are three files main.cpp, makefile, and fireworks.txt.
1.    To build the program, run command “make”.
2.    To run the program, run command “./firework”.
3.    Fireworks.txt file is used to setup the sequences of fireworks in the program, you can edit it by following the rules below:

      - The first line is the number of fireworks. Please set it to a positive integer number.
      - Each of the following lines is the setting of a firework. Each parameter is separate by comma with no space: position_x,position_y,time,color,type
        - Position_x: float number from -14 to 14.
        - Position_y: float number from -8 to 8.
        - Color: integer number from the list
          - 0: Red
          - 1: Green
          - 2: Blue
          - 3: Yellow
          - 4: Cyan
          - 5: Magenta
        - Type: integer number from the list
          - 0: Peony
          - 1: Ring
          - 2: Willow
          - 3: 2 color Peony
          - 4: 2 color Peony (white inside)
          - 5: Double Ring
          - 6: Palm
      - For example: 0,0,1,1,0 means the firework will explode at (0, 0) after 1 second, and the firework is a green peony.
      - The number of lines before comments should be the same as the number of fireworks.
      - The last part in the file is the command (start with //) provide some instructions.

4.    When running the program, you can choose to set up by loading from file (enter 0) or by input (enter 1), if you choose to set up by input, the rule of input value can refer from above section.  
5.    During the program, press “s” to start the animation, press “r” to restart from set up, press “ESC” to exit the program.

## Result
  please see the video in the following link:
  https://youtu.be/EIdNCZ7_Lf0
