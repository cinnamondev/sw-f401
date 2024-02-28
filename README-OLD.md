# sw-f401

## Quadrature encoder library

USE PA6,7 PB6,7 for TIM3 TIM4  respectively for quadrature.

Quadrature component will use STM32 peripheral directly!

PA6,7 PB6,7 shouldnt interfere with pins



## Guide: Changing to a shared instance.

By default, we use a local instance of mbed os initialized via git. This can be done via the git submodules feature or via mbed studio ('fix problems' in library tab).
1. `git submodule init` - Clones submodules according to the `.gitmodules` file
2. `git submodule update` - Fetches all data and checks out the relevant commit.

Alternatively, you can used a share instance. (highly reccomended, especially if you have many instances of Mbed OS, which if you are using this repo often, you will do so)

I would put your shared instances somewhere easily accessible. For example, on Windows, you might want to put a folder structure in the C:\ directory, but you can do it wherever as long as its accessible.

```
cd C:\
mkdir MBED
mkdir .\MBED\6.17
cd .\MBED\6.17
git clone https://github.com/ARMmbed/mbed-os.git
cd .\mbed-os\
git checkout 17dc3dc2e6e2817a8bd3df62f38583319f0e4fed
```

You could similarly do this with other editions of Mbed os, such as older/newer versions. The specific git commit hash we used here correlates to the release of 6.17. You should also be able to use the CE edition of Mbed if you are so inclined.

![shared instance button](./.attachments/shared-tutorial-0.png)
![shared instance selector](./.attachments/shared-tutorial-1.png)
![shared instance file sel](.attachments/shared-tutorial-2.png)
![final](./.attachments/shared-tutorial-3.png)

## Doxygen

Install doxygen either through its self-installer or via Chocolately.
GraphViz is also required. Make sure it is added to the PATH on windows when
installing it.

## other ideas

Visual studio code is certainly possible but troublesome. neither clangd nor intellisense seem to do a good enough job of handling mbed.

Never use keil studio. Only use mbed studio. Keil Studio will make debugging annoying. Install mbed studio with the st-link drivers and just use standard debug tools
