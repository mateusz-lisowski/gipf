#include <iostream>


struct Player {



};


struct Game {

    Player* current_player;

    int size;
    int pieces_limit;
    int white_player_pieces;
    int black_player_pieces;

    Game() {
        scan_config(this);
    }

    void scan_config(Game* game) {
        
    
    }

};


int main() {

    Game game;

    game.scan_config()

    Player white;
    Player black;

}
