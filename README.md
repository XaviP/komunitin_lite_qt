# Komunitin Lite Qt

Basic C++ Qt6 client for komunitin users.


![Screenshot](docs/komunitin_lite_qt.png)



### Install:

Check [Releases](https://github.com/komunitin/komunitin_lite_qt/releases) for binaries.


### Build notes:

- Qt6.2 LTS (with Network, Widgets and StateMachine modules).

- OpenSsl 1.1 is needed to run the app, Qt6 is compiled with this version.
  (If your system uses libssl version 3, it won't run)
  
- Build for linux desktop is well tested.
  You can try to build for other platform/s from QtCreator.
  

### Development:

There are two main branches in this repository:

- main: it works with Qt Widgets

- main_qml: development with Qt Quick QML


### TODO ([see issues](https://github.com/komunitin/komunitin_lite_qt/issues))

- CI to build on several platforms.

- Tests, tests, tests.

- ...


