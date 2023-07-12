#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define EMPTY '_'


struct Player {

    char sign;
    int pieces_number;

};


struct Game {

    Player* current_player;
    Player white;
    Player black;

    int size;
    int pieces_limit;
    int white_player_pieces;
    int black_player_pieces;

    std::string state;

    std::vector<char> board;

    void create_empty_board() {

        board.clear();
        board.resize((2 * size + 1) * (2 * size + 1), NULL);

        for (int i = 1; i < 2 * size; i++) {

            int offset = abs(size - i);
            int max_y = 2 * size - 1 - offset;

            for (int j = 1; j <= max_y; j++) {
                *calculate_index(i, j) = EMPTY;
            }
        }
    }

    char* calculate_index(int x, int y) {
        
        int i;

        if (x <= size) {
            i = (2 * size + 1) * y + x;
        }
        else
        {
            i = (2 * size + 1) * (y + x - size) + x;
        }

        return &board[i];
    }

    void load_game_board() {

        char s_current_player;

        scanf("%d %d %d %d\n", &size, &pieces_limit, &white_player_pieces, &black_player_pieces);
        scanf("%d %d %c", &white.pieces_number, &black.pieces_number, &s_current_player);

        if (s_current_player == white.sign) {
            current_player = &white;
        }
        else
        {
            current_player = &black;
        }

        create_empty_board();

        std::string line;
        std::getline(std::cin, line);

        auto it = board.begin();
        auto end = board.end();

        for (int i = 0; i < size * 2 - 1; i++) {

            int offset = abs(size - i - 1);

            std::getline(std::cin, line);

            if (offset + 2 * (size * 2 - 1 - offset) - 1 != line.size()) {
                std::cout << "WRONG_BOARD_ROW_LENGTH\n";
                return;
            }

            for (int j = 0; j < size * 2 - 1 - offset; j++) {

                char ch = line[offset + 2 * j];

                while (it != end && *it == NULL)
                    it++;

                *it++ = ch;

            }
        }

        int white_count = std::count(board.begin(), board.end(), white.sign);
        int black_count = std::count(board.begin(), board.end(), black.sign);

        if (white_count + white.pieces_number > white_player_pieces) {
            std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
            return;
        }
        if (black_count + black.pieces_number > black_player_pieces) {
            std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
            return;
        }

        std::cout << "BOARD_STATE_OK\n";
    }

    void print_game_board() {
    
        std::cout << size << " " << pieces_limit << " " << white_player_pieces << " " << black_player_pieces << "\n";
        std::cout << white.pieces_number << " " << black.pieces_number << " " << current_player->sign << "\n";
        
        auto it = board.begin();
        auto end = board.end();

        for (int i = 0; i < size * 2 - 1; i++) {

            int offset = abs(size - i - 1);
            std::cout << std::string(offset, ' ');

            for (int j = 0; j < size * 2 - 1 - offset; j++) {

                while (it != end && *it == NULL)
                    it++;

                char ch = *it++;

                std::cout << ch << " ";
            }

            std::cout << "\n";
        }
    
    }

    void print_game_state() {
        std::cout << state;
    }

    void do_move() {
    
    }

};


int main() {

    Game game;

    game.white.sign = 'W';
    game.black.sign = 'B';

    std::string line;
    while (std::getline(std::cin, line)) {

        if (line.rfind("LOAD_GAME_BOARD", 0) == 0) {
            game.load_game_board();
        }
        else if (line.rfind("PRINT_GAME_BOARD", 0) == 0) {
            game.print_game_board();
        }
        else if (line.rfind("DO_MOVE", 0) == 0) {
            game.do_move();
        }
        else if (line.rfind("PRINT_GAME_STATE", 0) == 0) {
            game.print_game_state();
        }

    }
}
