# MPT project

## Prerequisites

In order to build the software it is required to have cmake build system and a C++ compiler that supports C++17. The application supports Linux OS only.

## Getting started

After cloning the repository it is recommended to launch a terminal inside and execute the following instructions:

    mkdir build
    cd build
    cmake ..
    make

As a result, `mpt_project` executable is generated. Launching this application starts the underlying HTTP server. It is possible now to access it locally with a web browser by accessing `http://localhost:3490`. In order to terminate the application you use `Ctrl+C`.

