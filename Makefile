run:
	g++ main.cpp glad.c stb_image.cpp  io/Screen.cpp graphics/Shader.cpp io/Mouse.cpp io/Keyboard.cpp io/Joystick.cpp io/Camera.cpp -ldl -lglfw
	./a.out
