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

    bool operator==(const Board& other)
    {
        return white_reserve == other.white_reserve &&
            black_reserve == other.black_reserve &&
            board == other.board &&
            current_player == other.current_player;
    }
    bool operator<(const Board& other)
    {
        return board < other.board;
    }

    int white_reserve;
    int black_reserve;
    char current_player;

    std::vector<char> board;

};

struct Game {

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
        scanf("%d %d %c", &board.white_reserve, &board.black_reserve, &board.current_player);

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
        std::cout << board.white_reserve << " " << board.black_reserve << " " << board.current_player << "\n";
        
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

        if (b.current_player == WHITE) {
            b.current_player = BLACK;
            b.white_reserve--;
        }

        else if (b.current_player == BLACK) {
            b.current_player = WHITE;
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

    bool move(Board& b, char* from, int direction) {
        
        char token = b.current_player;

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

        if (!move(board, from, direction)) {
            board = copy;
            std::cout << "BAD_MOVE_ROW_IS_FULL\n";
            return;
        }

        if (line.size() > 5) {

            char col = toupper(line[6]);

            int from_coll_x = line[9] - 'a';
            int from_coll_y = line[10] - '1';

            int to_coll_x = line[12] - 'a';
            int to_coll_y = line[13] - '1';

            char* from_coll = get_char_at_index(board, from_coll_x, from_coll_y);
            char* to_coll = get_char_at_index(board, to_coll_x, to_coll_y);

            if (col != *from_coll) {
                std::cout << "WRONG_COLOR_OF_CHOSEN_ROW\n";
                board = copy;
                return;
            }

            int dir_coll = to_coll - from_coll;

            int dir_hor = calculate_direction(DIRECTION_HOR);
            int dir_ver = calculate_direction(DIRECTION_VERT);
            int dir_dig = calculate_direction(DIRECTION_DIAGONAL);

            if (dir_coll % dir_dig == 0) dir_coll = dir_dig * (dir_coll / abs(dir_coll));
            else if (dir_coll % dir_ver == 0) dir_coll = dir_ver * (dir_coll / abs(dir_coll));
            else if (dir_coll % dir_hor == 0) dir_coll = dir_hor * (dir_coll / abs(dir_coll));

            char token = *from_coll;
            if(*(from_coll - dir_coll) == token || *(to_coll + dir_coll) == token) {
                std::cout << "WRONG_INDEX_OF_CHOSEN_ROW\n";
                board = copy;
                return;
            }

            remove_collision(board, Collision{from_coll, to_coll, dir_coll});
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

    std::vector<Board> generate_uniq_outcomes(Board& b) {
        
        std::vector<Board> outcomes;

        auto collisions = generate_all_collisions(b);
        if (collisions.empty()) {
            outcomes.push_back(b);
        }
        else
        {
            for (auto& collision : collisions) {
                auto copy = b;
                remove_collision(b, collision);
                auto new_collisions = generate_uniq_outcomes(b);
                outcomes.insert(outcomes.end(), new_collisions.begin(), new_collisions.end());
                b.white_reserve = copy.white_reserve;
                b.black_reserve = copy.black_reserve;
                b.current_player = copy.current_player;
                std::copy(copy.board.begin(), copy.board.end(), b.board.begin());
            }
        }

        std::sort(outcomes.begin(), outcomes.end());
        outcomes.erase(std::unique(outcomes.begin(), outcomes.end()), outcomes.end());

        return outcomes;
        
    }

    std::vector<Board> generate_all_possible_uniq_moves(Board& b) {
        
        std::vector<Board> outcomes;

        for (auto& m : moves) {
            auto copy = b;
            char* ch = get_char_at_index(copy, m.x, m.y);

            if (!move(copy, ch, m.direction)) {
                continue;
            }

            auto new_outcomes = generate_uniq_outcomes(copy);
            outcomes.insert(outcomes.end(), new_outcomes.begin(), new_outcomes.end());
        }
        
        std::sort(outcomes.begin(), outcomes.end());
        outcomes.erase(std::unique(outcomes.begin(), outcomes.end()), outcomes.end());

        for (auto& outcome : outcomes) {
            advance_turn(outcome);
        }
        return outcomes;
    }

    void generate_all_possible_moves_count() {
        auto outcomes = generate_all_possible_uniq_moves(board);
        std::cout << outcomes.size() << "_UNIQUE_MOVES\n";
    }

    void generate_all_possible_moves_ext_count() {

        char current_player = board.current_player;
        auto outcomes = generate_all_possible_uniq_moves(board);

        for (auto& outcome : outcomes) {
            char winner = settle_winner(outcome);
            if (winner == current_player) {
                std::cout << "1_UNIQUE_MOVES\n";
                return;
            }
        }

        std::cout << outcomes.size() << "_UNIQUE_MOVES\n";
    }

    char settle_winner(Board& b) {
        
        if (b.current_player == WHITE && b.white_reserve == 0) {
            return BLACK;
        }
        else if (b.current_player == BLACK && b.black_reserve == 0) {
            return WHITE;
        }
        else
        {
            int count = std::count(b.board.begin(), b.board.end(), EMPTY);
            if (count == 0) {
                if (b.current_player == BLACK) {
                    return WHITE;
                }
                else if (b.current_player == WHITE) {
                    return BLACK;
                }
            }
        }

        return NULL;
    }

    void is_game_over() {

        char winner = settle_winner(board);

        if (winner == WHITE) {
            std::cout << "THE_WINNER_IS_WHITE\n";
        }
        else if (winner == BLACK) {
            std::cout << "THE_WINNER_IS_BLACK\n";
        }
        else
        {
            std::cout << "GAME_IN_PROGRESS\n";
        }

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
        else if (line.rfind("GEN_ALL_POS_MOV_NUM", 0) == 0) {
            game.generate_all_possible_moves_count();
            std::cout << "\n";
        }
        else if (line.rfind("IS_GAME_OVER", 0) == 0) {
            game.is_game_over();
            std::cout << "\n";
        }
        else if (line.rfind("GEN_ALL_POS_MOV_EXT_NUM", 0) == 0) {
            game.generate_all_possible_moves_ext_count();
            std::cout << "\n";
        }

    }
}
