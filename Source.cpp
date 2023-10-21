#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

//define dimensions for 2d array for map
#define COLUMN 20
#define ROW 20

//dimensions for 2d for items inventory
#define MAX_LINE 192
#define MAX_CHARS 200
#define INT_MAX 100

int findShortestPathLength(int mat[20][20], int x1, int y1, int x2, int y2);
int findShortestPath(int mat[20][20], int visited[20][20], int i, int j, int x, int y, int min_dist, int dist);
int isSafe(int mat[20][20], int visited[20][20], int x, int y, int row, int col);
int displayShortestPathLength(int mat[20][20], int t_mat[20][20], int t_mat2[20][20], int x1, int y1, int x2, int y2);
int displayShortestPath(int mat[20][20], int mat2[20][20], int t_map[20][20], int visited[20][20], int i, int j, int x, int y, int min_dist, int dist);


int main(void)
{
	//arrays for x and y coordinates of item's position and estra to ask user for nubmer of list items
	int x[192], y[192], item_no;

	//2d array to store map
	int location[20][20], route[20][20], t_route[20][20];

	//2d array to store items
	char items[192][200];

	//1d array to store index positions of desired items
	int index_positions[192];

	//2d array to input list
	char list[192][200];

	//coordinates of desired items in shopping list, temporary array
	int d_coordinates[192][2];
	int start[1][2];
	int stop[1][2];

	//file pointer for map
	FILE* map_pointer;


	//file pointer for items
	FILE* item_pointer;

	//open map
	map_pointer = fopen("map.txt", "r");

	//open file of items
	item_pointer = fopen("storeitem.txt", "r");

	//to check if there is any error openning file
	if (item_pointer == NULL)
	{
		printf("Error in openning file\n");
	}
	if (map_pointer == NULL)
	{
		printf("Error in openning file\n");
	}

	//declare temporary variable for row
	int temp_row = 0;

	//read map from file into 2d array
	while (!feof(map_pointer))
	{
		if (ferror(map_pointer))
		{
			printf("Error reading file\n");
		}
		for (int j = 0; j < COLUMN; j++)
		{
			if (fscanf(map_pointer, "%d", &location[temp_row][j]) == EOF)
			{
				break;
			}
		}
		temp_row++;
		if (temp_row == ROW)
		{
			break;
		}
	}
	//close the map file
	fclose(map_pointer);


	//variable to define the index of arrays containig coordinates
	int c = 0;

	//storing location of items in coordinates
	for (int a = 0; a < ROW; a++)
	{
		for (int b = 0; b < COLUMN; b++)
		{
			if (location[a][b] == 1)
			{
				x[c] = a;
				y[c] = b;
				c++;
			}
		}
	}


	//reading store items from items file
	int line = 0;
	while (!feof(item_pointer) && !ferror(item_pointer))
	{
		if (fgets(items[line], MAX_CHARS, item_pointer) != NULL)
		{
			line++;
		}
	}

	//closing items file
	fclose(item_pointer);


	printf("How many number of items you want to buy: ");
	scanf("%d\n", &item_no);
	fflush(stdin);

	for (int d = 0; d < item_no; d++)
	{
		fgets(list[d], 200, stdin);
	}

	//linear search
	for (int e = 0; e <= item_no; e++)
	{

		for (int f = 0; f <= 202; f++)
		{
			if (strcmp(items[f], list[e]) == 0)
			{
				index_positions[e] = f;
				break;
			}
			else
			{
				index_positions[e] = -1;
			}
		}
	}
	printf("Positions of items in map is: \n");
	for (int g = 0; g < item_no; g++)
	{
		printf("%d\n", index_positions[g]);
	}
	for (int pass = 1; pass < item_no; pass++)
	{
		for (int s_pass = 0; s_pass < item_no - 1; s_pass++)
		{
			if (index_positions[s_pass] > index_positions[s_pass + 1])
			{
				int hold = index_positions[s_pass];
				index_positions[s_pass] = index_positions[s_pass + 1];
				index_positions[s_pass + 1] = hold;
			}
		}
	}

	//allocate dynamic memory for adjacency matrix to store distances
	int** ad_mat = (int**)malloc(sizeof(int*) * (item_no + 2));
	for (int i = 0; i < item_no + 2; i++)
	{
		ad_mat[i] = (int*)malloc(sizeof(int*) * (item_no + 2));
	}

	//allocate memory for minimum spannig tree
	int** mst = (int**)malloc(sizeof(int*) * (item_no + 2));
	for (int i = 0; i < item_no + 2; i++)
	{
		mst[i] = (int*)malloc(sizeof(int*) * 2);
	}


	d_coordinates[0][0] = 19;
	d_coordinates[0][1] = 10;

	int k = 1;
	for (int h = 0; h < item_no; h++)
	{
		d_coordinates[k][0] = x[index_positions[h]];
		d_coordinates[k][1] = y[index_positions[h]];
		k++;
	}
	d_coordinates[k][0] = 19;
	d_coordinates[k][1] = 11;

	int m = 0;
	for (int i = 0; i <= item_no + 1; i++)
	{
		start[0][0] = d_coordinates[i][0];
		start[0][1] = d_coordinates[i][1];
		for (int l = 0; l <= item_no + 1; l++)
		{
			stop[0][0] = d_coordinates[l][0];
			stop[0][1] = d_coordinates[l][1];
			ad_mat[i][l] = findShortestPathLength(location, start[0][0], start[0][1], stop[0][0], stop[0][1]);
			if (ad_mat[i][l] == 0)
			{
				ad_mat[i][l] = 999;
			}
			m++;
		}
	}

	for (int j = 0; j <  item_no; j++)
	{
		for (int i = 0; i < item_no; i++)
		{
			printf("%d\t", ad_mat[j][i]);
		}
		printf("\n");
	}


	//kruskaal algorithm
	int min = 999, mincost = 0, v1, v2, ne = 1, u, v, ed1, ed2;
	int* parent = (int*)malloc(sizeof(int*) * (item_no + 2));
	for (int o = 0; o < item_no + 2; o++)
	{
		parent[o] = o;
	}
	//printf("The edges of Minimum Cost Spanning Tree are\n");
	int z = 0;
	while (ne < item_no + 2)
	{
		min = 999;
		for (int i = 0; i < item_no + 2; i++)
		{
			for (int j = 0; j < item_no + 2; j++)
			{
				if (ad_mat[i][j] < min)
				{
					min = ad_mat[i][j];
					v1 = u = ed1 = i;
					v2 = v = ed2 = j;
				}
			}
		}

		//while (parent[u])
			//u = parent[u];

		for (int p = 0; p < item_no + 2; p++)
		{
			if (p == ed1)
			{
				u = parent[ed1];
			}
			if (p == ed2)
			{
				v = parent[ed2];
			}
		}

		//while (parent[v])
			//v = parent[v];
		int go = 0;
		if (u != v)
		{
			parent[ed2] = u;
			go = 1;
		}
		else
		{
			go = 0;
		}

		if (go == 1)
		{
			mst[z][0] = v1;
			mst[z][1] = v2;
			printf("%d edge (%d,%d) =%d\n", ne++, v1, v2, min);
			mincost += min;
		}
		ad_mat[v1][v2] = ad_mat[v2][v1] = 999;
		z++;
	}
	printf("\n\tMinimum cost = %d\n", mincost);


	//output
	int value1, value2;
	//route[20][20] = location[20][20];
	//t_route[20][20] = location[20][20];
	memcpy(route, location, sizeof(location));
	memcpy(t_route, location, sizeof(location));
	for (int q = 0; q < item_no + 1; q++)
	{
		//t_route[20][20] = route[20][20];
		memcpy(t_route, route, sizeof(route));
		value1 = mst[q][0];
		value2 = mst[q][1];
		start[0][0] = d_coordinates[value1][0];
		start[0][1] = d_coordinates[value1][1];
		stop[0][0] = d_coordinates[value2][0];
		stop[0][1] = d_coordinates[value2][1];
		displayShortestPathLength(location, route,t_route, start[0][0], start[0][1], stop[0][0], stop[0][1]);

	}

	for (int s = 0; s < 20; s++)
	{
		for (int t = 0; t < 20; t++)
		{
			printf("%d ", route[s][t]);
		}
		printf("\n");
	}

	return 0;
	
}



int isSafe(int mat[20][20], int visited[20][20], int x, int y, int row, int col)
{
	return (x >= 0 && x < 20 && y >= 0 && y < 20) &&
		mat[x][y] == 0 && !visited[x][y];
}

int findShortestPath(int mat[20][20], int visited[20][20], int i, int j, int x, int y, int min_dist, int dist)
{

	if ((i == x) && (j == y))
	{
		min_dist = ((min_dist < dist) ? min_dist : dist);
		return min_dist;
	}
	//Check if we have reached near the destination
	if (  (i == x - 1 && j == y) || (i == x + 1 && j == y) || (i == x && j == y - 1) || (i == x && j == y + 1)  )
	{
		min_dist = ((min_dist < dist) ? min_dist : dist+1);
		return min_dist;
	}

	// set (i, j) cell as visited
	visited[i][j] = 1;
	// go to the bottom cell
	if (isSafe(mat, visited, i + 1, j, x, y)) {

		min_dist = findShortestPath(mat, visited, i + 1, j, x, y, min_dist, dist + 1);
	}
	// go to the right cell
	if (isSafe(mat, visited, i, j + 1, x, y)) {

		min_dist = findShortestPath(mat, visited, i, j + 1, x, y, min_dist, dist + 1);
	}
	// go to the top cell
	if (isSafe(mat, visited, i - 1, j, x, y)) {

		min_dist = findShortestPath(mat, visited, i - 1, j, x, y, min_dist, dist + 1);
	}
	// go to the left cell
	if (isSafe(mat, visited, i, j - 1, x, y)) {

		min_dist = findShortestPath(mat, visited, i, j - 1, x, y, min_dist, dist + 1);
	}
	// backtrack: remove (i, j) from the visited matrix
	visited[i][j] = 0;
	return min_dist;
}

int findShortestPathLength(int mat[20][20], int x1, int y1, int x2, int y2) {
	if ((mat[x1 - 1][y1] && mat[x1 + 1][y2] && mat[x1][y1 - 1] && mat[x1][y1 + 1]) || (mat[x2 - 1][y2] && mat[x2 + 1][y2] && mat[x2][y2 - 1] && mat[x2][y2 + 1]))
		return -1;
	if ((x1 == 19 && y1 == 10 && x2 == 19 && y2 == 11) || (x1 == 19 && y1 == 11 && x2 == 19 && y2 == 10))
	{
		return 0;
	}

	int visited[20][20];
	memset(visited, 0, sizeof(visited));

	int dis = INT_MAX;
	dis = findShortestPath(mat, visited, x1, y1, x2, y2, dis, 0);

	if (dis != INT_MAX)
		return dis;
	return -1;
}
int displayShortestPath(int mat[20][20], int mat2[20][20],int t_map[20][20], int visited[20][20], int i, int j, int x, int y, int min_dist, int dist)
{
	
	if ((i == x) && (j == y))
	{
		//mat2 = t_map;
		memcpy(mat2, t_map, sizeof(t_map));
		min_dist = ((min_dist < dist) ? min_dist : dist);
		for (int p1 = 0; p1 < 20; p1++)
		{
			for (int p2 = 0; p2 < 20; p2++)
			{
				if (visited[p1][p2] == 1)
				{
					mat2[p1][p2] = 2;
				}
			}
		}
		return min_dist;
	}
	//Check if we have reached near the destination
	if ((i == x - 1 && j == y) || (i == x + 1 && j == y) || (i == x && j == y - 1) || (i == x && j == y + 1))
	{
		//mat2 = t_map;
		memcpy(mat2, t_map, sizeof(t_map));
		min_dist = ((min_dist < dist) ? min_dist : dist+1);
		for (int p1 = 0; p1 < 20; p1++)
		{
			for (int p2 = 0; p2 < 20; p2++)
			{
				if (visited[p1][p2] == 1)
				{
					mat2[p1][p2] = 2;
				}
			}
		}
		return min_dist;
	}

	// set (i, j) cell as visited
	visited[i][j] = 1;
	// go to the bottom cell
	if (isSafe(mat, visited, i + 1, j, x, y)) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i + 1, j, x, y, min_dist, dist + 1);
	}
	// go to the right cell
	if (isSafe(mat, visited, i, j + 1, x, y)) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i, j + 1, x, y, min_dist, dist + 1);
	}
	// go to the top cell
	if (isSafe(mat, visited, i - 1, j, x, y)) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i - 1, j, x, y, min_dist, dist + 1);
	}
	// go to the left cell
	if (isSafe(mat, visited, i, j - 1, x, y)) {

		min_dist = displayShortestPath(mat, mat2, t_map, visited, i, j - 1, x, y, min_dist, dist + 1);
	}
	// backtrack: remove (i, j) from the visited matrix
	visited[i][j] = 0;
	return min_dist;
}
int displayShortestPathLength(int mat[20][20], int t_mat[20][20],int t_mat2[20][20], int x1, int y1, int x2, int y2) {
	if ((mat[x1 - 1][y1] && mat[x1 + 1][y2] && mat[x1][y1 - 1] && mat[x1][y1 + 1]) || (mat[x2 - 1][y2] && mat[x2 + 1][y2] && mat[x2][y2 - 1] && mat[x2][y2 + 1]))
		return -1;
	if ((x1 == 19 && y1 == 10 && x2 == 19 && y2 == 11) || (x1 == 19 && y1 == 11 && x2 == 19 && y2 == 10))
	{
		return 0;
	}

	int visited[20][20];
	memset(visited, 0, sizeof(visited));

	int dis = INT_MAX;
	dis = displayShortestPath(mat,t_mat,t_mat2, visited, x1, y1, x2, y2, dis, 0);

	if (dis != INT_MAX)
		return dis;
	return -1;
}