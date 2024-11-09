cmake_minimum_required(VERSION 3.20)
project(GameAI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("ext/raylib")

# Demo source files and libs to lined against:
include_directories("include")

# rpg_game executable
add_executable(rpg_game 
	source/main.cpp 
	source/Character.cpp 
	source/CharacterBase.cpp
	source/Prop.cpp
	source/Enemy.cpp)
	
target_link_libraries(rpg_game raylib)

add_executable(rpg_game_2 
	source/newMain.cpp
	source/Player.cpp
	source/newEnemy.cpp
	source/a_star.cpp
	source/newCharacterBase.cpp
	source/StateMachine.cpp
	source/ConcreteStates.cpp)
	
target_link_libraries(rpg_game_2 raylib)


# Copy assets from "assets" folder to output binary directory on post build step
add_custom_command(TARGET rpg_game POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/textures
	${PROJECT_BINARY_DIR}/textures
	COMMENT "Copying rpg_game texture files to binary directory")
