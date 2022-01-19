#include <iostream>
#include <limits>
#include <list>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <list>
#include <iterator>
#include <algorithm>
#include <iomanip>  
#include <conio.h> 
#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 



using namespace std;


class TrieNode;

/**
 * Pair class will be the nodes we going to maintain in
 * the hashmap. Here in templates
 * K --> Key
 * V --> Value
*/

template <class K, class V>
class Pair {
    public:

    K key;
    V value;

    Pair(K key, V value) {
        this->key = key;
        this->value = value;
    }

};

/**
 * We wll be using the hashmaps to implement a TRIE data
 * structure 
 * 
 * --> It has complexity of O(1) for insertion and lookup.
 * --> HashMap allows one null key and multiple null values.
 * --> HashMap does not maintain any order.
 * 
*/


template <class K, class V>
class HashMap {

    public:

    int size;           // num of keys added
    int buckets;        // num of buckets in the table
    list<Pair<K, V>*> *table;

    /**
     * Constructor
    */

    HashMap(int buckets) {
        
        this->buckets = buckets;
        table = new list<Pair<K, V>*>[buckets];

        size = 0;
        // buckets = 5;

    }

    /**
     * This will return the Hash for the specific 
     * Key
    */

    int hashFunction(K x) {
        int temp = x;
        return (x % buckets);
    }

    /**
     * Inserting a key-value pair
     * --> Calculate the index for the key using the hash function
     * --> Check the load factor
     * --> Rehash if required
     * 
     * --> COMPLEXITY : O(1)
    */

    void insert(K key, V value) {
        
        Pair<K, V>* pair = new Pair<K, V>(key, value);
        int index = hashFunction(key);
        table[index].push_back(pair);
        size++;

        /**
         * Checking the Load factor
         * if the lf > 0.75 -->> Rehashing
        */
        double loadFactor = (1.0 * size) / buckets; 
        // cout << "Load : " << loadFactor << endl;
        if (loadFactor > 0.7)
        {
            rehash();
        }

    }

    /**
     * --> Make a temp array to store the available buckets
     * --> Make a new array of double the size of previuos one
     * --> Again insert the each key-value pair from the old table 
     *      to the new table
     * 
     * COMPLEXITY : O(n)
     * 
     * --> Because it will be inserting again the "N" keys
     * --> It will happen rarely
    */

    void rehash() {

        list<Pair<K, V>*> *oldTable = table;
        table = new list<Pair<K, V>*>[2 * buckets];
        
        int oldBuckets = buckets;
        buckets *= 2;
        size = 0;
        
        /**
         * Inserting the keys from the old table
         * to the new table
        */

        for (int i = 0; i < oldBuckets; i++) { 
            for (Pair<K, V>* node : oldTable[i]) {
                K key = node->key;
                V value = node->value;
                insert(key, value);
            }
        } 

        // Delting the oldTable

        delete []oldTable;

    }

    /**
     * --> Finding the key in the table
     * --> If found delete it!
     * 
     * COMPLEXITY : O(1)
    */

    void deleteItem(K key) {

        int index = hashFunction(key); 
        
        typename list <Pair<K, V>*> :: iterator i; 
        for (i = table[index].begin(); i != table[index].end(); i++) { 
            if ((*i)->key == key) {
                break; 
            } 
        } 

        // if key found --> remove it
        if (i != table[index].end()) {
            table[index].erase(i);     
        }

    }

    /**
     * Searching the value for the given key
     * 
     * COMPLEXITY : O(1)
    */

    V getValue(K key) {

        int index = hashFunction(key); 
        
        typename list <Pair<K, V>*> :: iterator i; 
        for (i = table[index].begin(); i != table[index].end(); i++) { 
            if ((*i)->key == key) {
                break; 
            } 
        } 

        // if key found --> return it
        if (i != table[index].end()) {
            return (*i)->value;
        }

        return 0;
    }


    list<Pair<K, V>*>* getMap() {
        return table;
    }

    int getBuckets() {
        return buckets;
    }

    void displayHash() { 
        for (int i = 0; i < buckets; i++) { 
            cout << i;
            for (Pair<K, V>* x : table[i]) {
                cout << " --> " << x->key << " : ";
                cout  << x->value; 
            }
            cout << endl; 
        } 
    } 

    
};

/**
 * Word Data will store the data of the word
 * --> only the char (Trie Node) which is a last character 
 *      will have Word Data (Pointer)
 * 
 * --> If two or more words have the similar meanings there data will list
 *      will be maintained using next pointer
 * 
*/

class WordData {

    public:

    string partOfSpeech;
    string synonyms;
    WordData* next;

    WordData(string partOfSpeech, string synonyms) {
        this->partOfSpeech = partOfSpeech;
        this->synonyms = synonyms;
        next = NULL;
    }

};

/**
 * --> Each trie node will have a map to next node
 * --> Only the trie node which is last char of the word
 *      will contain a data set
*/
  
class TrieNode { 

    public:
  
    WordData* data;
  
    /* nodes store a map to child node */
    HashMap<char, TrieNode*> *map = new HashMap<char, TrieNode*>(5); 

    TrieNode() {
        data = NULL;
    }

}; 


/**
 * Complexity of creating a trie is O(W*L)
 * --> where W is the number of words
 * --> L is an average length of the word
*/
  

class Trie {

    public:

    TrieNode* root = new TrieNode();
    // list<WordData*>* suggestionList;
    list<string>* suggestionList;
    int suggestCount;


    Trie() {
        root = new TrieNode();
        // suggestionList = new list<WordData*>;
        suggestionList = new list<string>;
        suggestCount = 0;
    }

    
    /**
     * Inserting each char of the string in a separate
     * Trie Node. 
     * --> A hashmap will point to the next char (Trie Node)
     * --> Data Node will be exists (TRUE) for the end Char
     * --> If Data already point the current node to the next pointer
     * 
     * COMPLEXITY : O(n)
     *  >> Where n is the Length of the "KEY"
     *  >> Worst case is also O(n)
     * 
    */

    void insert(string str, string partOfSpeech, string synonyms) 
    { 
        if (root == nullptr) 
            root = new TrieNode();
    
        TrieNode* temp = root; 
        for (int i = 0; i < str.length(); i++) { 
            char x = str[i]; 
    
            /* make a new node if there is no path */
            if (!temp->map->getValue(x)) {
                temp->map->insert(x, new TrieNode());
            }

            temp = temp->map->getValue(x); 
        } 

        WordData* data = new WordData(partOfSpeech, synonyms);

        // if a smilar word exists add a next pointer node
        if (temp->data) {
            temp->data->next = data;
        }
        // else insert data at head
        else {
            temp->data = data;
        } 

    } 
    
    /**
     * Searching the string
     * --> comparing the each char of the string with the respective pointer
     *      through the hashmap
     * --> Also suggesting the smilar words
     * 
     *  COMPLEXITY : O(n)
     *  >> Where n is the Length of the "KEY"
     *  >> Worst case is also O(n)
    */

   WordData* search(string prefix) {

        TrieNode* loc = root; 
        TrieNode* nextValue; 
        string word;
        for (int i = 0; i < prefix.length(); i++) { 

            nextValue = loc->map->getValue(prefix[i]); 

            if (nextValue == nullptr) {
                // cout << "Not Found: " << prefix[i] << endl;
                break;
            }

            word.push_back(prefix[i]);
            loc = nextValue;
        } 

        suggestionList->clear();
        suggestCount = 0;

        /**
         * If found then return it
         * --> also find smilar words (in case if word not found)
        */

        if (prefix == word && loc->data != NULL) {
            findSuffix(loc, word);
            return loc->data;
        }
        else {
            cout << "No word found like -> " << prefix << endl;
            findSuffix(loc, word);
            return NULL;
        }        
    }

    /**
     * --> Adding the similar words to the suggestion list
     * --> Recursively finding the SUFFIX nodes through map
     * 
    */

    void findSuffix(TrieNode* value, string wordString) {
        
        if (!value->map){
            return;
        }

        HashMap<char, TrieNode*>* map = value->map;
        list <Pair<char, TrieNode*>*> *curMap = map->getMap();
        int buckets = map->getBuckets();
        
        /**
         * Recursively finding the next node
         * --> only going to one delpth for each hash map entry
        */
        for (int i = 0; i < buckets; i++) 
        {
            for (Pair<char, TrieNode*>* node : curMap[i])
            {
                TrieNode* trieNode = node->value;
                wordString.push_back(node->key);


                /**
                 * If a word found add to suggestion list
                */
                if (trieNode->data && suggestCount < 20)
                {
                    suggestionList->push_back(wordString);
                    suggestCount++;
                    findSuffix(trieNode, wordString);
                }
                else 
                {
                    findSuffix(trieNode,wordString);
                }
            }
        }

    }

  
    list<string>* getSuggestionsList() {
        return suggestionList;
    }

    /**
     * Printing out the attribute of the given word data
    */

    void printWord(WordData* value, string wordString) {

        WordData* temp = value;
        while (temp) {
            
            cout << "=====================" << endl;

            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 11 );
            cout << "WORD: " << wordString << endl;
            SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 10 );
            cout << "PART OF SPEECH: " << temp->partOfSpeech << endl;
            cout << "SYNONYMS: "  << endl;

            stringstream ss(temp->synonyms);
        
            while (ss.good()) {
                string synonym;
                getline(ss, synonym, ';');
                cout << "--> " << synonym << endl;
            }
            temp = temp->next;

            cout << "=====================" << endl;
            
        }
    }

    /**
     * Print the related words from the
     * suggestion list
    */

    void printSuggestions() {
        cout << "=====================" << endl;
        cout << "SIMILAR WORDS: " << endl;
        cout << "=====================" << endl;

        for ( auto word : *suggestionList) {
            cout << word << endl;
        }

        cout << "=====================" << endl;
        
    }

};


/**
 * Controller class will be use to read the data
 * from the csv file
 * and pushing the data to our datastructure
 * 
*/

class Controller  {

    public:

    void read_record(Trie* dic) {
 
        fstream fin;     
        fin.open("synonyms.csv", ios::in); 

        if(!fin.is_open()){
            cout <<"Eror Reading the file" << endl;
        }

        else{

            vector<string> row; 
            string line, word, temp; 
            int count = 0;

            getline(fin, line);

            while (fin.good()) { 
                row.clear(); 
         
                getline(fin, line); 
                stringstream s(line); 
                
                while (getline(s, word, ',')) 
                { 
                    // cout << " pushed: " << word;
                    row.push_back(word); 
                }

                dic->insert(row[0], row[1], row[2]);
                count++;
                
            } 
        }
    }


    void create(Trie* dic) { 
        // file pointer 
        ofstream fout; 
    
        // opens an existing csv file or creates a new file. 
        fout.open("synonyms.csv", ios::app); 
    
        int i, roll, phy, chem, math, bio; 
        string word; 
        string pof; 
        string syns; 
        string temp; 

        int choice = 13;

        cout << "Enter the word: " << endl; 
        getline(cin, word);

        cout << "Enter Part of Speech: " << endl; 
        getline(cin, pof);

        cout << "Enter Synonym: " << endl; 
        getline(cin, syns);

        while (1) {
            Sleep(1000);
            cout << "Press 1 to add an other synonym" << endl;
            cout << "Press 2 to continue." << endl;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            if (choice == 2) {
                break;
            }
            cout << "Enter the Synonym: " << endl; 
            getline(cin, temp);
            syns.push_back(';');
            syns = syns + temp;
        }

        cout << "Data inserted successfully!" << endl;
        cout << "Print:" << endl;

        cout << word << endl;
        cout << pof << endl;
        cout << syns << endl;
    
    
    
        // Insert the data to file 
        fout << word << "," << pof << "," << syns << "\n"; 

        dic->insert(word, pof, syns);
        
    }  


};



void display(string input, Trie* dic) {
    cout << "=====================" << endl;
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 12 );
    cout << "ENTER THE WORD: ";
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 10 );
    
    getline(cin, input);
    if (input == "0") {
        abort();
    }
    else {
        dic->printWord(dic->search(input), input);
        SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 9 );
        dic->printSuggestions();

        SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 12 );
        cout << "PRESS ENTER TO CONTINUE. 0 TO EXIT" << endl;
        SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 10 );
    }
}

int main() {

    Trie* dic = new Trie();
    
    Controller ctrl;
    ctrl.read_record(dic);
    // ctrl.create(dic);

    int choice;
    string input;

    while (1)
    {        
        // cout << string( 100, '\n' );
        // cout.flush();
        system("cls");
        cout << " You have to press enter two time! Its a bug😒" << endl;
        cout << " Enter 1 to search a word" << endl;
        cout << " Enter 2 to Add a word" << endl;
        cout << " Enter 0 to exit" << endl;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        
        if (choice == 1) {
            while (getchar() != '0') {
                system("cls");
                display(input, dic);
            }
        }
        else if (choice == 2) {
            ctrl.create(dic);
        }

        else if (choice == 0) {
            abort();
        }
    }

        // dic->printWord(dic->search("card"));
        // dic->printSuggestions();

}

