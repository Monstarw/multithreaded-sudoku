#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct{	//保存了传递给线程的所需参数，指定了线程开始验证的起点位置
    int row;	//起点行
    int col;	//起点列
    int (* board)[9];	//数独指针
} parameters;

int walk_rows(parameters * params);	//行检查函数声明
int walk_cols(parameters * params);	//列检查函数声明
int check_square(parameters * params);	//3*3子网格检查函数声明

int main(void){
    int board[9][9] = {
        {6, 2, 4, 5, 3, 9, 1, 8, 7},
        {5, 1, 9, 7, 2, 8, 6, 3, 4},
        {8, 3, 7, 6, 1, 4, 2, 9, 5},
        {1, 4, 3, 8, 6, 5, 7, 2, 9},
        {9, 5, 8, 2, 4, 7, 3, 6, 1},
        {7, 6, 2, 3, 9, 1, 4, 5, 8},
        {3, 7, 1, 9, 5, 6, 8, 4, 2},
        {4, 9, 6, 1, 8, 2, 5, 7, 3},
        {2, 8, 5, 4, 7, 3, 9, 1, 6}
    };
    
	//建立行与列检查的参数（起点均在左上角(0,0)）
    parameters * param0 = (parameters *) malloc(sizeof(parameters));
    param0->row = 0;
    param0->col = 0;
    param0->board = board;
    
    //建立3*3子网格检查的参数
	
    //第一个子网格起点(0,0)
    parameters * param1 = (parameters *) malloc(sizeof(parameters));
    param1->row = 0;
    param1->col = 0;
    param1->board = board;
    
    //第二个子网格起点(0,3)
    parameters * param2 = (parameters *) malloc(sizeof(parameters));
    param2->row = 0;
    param2->col = 3;
    param2->board = board;
    
    //第三个子网格起点(0,6)
    parameters * param3 = (parameters *) malloc(sizeof(parameters));
    param3->row = 0;
    param3->col = 6;
    param3->board = board;
    
    //第四个子网格起点(3,0)
    parameters * param4 = (parameters *) malloc(sizeof(parameters));
    param4->row = 3;
    param4->col = 0;
    param4->board = board;
    
    //第五个子网格起点(3,3)
    parameters * param5 = (parameters *) malloc(sizeof(parameters));
    param5->row = 3;
    param5->col = 3;
    param5->board = board;
    
    //第六个子网格起点(3,6)
    parameters * param6 = (parameters *) malloc(sizeof(parameters));
    param6->row = 3;
    param6->col = 6;
    param6->board = board;
    
    //第七个子网格起点(6,0)
    parameters * param7 = (parameters *) malloc(sizeof(parameters));
    param7->row = 6;
    param7->col = 0;
    param7->board = board;
    
    //第八个子网格起点(6,3)
    parameters * param8 = (parameters *) malloc(sizeof(parameters));
    param8->row = 6;
    param8->col = 3;
    param8->board = board;
    
    //第九个子网格起点(6,6)
    parameters * param9 = (parameters *) malloc(sizeof(parameters));
    param9->row = 6;
    param9->col = 6;
    param9->board = board;
    
    // ====== Create the threads ======
    pthread_t thread_rows, thread_cols, thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9;
    
    // ====== Create the return values for the threads ======
    void * all_rows;
    void * all_cols;
	void * squares[9];
    
    // ====== Initialize the threads ======
    pthread_create(&thread_rows, NULL, walk_rows, param0);
    pthread_create(&thread_cols, NULL, walk_cols, param0);
    pthread_create(&thread1, NULL, check_square, param1);
    pthread_create(&thread2, NULL, check_square, param2);
    pthread_create(&thread3, NULL, check_square, param3);
    pthread_create(&thread4, NULL, check_square, param4);
    pthread_create(&thread5, NULL, check_square, param5);
    pthread_create(&thread6, NULL, check_square, param6);
    pthread_create(&thread7, NULL, check_square, param7);
    pthread_create(&thread8, NULL, check_square, param8);
    pthread_create(&thread9, NULL, check_square, param9);

    // ======= Wait for all threads to finish their tasks =======
    pthread_join(thread_rows, &all_rows);
    pthread_join(thread_cols, &all_cols);
    pthread_join(thread1, &squares[0]);
    pthread_join(thread2, &squares[1]);
    pthread_join(thread3, &squares[2]);
    pthread_join(thread4, &squares[3]);
    pthread_join(thread5, &squares[4]);
    pthread_join(thread6, &squares[5]);
    pthread_join(thread7, &squares[6]);
    pthread_join(thread8, &squares[7]);
    pthread_join(thread9, &squares[8]);
    
    // ====== Check whether the Sudoku Puzzle was solved ======
    if (all_rows == 1 && all_cols == 1 &&
        squares[0] == 1 &&
        squares[1] == 1 &&
        squares[2] == 1 &&
        squares[3] == 1 &&
        squares[4] == 1 &&
        squares[5] == 1 &&
        squares[6] == 1 &&
        squares[7] == 1 &&
        squares[8] == 1 ){
			printf("The Sudoku Puzzle is solved!\n");
    }
    else {
        printf("The Sudoku Puzzle is NOT solved.\n");
    }
    
    return 0;
}

/**
 * Checks each row if it contains all digits 1-9.
 * @param   void *      The parameters (pointer).
 * @return  void *      1 if all rows contain all digits from 1-9, 0 otherwise.
 */
int walk_rows(parameters * params) {
    parameters * data = params;
    int startRow = data->row;
    int startCol = data->col;
    for (int i = startRow; i < 9; ++i) {
        int row[10] = {0};
        for (int j = startCol; j < 9; ++j) {
            int val = data->board[i][j];
            if (row[val] != 0) {
                return 0;
            }
            else{
                row[val] = 1;
            }
        }
    }
    return 1;
}

/**
 * Checks each column if it contains all digits 1-9.
 * @param   void *      The parameters (pointer).
 * @return  void *      1 if all rows contain all digits from 1-9, 0 otherwise.
 */
int walk_cols(parameters * params) {
    parameters * data = params;
    int startRow = data->row;
    int startCol = data->col;
    for (int i = startCol; i < 9; ++i) {
        int col[10] = {0};
        for (int j = startRow; j < 9; ++j) {
            int val = data->board[j][i];
            if (col[val] != 0) {
                return 0;
            }
            else{
                col[val] = 1;
            }
        }
    }
    return 1;
}

/**
 * Checks if a square of size 3x3 contains all numbers from 1-9.
 * @param   void *      The parameters (pointer).
 * @return  void *      1 if all rows contain all digits from 1-9, 0 otherwise.
 */
int check_square(parameters * params) {
    parameters * data = params;
    int startRow = data->row;
    int startCol = data->col;
    int saved[10] = {0};
    for (int i = startRow; i < startRow + 3; ++i) {
        for (int j = startCol; j < startCol + 3; ++j) {
            int val = data->board[i][j];
            if (saved[val] != 0) {
                return 0;
            }
            else{
                saved[val] = 1;
            }
        }
    }
    return 1;
}