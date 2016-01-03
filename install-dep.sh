#!/bin/bash

os=$(uname)
arq=$(uname -m)

apps_common="pandoc imagemagick "
apps_osx="mactex "
apps_linux_common="texlive-full"
apps_linux_rpi="texlive-full"
apps_linux_ubuntu="texlive-latex-extra"

#   Install Applications
#   ===============================================================
if [ $os == "Linux" ]; then

    # on Debian Linux distributions
    sudo apt-get update
    sudo apt-get upgrade
    sudo apt-get install $apps_common
    sudo apt-get install $apps_linux_common

    # on RaspberryPi
    if [ $arq == "armv7l" ]; then
        sudo apt-get install $apps_linux_rpi
    else
        sudo apt-get install $apps_linux_ubuntu
    fi

elif [ $os == "Darwin" ]; then
    
    # ON MacOX 
    
    if [ ! -e /usr/local/bin/brew ]; then
        ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
    fi

    brew install caskroom/cask/brew-cask
    brew update
    brew upgrade
    brew install $apps_common
    brew install $apps_osx
fi