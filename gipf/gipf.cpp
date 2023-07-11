#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>


struct Player {



};


struct Game {

    Player* current_player;

    int size;
    int pieces_limit;
    int white_player_pieces;
    int black_player_pieces;

    std::string state;

    char* board;


    void load_game_board(Game* game, Player* white, Player* black) {

        scanf("%d %d %d %d\n", &game->size, &game->pieces_limit, &game->white_player_pieces, &game->black_player_pieces);

        std::cout << "Game state: " 
            << game->size 
            << ", " 
            << game->pieces_limit 
            << ", " 
            << game->white_player_pieces 
            << ", " 
            << game->black_player_pieces << "\n";
    }

    void print_game_state() {
        std::cout << state;
    }

    void do_move() {
    
    }

};


int main() {

    Game game;

    Player white;
    Player black;

    while (true) {

        std::string line;
        std::getline(std::cin, line);

        if (line.rfind("LOAD_GAME_BOARD", 0) == 0) {
            game.load_game_board(&game, &white, &black);
        }
        else if (line.rfind("DO_MOVE", 0) == 0) {
            game.do_move();
        }
        else if (line.rfind("PRINT_GAME_STATE", 0) == 0) {
            game.print_game_state();
        }
        
        break;
    }

}
