
gcv() {
g++ -Wall -Wextra $@ $(pkg-config --cflags --libs opencv)
}
gcv homo_graphy.cpp -o calib
./calib
