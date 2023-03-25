#include <xapian.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <time.h>
// #include "common.h"

using namespace std;


// word_tokenize: tokenize a string into words
void word_tokenize(const std::string &s, std::vector<std::string> *tokens, std::vector<std::string> stop_words) {
    std::istringstream iss(s);
    std::string token;
    while(iss >> token) {
        std::vector<std::string>::iterator it = std::find(stop_words.begin(), stop_words.end(), token);

        // Check if the element was found
        if (it != stop_words.end()) {
            continue;
        }
        tokens->push_back(token);
    }
}

 
int main(int argc, char **argv) {
    if(argc < 3) {
        cerr << "Usage: <dataset_metadata> <idxname> <data_size>" << endl;
        exit(0);
    }
    int max_length = 50;
 
    char *idxname = argv[2];
    Xapian::WritableDatabase db(idxname, Xapian::DB_CREATE_OR_OPEN);
    Xapian::Document doc;
 
    std::ifstream data_file(argv[1]);  // Open the file for reading
    std::vector<std::string> words;     // Create a vector to hold the lines of the file

    // measure time
    clock_t start, end;
    start = clock();

    int data_size = stoi(argv[3]);
    int data_count = 0;

    std::string line;
    while (std::getline(data_file, line)) {  // Read each line of the file
        if(data_count > data_size) {
            break;
        }
        words.push_back(line);          // Add the line to the vector
        data_count++;
    }

    std::ifstream stopword_file("stopwords.txt");  // Open the file for reading
    std::vector<std::string> stop_words;     // Create a vector to hold the lines of the file


    std::string word;
    while (std::getline(stopword_file, word)) {  // Read each line of the file
        stop_words.push_back(word);          // Add the line to the vector
    }
 
    long lineno = 0;
    for(vector<string>::iterator it = words.begin(); 
                                 it != words.end(); 
                                 it++, lineno++) {
        // cout << "line " << lineno << endl;
        string &s = *it;
 
        // perform tokenization
        vector<string> tokens;
        word_tokenize(s, &tokens, stop_words);
 
        // populate the document
        // value[0] will be the original line
        doc.clear_terms();
        doc.clear_values();
        doc.add_value(0, string(s));
        // document is the bag of terms
        for(vector<string>::iterator it_token=tokens.begin(); 
            it_token != tokens.end(); 
            it_token++) {
            string &token = *it_token;
            if (token.size() > max_length) {
                continue;
            }

            vector<string>::iterator it = find(stop_words.begin(), stop_words.end(), token);
            // cout << token << " ";
            doc.add_term(token);
        }
        db.add_document(doc);

        if(lineno % 1000 == 0) {
            db.commit();
        }
    }
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Time taken by program is : " << fixed << time_taken << setprecision(5);
}