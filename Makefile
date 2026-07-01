# compiler to use
CC = clang

# check if we are on a Windows / Linux system
ifeq ($(OS),Windows_NT)
	# Windows system
  OUTPUT = build/program.exe
  LIBS = -lkernel32 -luser32 -lshell32 -lgdi32
else
	# Linux system
  OUTPUT = build/program
  LIBS = -lX11 -lXi -lXcursor -lGL -lasound -ldl -lm -pthread
endif

# our neccessary includes for 'cglm'
INCLUDES = -Idependencies/cglm/include

# local development ==> compiling, running and deleting
program: compile run clean

# compile the program according to system
compile:
	@$(CC) main.c model.c -Wall -Wextra $(INCLUDES) $(LIBS) -o $(OUTPUT)

# compile the program
run:
	@./$(OUTPUT)

# delete / remove any leftover compiled programs ( based on system )
clean:
	@$(RM) $(OUTPUT)

# to compile our ( triangle ) shader
shader:
	@./sokol-shdc --input assets/shaders/triangle.glsl --output triangle_shader.h --slang glsl430:hlsl5:metal_macos
