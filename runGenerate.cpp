//Arguments: <lhsCount> <rhsCount> <file output> <location weight> <global rank weight> <random weight>
int main(int argc, char *argv[]){

    if(!(argc == 4 || argc == 7 || argc == 3 || argc == 6)){
        cerr << "Usage: generateDataFile.o <lhsCount> <rhsCount> [<location weight> <global rank weight> <random weight>] > <output file> \n";
        return 0;
    }

    srand(unsigned(time(0)));

    int lhsCount, rhsCount;
    lhsCount = atoi(argv[1]);
    rhsCount = atoi(argv[2]);

    ostream *out;
    out = &cout;

    ofstream fout;
    if(argc == 4 || argc == 7){
        fout.open(argv[argc-1]);
        out = &fout;
    }

    double locationWeight, globalRankWeight, randomWeight;
    if(argc == 7 || argc == 6){
        locationWeight = atof(argv[3]);
        globalRankWeight = atof(argv[4]);
        randomWeight = atof(argv[5]);
    } else {
        locationWeight = 1;
        globalRankWeight = 1;
        randomWeight = 1;
    }

    /*
    generateRandomData(out, lhsCount);
    out << endl;
    generateRandomData(out, lhsCount);
    */

    Instance I = generateWeightedData(lhsCount, rhsCount, locationWeight,
        globalRankWeight, randomWeight);
    print_instance(I);

    //out.close();
    return 0;
}
