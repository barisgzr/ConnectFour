#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> 
#include <ctime>
#include <sstream>

using namespace std;

const int ROWS = 9;
const int COLS = 9;

char board[ROWS][COLS];

void load_from_file(){
    ifstream inFile;
    inFile.open("Tahta.txt");
    if (inFile.is_open()) {
        for(int row = 0; row < ROWS; row++) {
            for(int col = 0; col < COLS; col++) {
                inFile >> board[row][col];
            }
        }
    }
    inFile.close();
}

void init_board() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '.';
        }
    }
}

void reset_file(string filename) {
    ofstream outFile;
    outFile.open(filename);
    outFile.close();
}

void print_board(){
cout << endl << "  0 1 2 3 4 5 6 7 8" << endl;
    for (int i = 0; i < ROWS; i++) {
        cout << i << " ";
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == '\0') {
                cout << ". ";
            } else {
                cout << board[i][j] << " ";
            }
        }
        cout << endl;
    }
}

void write_board(){
 ofstream outfile;
    outfile.open("Tahta.txt");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            outfile << board[i][j];
        }
        outfile << endl;
    }
    outfile.close();
}


void save_to_file() {
    ofstream file("Tahta.txt");

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            file << board[i][j];
        }
        file << "\n";
    }

    file.close();
}

void write_move_to_file(int row, int col, int player) {
    ofstream outFile;
    outFile.open("Hamle.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "Player " << player <<" " << row << col << "\n";
    } else {
        cout << "Failed to open Hamle.txt for writing!\n";
    }
    outFile.close();
}

int get_next_open_row(int col) {
    for (int row = ROWS - 1; row >= 0; row--) {
        if (board[row][col] == 0) {
            return row;
        }
    }
    return -1;
}


bool make_move(int col, int player) {
    if (col < 0 || col >= COLS) {
        return false;
    }
    char token = (player == 1) ? 'R' : 'Y';
    for (int i = ROWS-1; i >= 0; i--) {
        if (board[i][col] == '\0') {
            int row = get_next_open_row(col);
            board[i][col] = token;
            write_move_to_file(row, col, player);
            save_to_file();
            return true;
        }
    }
    return false;
}



bool is_game_over() {
    // Check for horizontal wins
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] != '\0' &&
                board[i][j] == board[i][j+1] &&
                board[i][j] == board[i][j+2] &&
                board[i][j] == board[i][j+3]) {
                return true;
            }
        }
    }

    // Check for vertical wins
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] != '\0' &&
                board[i][j] == board[i+1][j] &&
                board[i][j] == board[i+2][j] &&
                board[i][j] == board[i+3][j]) {
                return true;
            }
        }
    }

    // Check for diagonal wins (top-left to bottom-right)
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 0; j < COLS - 3; j++) {
            if (board[i][j] != '\0' &&
                board[i][j] == board[i+1][j+1] &&
                board[i][j] == board[i+2][j+2] &&
                board[i][j] == board[i+3][j+3]) {
                return true;
            }
        }
    }

    // Check for diagonal wins (top-right to bottom-left)
    for (int i = 0; i < ROWS - 3; i++) {
        for (int j = 3; j < COLS; j++) {
            if (board[i][j] != '\0' &&
                board[i][j] == board[i+1][j-1] &&
                board[i][j] == board[i+2][j-2] &&
                board[i][j] == board[i+3][j-3]) {
                return true;
            }
        }
    }

    // Check for a tie game
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == '\0') {
                // If there is an empty cell, the game is not over yet
                return false;
            }
        }
    }

    // If no winning sequence is found and the board is full, the game is a tie
    return true;
}

void start_game() {
    srand(time(0));
    int startPlayer = rand() % 2 + 1;
    cout << "Player " << startPlayer << " starts the game.\n";

    int player = startPlayer;
    bool game_over = false;
    int col;
    
    while (!game_over) {
        print_board();

        cout << endl << "Player " << player << ", enter a column (0-8): ";
        cin >> col;

        if (col < 0 || col >= COLS) {
            cout << "Invalid column! Please enter a column between 0 and " << COLS - 1 << ".\n";
            continue;
        }
        
        int row = get_next_open_row(col);
        if (row == -1) {
            cout << "Column " << col << " is full! Please choose another column.\n";
            continue;
        }
        

        make_move(col, player);

        save_to_file();

        if (is_game_over()) {
            game_over = true;
            cout << "Player " << player << " wins!\n";
            print_board();
            break;
        }

        player = (player == 1) ? 2 : 1;
    }

}

int last_played_player(){

    ifstream infile("Hamle.txt");
    string line;
    int value;

    while (getline(infile, line)) {
        istringstream iss(line);
        string word;
        int count = 0;
        while (iss >> word) {
            if (++count == 2) {
                value = stoi(word);
            }
        }
    }

    infile.close();

    return value;


}


void continue_game(){
    
    int player = (last_played_player() == 1) ? 2 : 1;
    
    bool game_over = false;
    int col;
    
    while (!game_over) {
        print_board();

        cout << "Player " << player << ", enter a column (0-8): ";
        cin >> col;

        if (col < 0 || col >= COLS) {
            cout << "Invalid column! Please enter a column between 0 and " << COLS - 1 << ".\n";
            continue;
        }

        int row = get_next_open_row(col);
        if (row == -1) {
            cout << "Column " << col << " is full! Please choose another column.\n";
            continue;
        }

        make_move(col, player);

        save_to_file();

        if (is_game_over()) {
            game_over = true;
            cout << "Player " << player << " wins!\n";
            print_board();
            break;
        }

        player = (player == 1) ? 2 : 1;
    }

}

int main(){
    
    int choice;
    cout << "Welcome to Connect 4!\n";
    cout << "1.Continue, 2.New Game, or 3.Quit?\n";
    cin >> choice;
    
    if (choice == 1) {
        load_from_file();
        continue_game();
    } else if (choice == 2) {
        reset_file("Hamle.txt");
        reset_file("Tahta.txt");
        start_game();
    } else if (choice == 3) {
        return 0;
    } else {
        cout << "Invalid choice! Please choose 'Continue', 'New Game', or 'Quit'.\n";
    }
    return 0;

}