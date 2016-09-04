#include "HungarianAlgorithm.h"

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

double printMatches(vector<Point> matches, vector<vector<double>> &costs){
    double totalCost = 0;

    cout << "Matches:" << endl;
    for(int i=0; i<matches.size(); i++){
        int man = matches[i].first;
        int woman = matches[i].second;
        double cost = costs[man][woman];
        totalCost += cost;
        cout << man << " " << woman << " Cost: " << cost << endl;
    }
    //cout << "Total Cost: " << totalCost << endl;
    //cout << "Average Cost: " << totalCost / matches.size() << endl;
    return totalCost;
}
void initialSubtraction(vector<vector<double>> &costs, unsigned long width){

    for(int row = 0; row<width; row++){
        double rowMin = maxDist;
        for(int col = 0; col<width; col++){
            if(costs[row][col] < rowMin){
                rowMin = costs[row][col];
            }
        }

        for(int col = 0; col<width; col++){
            costs[row][col] -= rowMin;
        }
    }

    //Subtract min from each column
    for(int col = 0; col<width; col++){
        double colMin = maxDist;
        for(int row = 0; row<width; row++){
            if(costs[row][col] < colMin) {
                colMin = costs[row][col];
            }
        }

        for(int row = 0; row<width; row++){
            costs[row][col] -=colMin;
        }
    }
}

int solve(vector<vector<double>> &costs, unsigned long width, vector<bool> &markedRows, vector<bool> &markedCols){

    vector<vector<bool>> selected;
    vector<int> rowSelected(width, 0);
    vector<int> colSelected(width, 0);
    for(int i=0; i<width; i++){
        vector<bool> v(width, false);
        selected.push_back(v);
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
            //Mark rows with no selected 0's
            if(rowSelected[row] == 0){
                markedRows[row] = true;
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
    }

    //draw lines through each marked column and unmarked row
    for(int row = 0; row<width; row++){
        markedRows[row] = !markedRows[row];
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

//Checks if there are any other matched points on a given column, besides the given row position
//returns true if there are conflicting matches in the row
bool checkCol(vector<int> rowMatches, int checkRow, int col, unsigned long width){
    for(int row =0; row <width; row++){
        if(row != checkRow){
            if(rowMatches[row] == col)
                return true;
        }
    }
    return false;
}
//given a modified set of costs that is determined to contain a possible matching, find a set of valid matches
vector<Point> match(vector<vector<double>> &costs, unsigned long width){
    vector<vector<bool>> isMatched;
    for(int i=0; i<width; i++){
        vector<bool> v(width, false);
        isMatched.push_back(v);
    }
    vector<Point> matches;
    //Store the index of the selected match in each row to speedup calculations
    vector<int> rowMatches(width, -1);

    //Precompute available 0 locations in each row
    //In the worst case this does not affect running time, but significanly improves it in average cases
    //where 0s are sparse
    vector<vector<int>> rowZeroes;
    for(int row=0; row<width; row++){
        vector<int> v;
        for(int col=0; col<width; col++){
            if(costs[row][col] == 0)
                v.push_back(col);
        }
        rowZeroes.push_back(v);
    }

    //Initially selects the 1st 0 in every row as matched
    for(int row = 0; row<width; row++){
        bool rowMatched = false;
        for(int col = 0; col<width && !rowMatched; col++){
            if(costs[row][col] == 0){
                isMatched[row][col] = true;
                rowMatched = true;
                rowMatches[row] = col;
            }
        }
    }

    //Refines the matches until a valid set of matches is reached
    for(bool update = true; update;){
        update = false;
        for(int row = 0; row<width; row++){
            //If the current match for the row is invalid
            if(checkCol(rowMatches, row, rowMatches[row], width)){
                int current = rowMatches[row];
                bool moved = false;
                for(int i=0; i<rowZeroes[row].size() && !moved; i++) {
                    if (current != rowZeroes[row][i]){
                        //if other column is unoccupied
                        if (!checkCol(rowMatches, row, rowZeroes[row][i], width)) {
                            rowMatches[row] = rowZeroes[row][i];
                            moved = true;
                            update = true;
                        }
                    }
                }
            }
        }

    }

    //Assign match pairs from rowMatches
    for(int row = 0; row<width; row++){
        matches.push_back(Point(row, rowMatches[row]));
    }

    return matches;

}


double runHungarianLong(int argc, char *argv[]) {

    //Readin Data
    unsigned long width;
    vector<vector<double>> costs;

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
    vector<vector<double>> originalCosts(costs);

    initialSubtraction(costs, width);
    printCosts(costs, markedRows, markedCols);

    solve(costs, width, markedRows, markedCols);

    printCosts(costs, markedRows, markedCols);

    vector<Point> matches = match(costs, width);
    double totalCost = printMatches(matches, originalCosts);

    return totalCost;
}

double runHungarian(string dataFile) {

    //Readin Data
    unsigned long width;
    vector<vector<double>> costs;

    ifstream fin;
    fin.open(dataFile);

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
    vector<vector<double>> originalCosts(costs);

    initialSubtraction(costs, width);
    printCosts(costs, markedRows, markedCols);

    solve(costs, width, markedRows, markedCols);

    printCosts(costs, markedRows, markedCols);

    vector<Point> matches = match(costs, width);
    double totalCost = printMatches(matches, originalCosts);

    return totalCost;
}