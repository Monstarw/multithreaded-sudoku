#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct parameters{	//保存了传递给线程的所需参数，指定了线程开始验证的起点位置
    int row;	//起点行
    int col;	//起点列
    int (* board)[9];	//9*9数独表格的指针
} parameters;

int flag = 0;	//标志数独谜题解决是否有效，为0则有效，1则无效
void * check_rows(void * params);	//所有行行检查函数声明
void * check_cols(void * params);	//所有列检查函数声明
void * check_square(void * params);	//单个3*3子网格检查函数声明

int main(){
	int i, j, board[9][9];
	FILE *fp = fopen("sudoku_board", "r");	//建立外部输入文件sudoku_board的读取指针
	for(i = 0; i < 9; i++){	//所有行
		for(j = 0; j < 9; j++){	//所有列
			fscanf(fp, "%d", &board[i][j]);	//将文件内数字依次读入board数组
		}
	}
	fclose(fp);	//关闭文件
	
	parameters *param = malloc(sizeof *param * 9);	//分配9段连续空间，每段大小为一个结构体的大小
	for(i = 0; i < 9; i++){	
		param[i].row = i / 3 * 3;	//赋值行起点下标
		param[i].col = i * 3 % 9;	//赋值列起点下标
		//i的值分别为 0, 1, 2, 3, 4, 5, 6, 7, 8
		//row值分别为 0, 0, 0, 3, 3, 3, 6, 6, 6
		//col值分别为 0, 3, 6, 0, 3, 6, 0, 3, 6
		param[i].board = board;	//赋值数独表格的指针
	}
    pthread_t thread_rows, thread_cols, thread_square[9];	//声明所用线程（检查所有行、所有列、每个3*3网格）
    //初始化线程
    pthread_create(&thread_rows, NULL, check_rows, &param[0]);	//检查所有行，起点参数为[0, 0]
    pthread_create(&thread_cols, NULL, check_cols, &param[0]);	//检查所有列，起点参数为[0, 0]
	for(i = 0; i < 9; i++){	//检查九个3*3网格
		pthread_create(&thread_square[i], NULL, check_square, &param[i]);
	}
    //等待线程运行完毕
	pthread_join(thread_rows, NULL);
	pthread_join(thread_cols, NULL);
	for(i = 0; i < 9; i++){
		pthread_join(thread_square[i], NULL);
	}
    //检查标志，输出数独谜题解决是否有效
	if(flag == 1){	//无效
		printf("数独解决方案无效！\n");
	}else{	//有效
		printf("数独解决方案有效！\n");
	}
    return 0;
}

//检查所有行是否都包含1-9的所有数字
void * check_rows(void * params){
    parameters * data = (parameters *) params;	//将结构体指针赋给data
    int startRow = data->row;	//取出行起点并赋给startRow
    int startCol = data->col;	//取出列起点并赋给startCol
    for(int i = startRow; i < 9; i++){	//循环所有行
        int row[10] = {0};	//用数组标记1-9是否都出现且仅出现过一次
        for(int j = startCol; j < 9; j++){	//本行中循环每个数字
            int value = data->board[i][j];	//取出数独表中第i行第j列元素赋给value
            if(row[value] != 0){
				flag = 1;	//数字value在本行已经出现过，设标志为1，终止线程
				return NULL;
            }else{
                row[value] = 1;	//将第一次出现的数字value对应的数组元素设为1，表示数字value在该行出现了
            }
        }
    }
	return NULL;	//未出现重复数字，线程正常结束
}

//检查所有列是否都包含1-9的所有数字
void * check_cols(void * params){
    parameters * data = (parameters *) params;	//将结构体指针赋给data
    int startRow = data->row;	//取出行起点并赋给startRow
    int startCol = data->col;	//取出列起点并赋给startCol
    for(int i = startCol; i < 9; i++){	//循环所有列
        int col[10] = {0};	//用数组标记1-9是否都出现且仅出现过一次
        for(int j = startRow; j < 9; j++){	//本列中循环每个数字
            int value = data->board[j][i];	//取出数独表中第j行第i列元素赋给value
            if(col[value] != 0){
				flag = 1;	//数字value在本列已经出现过，设标志为1，终止线程
				return NULL;
            }else{
                col[value] = 1;	//将第一次出现的数字value对应的数组元素设为1，表示数字value在该列出现了
            }
        }
    }
	return NULL;	//未出现重复数字，线程正常结束
}

//检查单个3*3网格内是否包含1-9的所有数字
void * check_square(void * params){
    parameters * data = (parameters *) params;	//将结构体指针赋给data
    int startRow = data->row;	//取出行起点并赋给startRow
    int startCol = data->col;	//取出列起点并赋给startCol
    int square[10] = {0};	//用数组标记1-9是否都出现且仅出现过一次
    for(int i = startRow; i < startRow + 3; i++){	//循环的行：起点行、起点行+1、起点行+2
        for (int j = startCol; j < startCol + 3; j++){	//本行中循环的列：起点列、起点列+1、起点列+2
            int value = data->board[i][j];	//取出数独表中第j行第i列元素赋给value
            if(square[value] != 0){
				flag = 1;	//数字value在本网格已经出现过，设标志为1，终止线程
				return NULL;
            }else{
                square[value] = 1;	//将第一次出现的数字value对应的数组元素设为1，表示数字value在该网格出现了
            }
        }
    }
	return NULL;	//未出现重复数字，线程正常结束
}
