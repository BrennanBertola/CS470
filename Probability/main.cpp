#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <map>
using namespace std;

void paradox();
void montyHall();
void risk();

int main(int argc, char** argv) {
    if(argv[1]) {
        paradox();
    }
    if (argv[2]) {
        montyHall();
    }
    if (argv[3]) {
        risk();
    }
    return 0;
}

float average(vector<int> vec) {
    float sum = 0;
    for (int i : vec) {
        sum += i;
    }
    return sum / vec.size();
}

void paradox() {
    srand(time(NULL));

    vector<int> results;
    for (unsigned int i = 0; i < 100; ++i) {
        int k = 0;
        while (rand() % 2 == 0) {
            k++;
        }
        results.push_back(pow(2, k));
    }
    float avg1 = average(results);

    results.clear();
    for (unsigned int i = 0; i < 10000; ++i) {
        int k = 0;
        while (rand() % 2 == 0) {
            k++;
        }
        results.push_back(pow(2, k));
    }
    float avg2 = average(results);

    results.clear();
    for (unsigned int i = 0; i < 1000000; ++i) {
        int k = 0;
        while (rand() % 2 == 0) {
            k++;
        }
        results.push_back(pow(2, k));
    }
    float avg3 = average(results);

    cout << "St. Petersburg Paradox:\n" << "\t100 games: " << avg1 << endl;
    cout << "\t10000 games: " << avg2 << endl;
    cout << "\t1000000 games: " << avg3 << endl << endl;
    return;
}

void montyHall() {
    cout << "Monty Hall Problem:" << endl;

    int switchWins = 0;
    int stayWins = 0;
    int it = 1;
    srand(time(NULL));

    for (int i = 0; i < 10000; ++i) {
        int carDoor = rand() % 3;
        int guess = rand() % 3;
        int remove = rand() % 3;

        while (true) {
            if (remove != carDoor && remove != guess) {
                break;
            }
            remove = rand() % 3;
        }

        // If the original was the car door, then staying wins.
        if (guess == carDoor) {
            stayWins++;
        }
        // If the original guess was wrong then switching to the only other door would win.
        else {
            switchWins++;
        }

        if (i+1 == pow(10, it)) {
            cout << "\tResults after " << pow(10,it) << " games:" << endl;
            cout << "\tTimes Switching wins: " << switchWins << endl;
            cout << "\tTimes Staying wins: " << stayWins << endl;
            cout << "\tPercentage Switching wins: " << switchWins / (i + 1.0) << endl;
            cout << "\tPercentage Staying wins: " << stayWins / (i + 1.0) << endl << endl;
            it++;
        }
    }

    return;
}

class BattleResult {
public:
    int attackStat = 0;
    int defStat = 0;
    bool attackWon = false;
    BattleResult() {}

    BattleResult(int attackStat, int defStat, bool attackWon) : attackStat(attackStat), defStat(defStat),
                                                                attackWon(attackWon) {};

    void toString() {
        cout << "Attack Loses: " << attackStat << ", Defense Loses: " << defStat << endl;
    }


};

pair<vector<int>, vector<int>> simulateRoles(int a, int d) {

    vector<int> attackRoles = vector<int>();
    for (int i = 0; i < a; ++i) {
        attackRoles.push_back(rand() % 6 +1);
    }
    sort(attackRoles.begin(), attackRoles.end(), greater<int>());

    vector<int> defRoles = vector<int>();
    for (int i = 0; i < d; ++i) {
        defRoles.push_back(rand() % 6 +1);
    }
    sort(defRoles.begin(), defRoles.end(), greater<int>());
    return pair<vector<int>, vector<int>> (attackRoles, defRoles);
}

BattleResult simulateBattle(pair<vector<int>, vector<int>> roles) {
    vector<int> aRoles = roles.first;
    vector<int> dRoles = roles.second;

    int min = dRoles.size();
    if (aRoles.size() < dRoles.size()) {
        min = aRoles.size();
    }

    BattleResult result = BattleResult();

    for (int i = 0; i < min; ++i) {
        if (aRoles.at(i) <= dRoles.at(i)) {
            result.attackStat++;
        }
        else {
            result.defStat++;
        }
    }

    return result;
}

BattleResult simulateWar(int atkSize, int defSize) {


    while(atkSize > 1 && defSize > 0) {
        int a = 3;
        int d = 2;
        if (atkSize < 4) {a = atkSize-1;}
        if (defSize < 2) {d = 1;}

        BattleResult result = simulateBattle(simulateRoles(a,d));
        atkSize -= result.attackStat;
        defSize -= result.defStat;
    }

    if (atkSize < 2) {
        return BattleResult(atkSize, defSize, false); //attack has lost the war
    }
    else {
        return BattleResult(atkSize, defSize, true); //def has lost the war
    }
}

void risk() {
    srand(time(NULL));
    cout << "Risk Problem:" << endl << endl;

    cout << "Situation 1:" << endl;
    map<pair<int, int>, vector<int>> aResults;
    map<pair<int, int>, vector<int>> dResults;

    for (int a = 1; a <=3; a++) {
        for (int d = 1; d <=2; d++) {
            aResults.insert({{a,d}, vector<int>()});
            dResults.insert({{a,d}, vector<int>()});
        }
    }

    int runs = 0;
    while (runs < 5000) {
        for (int a = 1; a <= 3; a++) {
            for (int d = 1; d <= 2; d++) {
                BattleResult result = simulateBattle(simulateRoles(a, d));
                aResults.at({a, d}).push_back(result.attackStat);
                dResults.at({a, d}).push_back(result.defStat);
            }
        }
        ++runs;
    }

    map<pair<int, int>, float> atkMonte;
    map<pair<int, int>, float> defMonte;

    for (int a = 1; a <= 3; a++) {
        for (int d = 1; d <= 2; d++) {
            atkMonte.insert({{a,d}, average(aResults.at({a,d}))});
            defMonte.insert({{a,d}, average(dResults.at({a,d}))});
        }
    }

    cout << "  Average Attack Loses Across 5000 Games (Attack Dice, Defense Dice):" << endl;
    for (int a = 1; a <= 3; a++) {
        for (int d = 1; d <= 2; d++) {
            cout << "\t(" << a << ", " << d << "): " << atkMonte.at({a,d}) << endl;
        }
    }

    cout << endl << "  Average Defense Loses Across 5000 Games (Attack Dice, Defense Dice):" << endl;
    for (int a = 1; a <= 3; a++) {
        for (int d = 1; d <= 2; d++) {
            cout << "\t(" << a << ", " << d << "): " << defMonte.at({a,d}) << endl;
        }
    }

    cout << endl << "Situation 2:" << endl;
    cout << "  Win Rate Of Attacking Army Count Across 500 Battles:" << endl;

    for (int i = 2; i <= 20; ++i) {
        runs = 0;
        vector<int> results;
        while (runs < 500) {
            if (simulateWar(i, 5).attackWon) {
                results.push_back(1);
            }
            else {
                results.push_back(0);
            }
            ++runs;
        }
        float wRate = average(results);
        cout << "\t" << i << ": " << wRate << endl;
    }

    cout << endl << "Situation 3:" << endl;

    //creat structures used to track results
    int atkCount = 0;
    int defCount = 0;
    map <int, int> atkWins;
    map <int, int> defWins;
    for (int i = 0; i <= 10; ++i) {
        atkWins.insert({i, 0});
        defWins.insert({i, 0});
    }

    runs = 0;
    while (runs < 10000) {
        BattleResult result = simulateWar(10, 10);
        if (result.attackWon) {
            ++atkCount;
            ++atkWins.at(result.attackStat);
        }
        else {
            ++defCount;
            ++defWins.at(result.defStat);
        }
        ++runs;
    }

    cout << "  Remaining Army Counts For Attack Victories:" << endl;
    cout << "\tAttack won a total of " << atkCount << " times" << endl;
    for (int i = 2; i <=10; ++i) {
        cout << "\t" << i << ": " << atkWins.at(i) / (atkCount + 0.0) << endl;
    }
    cout << endl << "  Remaining Army Counts For Defense Victories:" << endl;
    cout << "\tDefense won a total of " << defCount << " times" << endl;
    for (int i = 1; i <=10; ++i) {
        cout << "\t" << i << ": " << defWins.at(i) / (defCount + 0.0) << endl;
    }

    return;
}