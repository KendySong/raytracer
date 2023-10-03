import os

if __name__ == '__main__':
    os.system('premake5 gmake')
    os.system('mingw32-make -j 8 -C build -lSDL2 -lSDL2main')
    os.system('start build/bin/Release/raytracer.exe')
    pass