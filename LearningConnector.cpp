#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

vector<int> getVectorFromString(const string &x){
    string n = "";

    for(int i=0;i<x.length();++i){
        char j = x[i];
        if(j!='[' && j!=']' && j!='\n' && j!=' '){
            n+=j;
        }
    }

    vector<int> r;
    int i;
    stringstream ss(n);
    while(ss>>i){
        r.push_back(i);
        if(ss.peek() == ','){
            ss.ignore();
        }

    }
    for (i=0; i< r.size(); i++)
    
	return r;

}

string exec(const string& mStr)
{
    string result, file;
    FILE* pipe{popen(mStr.c_str(), "r")};
    char buffer[256];

    while(fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        file = buffer;
        result += file.substr(0, file.size() - 1);
    }

    pclose(pipe);
    return result;
}

vector<int> learnFile(string filename){
    string x = exec("python3 main.py "+filename);
    vector<int> y = getVectorFromString(x);
    return y;
}

/*
int main(int argc, char** argv){
    vector<int> res = learnFile("10Sample.txt");

    for(int i:res){
        cout<<i<<endl;
    }

	//cout<<x<<endl;

}
*/
