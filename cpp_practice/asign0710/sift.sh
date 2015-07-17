gcv() {
g++   $@ $(pkg-config --cflags --libs opencv)
}
gcv sift_descriptor.cpp -o sift_descriptor
./sift_descriptor




