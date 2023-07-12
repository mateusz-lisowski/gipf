#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define EMPTY '_'
#define WHITE 'W'
#define BLACK 'B'


struct Collision {

    int begin;
    int end;

};

struct Board {

    int white_reserve;
    int black_reserve;

    std::vector<char> board;

};

struct Game {

    char current_player;

    int size;
    int pieces_limit;
    int white_player_pieces;
    int black_player_pieces;

    Board board;

    void create_empty_board() {

        board.board.clear();
        board.board.resize((2 * size + 1) * (2 * size + 1), NULL);

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

        return &board.board[i];
    }

    void load_game_board() {

        scanf("%d %d %d %d\n", &size, &pieces_limit, &white_player_pieces, &black_player_pieces);
        scanf("%d %d %c", &board.white_reserve, &board.black_reserve, &current_player);

        create_empty_board();

        std::string line;
        std::getline(std::cin, line);

        auto it = board.board.begin();
        auto end = board.board.end();

        for (int i = 0; i < size * 2 - 1; i++) {

            int offset = abs(size - i - 1);

            std::getline(std::cin, line);

            if (offset + 2 * (size * 2 - 1 - offset) - 1 != line.size()) {
                std::cout << "WRONG_BOARD_ROW_LENGTH\n";
                board.board.clear();
                return;
            }

            for (int j = 0; j < size * 2 - 1 - offset; j++) {

                char ch = line[offset + 2 * j];

                while (it != end && *it == NULL)
                    it++;

                *it++ = ch;

            }
        }

        int white_count = std::count(board.board.begin(), board.board.end(), WHITE);
        int black_count = std::count(board.board.begin(), board.board.end(), BLACK);

        if (white_count + board.white_reserve > white_player_pieces) {
            std::cout << "WRONG_WHITE_PAWNS_NUMBER\n";
            board.board.clear();
            return;
        }
        if (black_count + board.black_reserve > black_player_pieces) {
            std::cout << "WRONG_BLACK_PAWNS_NUMBER\n";
            board.board.clear();
            return;
        }

        std::cout << "BOARD_STATE_OK\n";
    }

    void print_game_board() {
    
        if (board.board.empty()) {
            
            std::cout << "EMPTY_BOARD\n";
            return;
        }

        std::cout << size << " " << pieces_limit << " " << white_player_pieces << " " << black_player_pieces << "\n";
        std::cout << board.white_reserve << " " << board.black_reserve << " " << current_player << "\n";
        
        auto it = board.board.begin();
        auto end = board.board.end();

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
    
        std::cout << "\n";
    }

    void do_move(const std::string& line) {
        


    }

};


int main() {

    Game game;

    std::string line;
    while (std::getline(std::cin, line)) {

        if (line.rfind("LOAD_GAME_BOARD", 0) == 0) {
            game.load_game_board();
        }
        else if (line.rfind("PRINT_GAME_BOARD", 0) == 0) {
            game.print_game_board();
        }
        else if (line.rfind("DO_MOVE", 0) == 0) {
            game.do_move(std::string(line.begin() + 8, line.end()));
        }

    }
}
