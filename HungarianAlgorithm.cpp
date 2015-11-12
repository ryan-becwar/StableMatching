#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <limits>
#include <algorithm>
#include <set>
#include <utility>
#include <iterator>

using namespace std;
const double maxDist = numeric_limits<double>::infinity();
typedef pair<int, int> Point;

void printCosts(vector<vector<double>> &costs, vector<bool> markedRows, vector<bool> markedCols){
    for(int i=0; i<costs.size(); i++){
        for(int j=0; j<costs[i].size(); j++){
            cout << costs[i][j] << "\t";
        }
        if(markedRows[i])
            cout << "x\t";
        cout << endl;
    }
    for(int i=0; i<costs.size(); i++){
        if(markedCols[i])
            cout << "x";
        cout << "\t";
    }
    cout << endl;
}
void initialSubtraction(vector<vector<double>> &costs, unsigned long width){


    for(int row = 0; row<width; row++){
        double rowMin = maxDist;
        int minIndex = -1;
        for(int col = 0; col<width; col++){
            if(costs[row][col] < rowMin){
                rowMin = costs[row][col];
                minIndex = col;
            }
        }

        for(int col = 0; col<width; col++){
            costs[row][col] -= rowMin;
        }
    }

    //Subtract min from each column
    for(int col = 0; col<width; col++){
        double colMin = maxDist;
        int minIndex = -1;
        for(int row = 0; row<width; row++){
            if(costs[row][col] < colMin) {
                colMin = costs[row][col];
                minIndex = row;
            }
        }

        for(int row = 0; row<width; row++){
            costs[row][col] -=colMin;
        }
    }
}

int solve(vector<vector<double>> &costs, unsigned long width, vector<bool> &markedRows, vector<bool> &markedCols, vector<Point> &zeroes){

    vector<vector<bool>> selected;
    vector<int> rowSelected(width, 0);
    vector<int> colSelected(width, 0);
    for(int i=0; i<width; i++){
        vector<bool> v(width, false);
        selected.push_back(v);
    }

    for(int row = 0; row<width; row++){
        for(int col = 0; col<width; col++){
            if(costs[row][col] == 0){
                zeroes.push_back(make_pair(row, col));
            }
        }
    }

    //Assign initial zeroes
    for(int row = 0; row<width; row++){
        for(int col = 0; col<width; col++){
            if(costs[row][col] == 0 && rowSelected[row] == 0 && colSelected[col] == 0){
                selected[row][col] = true;
                rowSelected[row]++;
                colSelected[col]++;
            }
        }
    }

    //Debugging to list selected 0's
    for(int row = 0; row<width; row++) {
        for (int col = 0; col < width; col++) {
            if(selected[row][col])
                cout << "x ";
            else
                cout << "- ";
        }
        cout << endl;
    }
    cout << endl;


    //mark all rows with selected 0's
    for(int row = 0; row<width; row++){
        if (!markedRows[row]) {
            if(rowSelected[row] == 0){
                markedRows[row] = true;
            }
            //Mark all columns with zeroes in the row
            for(int col = 0; col<width; col++){
                if(costs[row][col] == 0){
                    markedCols[col] = true;
                    //Mark all rows with selected 0's in column
                    for(int r2 =0; r2<width; r2++){
                        if(selected[r2][col]){
                            markedRows[r2] == true;
                        }
                    }
                }
            }
        }
    }

    //draw lines through each marked column and unmarked row
    for(int row = 0; row<width; row++){
        markedRows[row] == !markedRows[row];
    }

    //sum of lines
    int markedCount = 0;
    for(int i=0; i<width; i++){
        if(markedRows[i])
            markedCount++;
        if(markedCols[i])
            markedCount++;
    }

    if(markedCount == width){
        return markedCount;
    } else {
        return markedCount;
    }

}

int main(int argc, char *argv[]){

    //Readin Data
    unsigned long width;
    vector<vector<double>> costs;
    vector<int> rowZeroes;
    vector<int> colZeroes;
    vector<Point> zeroes;

    ifstream fin;
    fin.open(argv[1]);

    fin >> width;

    vector<bool> markedRows(width, false);
    vector<bool> markedCols(width, false);

    //Read in cost table
    for(int i=0; i< width; i++){
        int connectionCount;
        fin >> connectionCount;
        vector<double> connectionCosts;
        for(int j=0; j<connectionCount; j++){
            double cost;
            fin >> cost;
            connectionCosts.push_back(cost);
        }
        costs.push_back(connectionCosts);
    }

    initialSubtraction(costs, width);
    printCosts(costs, markedRows, markedCols);

    solve(costs, width, markedRows, markedCols, zeroes);

    printCosts(costs, markedRows, markedCols);


    return 0;
}