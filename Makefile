# Out
OUT = -o ./out/bin

# Flags
# -Wall = Warn ALl
FLAGS = -Wall

# C++ Standard Version
CPPSTDV = -std=c++2a

# File(s)
FILES = ./source/main.cpp

# Default / Main
default: clear compile run

# Builds executable
compile:
	g++ $(FILES) $(OUT) $(CPPSTDV) $(FLAGS)

# Runs the outputed file
run: clear
	./out/bin

# Clears The Terminal
clear:
	clear
