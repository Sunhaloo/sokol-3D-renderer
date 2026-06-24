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

# local development ==> compiling, running and deleting
program: compile run clean

# compile the program according to system
compile:
	@$(CC) main.c -Wall -Wextra $(LIBS) -o $(OUTPUT)

# compile the program
run:
	@./$(OUTPUT)

# delete / remove any leftover compiled programs ( based on system )
clean:
	@$(RM) $(OUTPUT)
