AUTHOR: "djames1"  
GITHUB: https://github.com/djames1 #If it exists  

# Old Time Desktop Environment (OTDE)  

A fork of KDE1 and QT1 for modern Linux distributions.  

This is the meta repository, which contains links to all of the other repositories. Please checkout all of the submodules in order to compile the project. The respective compile instructions for each package are located in each submodule folder.  

The goals of this project are as follows:  
* Maintain versions of QT1 and all of the KDE1 applications that will compile and run on modern Linux/BSD/Solaris systems.
* Fix all compiler warnings in the code when using both GCC and Clang, as warnings should always be treated as errors.
* Fix any bugginess when using KDE1 on modern Linux distros.
* Better ALSA/PulseAudio integration.
* Make KDE1 installable alongside any other version of KDE/TDE.
* Support as many architectures as possible. I use a variety of systems including x86, ARM, and PowerPC systems.
* Remove the legacy autotools build system. Everything should be built with cmake.
* Support for running KDE1 directly from modern display managers

Potential nice to have goals in the future:  
* Musl support
* Support every BSD OS
* Find out if QT1 is still proprietary licensed or if it's been changed to a GPL like license.
* Port to a newer C++ standard such as C++ 2017 or C++2x
* Add new features for things such as modern fonts, tiling keyboard shortcuts, etc.

This project was inspired by the following post:  
http://www.heliocastro.info/?p=291  

To clone all the necessary branches, use this Shell snippet:  
```sh
git clone http://github.com/Arquivotheca/otde && \
  cd otde
  for i in $(git branch -a | grep remotes | grep -v HEAD | grep -v master); do
    b=$(basename $i)
    git clone -b $i --single-branch http://github.com/Arquivotheca/otde $i
  done
```
