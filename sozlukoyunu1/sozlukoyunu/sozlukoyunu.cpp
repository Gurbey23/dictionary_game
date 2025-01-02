#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>  
#include <random>     

//******************************************************************************
// ************************* Student name : Gürbüz Cem Çiven *******************
//************************** Student Number : B241202068 ***********************
//************************** -HONOR CODE- **************************************


using namespace std;

struct Word {
    string english;
    string turkish;
};

vector<Word> loadDictionary(const string& filename) {
    vector<Word> dictionary;
    ifstream file(filename);
    string eng, tr;

    if (!file) {
        cout << "Dictionary file could not be opened!" << endl;
        return dictionary;
    }

    while (getline(file, eng) && getline(file, tr)) {
        dictionary.push_back({ eng, tr });
    }

    file.close();
    return dictionary;
}

void showMenu() {
    cout << "\n--- START SCREEN ---\n";
    cout << "1. Start Game\n";
    cout << "2. List Words\n";
    cout << "3. Add New Word\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

void listWords(const vector<Word>& dictionary) {
    cout << "\n--- List of Words ---\n";
    for (size_t i = 0; i < dictionary.size(); i++) {
        cout << i + 1 << ". " << dictionary[i].english << " - " << dictionary[i].turkish << endl;
    }
    cout << endl;
}

void addNewWord(vector<Word>& dictionary) {
    string eng, tr;
    cout << "\nEnter the English word: ";
    cin >> eng;
    cout << "Enter the Turkish meaning: ";
    cin >> tr;
    dictionary.push_back({ eng, tr });

    sort(dictionary.begin(), dictionary.end(), [](const Word& a, const Word& b) {
        return a.english < b.english;
        });

    ofstream file("dictionary.txt");
    if (!file) {
        cout << "Could not open the file for writing!" << endl;
        return;
    }

    for (const auto& word : dictionary) {
        file << word.english << endl << word.turkish << endl;
    }

    file.close();
    cout << "New word added and dictionary sorted!" << endl;
}

// Define the function to initiate the game logic
int askQuestion(const Word& word, int& score, vector<Word>& dictionary) {
    string options[4];
    int correctOption = rand() % 4;

    // Place the correct meaning in one of the options
    options[correctOption] = word.turkish;

    // Select incorrect meanings
    for (int i = 0; i < 4; i++) {
        if (i != correctOption) {
            int randomIndex;
            do {
                randomIndex = rand() % dictionary.size();
            } while (dictionary[randomIndex].turkish == word.turkish); // To avoid repeating the same answer
            options[i] = dictionary[randomIndex].turkish;
        }
    }

    // Shuffle the options
    random_device rd;
    mt19937 g(rd());
    shuffle(options, options + 4, g);

    cout << "What is the meaning of the word: " << word.english << "?" << endl;
    for (int i = 0; i < 4; i++) {
        cout << i + 1 << ". " << options[i] << endl;
    }

    int userAnswer;
    cout << "Enter your answer (1-4): ";
    cin >> userAnswer;

    // If the answer is correct, add 3 points; if incorrect, subtract 1 point
    if (options[userAnswer - 1] == word.turkish) {
        score += 3;
        cout << "Correct! You earned 3 points." << endl;
        return 1;  // Correct answer
    }
    else {
        score -= 1;
        cout << "Incorrect! You lost 1 point." << endl;
        return -1;  // Incorrect answer
    }
}

void startGame(vector<Word>& dictionary) {
    srand(time(0)); // Seed for random selection
    int score = 0;
    int wrongAnswers = 0;
    int questionsAnswered = 0;

    while (wrongAnswers < 3 && questionsAnswered < 10) {
        int randomIndex = rand() % dictionary.size();
        Word word = dictionary[randomIndex];

        int result = askQuestion(word, score, dictionary);
        if (result == -1) {
            wrongAnswers++;
        }
        questionsAnswered++;
    }

    cout << "\n--- Game Over ---\n";
    cout << "Your final score: " << score << endl;

    // Game end menu
    cout << "1. Replay the Game\n";
    cout << "2. Previous Menu\n";
    cout << "3. Exit the Game\n";
    cout << "Choose an option: ";
    int option;
    cin >> option;

    if (option == 1) {
        startGame(dictionary);  // Restart the game
    }
    else if (option == 2) {
        return;  // Return to the main menu
    }
    else {
        exit(0);  // Exit the game
    }
}

int main() {
    vector<Word> dictionary = loadDictionary("dictionary.txt");

    if (dictionary.empty()) {
        cout << "The dictionary is empty or could not be loaded." << endl;
        return 1;
    }

    int choice;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            startGame(dictionary);  // Start the game
            break;
        case 2:
            listWords(dictionary);
            break;
        case 3:
            addNewWord(dictionary);
            break;
        case 4:
            cout << "Exiting the game. Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid option, please try again." << endl;
        }
    }

    return 0;
}
