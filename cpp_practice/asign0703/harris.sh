gcv() {
g++   $@ $(pkg-config --cflags --libs opencv)
}
gcv harris_corner.cpp -o harris_corner
./harris_corner


