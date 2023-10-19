#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using std::cerr;
using std::endl;
using std::ofstream;
#include <cstdlib>

using namespace std;

//Always taken
int predictor_1(vector<string> b){
    int correct = 0;
    for(int i = 0; i < b.size(); i++){
        if(b.at(i) == "T"){
            correct++;
        }
    }
    return correct;
}

//Always not taken
int predictor_2(vector<string> b){
    int correct = 0;
    for(int i = 0; i < b.size(); i++){
        if(b.at(i) != "T"){
            correct++;
        }
    }
    return correct;
}

//bimodal single
int predictor_3(int table_size, vector<long> a, vector<string> b){
    int correct = 0;
    vector<int> predictions;
    int p = 0;
    int index = 0;
    for(int i = 0; i < table_size; i++){
        predictions.push_back(1);
    }
    for(int i = 0; i < a.size(); i++){
        index = a.at(i) % table_size;
        p = predictions.at(index);
        if(p == 1){
            if(b.at(i) == "T"){
                correct++;
            }
            else{
                predictions.at(index) = 0;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions.at(index) = 1;
            }
            else{
                correct++;
            }
        }
    }
    return correct;
}

//bimodal double
int predictor_4(int table_size, vector<long> a, vector<string> b){
    int correct = 0;
    vector<int> predictions;
    int p = 0;
    int index = 0;
    for(int i = 0; i < table_size; i++){
        predictions.push_back(3);
    }
    for(int i = 0; i < a.size(); i++){
        index = a.at(i) % table_size;
        p = predictions.at(index);
        if(p >= 2){
            if(b.at(i) == "T"){
                correct++;
                predictions.at(index) = 3;
            }
            else{
                predictions.at(index)--;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions.at(index)++;
            }
            else{
                correct++;
                predictions.at(index) = 0;
            }
        }
    }
    return correct;
}

//gshare
int predictor_5(int history_length, vector<long> a, vector<string> b){
    int correct = 0;
    int table_size = 2048;
    vector<int> predictions;
    long history = 0;
    int p = 0;
    long index = 0;
    long xor_index = 0;
    int correct_behavior = 0;
    for(int i = 0; i < table_size; i++){
        predictions.push_back(3);
    }
    for(long i = 0; i < a.size(); i++){
        //cout << i << " " << a.size() << " " << b.size() << " " << index << " " << predictions.size() << endl;
        xor_index = a.at(i) ^ history;
        index =  xor_index % table_size;
        //cout << i << " " << a.size() << " " << b.size() << " " << index << " " << predictions.size() << endl;
        p = predictions.at(index);
        if(p >= 2){
            if(b.at(i) == "T"){
                correct++;
                predictions.at(index) = 3;
                correct_behavior = 1;
            }
            else{
                predictions.at(index)--;
                correct_behavior = 0;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions.at(index)++;
                correct_behavior = 1;
            }
            else{
                correct++;
                predictions.at(index) = 0;
                correct_behavior = 0;
            }
        }
        history = (long)history << 1;
        history = (long)history | correct_behavior;
        history = (long)history & (long)pow(2, history_length) - 1;
    }
    
    return correct;
}

//tournament
int predictor_6(vector<long> a, vector<string> b){
    int correct = 0;
    int table_size = 2048;
    int history_length = 11;

    vector<int> selectors;
    vector<int> predictions_bimodal;
    vector<int> predictions_gshare;

    int bimodal_behavior = 0;
    int gshare_behavior = 0;

    int p_bimodal = 0;
    int p_gshare = 0;
    int index_bimodal = 0;
    int index_gshare = 0;
    long xor_index = 0;

    long history = 0;
    int correct_behavior = 0;

    int p_final = 0;

    for(int i = 0; i < table_size; i++){
        selectors.push_back(0);
        predictions_bimodal.push_back(3);
        predictions_gshare.push_back(3);
    }

    for(int i = 0; i < a.size(); i++){
        index_bimodal = a.at(i) % table_size;
        p_bimodal = predictions_bimodal.at(index_bimodal);

        index_bimodal = a.at(i) % table_size;
        p_bimodal = predictions_bimodal.at(index_bimodal);
        if(p_bimodal >= 2){
            if(b.at(i) == "T"){
                //correct++;
                predictions_bimodal.at(index_bimodal) = 3;
            }
            else{
                predictions_bimodal.at(index_bimodal)--;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions_bimodal.at(index_bimodal)++;
            }
            else{
                //correct++;
                predictions_bimodal.at(index_bimodal) = 0;
            }
        }

        xor_index = a.at(i) ^ history;
        index_gshare =  xor_index % table_size;
        p_gshare = predictions_gshare.at(index_gshare);

        if(p_gshare >= 2){
            if(b.at(i) == "T"){
                //correct++;
                predictions_gshare.at(index_gshare) = 3;
                correct_behavior = 1;
            }
            else{
                predictions_gshare.at(index_gshare)--;
                correct_behavior = 0;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions_gshare.at(index_gshare)++;
                correct_behavior = 1;
            }
            else{
                //correct++;
                predictions_gshare.at(index_gshare) = 0;
                correct_behavior = 0;
            }
        }

        if(p_bimodal == p_gshare){
            p_final = p_bimodal;
            if(p_final >= 2){
                if(b.at(i) == "T"){
                    correct++;
                }
            }
            else{
                if(b.at(i) != "T"){
                    correct++;
                }
            }
        }
        else{
            //false -> gshare
            //true -> bimodal
            bool which_one = false;
            if(selectors.at(index_bimodal) >= 2){
                p_final = p_bimodal;
                which_one = true;
            }
            else{
                p_final = p_gshare;
            }
            if(p_final >= 2){
                if(b.at(i) == "T"){
                    correct++;
                    if(which_one){
                        selectors.at(index_bimodal) = 3;
                    }
                    else{
                        selectors.at(index_bimodal) = 0;
                    }
                }
                else{
                    if(which_one){
                        selectors.at(index_bimodal)--;
                    }
                    else{
                        selectors.at(index_bimodal)++;
                    }
                }
            }
            else{
                if(b.at(i) == "T"){
                    if(which_one){
                        selectors.at(index_bimodal)++;
                    }
                    else{
                        selectors.at(index_bimodal)--;
                    }
                }
                else{
                    correct++;
                    if(which_one){
                        selectors.at(index_bimodal) = 3;
                    }
                    else{
                        selectors.at(index_bimodal) = 0;
                    }
                }
            }
        }

        history = (long)history << 1;
        history = (long)history | correct_behavior;
        history = (long)history & (long)pow(2, history_length) - 1;
    }
    
    return correct;
}

int predictor_6_2(vector<long> a, vector<string> b){
    int correct = 0;
    int table_size = 2048;
    int history_length = 11;

    vector<int> selectors;
    vector<int> predictions_bimodal;
    vector<int> predictions_gshare;

    int bimodal_behavior = 0;
    int gshare_behavior = 0;

    int p_bimodal = 0;
    int p_gshare = 0;
    int index_bimodal = 0;
    int index_gshare = 0;
    int xor_index = 0;

    int history = 0;
    int correct_behavior = 0;

    int p_final = 0;

    for(int i = 0; i < table_size; i++){
        selectors.push_back(0);
        predictions_bimodal.push_back(3);
        predictions_gshare.push_back(3);
    }

    for(int i = 0; i < a.size(); i++){
        index_bimodal = a.at(i) % table_size;
        p_bimodal = predictions_bimodal.at(index_bimodal);

        if(p_bimodal >= 2){
            if(b.at(i) == "T"){
                //correct++;
                predictions_bimodal.at(index_bimodal) = 3;
            }
            else{
                predictions_bimodal.at(index_bimodal)--;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions_bimodal.at(index_bimodal)++;
            }
            else{
                //correct++;
                predictions_bimodal.at(index_bimodal) = 0;
            }
        }

        xor_index = a.at(i) ^ history;
        index_gshare =  xor_index % table_size;
        p_gshare = predictions_gshare.at(index_gshare);

        if(p_gshare >= 2){
            if(b.at(i) == "T"){
                //correct++;
                predictions_gshare.at(index_gshare) = 3;
                correct_behavior = 1;
            }
            else{
                predictions_gshare.at(index_gshare)--;
                correct_behavior = 0;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions_gshare.at(index_gshare)++;
                correct_behavior = 1;
            }
            else{
                //correct++;
                predictions_gshare.at(index_gshare) = 0;
                correct_behavior = 0;
            }
        }

        if(p_bimodal == p_gshare){
            p_final = p_bimodal;
            if(p_final >= 2){
                if(b.at(i) == "T"){
                    correct++;
                }
            }
            else{
                if(b.at(i) != "T"){
                    correct++;
                }
            }
        }
        else{
            //false -> gshare
            //true -> bimodal
            bool which_one = false;
            if(selectors.at(index_bimodal) >= 2){
                p_final = p_bimodal;
                which_one = true;
            }
            else{
                p_final = p_gshare;
            }
            if(p_final >= 2){
                if(b.at(i) == "T"){
                    correct++;
                    if(which_one){
                        selectors.at(index_bimodal) = 3;
                    }
                    else{
                        selectors.at(index_bimodal) = 0;
                    }
                }
                else{
                    if(which_one){
                        selectors.at(index_bimodal)--;
                    }
                    else{
                        selectors.at(index_bimodal)++;
                    }
                }
            }
            else{
                if(b.at(i) == "T"){
                    if(which_one){
                        selectors.at(index_bimodal)++;
                    }
                    else{
                        selectors.at(index_bimodal)--;
                    }
                }
                else{
                    correct++;
                    if(which_one){
                        selectors.at(index_bimodal) = 3;
                    }
                    else{
                        selectors.at(index_bimodal) = 0;
                    }
                }
            }
        }

        history = (int)history << 1;
        history = (int)history | correct_behavior;
        history = (int)history & (int)pow(2, history_length) - 1;
    }
    
    return correct;
}

//BTB
pair<int, int> predictor_7(vector<long> a, vector<string> b, vector<long> t){
    int correct = 0;
    int total = 0;
    int table_size = 512;
    vector<int> predictions;
    vector<int> btb;
    int p = 0;
    int index = 0;
    for(int i = 0; i < table_size; i++){
        predictions.push_back(1);
        btb.push_back(0);
    }
    for(int i = 0; i < a.size(); i++){
        index = a.at(i) % table_size;
        p = predictions.at(index);
        if(p == 1){
            total++;
            //index = t.at(i) % table_size;
            p = btb.at(index);
            if(b.at(i) == "T"){
                btb.at(index) = 1;
                correct++;
            }
            else{
                predictions.at(index) = 0;
            }
        }
        else{
            if(b.at(i) == "T"){
                predictions.at(index) = 1;
            }
            else{
                //correct++;
            }
        }
    }
    //cout << correct << ",";
    pair<int, int> ret (correct, total);
    return ret;
}

int main(int argc, char *argv[]) {

    unsigned long addr;
    unsigned long target;
    string behavior;

    vector<long> addresses;
    vector<long> targets;
    vector<string> behaviors;

    ifstream infile(argv[1]);

    while(infile >> std::hex >> addr >> behavior >> std::hex >> target){
        /*
        cout << addr;
        if(behavior == "T"){
            cout << " -> taken, ";
        }
        else{
            cout << " -> not taken, ";
        }
        cout << "target = " << target << endl;
        */
        addresses.push_back(addr);
        targets.push_back(target);
        behaviors.push_back(behavior);
    }

    ofstream outdata;
    outdata.open(argv[2]);
    outdata << predictor_1(behaviors) << "," << behaviors.size() << ";" << endl;
    outdata << predictor_2(behaviors) << "," << behaviors.size() << ";" << endl;
    outdata << predictor_3(16, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_3(32, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_3(128, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_3(256, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_3(512, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_3(1024, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_3(2048, addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    outdata << predictor_4(16, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_4(32, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_4(128, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_4(256, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_4(512, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_4(1024, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_4(2048, addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    outdata << predictor_5(3, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(4, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(5, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(6, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(7, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(8, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(9, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(10, addresses, behaviors) << "," << behaviors.size() << "; ";
    outdata << predictor_5(11, addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    outdata << predictor_6(addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    pair<int, int> p7 = predictor_7(addresses, behaviors, targets);
    outdata << p7.first << "," << p7.second << ";" << endl;

    //cout << std::hex << addresses.at(1) << " " << behaviors.at(1) << " " << targets.at(1) << endl;
    /*
    cout << predictor_1(behaviors) << "," << behaviors.size() << ";" << endl;
    cout << predictor_2(behaviors) << "," << behaviors.size() << ";" << endl;
    cout << predictor_3(16, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_3(32, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_3(128, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_3(256, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_3(512, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_3(1024, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_3(2048, addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    cout << predictor_4(16, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_4(32, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_4(128, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_4(256, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_4(512, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_4(1024, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_4(2048, addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    cout << predictor_5(3, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(4, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(5, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(6, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(7, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(8, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(9, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(10, addresses, behaviors) << "," << behaviors.size() << "; ";
    cout << predictor_5(11, addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    cout << predictor_6(addresses, behaviors) << "," << behaviors.size() << ";" << endl;
    cout << predictor_7(addresses, behaviors, targets) << ";" << endl;
    */
    return 0;

}






































