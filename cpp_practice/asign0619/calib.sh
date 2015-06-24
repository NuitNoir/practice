
gcv() {
g++ -Wall -Wextra $@ $(pkg-config --cflags --libs opencv)
}
gcv calib_zhang.cpp -o calib_zhang
./calib_zhang
