
gcv() {
g++   $@ $(pkg-config --cflags --libs opencv)
}
gcv fundamental_matrix.cpp -o fundamental_matrix
./fundamental_matrix
