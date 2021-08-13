#include <iostream>
#include <fstream>
#include <vector>
#include <list>

using namespace std;

class Point 
{
	public:
	  Point(int x0, int y0, string s, bool u)
	  {
	  	x = x0;
	  	y = y0; 
	  	word = s;
	  	up = u;
	  }

	  int x;
	  int y;
	  string word;
	  bool up;
};

//delcare other functions
void findOneSolution(vector<string> dictionary, list<int> constraints, const vector<string>& board, vector<Point>& words);
bool findWholeWord(int i, int j, const vector<string>& board, const string& dict_word, string& word, int count, bool up);
void printBoard(vector<string>& board, const vector<Point>& words);

int main(int argc, char* argv[])
{
	//usage check
	if(argc != 5) //or 6 later
	{
		cerr <<  "Wrong number of arguments entered!" << endl;
		return 1;
	}

	//check if dictionary file works
    ifstream infile(argv[1]);
    if(!infile.good())
    {
        cerr << "Can't open dictionary " << string(argv[1]) << " to read" << endl;
        return 1;
    }

    //read in dictionary
    vector<string> dictionary;

    string word;
    while (infile >> word)
    {
       dictionary.push_back(word);
    }
    infile.close();

    //reading in initial file- check if it works
    ifstream new_infile(argv[2]);
    if(!new_infile.good())
    {
        cerr << "Can't open " << string(argv[1]) << " to read" << endl;
        return 1;
    }

    //read in board and constraints
    vector<string> board;
    list<int> constraints;

    string line;
    while (new_infile >> line)
    {
       if(line[0] == '+')
       {
       	line = line.substr(1);
       	constraints.push_back(stoi(line));
       }
       else if(line[0] != '!')
       	board.push_back(line);
       	
    }
    new_infile.close();

    //output mode
    bool one_solution;
    if(string(argv[3]) == "one_solution")
    	one_solution = true;
    else if(string(argv[3]) == "all_solutions")
    	one_solution = false;
    else
    {
    	cerr << "Unrecognized fourth argument. Do you want all solutions or only one?" << endl;
    	return 1;
    }

    //output formatting
    bool print_boards;
    if(string(argv[4]) == "count_only")
    	print_boards = false;
    else if(string(argv[4]) == "print_boards")
    	print_boards = true;
    else
    {
    	cerr << "Unrecognized fifth argument. Do you want to print solutions or just number of solutions?" << endl;
    	return 1;
    }

    //these depend on output formatting
    vector<Point> words;
    if(one_solution)
    	cout << "Hi" << endl;
    findOneSolution(dictionary, constraints, board, words);
    if(print_boards)
    	printBoard(board, words);
    else
    	cout << "one solution found" << endl;
}

void findOneSolution(vector<string> dictionary, list<int> constraints, const vector<string>& board, vector<Point>& words) //not void?
{
	for(unsigned int i = 0; i < dictionary.size(); i++)
	{
		unsigned int max_constraint = constraints.front();
		for(list<int>::iterator int_itr = constraints.begin(); int_itr != constraints.end(); int_itr++)
			if(*int_itr > int(max_constraint))
				max_constraint = *int_itr;

		if(dictionary[i].size() > max_constraint) //erase all words too big for the board
			dictionary.erase(dictionary.begin() + i);
		else //don't wanna double erase, hence the else
		{
			bool found = false;							//erase all words that aren't on the constraint list
			for(list<int>::iterator int_itr = constraints.begin(); int_itr != constraints.end(); int_itr++)
				if(*int_itr == int(dictionary[i].size()))
				{
					found = true;
					break;
				}

			if(!found)
				dictionary.erase(dictionary.begin() + i);
		}
	}

	for(unsigned int i = 0; i < board.size(); i++) //every row on the board
	{
		for(unsigned int j = 0; j < board[i].size(); j++) //every letter in every board
		{
			for(unsigned int k = 0; k < dictionary.size(); k++) //every word in the dictionary
			{
				list<int>::iterator int_itr = constraints.begin();
				while(int_itr != constraints.end() && *int_itr != int(dictionary[k].size()))
					int_itr++;
				if(int_itr != constraints.end())
				{
					string word = "";
					if(board[i][j] == dictionary[k][0] && (dictionary[k].size() <= board[i].size()-j || dictionary[k].size() <= board.size()-i))
					{
						word += board[i][j];
						bool found = false;
						if(board[i][j+1] == dictionary[k][1])
						{
							word += board[i][j+1];
							found = findWholeWord(i, j+2, board, dictionary[k], word, 2, false);
							if(found)
								words.push_back(Point(i, j, dictionary[k], false));
						}
						else if(board[i+1][j] == dictionary[k][1]) //can't be same word across and down
						{
							word += board[i+1][j];
							found = findWholeWord(i+2, j, board, dictionary[k], word, 2, true);
							if(found)
								words.push_back(Point(i, j, dictionary[k], true));
						}

						if(found)
						{
							constraints.erase(int_itr);
							vector<string>::iterator str_itr = dictionary.begin() + k;
							dictionary.erase(str_itr);
						}
					}
				}
				
			}
		}
	}
}

bool findWholeWord(int i, int j, const vector<string>& board, const string& dict_word, string& word, int count, bool up)
{
	if(board[i][j] == dict_word[count])
	{
		word += board[i][j];
		count++;
	}
	else
		return false;

	if(word == dict_word)
		return true;

	if(up)
	{
		if(i+1 > int(board.size()-1)) //check if we will be in bounds if we step
			return false;
		if(findWholeWord(i+1, j, board, dict_word, word, count, up))
			return true;
	}
	else
	{
		if(j+1 > int(board[i].size()-1)) //check if we will be in bounds if we step
			return false;
		if(findWholeWord(i, j+1, board, dict_word, word, count, up))
			return true;
	}

	return false;
}

void printBoard(vector<string>& board, const vector<Point>& answers)
{
	cout << "Board:" << endl;
	for(unsigned int i = 0; i < board.size(); i++) //black out board
		for(unsigned int j = 0; j < board[i].size(); j++)
			board[i][j] = '#';

	for(unsigned int i = 0; i < answers.size(); i++) //fill in the words that we found
	{
		if(answers[i].up)
		{
			/*cout << "goin up and down" << endl;
			cout << " word we're filling in is: " << answers[i].word << " which starts at ("
				 << answers[i].x << ", " << answers[i].y << ")" << endl;*/
			int count = 0; //to keep track of which letter of the found word we are on
			for(unsigned int j = answers[i].x; j < answers[i].x + answers[i].word.size(); j++)
			{
				board[j][answers[i].y] = answers[i].word[count];
				count++;
			}
		}
		else
		{
			int count = 0; //to keep track of which letter of the found word we are on
			for(unsigned int j = answers[i].y; j < answers[i].y + answers[i].word.size(); j++)
			{
				board[answers[i].x][j] = answers[i].word[count];
				count++;
			}
		}
	}

	for(unsigned int i = 0; i < board.size(); i++) //print board
		cout << board[i] << endl;
}
