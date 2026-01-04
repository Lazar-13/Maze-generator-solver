#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WALL '#'
#define PATH ' '
#define SOLUTION '.'

typedef struct {
    int width;
    int height;
    char **grid;
} Maze;

void allocateMaze(Maze *maze) {
    maze->grid = (char **)malloc(maze->height * sizeof(char *));
    for (int i = 0; i < maze->height; i++) {
        maze->grid[i] = (char *)malloc(maze->width * sizeof(char));
    }
}

void freeMaze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) free(maze->grid[i]);
    free(maze->grid);
}

void initMaze(Maze *maze) {
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            maze->grid[i][j] = WALL;
        }
    }
}

void printMaze(Maze *maze) {
    printf("\n");
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            printf("%c ", maze->grid[i][j]);
        }
        printf("\n");
    }
}

void saveToFile(Maze *maze, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    fprintf(f, "Maze Dimensions: %d x %d\n", maze->width, maze->height);
    for (int i = 0; i < maze->height; i++) {
        for (int j = 0; j < maze->width; j++) {
            fputc(maze->grid[i][j], f);
        }
        fputc('\n', f);
    }
    fclose(f);
}

void generateMaze(Maze *maze, int r, int c) {
    int dirs[4][2] = {{0, 2}, {2, 0}, {0, -2}, {-2, 0}};
    for (int i = 0; i < 4; i++) {
        int r1 = rand() % 4, r2 = rand() % 4;
        int tx = dirs[r1][0], ty = dirs[r1][1];
        dirs[r1][0] = dirs[r2][0]; dirs[r1][1] = dirs[r2][1];
        dirs[r2][0] = tx; dirs[r2][1] = ty;
    }

    for (int i = 0; i < 4; i++) {
        int nr = r + dirs[i][0], nc = c + dirs[i][1];
        if (nr > 0 && nr < maze->height - 1 && nc > 0 && nc < maze->width - 1 && maze->grid[nr][nc] == WALL) {
            maze->grid[nr][nc] = PATH;
            maze->grid[r + dirs[i][0]/2][c + dirs[i][1]/2] = PATH;
            generateMaze(maze, nr, nc);
        }
    }
}

int solveMaze(Maze *maze, int r, int c) {
    if (r == maze->height - 2 && c == maze->width - 2) {
        maze->grid[r][c] = 'E'; // Exit
        return 1;
    }
    if (r > 0 && r < maze->height - 1 && c > 0 && c < maze->width - 1 && (maze->grid[r][c] == PATH || maze->grid[r][c] == 'S')) {
        char prev = maze->grid[r][c];
        maze->grid[r][c] = SOLUTION;
        if (solveMaze(maze, r + 1, c)) return 1;
        if (solveMaze(maze, r, c + 1)) return 1;
        if (solveMaze(maze, r - 1, c)) return 1;
        if (solveMaze(maze, r, c - 1)) return 1;
        maze->grid[r][c] = prev;
    }
    return 0;
}

int main() {
    srand(time(NULL));
    Maze myMaze;
    int w, h, choice;

    printf("Enter maze size (e.g. 21 21): ");
    scanf("%d %d", &w, &h);

    myMaze.width = (w % 2 == 0) ? w - 1 : w;
    myMaze.height = (h % 2 == 0) ? h - 1 : h;

    allocateMaze(&myMaze);
    initMaze(&myMaze);
    myMaze.grid[1][1] = PATH;
    generateMaze(&myMaze, 1, 1);
    myMaze.grid[1][1] = 'S'; 
    myMaze.grid[myMaze.height-2][myMaze.width-2] = 'E'; 

    printf("Maze Generated (%dx%d):\n", myMaze.width, myMaze.height);
    printMaze(&myMaze);

    printf("\n1. Solve\n2. Save\n3. Exit\nChoice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        solveMaze(&myMaze, 1, 1);
        myMaze.grid[1][1] = 'S'; 
        printMaze(&myMaze);
    } else if (choice == 2) {
        saveToFile(&myMaze, "maze.txt");
        printf("Saved!\n");
    }

    freeMaze(&myMaze);
    return 0;
}