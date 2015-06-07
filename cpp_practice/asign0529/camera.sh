
gcv() {
g++ -Wall -Wextra $@ $(pkg-config --cflags --libs opencv)
}
gcv camera.cpp -o camera
./camera
# make
./camera

convert -delay 10 -loop 0 img/camera*.png camera.gif
