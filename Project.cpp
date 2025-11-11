#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class TextAnalyzer {
protected:
    string text;
public:
    TextAnalyzer(string t) {
        text = t;
    }

    virtual int analyze() = 0; 
    string getText() {
        return text;
    }
};



class LetterCounter : public TextAnalyzer {
public:
    LetterCounter(string t) : TextAnalyzer(t) {}

    int analyze() {
        int count = 0;
        for (int i = 0; i < text.length(); i++) {
            if (isalpha(text[i])) {
                count++;
            }
        }
        return count;
    }
};


class WordCounter : public TextAnalyzer {
public:
    WordCounter(string t) : TextAnalyzer(t) {}

    int analyze() {
        int count = 0;
        bool inWord = false;

        for (int i = 0; i < text.length(); i++) {
            if (isspace(text[i])) {
                if (inWord) {
                    count++;
                    inWord = false;
                }
            } else {
                inWord = true;
            }
        }

        if (inWord) count++; 
        return count;
    }
};


class SentenceCounter : public TextAnalyzer {
public:
    SentenceCounter(string t) : TextAnalyzer(t) {}

    int analyze() {
        int count = 0;
        for (int i = 0; i < text.length(); i++) {
            if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
                count++;
            }
        }
        return count;
    }
};


class GradeCalculator {
private:
    int letters, words, sentences;
public:
    GradeCalculator(int l, int w, int s) {
        letters = l;
        words = w;
        sentences = s;
    }

    int calculateGrade() {
        float L = (float)letters / words * 100;
        float S = (float)sentences / words * 100;
        float index = 0.0588 * L - 0.296 * S - 15.8;
        return (int)(index + 0.5); 
    }

    void printGrade() {
        int grade = calculateGrade();
        if (grade > 16)
            cout << "Grade 16+" << endl;
        else if (grade < 1)
            cout << "Before Grade 1" << endl;
        else
            cout << "Grade " << grade << endl;
    }
};


class FileHandler {
public:
    string readFile(string filename) {
        ifstream fin(filename);
        string content = "", line;
        if (!fin) {
            cout << "Error: File not found!" << endl;
            return "";
        }

        while (getline(fin, line)) {
            content += line + "\n";
        }
        fin.close();
        return content;
    }

    void writeResult(string filename, string text, int grade) {
        ofstream fout(filename);
        if (!fout) {
            cout << "Error writing file!" << endl;
            return;
        }
        fout << "Text analyzed:\n" << text << endl;
        fout << "Grade level: " << grade << endl;
        fout.close();
        cout << "Results saved to " << filename << endl;
    }
};


int main() {
    string text;
    char choice;

    cout << "Do you want to read from file or enter text? (f/t): ";
    cin >> choice;
    cin.ignore();

    FileHandler fh;

    if (choice == 'f' || choice == 'F') {
        string filename;
        cout << "Enter filename: ";
        getline(cin, filename);
        text = fh.readFile(filename);
        if (text == "") return 0;
    } else {
        cout << "Enter text: ";
        getline(cin, text);
    }

    
    TextAnalyzer* letter = new LetterCounter(text);
    TextAnalyzer* word = new WordCounter(text);
    TextAnalyzer* sentence = new SentenceCounter(text);

    int letters = letter->analyze();
    int words = word->analyze();
    int sentences = sentence->analyze();

    cout << "\nLetters: " << letters;
    cout << "\nWords: " << words;
    cout << "\nSentences: " << sentences << endl;

    
    GradeCalculator gc(letters, words, sentences);
    gc.printGrade();

    
    cout << "Do you want to save the results? (y/n): ";
    cin >> choice;
    cin.ignore();

    if (choice == 'y' || choice == 'Y') {
        string outFile;
        cout << "Enter output filename: ";
        getline(cin, outFile);
        fh.writeResult(outFile, text, gc.calculateGrade());
    }
    
    delete letter;
    delete word;
    delete sentence;

    return 0;
}
