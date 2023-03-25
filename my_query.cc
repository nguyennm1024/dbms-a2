#include <xapian.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[])
{
    Xapian::Database db(argv[1]);
    std::vector<string> terms;
    for(int i = 2; i < argc; i++)
        terms.push_back(string(argv[i]));

    terms.push_back(string(argv[1]));
    Xapian::Query query(
        Xapian::Query::OP_OR,
        terms.begin(),
        terms.end()
    );
    Xapian::Enquire enquire(db);
    enquire.set_query(query);
    Xapian::MSet matches = enquire.get_mset(0, stoi(argv[2]));
    std::cout << matches.size() << std::endl;
    for(Xapian::MSetIterator match = matches.begin(); 
    match != matches.end();
    match ++) {
    
        Xapian::Document doc = match.get_document();
    
        std::string value0 = doc.get_value(0);
        cout << value0 << endl;
        cout << endl;
    }
    return 0;
}
