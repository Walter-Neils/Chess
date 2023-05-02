cmake_minimum_required(VERSION 3.24)
project(Chess)

set(CMAKE_CXX_STANDARD 23)

# Nlohmann JSON library
find_package(nlohmann_json 3.9.1 REQUIRED)

# If it compiles under these flags, it's good enough lol
add_compile_options(-pedantic -Wuninitialized -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wswitch-default -Wundef -Wno-unused)
# Can't have nice things :(
# STL type_traits doesn't have noexcept specifiers, so we have to disable this warning
# add_compile_options(-Wnoexcept)

# If you're feeling brave, uncomment this line to treat warnings as errors
add_compile_options(-Werror)

add_executable(Chess main.cpp IRenderable.cpp IRenderable.h IRenderable.cpp RenderContext.cpp RenderContext.h ChessBoard.cpp ChessBoard.h InputHandler.cpp InputHandler.h InputHandlerEventStream.cpp InputHandlerEventStream.h AssetLoader.cpp AssetLoader.h IBehaviour.cpp IBehaviour.h PieceBehaviour.cpp PieceBehaviour.h ChessPieceInstance.cpp ChessPieceInstance.h Logging.h GameMode.cpp GameMode.h PieceController.cpp PieceController.h CorruptionDetector.h MemoryWatcher.h Configuration.cpp Configuration.h)

# Link SFML libraries
target_link_libraries(Chess sfml-graphics sfml-window sfml-system)
# Link Nlohmann JSON library
target_link_libraries(Chess nlohmann_json::nlohmann_json)

# Delete assets folder in build directory
add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        $<TARGET_FILE_DIR:Chess>/assets)

# Always copy assets folder to build directory
add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Chess>/assets)

# Copy config.json to build directory
add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/config.json $<TARGET_FILE_DIR:Chess>/config.json)