cmake_minimum_required(VERSION 3.13)
project(ZombieDash)

set(CMAKE_CXX_STANDARD 14)

include_directories(ZombieDash)
include_directories(ZombieDash/irrKlang)

add_executable(ZombieDash
        ZombieDash/irrKlang/ik_ESoundEngineOptions.h
        ZombieDash/irrKlang/ik_ESoundOutputDrivers.h
        ZombieDash/irrKlang/ik_EStreamModes.h
        ZombieDash/irrKlang/ik_IAudioRecorder.h
        ZombieDash/irrKlang/ik_IAudioStream.h
        ZombieDash/irrKlang/ik_IAudioStreamLoader.h
        ZombieDash/irrKlang/ik_IFileFactory.h
        ZombieDash/irrKlang/ik_IFileReader.h
        ZombieDash/irrKlang/ik_IRefCounted.h
        ZombieDash/irrKlang/ik_irrKlangTypes.h
        ZombieDash/irrKlang/ik_ISound.h
        ZombieDash/irrKlang/ik_ISoundDeviceList.h
        ZombieDash/irrKlang/ik_ISoundEffectControl.h
        ZombieDash/irrKlang/ik_ISoundEngine.h
        ZombieDash/irrKlang/ik_ISoundMixedOutputReceiver.h
        ZombieDash/irrKlang/ik_ISoundSource.h
        ZombieDash/irrKlang/ik_ISoundStopEventReceiver.h
        ZombieDash/irrKlang/ik_IVirtualRefCounted.h
        ZombieDash/irrKlang/ik_SAudioStreamFormat.h
        ZombieDash/irrKlang/ik_vec3d.h
        ZombieDash/irrKlang/irrKlang.h
        ZombieDash/Actor.cpp
        ZombieDash/Actor.h
        ZombieDash/freeglut.h
        ZombieDash/freeglut_ext.h
        ZombieDash/freeglut_std.h
        ZombieDash/GameConstants.h
        ZombieDash/GameController.cpp
        ZombieDash/GameController.h
        ZombieDash/GameWorld.cpp
        ZombieDash/GameWorld.h
        ZombieDash/GraphObject.h
        ZombieDash/Level.h
        ZombieDash/main.cpp
        ZombieDash/SoundFX.h
        ZombieDash/SpriteManager.h
        ZombieDash/StudentWorld.cpp
        ZombieDash/StudentWorld.h)
