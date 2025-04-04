#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <typeinfo>
#include <algorithm>
using namespace std;

vector <int> text2dec(string text){
    vector<int> required_trans;
    
    for (int i=0 ; i< text.length()-2; i+=3){
	    int b2 = (text[i]  == '1')? 	4 : 0;
	    int b1 = (text[i+1] == '1')?	2 : 0;
	    int b0 = (text[i+2] == '1')?	1 : 0;
        required_trans.push_back(b2 + b1 + b0);
    }
    return required_trans;
}

set<tuple<int, int, int>> defined_paths(vector<tuple<int, int, int>> given_paths){
    set<tuple<int, int, int>> defined_paths;
    for (auto path : given_paths){
        defined_paths.insert(path);
    }
    return defined_paths;
}

vector<tuple<int, int, int>> gen_tuple(vector<int> required_trans){
    vector<tuple<int, int, int>> all_tuples;
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            for (auto k:required_trans){
                all_tuples.push_back(make_tuple(i, k, j));
            }
        }
    }
    return all_tuples;
}

vector<vector<tuple<int, int, int>>> step_solution(vector<tuple<int, int, int>> all_tuples, vector<int> required_trans){
    vector<vector<tuple<int, int, int>>> candidate_solution;
    for (auto step:required_trans){
        vector<tuple<int, int, int>> step_solution;
        for (auto tuple:all_tuples){
            if (get<1>(tuple) == step){
                step_solution.push_back(tuple);
            }
        }
        candidate_solution.push_back(step_solution);
    }
    return candidate_solution;
}

//make chains recursively if the first element of the tuple is equal to the last element of the previous tuple
vector<vector<tuple<int, int, int>>> make_chains(vector<vector<tuple<int, int, int>>> step_solutions, vector<tuple<int, int, int>> current_path, int step){
    vector<vector<tuple<int, int, int>>> chains;
    if (step == step_solutions.size()){
        return {current_path};
    }
    
    for (auto candidate : step_solutions.at(step)){
        if (current_path.empty() || get<0>(candidate) == get<2>(current_path.back())){
            auto new_path = current_path;
            new_path.push_back(candidate);
            auto new_chains = make_chains(step_solutions, new_path, step + 1);
            cout <<"New chains is " << endl;
            for (auto chain : new_chains){
                cout << "{";
                for (auto tuple : chain){
                    cout << "(" << get<0>(tuple) << ", " << get<1>(tuple) << ", " << get<2>(tuple) << "), ";
                }
                cout << "}" << endl;
            };
        }
    }
    return chains;
}

int main(){
    string text = "001010010101100001110110";
    vector<int> required_trans;
    required_trans = text2dec(text);
    
    vector<tuple<int, int, int>> given_paths = {
        make_tuple(0, 3, 1),
        make_tuple(0, 6 ,1),
        make_tuple(0, 4, 2),
        make_tuple(1, 3, 3),
        make_tuple(2, 6, 1),
        make_tuple(2, 1, 3),
        make_tuple(2, 4, 3),
        make_tuple(3, 3, 0),
    };
    set<tuple<int, int, int>> defined_paths_set = defined_paths(given_paths);
    vector<tuple<int, int, int>> all_tuples = gen_tuple(required_trans);
    vector<vector<tuple<int, int, int>>> step_solutions = step_solution(all_tuples, required_trans);
    vector<vector<tuple<int, int, int>>> all_paths = make_chains(step_solutions, {}, 7);
    /*
    cout << "All paths:" << endl;
    for (auto path : all_paths){
        cout << "{";
        for (auto tuple : path){
            cout << "(" << get<0>(tuple) << ", " << get<1>(tuple) << ", " << get<2>(tuple) << "), ";
        }
        cout << "}" << endl;
    };
    cout << "Total number of paths: " << all_paths.size() << endl;
    */
    return 0;
}
