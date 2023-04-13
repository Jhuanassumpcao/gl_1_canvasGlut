gcc -c -Iinclude src/main.cpp
gcc -c -Iinclude src/gl_canvas2d.cpp
gcc -o teste.exe main.o gl_canvas2d.o -m32 -Iinclude -Llib -lopengl32 -lfreeglut

./teste.exe