#include <iostream>
#include <queue>
#include <string>

using namespace std;

struct Data{                                                // items' information
    int weight;
    int profit;
};

bool valid(int N, int W, Data item[]);                      // check if there is at least one item that can be included in the knapsack
bool include_in_L(string &S, queue<string> &L);             // check if a solution is already in the list
int Quality(string &S, Data item[]);                        // calculate the total profit of a solution
int totalWeight(string &S, Data item[]);                    // calculate the total weight of a solution
string Tweak(string &S, int W, Data item[]);                // generate a new solution by random flipping a bit in the current solution
void print_result(string &Best, Data item[]);               // print the best solution

int main(){
    int N, W;                                               // number of items and the maximum weight of the knapsack
    cin >> N >> W;
    Data item[N];

    int l = INT_MAX, n = 30;                                // max size of the list and number of iterations

    string S(N, '0');                                       // initial solution (all items are excluded)
    string Best = S;                                        // initial best solution

    queue<string> L;                                        // a tabu list of maximum length l
    L.push(S);                                              // add the initial solution to the list

    for(int i = 0; i < N; i++){                             // read the items' information
        int w, p;
        cin >> w >> p;
        item[i].weight = w;
        item[i].profit = p;
    }

    for(int i = 0; i < 1000; i++){                          // the iteration number is set to 1000
        if(!valid(N, W, item))                              // if there is no item that can be included in the knapsack, break
            break;
        if(L.size() > l)                                    // if the list size exceeds the maximum size, remove the oldest solution
            L.pop();

        string R = Tweak(S, W, item);                       // generate a new solution by flipping a bit in the current solution
        for(int j = 0; j < n - 1; j++){                     // try n-1 times to find a better solution
            string ω = Tweak(S, W, item);
            if(!include_in_L(ω, L) && (Quality(ω, item) > Quality(R, item) || include_in_L(R, L))){
                R = ω;
            }
        }
        if(!include_in_L(R, L)){                            // if the new solution is not in the list, add it to the list
            S = R;                                          // update the current solution
            L.push(R);
        }
        if(Quality(S, item) > Quality(Best, item)){
            Best = S;                                       // update the best solution if the current solution is better
        }
    }

    print_result(Best, item);                               // print the best solution

    return 0;
}

bool valid(int N, int W, Data item[]){
    for(int i = 0; i < N; i++){
        if(item[i].weight <= W){
            return true;
        }
    }
    return false;
}

bool include_in_L(string &S, queue<string> &L){
    queue<string> temp = L;
    while(!temp.empty()){
        if(temp.front() == S)
            return true;
        temp.pop();
    }
    return false;
}

int Quality(string &S, Data item[]){
    int profit = 0;
    for(int i = 0; i < S.size(); i++){
        if(S[i] == '1'){
            profit += item[i].profit;
        }
    }
    return profit;
}

int totalWeight(string &S, Data item[]){
    int weight = 0;
    for (int i = 0; i < S.size(); i++) {
        if (S[i] == '1') {
            weight += item[i].weight;
        }
    }
    return weight;
}

string Tweak(string &S, int W, Data item[]){
    string new_S;
    do{
        new_S = S;
        int index = rand() % S.size();
        if(new_S[index] == '0')
            new_S[index] = '1';
        else
            new_S[index] = '0';
    }while(totalWeight(new_S, item) > W);

    return new_S;
}

void print_result(string &Best, Data item[]){
    cout << "\033[1;32m" << "max profit: " << "\033[1;31m" << Quality(Best, item) << endl << "\033[0m";
    cout << "\033[1;32m" << "solution: ";
    for(int i = 0; i < Best.size(); i++){
        if(Best[i] == '1')
            cout << "\033[1;31m" << Best[i] << "\033[0m";
        else
            cout << "\033[33m" << Best[i] << "\033[0m";
    }
}