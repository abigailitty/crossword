#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

class Point {
public:
	Point(int x0, int y0, int s0, bool u, string word0) {
	  	x = x0;
	  	y = y0; 
	  	size = s0;
	  	up = u;
		word = word0;
	}
	int x;
	int y;
	int size;
	bool up;
	string word;
};

//declare other functions
void findAllMatches(vector<string> dictionary, list<int> constraints, vector<string>& board, vector< vector<Point> >& all_solutions);
vector<Point> findOneSolution(vector<string> dictionary, list<int> constraints, vector<string> board);
bool findWholeWord(int i, int j, const vector<string>& board, const string& dict_word, string& word, int count, bool up);
void print(vector<string>& board);
bool checkSolution(list<int> constraints, vector<string> board, vector<Point> current);
vector<string> applySolution(vector<string> board, vector<Point> current);

bool one_solution = false;


int main(int argc, char* argv[])
{
	//usage check
	if(argc != 5) //or 6 later
	{
		cerr <<  "Wrong number of arguments entered!" << endl;
		return 1;
	}

	//reading in dictionary - check if it works
	std::ifstream dictfile(argv[1]);
	if(!dictfile.good())
	{
		cerr << "Can't open dictionary " << string(argv[1]) << " to read" << endl;
		return 1;
	}
	
	//read in everyone
	vector<string> dictionary;
	
	string word;
	while (dictfile >> word)
	{
		dictionary.push_back(word);
	}
	dictfile.close();

	//reading in initial file- check if it works
	std::ifstream infile(argv[2]);
	if(!infile.good())
	{
		cerr << "Can't open " << string(argv[1]) << " to read" << endl;
		return 1;
	}
	
	//read in everyone
	vector<string> board;
	list<int> constraints;
	string line;
	while (infile >> line)
	{
		if(line[0] == '+')
		{
			line = line.substr(1);
			int constraint = stoi(line);
			constraints.push_back(constraint);
		}
		else if(line[0] != '!')
			board.push_back(line);
	}
	infile.close();
	
	// output mode
	if(string(argv[3]) == "one_solution")
		one_solution = true;
	else if(string(argv[3]) == "all_solutions")
		one_solution = false;
	else
	{
		cerr << "Unrecognized fourth argument. Do you want all solutions or only one?" << endl;
		return 1;
	}
	
	// output formatting
	bool print_boards;
	if(string(argv[4]) == "count_only")
		print_boards = false;
	else if(string(argv[4]) == "print_boards")
		print_boards = true;
	else
	{
		cerr << "Unrecognized fifth argument. Do you want to print solutions (print_boards) or just number of solutions (count_only)?" << endl;
		return 1;
	}

	//sort dictionary
	/*for(unsigned int i = 0; i < dictionary.size()-1; i++)
		for(unsigned int j = i; j < dictionary.size(); j++)
			if(dictionary[i].size() < dictionary[j].size())
			{
				string temp = dictionary[i];
				dictionary[i] = dictionary[j];
				dictionary[j] = temp;
			}*/

	//whittle down dictionary
	int max_constraint = 0; //find largest word in crossword
	for (list<int>::iterator p =constraints.begin(); p != constraints.end(); ++p) {
		if (*p > max_constraint) max_constraint = *p;
	}
	for(unsigned int i = 0; i < dictionary.size(); i++)
	{
		/*if(int(dictionary[i].size()) > max_constraint) //erase all words too big for the board
			dictionary.erase(dictionary.begin() + i);
		else
		{*/
			bool found = false;							//erase all words that aren't on the constraint list
			for(list<int>::iterator int_itr = constraints.begin(); int_itr != constraints.end(); int_itr++)
			{
				if(*int_itr == int(dictionary[i].size()))
				{
					found = true;
					break;
				}
			}
			
			if(!found) {
				dictionary.erase(dictionary.begin() + i);
			}
		//}
	}

	vector< vector<Point> > all_solutions;
	findAllMatches(dictionary, constraints, board, all_solutions);
	cout << "Number of solution(s): " << all_solutions.size() << endl;

	if (print_boards) {
		for (unsigned int i = 0; i < all_solutions.size(); i++) {
			vector<string> cboard = applySolution(board, all_solutions[i]);
			print(cboard);
		}
	}

}

vector<string> makeEmptyBoard(vector<string>& board) {
	vector<string> out;
	out.resize(board.size());
	for (unsigned int i = 0; i < board.size(); i++) {
		for (unsigned int j = 0; j < board[i].size(); j++) {
			out[i].push_back('#');
		}
	}
	return out;
}

vector<string> applySolution(vector<string> board, vector<Point> current) {
	vector<string> solution = makeEmptyBoard(board);
	for (unsigned int i = 0; i < current.size(); ++i) {
		for (unsigned int j = 0; j < current[i].word.size(); j++) {
			if (current[i].up) {
				solution[current[i].x + j][current[i].y] = current[i].word[j];
			} else {
				solution[current[i].x][current[i].y + j] = current[i].word[j];
			}
		}
	}
	return solution;
}

vector<string> extractWords(vector<string> board) {
	vector<string> words;
	for (unsigned int i = 0; i < board.size(); ++i) {
		for (unsigned int j = 0; j < board[i].size(); j++) {
			if ((board[i][j] != '#' && j ==0) ||
			    (board[i][j] != '#' && board[i][j-1] == '#')) {
				string current_word;
				current_word.push_back(board[i][j]);
				bool found = false;
				for (unsigned int k = j+1; k < board[i].size(); k++) {
					if (board[i][k] != '#') {
						found = true;
						current_word.push_back(board[i][k]);
					}
					else {
						break;
					}
				}
				if (found) words.push_back(current_word);
			}
			if ((board[i][j] != '#' && i ==0)||
			    (board[i][j] != '#' && board[i-1][j] == '#')) {
				string current_word;
				current_word.push_back(board[i][j]);
				bool found = false;
				for (unsigned int k = i+1; k < board.size(); k++) {
					if (board[k][j] != '#') {
						found = true;
						current_word.push_back(board[k][j]);
					}
					else {
						break;
					}
				}
				if (found) words.push_back(current_word);
			}

		}
	}
	return words;
}

bool checkSolution(list<int> constraints, vector<string> board, vector<Point> current) {
	vector<string> cboard = applySolution(board, current);
	vector<string> extractions = extractWords(cboard);
	for (unsigned int i = 0; i < extractions.size(); i++) {
		list<int>::iterator int_itr = find(constraints.begin(), constraints.end(), extractions[i].size());
		if(int_itr != constraints.end())
		{
			constraints.erase(int_itr);
		} else {
			return false;
		}
	}
	return true;
}


bool uniqueSolution(vector<Point> current, vector < vector < Point > > all_solutions) {
	for (unsigned int i = 0; i < all_solutions.size(); i++) {
		vector< Point >& previous = all_solutions[i];
		if (previous.size() != current.size()) return false;
		unsigned int num_match = 0;
		for (unsigned int j = 0 ; j < previous.size(); j++) {
			for (unsigned int k = 0 ; k < current.size(); k++) {
				if (previous[j].x == current[k].x &&
				    previous[j].y == current[k].y &&
				    previous[j].word == current[k].word &&
				    previous[j].up == current[k].up) {
					++num_match;
				}
			}
		}
		if (num_match == previous.size()) return false;
	}
	return true;
}


void findAllMatches(vector<string> dictionary, list<int> constraints, vector<string>& board, vector< vector<Point> >& all_solutions) {
	bool done = false;
	if (dictionary.size() == 0) return;
	if (one_solution && all_solutions.size() >= 1) return;
	while (!done) {
		vector<Point> current_solution = findOneSolution(dictionary, constraints, board);
		if (current_solution.size() == 0) return;
		if (uniqueSolution(current_solution, all_solutions)) {
			vector<string> orig_dictionary = dictionary;
			for (unsigned int match_index = 0; match_index < current_solution.size() ; match_index++) {
				dictionary = orig_dictionary;
				vector<string>::iterator str_itr = find(dictionary.begin(), dictionary.end(), current_solution[match_index].word);
				if (str_itr != dictionary.end()) {
					dictionary.erase(str_itr);
					if (dictionary.size() >= constraints.size()) {
						findAllMatches(dictionary, constraints, board, all_solutions);
					}
				}
			}
			if (checkSolution(constraints, board, current_solution)) {
				if (one_solution && all_solutions.size() == 1) return;
				all_solutions.push_back(current_solution);
			}
		}
		else {
			done = true;
			break;
		}
	}
}

vector<Point> findOneSolution(vector<string> dictionary, list<int> constraints, vector<string> board) //not void?
{
	vector<Point> words, emptyWords;
	if (dictionary.size() == 0) return emptyWords; 
	for(unsigned int i = 0; i < board.size(); i++) //every row on the board
	{
		for(unsigned int j = 0; j < board[i].size(); j++) //every letter in every row
		{
			for(unsigned int k = 0; k < dictionary.size(); k++) //every word in the dictionary
			{
				string word = "";
				if(board[i][j] == dictionary[k][0] && //first letter matches
					(dictionary[k].size() <= board[i].size()-j || dictionary[k].size() <= board.size()-i)) //dictionary word has room to fit one way
				{
					word += board[i][j];
					bool found = false;
					if(j+1 < board[i].size() && board[i][j+1] == dictionary[k][1]) //word goes across
					{
						word += board[i][j+1];
						found = findWholeWord(i, j+2, board, dictionary[k], word, 2, false);
						if(found) {
							words.push_back(Point(i, j, dictionary[k].size(), false, dictionary[k]));
						}
					}
					else if(i+1 < board.size() && board[i+1][j] == dictionary[k][1]) //word goes down (got rid of an else)
					{
						word += board[i+1][j];
						found = findWholeWord(i+2, j, board, dictionary[k], word, 2, true);
						if(found) {
							words.push_back(Point(i, j, dictionary[k].size(), true, dictionary[k]));
						}
					}
					
					if(found)
					{
						list<int>::iterator int_itr = find(constraints.begin(), constraints.end(), dictionary[k].size());
						if(int_itr != constraints.end())
						{
							constraints.erase(int_itr);
						}
						else {
							// Didn't find this match in the list of constraints.
							return emptyWords;
						}
						 
						if (constraints.size() == 0) {
							return words;
						}
					}
				}
			}
		}
	}
	return emptyWords;
}

bool findWholeWord(int i, int j, const vector<string>& board, const string& dict_word, string& word, int count, bool up)
{
	if(board[i][j] == dict_word[count])
	{
		word += board[i][j];
		count++;
	}
	else {
		return false;
	}
	
	if(word == dict_word) {
		return true;
	}
	
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

void print( vector<string>& board)
{
	cout << "Board:" << endl;
	for(unsigned int i = 0; i < board.size(); i++)
	{
		cout << board[i] << endl;
	}	
}
