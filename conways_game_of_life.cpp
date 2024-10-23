#include <iostream>
#include <iomanip>
using namespace std;

/* Function to expand the size of the universe dynamically. 
   It doubles the size of the current universe array when needed. */
void expand_universe(char * &universe, int &size) {
    char *temp_arr = new char[size * 2];
    for(int i = 0; i < size; ++i) {
        temp_arr[i] = universe[i];
    }
    delete[] universe;
    universe = temp_arr;
    size *= 2;
}

/* Function to add a character to the universe array. 
   It handles the expansion of the array and tracks rows. */
void add(char * &universe, int &size, int &position, char x, int &rows) {
    if(size == position) {
        expand_universe(universe, size);
    }
    if(x == '\n') {
        ++rows;  // Count newlines to track rows in the grid
    } else {
        universe[position] = x;  // Add the character to the universe
        ++position;
    }
}

/* Function to print the current state of the universe. 
   It outputs the grid with pipes '|' at the top and bottom. */
void universe_printer(char *universe, int cells, int columns) {
    for(int i = 1; i < columns + 1; ++i) cout << "|";  // Print top border
    cout << endl;

    for(int i = 1, j = 1; i < cells + 1; ++i, ++j) {
        cout << universe[i - 1];  // Print the current cell
        if(j == columns) {  // Go to the next row
            j = 0;
            cout << endl;
        }
    }

    for(int i = 1; i < columns + 1; ++i) cout << "|";  // Print bottom border
    cout << endl;
}

/* Function to count the number of live ('O') and dead ('.') neighbors. */
void dead_o_alive(char neighbor, int &alive, int &dead) {
    if(neighbor == '.') ++dead;
    else ++alive;
}

/* Function to decide the next state of a cell based on its neighbors. */
void genesis(char *new_universe, int cell, int alive, int dead, char current_state) {
    if(current_state == 'O') {
        // Rule: Stay alive if it has 2 or 3 live neighbors
        if(alive == 2 || alive == 3) {
            new_universe[cell] = 'O';
        } else {
            new_universe[cell] = '.';
        }
    } else {
        // Rule: Reproduce if exactly 3 neighbors are alive
        if(alive == 3) new_universe[cell] = 'O';
        else new_universe[cell] = '.';
    }
}

/* Function to calculate the next generation of the universe. */
void universe_calculation(char * &universe, const int cells, const int rows, const int columns) {
    int alive = 0;
    int dead = 0;
    char *new_universe = new char[cells];

    for(int cell = 0, row = 1, column = 1; cell < cells; ++cell, ++column) {
        if (column == 1 && row == 1) {  // Top left corner
            dead_o_alive(universe[cell + columns], alive, dead);
            dead_o_alive(universe[cell + columns + 1], alive, dead);
            dead_o_alive(universe[cell + 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else if (column == columns && row == 1) {  // Top right corner
            dead_o_alive(universe[cell - 1], alive, dead);
            dead_o_alive(universe[cell + columns], alive, dead);
            dead_o_alive(universe[cell + columns - 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else if (column == 1 && row == rows) {  // Bottom left corner
            dead_o_alive(universe[cell + 1], alive, dead);
            dead_o_alive(universe[cell - columns], alive, dead);
            dead_o_alive(universe[cell - columns + 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else if (column == columns && row == rows) {  // Bottom right corner
            dead_o_alive(universe[cell - 1], alive, dead);
            dead_o_alive(universe[cell - columns], alive, dead);
            dead_o_alive(universe[cell - columns - 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else if (row == 1) {  // Top edge
            dead_o_alive(universe[cell - 1], alive, dead);
            dead_o_alive(universe[cell + 1], alive, dead);
            dead_o_alive(universe[cell + columns - 1], alive, dead);
            dead_o_alive(universe[cell + columns], alive, dead);
            dead_o_alive(universe[cell + columns + 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else if (column == 1) {  // Left edge
            dead_o_alive(universe[cell + 1], alive, dead);
            dead_o_alive(universe[cell + columns], alive, dead);
            dead_o_alive(universe[cell + columns + 1], alive, dead);
            dead_o_alive(universe[cell - columns], alive, dead);
            dead_o_alive(universe[cell - columns + 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else if (column == columns) {  // Right edge
            dead_o_alive(universe[cell - 1], alive, dead);
            dead_o_alive(universe[cell + columns], alive, dead);
            dead_o_alive(universe[cell + columns - 1], alive, dead);
            dead_o_alive(universe[cell - columns], alive, dead);
            dead_o_alive(universe[cell - columns - 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else if (row == rows) {  // Bottom edge
            dead_o_alive(universe[cell - 1], alive, dead);
            dead_o_alive(universe[cell + 1], alive, dead);
            dead_o_alive(universe[cell - columns - 1], alive, dead);
            dead_o_alive(universe[cell - columns], alive, dead);
            dead_o_alive(universe[cell - columns + 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);

        } else {  // General case (non-edge cells)
            dead_o_alive(universe[cell - 1], alive, dead);
            dead_o_alive(universe[cell + 1], alive, dead);
            dead_o_alive(universe[cell - columns - 1], alive, dead);
            dead_o_alive(universe[cell - columns], alive, dead);
            dead_o_alive(universe[cell - columns + 1], alive, dead);
            dead_o_alive(universe[cell + columns], alive, dead);
            dead_o_alive(universe[cell + columns + 1], alive, dead);
            dead_o_alive(universe[cell + columns - 1], alive, dead);
            genesis(new_universe, cell, alive, dead, universe[cell]);
        }

        // Move to the next row when reaching the last column
        if(column == columns) {
            ++row;
            column = 0;
        }

        alive = 0;
        dead = 0;
    }

    delete[] universe;
    universe = new_universe;
}

int main() {
    int rows = 0;  // Number of new line characters in the input
    int columns = 0;  // Number of columns in the grid

    int uni_size = 4;  // Initial size of the universe array
    char *universe = new char[uni_size];
    char x = '?';  // Input character
    int cells = 0;

    // Read the input universe until the character 'x' is encountered
    while(x != 'x') {
        cin >> noskipws >> x;
        if(x == ' ') continue;  // Skip spaces
        add(universe, uni_size, cells, x, rows);
    }
    --cells;  // Decrement to remove the 'x' character
    cin >> skipws;
    columns = (cells + 1) / rows;  // Calculate number of columns in the grid

    // Read commands: 'p' to print the universe, 's' to simulate the next step
    while(cin >> x) {
        if(x == 'p') {
            universe_printer(universe, cells, columns);
        } else if(x == 's') {
            universe_calculation(universe, cells, rows, columns);
        }
    }

    delete[] universe;  // Clean up the dynamically allocated memory
    return 0;
}
