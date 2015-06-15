gcv() {
g++ -Wall -Wextra $@ $(pkg-config --cflags --libs opencv)
}
gcv projection_matrix.cpp -o projection_matrix
./projection_matrix