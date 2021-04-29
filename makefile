CC=g++
STD=-std=c++11 -Wall -pedantic
CF=$(STD)
BUILD_DIR=build
TEMPLATE_DIR=.template

all: $(BUILD_DIR) utunes.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/template_parser.o: utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp
	$(CC) $(CF) -c utils/template_parser.cpp -o $(BUILD_DIR)/template_parser.o

$(BUILD_DIR)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) $(CF) -c utils/response.cpp -o $(BUILD_DIR)/response.o

$(BUILD_DIR)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) $(CF) -c utils/request.cpp -o $(BUILD_DIR)/request.o

$(BUILD_DIR)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) $(CF) -c utils/utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp
	$(CC) $(CF) -c server/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/route.cpp -o $(BUILD_DIR)/route.o

$(BUILD_DIR)/handlers.o: examples/handlers.cpp UTunes/UTunes.hpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c examples/handlers.cpp -o $(BUILD_DIR)/handlers.o

$(BUILD_DIR)/my_server.o: examples/my_server.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c examples/my_server.cpp -o $(BUILD_DIR)/my_server.o

$(BUILD_DIR)/main.o: examples/main.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c examples/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/UTunes.o: UTunes/UTunes.hpp UTunes/UTunes.cpp
	$(CC) $(CF) -c UTunes/UTunes.cpp -o $(BUILD_DIR)/UTunes.o

$(BUILD_DIR)/Matrix.o: UTunes/Matrix.hpp UTunes/Matrix.cpp
	$(CC) $(CF) -c UTunes/Matrix.cpp -o $(BUILD_DIR)/Matrix.o

$(BUILD_DIR)/User.o: UTunes/User.hpp UTunes/User.cpp
	$(CC) $(CF) -c UTunes/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DIR)/Song.o: UTunes/Song.hpp UTunes/Song.cpp
	$(CC) $(CF) -c UTunes/Song.cpp -o $(BUILD_DIR)/Song.o

$(BUILD_DIR)/Comment.o: UTunes/Comment.hpp UTunes/Comment.cpp
	$(CC) $(CF) -c UTunes/Comment.cpp -o $(BUILD_DIR)/Comment.o

$(BUILD_DIR)/Playlist.o: UTunes/Playlist.hpp UTunes/Playlist.cpp
	$(CC) $(CF) -c UTunes/Playlist.cpp -o $(BUILD_DIR)/Playlist.o

utunes.out: $(BUILD_DIR)/Matrix.o $(BUILD_DIR)/Comment.o $(BUILD_DIR)/Song.o $(BUILD_DIR)/Playlist.o $(BUILD_DIR)/User.o $(BUILD_DIR)/UTunes.o $(BUILD_DIR)/my_server.o $(BUILD_DIR)/main.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o
	$(CC) $(CF) $(BUILD_DIR)/Matrix.o $(BUILD_DIR)/Comment.o $(BUILD_DIR)/Song.o $(BUILD_DIR)/Playlist.o $(BUILD_DIR)/User.o $(BUILD_DIR)/UTunes.o $(BUILD_DIR)/my_server.o $(BUILD_DIR)/main.o $(BUILD_DIR)/handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o  -o utunes.out

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out &> /dev/null
