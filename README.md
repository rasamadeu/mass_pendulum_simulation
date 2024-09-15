# Simulation of two strings attached to a pendulum

This was one of my first programming projects. It is a simulation of a physical system containing two springs attached to a mass, which is itself holding a pendulum. The project is written in C, and the GUI was built using the GTK3+ C library.

## Prerequisites

To build the project, you need to install a ***C compiler*** and the ***GTK3+ library***. There are plenty of tutorials regarding the installation of a C compiler. For the GTK3+ library, follow the instructions in [the GTK project](https://www.gtk.org/docs/installations/).

> [!IMPORTANT]
> The current stable GTK library is 4.0. However, <span style="background-color: red;">GTK4 lacks backwards compatibility in major updates</span>. To ensure that the project builds, I advise the installation of a GTK 3.x library.

## Run

To build the project, type `make` in a terminal.
To run, just type `./projecto` in the terminal and the GUI will open up!
