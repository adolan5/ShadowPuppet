# ShadowPuppet
###### An asymmetric platformer using Kinect technology
This repository was for the term project in the spring term of cs370, Spring 2017.

## Description
ShadowPuppet is an asymmetric platformer in which one player controls a character who must traverse a level, while another player creates the platforms for the character to jump along, via an Xbox Kinect and 'edge detection'.

## Technical Dependencies

ShadowPuppet was developed and tested on/for Ubuntu 16.04 LTS.

###### Third-party prerequisites:
- SDL2
- SDL2-image (Note: Be sure to use **SDL2**-image as opposed to SDL-image!)
- OpenGL (native on most systems; v2.1)
- libfreenect

## Installation and Usage

```sh
# install dependencies
$ sudo apt-get install libsdl2-dev
$ sudo apt-get install libsdl2-image-dev
$ sudo apt-get install libfreenect-dev

# clone repo
$ git clone https://github.com/adolan5/shadowpuppet.git
$ cd shadowpuppet

# create 'obj' directory
$ mkdir obj

# compile
$ make

# run game
$ ./Game
```

## Troubleshooting
###### Game crashes on start; 'NO KINECT':
Make sure your Kinect is plugged in!
###### "Fatal error: can't create obj/main.o: No such file or directory":
Make sure that you have created the obj directory in the **root** directory of the repository
```sh
# current directory: shadowpuppet
$ mkdir obj
```
