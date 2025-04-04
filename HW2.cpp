#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <cmath>
using namespace std;

set<tuple<int, int, int>> const defined_paths(vector<tuple<int, int, int>> given_paths){
    set<tuple<int, int, int>> defined_paths;
    for (auto path : given_paths){
        defined_paths.insert(path);
    }
    return defined_paths;
}

vector <int> const text2dec(string &text){
    vector<int> required_trans;
    
    for (int i=0 ; i< text.length()-2; i+=3){
	    int b2 = (text[i]  == '1')? 	4 : 0;
	    int b1 = (text[i+1] == '1')?	2 : 0;
	    int b0 = (text[i+2] == '1')?	1 : 0;
        required_trans.push_back(b2 + b1 + b0);
    }
    return required_trans;
}

vector<vector<int>> gen_all_string(vector<int> required_trans, int state){
    int all_trans = pow(state, required_trans.size()+ 1) ;
    vector<vector<int>> all_strings;
    vector<int> one_string;
    for (int j=0; j < all_trans; j++){
        int remainder = j;
        for (int i=8; i>=0; i--){
            int quotient = remainder / static_cast<int>(pow(state, i));
            remainder = remainder % static_cast<int>(pow(state, i));
            one_string.push_back(quotient);
        };
        all_strings.push_back(one_string);
        one_string.clear();
    };
    return all_strings;
}

vector<vector<int>> gen_all_path(vector<vector<int>> all_strings, vector<int> required_trans){
    vector<vector<int>> all_path;
    vector<int> one_path;
    for (auto one_string : all_strings){
        for (int i=0; i< one_string.size()-1; i++){
            one_path.push_back(one_string[i]);
            one_path.push_back(required_trans[i]);
        }
        one_path.push_back(one_string[one_string.size()-1]);
        all_path.push_back(one_path);
        one_path.clear();
    };
    return all_path;
}

set<tuple<int, int, int>> make_tuple_set(vector<int> path){
    set<tuple<int, int, int>> path_set;
    for (int i=0; i<path.size()-2 ; i+=2){
        int b0 = path[i];
        int b1 = path[i+1];
        int b2 = path[i+2];
        path_set.insert(make_tuple(b0, b1, b2));
    };

    return path_set;
}

vector<vector<tuple<int, int, int>>> extra_path_num(
    set<tuple<int, int, int>> path_set,
    set<tuple<int, int, int>> defined_paths_set
){
    vector<tuple<int, int, int>> used_extra_path;
    vector<tuple<int, int, int>> used_defined_paths;
    vector<vector<tuple<int, int, int>>> all_used_paths;
    for (auto path : defined_paths_set){
        int erase_num = path_set.erase(path);
        if (erase_num !=0){
            used_defined_paths.push_back(path);
        }
    };
    for (auto path : path_set){
        used_extra_path.push_back(path);
    };
    all_used_paths.push_back(used_extra_path);
    all_used_paths.push_back(used_defined_paths);
    return all_used_paths;
}


int main(){
    //string text = "001010010101100001110110";
    string text = "111010000100110101110000";
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

    vector<vector<int>> all_pos = gen_all_string(required_trans, 4);
    vector<vector<int>> all_path = gen_all_path(all_pos, required_trans);
    int min_extra_num = 8;
    for (auto one_path : all_path){
        set<tuple<int, int, int>> path_set = make_tuple_set(one_path);
        vector<vector<tuple<int, int, int>>> all_used_paths = extra_path_num(path_set,defined_paths_set);
        int extra_num = all_used_paths[0].size();
        if (extra_num < min_extra_num){
            min_extra_num = extra_num;
            for (auto path : all_used_paths[0]){
                cout << "extra_path: " << get<0>(path) << " " << get<1>(path) << " " << get<2>(path) << endl;
            };
            for (auto path : all_used_paths[1]){
                cout << "used_defined_path: " << get<0>(path) << " " << get<1>(path) << " " << get<2>(path) << endl;
            };
            cout << endl;
        };
    };
    cout << "extra_num: " << min_extra_num << endl;
    return 0;
}
