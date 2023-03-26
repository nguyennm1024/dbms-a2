#include <xapian.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <time.h>
#include <regex>

using namespace std;

std::string highlight(const std::string& input, const std::string& pattern, const std::string& replacement) {
    std::string openingTag = "==";
    std::string closingTag = "==";
    std::regex regexPattern(pattern);
    std::string highlighted = std::regex_replace(input, regexPattern, openingTag + replacement + closingTag);
    return highlighted;
}

int main(int argc, char const *argv[])
{
    Xapian::Database db(argv[1]);
    std::vector<string> and_terms;
    std::vector<string> or_terms;
    for(int i = 3; i < argc; i++)
        if (argv[i][0] == '+') {
            and_terms.push_back(string(argv[i]+1));
            // cout << string(argv[i]) << endl;
        }
        else {
            // cout << string(argv[i]) << endl;
            or_terms.push_back(string(argv[i]));
        }
        // terms.push_back(string(argv[i]));

    // terms.push_back(string(argv[1]));

    // time measure
    clock_t start, end;
    start = clock();
    Xapian::Query or_query(
        Xapian::Query::OP_OR,
        or_terms.begin(),
        or_terms.end()
    );
    Xapian::Enquire enquire(db);
    enquire.set_query(or_query);
    Xapian::MSet matches = enquire.get_mset(0, stoi(argv[2]));
    end = clock();
    double time_taken_or = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "Or query" << endl;
    std::cout << matches.size() << std::endl;
    for(Xapian::MSetIterator match = matches.begin(); 
    match != matches.end();
    match ++) {
    
        Xapian::Document doc = match.get_document();
    
        std::string value0 = doc.get_value(0);
        for (int i = 0; i < or_terms.size(); i++) {
            value0 = highlight(value0, or_terms[i], or_terms[i]);
        }
        cout << value0 << endl;
        cout << endl;
    }

    cout << "====================" << endl;
    cout << "And query" << endl;

    start = clock();
    Xapian::Query and_query(
        Xapian::Query::OP_AND,
        and_terms.begin(),
        and_terms.end()
    );
    // Xapian::Enquire enquire2(db);
    enquire.set_query(and_query);
    matches = enquire.get_mset(0, stoi(argv[2]));
    end = clock();
    double time_taken_and = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << matches.size() << std::endl;
    for(Xapian::MSetIterator match = matches.begin(); 
    match != matches.end();
    match ++) {
    
        Xapian::Document doc = match.get_document();
    
        std::string value0 = doc.get_value(0);
        for (int i = 0; i < and_terms.size(); i++) {
            value0 = highlight(value0, and_terms[i], and_terms[i]);
        }
        cout << value0 << endl;
        cout << endl;
        }

    cout << "Time taken by or query is : " << fixed << time_taken_or <<endl;
    cout << "Time taken by and query is : " << fixed << time_taken_and <<endl;
    
    return 0;
}
