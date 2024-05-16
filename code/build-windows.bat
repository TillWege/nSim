mkdir build
cd build
cmake ../
cmake --build .
cd ..
copy .\build\Debug\nSim.exe .\nSim.exe
pause