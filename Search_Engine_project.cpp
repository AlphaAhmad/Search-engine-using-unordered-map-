#include "utility.h"
#include "doc_info_class.cpp"

int number_of_files = 30;
int max_file_limit = 39;

bool WordInfo_comparator(const WordInfo* p1, const  WordInfo* p2)
{
    return p1->index_in_doc.size() > p2->index_in_doc.size();
}

bool WordInfo_comparator_accending(const WordInfo* p1, const  WordInfo* p2)
{
    return p1->index_in_doc.size() < p2->index_in_doc.size();
}

bool compare_for_intersection(const WordInfo* p1, const  WordInfo* p2)
{
    return p1->inwhich_file < p2->inwhich_file;
}

bool power_set_sort_comparator(vector<string>& v1, vector<string>& v2)
{
    return v1.size() < v2.size();
}




class unorderedMap_SearchEngine
{
    // this is actually our dictionary in which we are going to save words as keys and
    // all documents as in which the word occured as documents 
    unordered_map<string, vector<WordInfo*>> dict;

public:

    // check the name of the file in the vector that does the file already exists
    bool file_searcher_in_vector(const vector<WordInfo*>& vs, const string& _f, int& index)
    {
        for (int i = 0; i < vs.size(); i++)
        {
            if (vs[i]->get_filename() == _f)
            {
                index = i;
                return true;
            }
        }
        index = -1;
        return false;
    }

    // to save the created search engine to a text file
    void saveSearchEngineToFile()
    {
        ofstream wrt("Save Search Engine.txt");
        wrt << dict.size() << endl; // 1st is the size of whole dict
        for (auto f : this->dict)
        {
            wrt << f.first << endl; // 1st the word itself
            wrt << f.second.size() << endl; // then the size of vector which contains the info of the word in all files vector<WordInfo*>
            // now we are going to save the struct info (all files and the indexes of the word in those files)
            for (int i = 0; i < f.second.size(); i++)
            {
                wrt << f.second[i]->get_filename() << endl;// then name of the file
                wrt << f.second[i]->index_in_doc.size() << endl; // then size of the vector of indices
                for (int j = 0; j < f.second[i]->index_in_doc.size(); j++)
                {
                    // now printing the indexes
                    wrt << f.second[i]->index_in_doc[j] << " ";
                }
                wrt << "\n";
            }
            wrt << "\n\n";
        }
    }

    void LoadSavedSearchEngine()
    {
        ifstream rdr("Save Search Engine.txt");
        int dict_size = 0;
        rdr >> dict_size; // 1st is the size of the dict

        string word;
        int initial_vector_size = 0;
        int index_val = 0;
        string file_name;
        int index_vector_size = 0;
        for (int i = 0; i < dict_size; i++)
        {
            rdr >> word; // saving the word
            rdr >> initial_vector_size;
            vector<WordInfo*> vs;
            for (int j = 0; j < initial_vector_size; j++)
            {
                rdr >> file_name;
                rdr >> index_vector_size;
                WordInfo* wf = new WordInfo(file_name);
                for (int k = 0; k < index_vector_size; k++)
                {
                    rdr >> index_val;
                    wf->index_in_doc.push_back(index_val);
                }
                vs.push_back(wf);
            }
            dict[word] = vs;
        }
    }

    void vector_to_engine_storer(vector<string>& file_in_vector,string file_path)
    {
        for (int j = 0; j < file_in_vector.size(); j++)
        {
            // finding if the word already exists in the map or not
            auto f = dict.find(file_in_vector[j]);

            // if the word does not exists
            if (f == dict.end())
            {
                WordInfo* WI = new WordInfo(file_path); // word count defalut set to 1
                WI->get_index_vector().push_back(j); // saving index of word in the file

                // these pointers are being tored in a vector because each
                // is correspoding to only 1 file and a word can be in multiple files
                dict[file_in_vector[j]].push_back(WI);

            }// if it exists
            else
            {
                // then check that are we in the same file or its a different file
                int index = 0;
                if (file_searcher_in_vector(f->second, file_path, index))
                {
                    // coming here means that file already exists in the vector of 
                    // the word we are searching
                    // so we will only update the vector that is keeping the count
                    // and index in check
                    f->second[index]->get_index_vector().push_back(j);
                }
                else
                {
                    // coming here means ka exist nhi karti means ka new file ha
                    WordInfo* WI = new WordInfo(file_path); // word count defalut set to 1
                    WI->get_index_vector().push_back(j); // saving index of word in the file
                    dict[file_in_vector[j]].push_back(WI);
                }

            }
        }
    }

    void update_searchEngine()
    {
        number_of_files++; // goto the next fie number
        string file_path = "review_" + to_string(number_of_files) + ".txt";
        ifstream rdr(file_path); // opening the file
        if (rdr.is_open()) //agr file open ho gai ha to update kara warna na kara
        {
            vector<string>file_in_vector;
            string storer;
            while (rdr >> storer) // all words in the file are being saved in a vector
            {
                file_in_vector.push_back(storer);
            }
            vector_to_engine_storer(file_in_vector,file_path);
            saveSearchEngineToFile();
        }
    }


    // constructor
    unorderedMap_SearchEngine()
    {
        char x;
        cout << "Do you want to load the previous Engine? (y/n): ";
        cin >> x;
        system("cls");
        if (x == 'n' || x == 'N')
        {
            string file_path = " ";
            // we are only going to read from 30 files
            for (int i = 1; i <= number_of_files; i++)
            {
                file_path = "review_" + to_string(i) + ".txt";
                ifstream rdr(file_path);
                // checking is the file open or not
                if (!rdr.is_open())
                {
                    cout << "File not open\n";
                    return;
                }
                // the vector for whole file is loaded
                string s;
                vector<string>file_in_vector;

                while (rdr >> s)
                {
                    //cout << s << "  " << count << endl;
                    file_in_vector.push_back(s);
                    //count++;
                }
                //cout << "\n__________________________________________\n\n";
                // now we are going to populate this vector in the search engine map
                vector_to_engine_storer(file_in_vector,file_path);
            }
            saveSearchEngineToFile(); // saving the created engine to file
        }
        else
        {
            LoadSavedSearchEngine();
        }

    }

    vector<string> tokenizer(string command, char delimiter = '+')
    {
        stringstream ss(command);
        string token;
        vector<string> tokens;
        while (getline(ss, token, delimiter))
        {
            string_trimmer(token);
            tokens.push_back(token);
        }
        return tokens;
    }


    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //----------------  Searching functions ------------------
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    struct set_comparator
    {
        bool operator() (pair<string, int>& p1, pair<string, int>& p2)
        {
            return p1.second < p2.second;
        }
    }; // not needed for now

    // it will return the whole vector which contains the struct of all files regarding 
    // info of the word
    vector<WordInfo*>  single_word_search(const string& s)
    {
        auto x = dict.find(s); // to check wheather the word even exists or not
        if (x == dict.end())
        {
            cout << "Word: " << s << " not found in any document\n\n";
            vector<WordInfo*> null_vector;
            return null_vector;
        }
        else
        {
            //set<pair<string, int>, set_comparator> sp(x->second.begin())

            // sorting the vector on the spot and then returning it
            sort(x->second.begin(), x->second.end(), WordInfo_comparator);
            return x->second;
        }
    }

    //++++ multi word search is done in the function of +++



    // simple sentence search

    bool recursive_sentence_search_helper(string filename, int word_index_in_sentence, vector<string> sentence, int recent_word_index_in_doc)
    {
        // base case 1
        if (word_index_in_sentence >= sentence.size())
        {
            return true;
        }

        auto x = dict.find(sentence[word_index_in_sentence]);
        if (x != dict.end()) // 1st searching that even the words exits in our search engine data (dict) or not
        {
            int index = 0;
            if (file_searcher_in_vector(x->second, filename, index))  // then searching wheather the word  has a same file as previous word or not
            {
                // now we are going to search wheather the word has index next to the previous word or not in the same file
                auto curr_index = find(x->second[index]->index_in_doc.begin(), x->second[index]->index_in_doc.end(), recent_word_index_in_doc + 1);
                if (curr_index != x->second[index]->index_in_doc.end())
                {
                    //int ind = distance(x->second[index]->index_in_doc.begin(),curr_index);
                    // after all these checks, we will send another recursive call
                    if (recursive_sentence_search_helper(filename, word_index_in_sentence + 1, sentence, recent_word_index_in_doc + 1))
                    {
                        return true; // agr aga bhi ksi word ma masla nhi aya to all ok ha
                    }

                }

            }
        }

        return false;
    }

    vector<WordInfo*> sentence_search(string command)
    {
        // command string will lose its appose trophys ("") before coming here   
        vector<string> sentence = tokenizer(command, ' '); // separating on base of space
        vector<WordInfo*> result;
        auto first_word = dict.find(sentence[0]);
        if (first_word != dict.end())
        {
            for (int i = 0; i < first_word->second.size(); i++)
            {
                WordInfo* wf = new WordInfo(first_word->second[i]->get_filename());
                //cout << first_word->second[i]->get_index_vector().size() << endl;
                for (int j = 0; j < first_word->second[i]->get_index_vector().size(); j++)
                {

                    if (recursive_sentence_search_helper(first_word->second[i]->get_filename(), 1, sentence, first_word->second[i]->get_index_vector()[j]))
                    {
                        // saving the index of 1st word according to the file if the sentence is found
                        wf->index_in_doc.push_back(first_word->second[i]->get_index_vector()[j]);
                    }
                }
                result.push_back(wf);
            }
        }

        return result;

    }

    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //--------------- console handeling function ------------------
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



    //+++++++++++++++++ printing on console

    void PrintLiveCommandONConsole(string s, HANDLE hConsole, COORD cursorPos)
    {
        if (s.size() >= 0)
        {
            SetConsoleCursorPosition(hConsole, cursorPos);
            cout << "                                                                                                                             ";
            SetConsoleCursorPosition(hConsole, cursorPos);
            cout << s;
        }
    }

    COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
    {
        CONSOLE_SCREEN_BUFFER_INFO cbsi;
        if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
        {
            return cbsi.dwCursorPosition;
        }
        else
        {
            // The function failed. Call GetLastError() for details.
            COORD invalid = { 0, 0 };
            return invalid;
        }
    }

    string getCommand()
    {

        string command;
        char x = ' ';
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD cursorPos;
        cursorPos = GetConsoleCursorPosition(hConsole);

        while (true) //  \r  =  enter key
        {
            x = _getch();
            if (x == -32) // if arrow keys are pressed then just ignore them
            {
                x = _getch();
                continue;
            }
            if (x == '\r')
            {
                cout << '\n';
                break;
            }
            else if (x == 8) // backspace pressed
            {
                if (command.size() > 0)
                {
                    command.pop_back();
                }
            }
            else
            {
                command.push_back(x);
            }

            PrintLiveCommandONConsole(command, hConsole, cursorPos);
        }

        return command;
    }


    // helper of command_handeler
    void string_trimmer(string& st)
    {
        while (st[0] == ' ')
        {
            st.erase(st.begin());
        }
        while (st[st.size() - 1] == ' ')
        {
            st.pop_back();
        }
    }

    void print_pointer_vector(const vector<WordInfo*>& vx)
    {
        for (auto i : vx)
        {
            cout << "File name: " << i->get_filename() << "  " << "and frequency in is: " << i->word_frequency() << endl;
        }
    }

    vector<WordInfo*> intersection_function(vector<WordInfo*>& v1, vector<WordInfo*>& v2)
    {
        vector<WordInfo*>* pt1;
        vector<WordInfo*>* pt2;
        if (v1.size() < v2.size())
        {
            pt1 = &v1;
            pt2 = &v2;
        }
        else
        {
            pt1 = &v2;
            pt2 = &v1;
        }
        vector<WordInfo*> result;
        for (auto i : *pt1)
        {
            int index = 0;
            if (file_searcher_in_vector(*pt2, i->get_filename(), index))
            {
                // if file is found in both vectors 
                WordInfo* fx = new WordInfo(i->get_filename());
                // copying both vectors
                fx->index_in_doc.insert(fx->index_in_doc.end(), i->index_in_doc.begin(), i->index_in_doc.end());
                fx->index_in_doc.insert(fx->index_in_doc.end(), (*pt2)[index]->index_in_doc.begin(), (*pt2)[index]->index_in_doc.end());
                result.push_back(fx);
            }
        }
        return result;
    }


    vector<WordInfo*> generic_Intersection_function(vector<string> words)
    {
        // only vectors with atleast length 2 can enter this function so its safe
        vector<WordInfo*> v1 = single_word_search(words[0]);
        vector<WordInfo*> v2 = single_word_search(words[1]);
        //sort(v1.begin(), v1.end(), WordInfo_comparator_accending);
        //sort(v2.begin(), v2.end(), WordInfo_comparator_accending);
        vector<WordInfo*> intersection;
        int i = 2;
        do
        {
            if (v1.empty() || v2.empty())
            {
                return intersection;
            }
            // agr koi asa word aa gia ha jo ksi file ma nhi too usi waqt result discard
            // kar do or error da do ka following word doesnt exists
            if (v1.empty() || v2.empty())
            {
                return intersection; // at this point interection vector is always empty
            }
            intersection = intersection_function(v1, v2);
            v1 = intersection;
            if (i < words.size())
            {
                v2 = single_word_search(words[i]);
                //sort(v2.begin(), v2.end(), WordInfo_comparator_accending);
                intersection.clear();
                i++;
            }
            else
            {
                break;
            }

        } while (true);
        return v1;
    }

    //  power set function using bit manipulation
    template<typename T>
    vector<vector<T>> powerSet(const vector<T>& input) {
        vector<vector<T>> result;

        // Calculate the number of subsets (2^n)
        std::size_t numSubsets = 1 << input.size();

        for (std::size_t i = 0; i < numSubsets; ++i) {
            std::vector<T> subset;

            for (std::size_t j = 0; j < input.size(); ++j) {
                // Check if the j-th bit is set in the binary representation of i
                if ((i & (1 << j)) != 0) {
                    subset.push_back(input[j]);
                }
            }

            result.push_back(subset);
        }

        return result;
    }


    void command_handeler(string command)
    {
        if (command.empty()) // agr empty string aya ha as command
        {
            cout << "Please give something as input, empty command is not an input\n\n";
            return;
        }

        if (command == "-upload-")
        {
            update_searchEngine();
            return;
        }

        // trimming the command string for extra spaces on front and back of word 
        string_trimmer(command);
        // now we are going to check for type of command

        // 1.   +++++ 1st checking if it is a sentence search

        if ((command.size() >= 4))
        {   // if the first and last 
            if ((command[0] == 34 || command[0] == 39) && (command[command.size() - 1] == 34 || command[command.size() - 1] == 39))
            {
                // coming here means it is a sentence search
                command.erase(command.begin());
                command.pop_back();
                vector<WordInfo*> res = sentence_search(command);
                cout << "Sentence: " << command << endl;
                for (auto i : res)
                {
                    if (i->word_frequency() != 0)
                    {
                        cout << "In File: " << i->inwhich_file << " sentence has frequency: " << i->word_frequency() << endl;
                    }
                }
                return;
            }
            // this is bonus of sentence search
            // in this bonus we will find all the files of with the sentence then just delete all those files which have the word being
            // subtracted
            if ((command[0] == 34 || command[0] == 39)) // this means that there is a word that is being subtracted from the sentence
            {
                command.erase(command.begin());
                int index = 0;
                string sentence;
                while (true)
                {
                    if (command[index] == 34 || command[index] == 39) // equals to appose trophy
                    {
                        index++;
                        break;
                    }
                    else
                    {
                        sentence.push_back(command[index]);
                        index++;
                    }
                }
                command.erase(command.begin(), command.begin() + index);
                // woking on the sentence
                vector<WordInfo*> res1 = sentence_search(sentence);

                // now we are going to find the files in which the word being subtracted is located
                string_trimmer(command);
                command.erase(command.begin());
                string_trimmer(command);
                // now we have pure word which is the subtracted word
                vector<WordInfo*> res2 = single_word_search(command);
                // now we are going to remove all files from res1 that are common in res1 and res2
                for (int i = 0; i < res2.size(); i++)
                {
                    int index = 0;
                    if (file_searcher_in_vector(res1, res2[i]->get_filename(), index) && !res1.empty()) // agr file2 ka name file
                    {
                        res1[index] = nullptr;
                        res1.erase(res1.begin() + index); // remove that specific pointer at index (dont delete its memory)
                    }

                }
                // now we are going to print only those files which have frequency more than 0
                for (int i = 0; i < res1.size(); i++)
                {
                    if (res1[i]->word_frequency() > 0)
                    {
                        cout << "Filename is: " << res1[i]->inwhich_file << " and frequency is: " << res1[i]->word_frequency() << endl;
                    }
                }
            }
        }

        // 2.  +++++ now here we are going to check multiword search

        auto plus_find = command.find("+"); // checking wheather the command contains + or not

        // the find function of string returns size_t not an iterator or 
        // if the passed substring not found it will return 'string::npos'

        if (plus_find != string::npos)
        {
            // coming here means we have found a + sign (multi word command)
            
            // +++++  now first we are going to check wheather it is the bonus of multiword
            // in which a word can be subtracted
            bool is_bonus = false;
            auto minus_find = command.find("subt");
            string word_to_subt;
            if (minus_find!=string::npos) // if "subt" is also found
            {
                is_bonus = true;
                word_to_subt = command.substr(minus_find); // extracting second part
                command.erase(command.begin() + minus_find,command.end());
                string_trimmer(command);
                word_to_subt.erase(word_to_subt.begin(), word_to_subt.begin() + 4);
                string_trimmer(word_to_subt);
            }

            // tokenizing
            vector<string> tokens = tokenizer(command, '+');
            // now we have to find the powerset of the vector given above
            vector<vector<string>> pow_set = powerSet(tokens);
            pow_set.erase(pow_set.begin()); // removing the empty set
            sort(pow_set.begin(), pow_set.end(), power_set_sort_comparator);
            vector<vector<WordInfo*>> final_result;

            // we will store the single values subset first
            while (true)
            {
                if (pow_set[0].size() == 1)
                {
                    final_result.push_back(single_word_search(pow_set[0][0]));
                    if (final_result[final_result.size() - 1].empty())
                    {
                        return;
                    }
                    pow_set.erase(pow_set.begin());
                }
                else
                {
                    break;
                }
            }

            // now we will go to the vectors containing more than 1 word
            for (int j = 0; j < pow_set.size(); j++)
            {
                vector<WordInfo*> x = generic_Intersection_function(pow_set[j]);
                /*if (x.empty())
                {
                    cout << "We have found a word in your multi word find command which does not"
                        << " exists please make sure all the words exist\n\n";
                    return ;
                }*/
                final_result.push_back(x);
            }
            // this is special if only if the bonus query is run
            if (is_bonus)
            {
                vector<WordInfo*> del = single_word_search(word_to_subt);
                for (auto i : del)
                {
                    string filename = i->inwhich_file;
                    for (int j = 0; j < final_result.size(); j++)
                    {
                        for (int k = 0; k < final_result[j].size(); k++)
                        {
                            if (final_result[j][k]->get_filename() == filename)
                            {
                                final_result[j][k] = nullptr;
                                final_result[j].erase(final_result[j].begin() + k);
                            }
                        }

                    }

                }

            }

            // now printing the results
            for (int i = 0; i < tokens.size(); i++)
            {
                cout << "Word: " << tokens[i] << endl;

                // now printing there info
                if (!final_result[i].empty())
                {
                    print_pointer_vector(final_result[i]);
                }
            }
            for (int i = 0; i < pow_set.size(); i++)
            {
                // printing the words
                cout << "Words: ";
                for (auto j = 0; j < pow_set[i].size(); j++)
                {
                    cout << pow_set[i][j] << " ";
                }
                cout << endl;
                // now printing there info
                if (!final_result[i].empty())
                {
                    print_pointer_vector(final_result[i]);
                }
                else
                {
                    cout << "these words have no common file\n\n";
                }

            }
            cout << endl;
            return;
        }


        //3. we are handeling the bonus of word search  (Word1-Word2)
        // I am going to use word 'subt' b/w the words to initialte (Word1-Word2) search : uing subt so that clashes wont occur (like in by-product word)

        auto minus_search = command.find("subt"); // if the sentence contains subt b/w them then it is the bonus word search
        if (minus_search != string::npos)
        {
            string first_part, second_part;
            first_part = command.substr(0, minus_search - 1);
            second_part = command.substr(minus_search + 4);
            string_trimmer(first_part);
            string_trimmer(second_part);
            vector<WordInfo*> r1 = single_word_search(first_part);
            vector<WordInfo*> r2 = single_word_search(second_part);
            if (r1.empty())
            {
                cout << "Since Your 1st word does not exits there are no results to show" << endl;
                return;
            }
            if (r2.empty())
            {
                print_pointer_vector(r1);
                return;
            }
            for (int i = 0; i < r2.size(); i++)
            {
                int index = 0;
                if (file_searcher_in_vector(r1, r2[i]->get_filename(), index))
                {
                    r1[index] = nullptr;
                    r1.erase(r1.begin() + index);
                }
            }
            // now printing the remains
            print_pointer_vector(r1);
            return;
        }


        // 4.   Now we are handeling single word search which is our last option

        vector<WordInfo*> single_word_info = single_word_search(command);
        if (!single_word_info.empty())
        {
            cout << "Word:  " << command << endl;
            print_pointer_vector(single_word_info);
        }
        return;

    }




    //++++++++ main handeler function

    void main_handler()
    {
        cout << "\n        Welcome to search engine by Muhammad Ahmad\n\n";
        while (true)
        {
            cout << "------------------------------------------\n";
            cout << "Search Command:  ";
            string command = getCommand();
            cout << "------------------------------------------\n";
            command_handeler(command);
        }

    }


};





int main_1()
{
    unorderedMap_SearchEngine mse;
    mse.main_handler();
    return _getch();
}


// this main is for testing the time


int main()
{
    unorderedMap_SearchEngine mse;
    auto start = std::chrono::high_resolution_clock::now();
    string query_1 = "is";
    string query_2 = "this";
    string query_3 = "\"is this\"";
    string query_4 = "-upload-";
    string query_5 = "\"is then\"-this";
    int time_c = 200;
    for (int i = 0; i < time_c; i++)
    {
        mse.command_handeler(query_1);
    }

    for (int j = 0; j < time_c; j++)
    {
        mse.command_handeler(query_2);
    }

    for (int j = 0; j < time_c; j++)
    {
        mse.command_handeler(query_3);
    }
    for (int j = 0; j < 4; j++)
    {
        mse.command_handeler(query_4);
    }
    for (int j = 0; j < time_c; j++)
    {
        mse.command_handeler(query_5);
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Print the duration in milliseconds
    std::cout << "Time taken by unordered map(hashmap) search engine: " << duration.count() << " milliseconds" << std::endl;

}



