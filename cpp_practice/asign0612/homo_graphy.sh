
gcv() {
g++ -Wall -Wextra $@ $(pkg-config --cflags --libs opencv)
}
gcv homo_graphy.cpp -o homo_graphy
./homo_graphy
