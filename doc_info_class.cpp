#include "utility.h"

struct WordInfo
{
    string inwhich_file;
    // this map will have the word as key and the "pair<number of occurences and index of occurances of the word>" as value
    
    // this keeps all the info of words in a document (indexes and number of occurances both)
    
    //map<string, pair<int, vector<int>>> word_places;
    vector<int> index_in_doc;
    // custom constructor
    WordInfo(string _fname = "none") // initially the word count will be 1
    :inwhich_file(_fname)
    {

    }

    vector<int>&  get_index_vector()
    {
        return this->index_in_doc;
    }

    void set_filename(string &s)
    {
        this->inwhich_file = s;
    }

    const string& get_filename()
    {
        return this->inwhich_file;
    }
    
    int word_frequency()
    {
        return index_in_doc.size();
    }

};