#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <fstream>
#include <vector>
#include <memory>

using namespace std;

class TextAnalyzer {
protected:
    string text;
public:
    TextAnalyzer(const string& t) : text(t) {}
    virtual int analyze() = 0; 
    virtual ~TextAnalyzer() = default;

    string getText() const { return text; }
};


class LetterCounter : public TextAnalyzer {
public:
    LetterCounter(const string& t) : TextAnalyzer(t) {}
    
    int analyze() override {
        int counter = 0;
        for (char c : text) {
            if (isalpha(c)) {
                counter++;
            }
        }
        cout << "Number of letters: " << counter << endl;
        return counter;
    }
};

class WordCounter : public TextAnalyzer {
public:
    WordCounter(const string& t) : TextAnalyzer(t) {}
    
    int analyze() override {
        int counter = 1; 
        for (char c : text) {
            if (isspace(c) || c == '\n') {
                counter++;
            }
        }
        return counter;
    }
};


class SentenceCounter : public TextAnalyzer {
public:
    SentenceCounter(const string& t) : TextAnalyzer(t) {}
    
    int analyze() override {
        int counter = 0;
        for (char c : text) {
            if (c == '.' || c == '!' || c == '?') {
                counter++;
            }
        }
        cout << "Number of sentences: " << counter << endl;
        return counter;
    }
};

class GradeCalculator {
private:
    double letters;
    double words;
    double sentences;

public:
    GradeCalculator(int l, int w, int s) 
        : letters(l), words(w), sentences(s) {}
    
    int calculateGrade() {
        double L = (letters / words) * 100;
        double S = (sentences / words) * 100;
        return round(0.0588 * L - 0.296 * S - 15.8);
    }
    
    void printGrade() {
        int index = calculateGrade();
        if (index > 16) {
            cout << "Grade 16+" << endl;
        }
        else if (index < 1) {
            cout << "Before Grade 1" << endl;
        }
        else {
            cout << "Grade " << index << endl;
        }
    }
};

class FileHandler {
public:
    static bool writeResult(const string& filename, const string& text, int grade) {
        ofstream file(filename);
        if (!file) return false;
        
        file << "Text analyzed: " << text << endl;
        file << "Grade level: " << grade << endl;
        return true;
    }
    
    static string readFile(const string& filename) {
        ifstream file(filename);
        if (!file) throw runtime_error("Unable to open file");
        
        string content, line;
        while (getline(file, line)) {
            content += line + "\n";
        }
        return content;
    }
};

int main() {
    string text;
    char choice;
    
    cout << "Would you like to (r)ead from file or (e)nter text directly? ";
    cin >> choice;
    cin.ignore(); 
    
    try {
        if (choice == 'r' || choice == 'R') {
            string filename;
            cout << "Enter filename to read: ";
            getline(cin, filename);
            text = FileHandler::readFile(filename);
        } else {
            cout << "Enter text: ";
            getline(cin, text);
        }
        
        vector<unique_ptr<TextAnalyzer>> analyzers;
        analyzers.push_back(make_unique<LetterCounter>(text));
        analyzers.push_back(make_unique<WordCounter>(text));
        analyzers.push_back(make_unique<SentenceCounter>(text));
        
        int letters = analyzers[0]->analyze();
        int words = analyzers[1]->analyze();
        int sentences = analyzers[2]->analyze();
        
        GradeCalculator calc(letters, words, sentences);
        calc.printGrade();
        
        cout << "Would you like to save the results? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            string outputFile;
            cout << "Enter output filename: ";
            cin.ignore();
            getline(cin, outputFile);
            if (FileHandler::writeResult(outputFile, text, calc.calculateGrade())) {
                cout << "Results saved successfully!" << endl;
            } else {
                cout << "Error saving results." << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;

}
