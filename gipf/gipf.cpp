#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define EMPTY '_'
#define WHITE 'W'
#define BLACK 'B'

#define DIRECTION_HOR 0
#define DIRECTION_VERT 1
#define DIRECTION_DIAGONAL 2


struct Collision {

    char* begin;
    char* end;
    int direction;
};

struct Move {

    int x;
    int y;
    int direction;
    bool positive;

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
    std::vector<Move> moves;


    void create_empty_board() {

        board.board.clear();
        board.board.resize((2 * size + 1) * (2 * size + 1), NULL);

        for (int i = 1; i < 2 * size; i++) {

            int offset = abs(size - i);
            int max_y = 2 * size - 1 - offset;

            for (int j = 1; j <= max_y; j++) {
                *get_char_at_index(board, i, j) = EMPTY;
            }
        }

        generate_all_moves();
    }

    int calculate_direction(int dir) {

        if (dir == DIRECTION_HOR) {
            return 1;
        }
        else if (dir == DIRECTION_VERT) {
            return 2 * size + 1;
        }
        else {
            return 2 * size + 2;
        }
    }

    char* get_char_at_index(Board& b, int x, int y) {
        
        int i;

        if (x > 2 * size) {
            return nullptr;
        }

        if (y > 2 * size - abs(size - x)) {
            return nullptr;
        }

        if (x <= size) {
            i = (2 * size + 1) * y + x;
        }
        else
        {
            i = (2 * size + 1) * (y + x - size) + x;
        }

        return &b.board[i];
    }

    void load_game_board() {

        scanf("%d %d %d %d\n", &size, &pieces_limit, &white_player_pieces, &black_player_pieces);
        scanf("%d %d %c", &board.white_reserve, &board.black_reserve, &current_player);

        create_empty_board();

        int dir_dig = calculate_direction(DIRECTION_DIAGONAL);

        std::string line;
        std::getline(std::cin, line);

        for (int i = 0; i < size * 2 - 1; i++) {

            int x, y;

            if (i < size) {
                x = 1;
                y = size - i;
            }
            else
            {
                x = i - size + 2;
                y = 1;
            }

            int offset = abs(size - i - 1);

            std::getline(std::cin, line);
            while (line.back() == ' ')
                line.pop_back();

            if (offset + 2 * (size * 2 - 1 - offset) - 1 != line.size()) {
                std::cout << "WRONG_BOARD_ROW_LENGTH\n";
                board.board.clear();
                return;
            }

            char* board_ch = get_char_at_index(board, x, y);

            for (int j = 0; j < size * 2 - 1 - offset; j++) {

                char ch = line[offset + 2 * j];

                *board_ch = ch;
                board_ch += dir_dig;

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

        auto collisions = generate_all_collisions(board);
        if (!collisions.empty()) {

            if (collisions.size() > 1) {
                std::cout << "ERROR_FOUND_" << collisions.size() << "_ROWS_OF_LENGTH_K\n";
                board.board.clear();
                return;
            }
            else {
                std::cout << "ERROR_FOUND_" << collisions.size() << "_ROW_OF_LENGTH_K\n";
                board.board.clear();
                return;
            }
            
        }

        std::cout << "BOARD_STATE_OK\n";
    }

    void print_game_board() {
    
        int dir_dig = calculate_direction(DIRECTION_DIAGONAL);

        if (board.board.empty()) {
            
            std::cout << "EMPTY_BOARD\n";
            return;
        }

        std::cout << size << " " << pieces_limit << " " << white_player_pieces << " " << black_player_pieces << "\n";
        std::cout << board.white_reserve << " " << board.black_reserve << " " << current_player << "\n";
        
        auto it = board.board.begin();
        auto end = board.board.end();

        for (int i = 0; i < size * 2 - 1; i++) {

            int x, y;

            if (i < size) {
                x = 1;
                y = size - i;
            }
            else
            {
                x = i - size + 2;
                y = 1;
            }

            int offset = abs(size - i - 1);
            std::cout << std::string(offset, ' ');

            char* board_ch = get_char_at_index(board, x, y);

            for (int j = 0; j < size * 2 - 1 - offset; j++) {

                char ch = *board_ch;
                board_ch += dir_dig;

                std::cout << ch << " ";
            }
                        
            std::cout << "\n";
        }
    
        std::cout << "\n";
    }

    void advance_turn(Board& b) {

        if (current_player == WHITE) {
            current_player = BLACK;
            b.white_reserve--;
        }

        else if (current_player == BLACK) {
            current_player = WHITE;
            b.black_reserve--;
        }
    }

    void generate_all_moves() {
        
        moves.clear();

        int dir_hor = calculate_direction(DIRECTION_HOR);
        int dir_ver = calculate_direction(DIRECTION_VERT);
        int dir_dig = calculate_direction(DIRECTION_DIAGONAL);

        for (int i = 1; i < 2 * size; i++) {
            for (int j = 1; j < 2 * size; j++) {

                char* ch = get_char_at_index(board,i, j);

                if (ch == nullptr || *ch == NULL) {
                    continue;
                }

                if (*(ch + dir_hor) == NULL) moves.push_back(Move{ i, j, -dir_hor, false });
                if (*(ch - dir_hor) == NULL) moves.push_back(Move{ i, j, +dir_hor, true });

                if (*(ch + dir_ver) == NULL) moves.push_back(Move{ i, j, -dir_ver, false });
                if (*(ch - dir_ver) == NULL) moves.push_back(Move{ i, j, +dir_ver, true });

                if (*(ch + dir_dig) == NULL) moves.push_back(Move{ i, j, -dir_dig, false });
                if (*(ch - dir_dig) == NULL) moves.push_back(Move{ i, j, +dir_dig, true });
            }
        }

    
    }

    bool move(char* from, int direction) {
        
        char token = current_player;

        while (true) {
            
            if (*from == NULL) {
                return false;
            }

            char curr = *from;
            *from = token;

            if (curr == EMPTY) {
                return true;
            }
            
            token = curr;
            from += direction;
        }

    }

    std::vector<Collision> generate_all_collisions(Board& b) {
        
        std::vector<Collision> collisions;

        for (auto& move : moves) {

            if (!move.positive) {
                continue;
            }

            char* ch = get_char_at_index(b, move.x, move.y);

            char token = *ch;
            int count = 1;

            while (*ch != NULL) {
                ch += move.direction;
                char curr = *ch;

                if (curr == token) {
                    count++;
                }
                else
                {
                    if (count >= pieces_limit && token != EMPTY) {

                        char* from = ch - count * move.direction;
                        char* to = ch - move.direction;

                        collisions.push_back(Collision{ from, to, move.direction });
                    }

                    token = curr;
                    count = 1;
                }
            }

        }

        return collisions;
    }

    void remove_collision(Board& b, Collision c) {
        
        char* ch = c.begin;
        char token = *ch;

        while (*ch != EMPTY && *ch != NULL) {
            ch -= c.direction;
        }

        ch += c.direction;

        while (*ch != EMPTY && *ch != NULL) {
            
            char curr = *ch;

            if (curr == token) {
                
                if (token == WHITE) {
                    b.white_reserve++;
                }
                else if (token == BLACK) {
                    b.black_reserve++;
                }

            }
               
            *ch = EMPTY;

            ch += c.direction;
        }
    }

    void do_move(const std::string& line) {
        
        int from_x = line[0] - 'a';
        int from_y = line[1] - '1';

        int to_x = line[3] - 'a';
        int to_y = line[4] - '1';

        char* from = get_char_at_index(board, from_x, from_y);
        char* to = get_char_at_index(board, to_x, to_y);

        if (from == nullptr) {
            std::cout << "BAD_MOVE_" << std::string(line.begin() + 0, line.begin() + 2) << "_IS_WRONG_INDEX\n";
            return;
        }
        if (to == nullptr) {
            std::cout << "BAD_MOVE_" << std::string(line.begin() + 3, line.begin() + 5) << "_IS_WRONG_INDEX\n";
            return;
        }

        if (*from != NULL) {
            std::cout << "BAD_MOVE_" << std::string(line.begin() + 0, line.begin() + 2) << "_IS_WRONG_STARTING_FIELD\n";
            return;
        }
        if (*to == NULL) {
            std::cout << "BAD_MOVE_" << std::string(line.begin() + 3, line.begin() + 5) << "_IS_WRONG_DESTINATION_FIELD\n";
            return;
        }

        int direction = to - from;

        int dir_hor = calculate_direction(DIRECTION_HOR);
        int dir_ver = calculate_direction(DIRECTION_VERT);
        int dir_dig = calculate_direction(DIRECTION_DIAGONAL);

        if (direction != dir_hor && direction != -dir_hor &&
            direction != dir_ver && direction != -dir_ver &&
            direction != dir_dig && direction != -dir_dig) {

            std::cout << "UNKNOWN_MOVE_DIRECTION\n";
            return;
        }

        from = to;
        auto copy = board;

        if (!move(from, direction)) {
            board = copy;
            std::cout << "BAD_MOVE_ROW_IS_FULL\n";
            return;
        }

        auto collisions = generate_all_collisions(board);
        if (!collisions.empty()) {
            for (auto& collision : collisions) {
                remove_collision(board, collision);
            }
        }

        std::cout << "MOVE_COMMITTED\n";
        advance_turn(board);
    }

};


int main() {

    Game game;

    std::string line;
    while (std::getline(std::cin, line)) {

        if (line.rfind("LOAD_GAME_BOARD", 0) == 0) {
            game.load_game_board();
            std::cout << "\n";
        }
        else if (line.rfind("PRINT_GAME_BOARD", 0) == 0) {
            game.print_game_board();
        }
        else if (line.rfind("DO_MOVE", 0) == 0) {
            game.do_move(std::string(line.begin() + 8, line.end()));
            std::cout << "\n";
        }

    }
}
