#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include <stdexcept>
using namespace std;

void FindSeqHelper(int i, vector<int>& sequence, vector<int> foundSeq, set<vector<int>>& sequenceSet, int n, char mode) {
	if (foundSeq.size() == (n + 1)) {
		sequenceSet.insert(foundSeq);
	}
	for (int j = i; j < sequence.size(); j++) {
		bool comparison = (foundSeq.size() == 0 || foundSeq[foundSeq.size() - 1] <= sequence[j]);
		if (mode == 'D') {
			comparison = (foundSeq.size() == 0 || foundSeq[foundSeq.size() - 1] >= sequence[j]);
		}
		if (comparison == true) {
			foundSeq.push_back(sequence[j]);
		}
		FindSeqHelper(j + 1, sequence, foundSeq, sequenceSet, n, mode);
		foundSeq.pop_back();
	}
}
vector<vector<int>> FindSequences(vector<int>& sequence, int n, char mode) {
	vector<int> foundSeq;
	set<vector<int>> sequenceSet;
	FindSeqHelper(0, sequence, foundSeq, sequenceSet, n, mode);
	vector<vector<int>> result;
	auto iter = sequenceSet.begin();
	for (; iter != sequenceSet.end(); iter++) {
		result.push_back(*iter);
	}
	return result;
}

int main(int argc, const char** argv) {
	try {
		// Check if there is 1 string entered
		if (argc != 2) {
			throw runtime_error("Wrong number of arguments");
		}

		// Read the filename from argument
		string filename = argv[1];

		if (filename.size() > 4 &&
			filename.substr(filename.size() - 4, filename.size()) != ".txt") {
			throw runtime_error("Invalid extension");
		}

		map<int, vector<int>> sequences;

		ifstream inFile;
		inFile.open(filename);

		// Read data file
		if (inFile.is_open()) {
			string currLine;
			int i = 0;

			while (getline(inFile, currLine)) {
				istringstream stream(currLine);
				vector<int> integers;

				string elem;
				while (getline(stream, elem, ',')) {
					integers.push_back(stoi(elem));
				}

				sequences[i] = integers;
				i++;
			}

			cout << filename << " read" << endl;
		}
		else {
			cout << "Could not open " << filename << endl;
		}
		inFile.close();

		ofstream outFile("output.txt");

		// Output results to file
		if (outFile.is_open()) {
			for (int i = 0; i < sequences.size(); i++) {
				outFile << "=================================" << endl;
				outFile << "Sequence: ";
				for (int j = 0; j < sequences[i].size(); j++) {
					outFile << sequences[i][j];
					if (j != sequences[i].size() - 1) {
						outFile << ",";
					}
				}
				outFile << endl << endl;

				int n = sqrt(sequences[i].size() - 1);

				outFile << "\tn = " << n << endl;
				outFile << endl;

				vector<vector<int>> incSeqs;
				incSeqs = FindSequences(sequences[i], n, 'I');
				outFile << "\tStrictly Increasing:" << endl;

				for (int i = 0; i < incSeqs.size(); i++) {
					outFile << "\t\t> ";
					for (int j = 0; j < incSeqs[i].size(); j++) {
						outFile << incSeqs[i][j];
						if (j != incSeqs[i].size() - 1) {
							outFile << ",";
						}
						
					}
					outFile << endl;
				}
				if (incSeqs.size() == 0) {
					outFile << "\t\tnone" << endl;
				}
				outFile << endl;

				vector<vector<int>> decSeqs;
				decSeqs = FindSequences(sequences[i], n, 'D');
				outFile << "\tStrictly Decreasing:" << endl;

				for (int i = 0; i < decSeqs.size(); i++) {
					outFile << "\t\t> ";
					for (int j = 0; j < decSeqs[i].size(); j++) {
						outFile << decSeqs[i][j];
						if (j != decSeqs[i].size() - 1) {
							outFile << ",";
						}

					}
					outFile << endl;
				}
				if (decSeqs.size() == 0) {
					outFile << "\t\tnone" << endl;
				}
				outFile << endl;

				outFile << "=================================" << endl;
			}

			cout << "Results written to output.txt" << endl;
		}
		else {
			cout << "Could not write to file" << endl;
		}
	}
	catch (runtime_error& excpt) {
		cout << "ERROR: " << excpt.what() << endl;
		cout << "Please enter a filename to read with extension .txt (ex: sequences.txt)" << endl;
	}
}