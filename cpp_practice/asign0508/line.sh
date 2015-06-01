gcv() {
    g++ -Wextra $@ $(pkg-config --cflags --libs opencv)
  }
gcv line.cpp -o line
./line
