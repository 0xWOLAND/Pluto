run:
	g++ main.cpp glad.c stb_image.cpp Shader.cpp io/Mouse.cpp io/Keyboard.cpp io/Joystick.cpp -ldl -lglfw
	./a.out
