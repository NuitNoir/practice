gcv() {
g++   $@ $(pkg-config --cflags --libs opencv)
}
gcv kaczmarz.cpp -o kaczmarz
./kaczmarz




