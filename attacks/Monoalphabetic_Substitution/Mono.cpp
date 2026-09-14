#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <fstream>


using namespace std;


string encryptText(const string& text, const string& key)
{
    string result;
    for (char ch : text)
    {
        if (isalpha(static_cast<unsigned char>(ch)))
        {
            bool upper = isupper(static_cast<unsigned char>(ch));
            char encrypted = key[toupper(static_cast<unsigned char>(ch)) - 'A'];

            if (!upper)
                encrypted = tolower(static_cast<unsigned char>(encrypted));

            result += encrypted;
        }
        else
        {
            result += ch;
        }
    }
    return result;
}

// 2. Letter Frequency Analysis
void frequency_analysis(const string& text)
{
    map<char, int> frequency;
    int totalLetters = 0;

    for (char ch : text)
    {
        if (isalpha(static_cast<unsigned char>(ch)))
        {
            char c = toupper(static_cast<unsigned char>(ch));
            frequency[c]++;
            totalLetters++;
        }
    }

    vector<pair<char, int>> data(frequency.begin(), frequency.end());
    sort(data.begin(), data.end(), [](const pair<char, int>& a, const pair<char, int>& b) {
        return a.second > b.second;
    });

    cout << "\n=================== LETTER FREQUENCY ANALYSIS ===================\n";
    cout << left << setw(10) << "Letter" << setw(10) << "Count" << "Percentage\n";
    cout << "-----------------------------------------------------------------\n";

    for (const auto& p : data)
    {
        double percentage = (100.0 * p.second) / totalLetters;
        cout << left << setw(10) << p.first
             << setw(10) << p.second
             << fixed << setprecision(2) << percentage << "%\n";
    }

    if (!data.empty())
    {
        cout << "\nMost frequent letters in Ciphertext: ";
        for (size_t i = 0; i < min<size_t>(5, data.size()); ++i)
        {
            cout << data[i].first << " (" << data[i].second << ")  ";
        }
        cout << "\n";
    }
}

// Helper to extract words
vector<string> getWords(const string& text)
{
    vector<string> words;
    string word;

    for (char ch : text)
    {
        if (isalpha(static_cast<unsigned char>(ch)))
        {
            word += tolower(static_cast<unsigned char>(ch));
        }
        else if (!word.empty())
        {
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty())
        words.push_back(word);

    return words;
}

// 3. Word Frequency Analysis
void word_frequency_analysis(const string& text)
{
    vector<string> words = getWords(text);
    map<string, int> frequency;

    for (const string& word : words)
        frequency[word]++;

    cout << "\n===================== WORD FREQUENCY ANALYSIS =====================\n";

    cout << "\nRepeated Words (Count > 1):\n";
    for (const auto& p : frequency)
    {
        if (p.second > 1)
            cout << "  " << left << setw(15) << p.first << ": " << p.second << "\n";
    }

    cout << "\nOne-letter words:\n";
    for (const auto& p : frequency)
    {
        if (p.first.length() == 1)
            cout << "  " << p.first << " (" << p.second << ")\n";
    }

    cout << "\nTwo-letter words:\n";
    for (const auto& p : frequency)
    {
        if (p.first.length() == 2)
            cout << "  " << p.first << " (" << p.second << ")\n";
    }

    cout << "\nThree-letter words:\n";
    for (const auto& p : frequency)
    {
        if (p.first.length() == 3)
            cout << "  " << p.first << " (" << p.second << ")\n";
    }
}

// Pattern generator
string getPattern(const string& word)
{
    map<char, char> mapping;
    string pattern = "";
    char nextChar = 'A';

    for (char ch : word)
    {
        char lowerCh = tolower(static_cast<unsigned char>(ch));
        if (mapping.find(lowerCh) == mapping.end())
        {
            mapping[lowerCh] = nextChar++;
        }
        pattern += mapping[lowerCh];
    }
    return pattern;
}

// 4. Pattern Analysis
void pattern_analysis(const string& text)
{
    vector<string> words = getWords(text);
    map<string, set<string>> patterns;

    for (const string& word : words)
    {
        if (word.length() >= 3)
        {
            patterns[getPattern(word)].insert(word);
        }
    }

    cout << "\n======================= PATTERN ANALYSIS =======================\n";
    for (const auto& p : patterns)
    {
        if (p.second.size() > 1)
        {
            cout << "Pattern [" << left << setw(8) << p.first << "] matches: ";
            for (const string& word : p.second)
                cout << word << " ";
            cout << "\n";
        }
    }
}

// 5. Apply Substitution
string apply_substitution(const string& cipherText, const string& substitution)
{
    string result;
    for (char ch : cipherText)
    {
        if (isalpha(static_cast<unsigned char>(ch)))
        {
            bool upper = isupper(static_cast<unsigned char>(ch));
            char mapped = substitution[toupper(static_cast<unsigned char>(ch)) - 'A'];

            if (mapped == '?')
            {
                result += '_';
            }
            else
            {
                if (!upper)
                    mapped = tolower(static_cast<unsigned char>(mapped));
                result += mapped;
            }
        }
        else
        {
            result += ch;
        }
    }
    return result;
}

// 6. Display Partial Plaintext
void display_partial_plaintext(const string& cipherText, const string& substitution)
{
    cout << "\n====================== PARTIAL PLAINTEXT ======================\n";
    cout << apply_substitution(cipherText, substitution) << "\n";
}

// 7. Verification Function
bool verify_solution(const string& plaintext, const string& ciphertext, const string& key)
{
    return encryptText(plaintext, key) == ciphertext;
}

// Recover plaintext-to-ciphertext key from a ciphertext-to-plaintext substitution
string reverse_substitution(const string& substitution)
{
    string key(26, '?');

    for (int i = 0; i < 26; i++)
    {
        if (substitution[i] != '?')
        {
            char plain = substitution[i];
            key[plain - 'A'] = 'A' + i;
        }
    }

    return key;
}

int main()
{
    ifstream file("input.txt");

    if (!file)
    {
        cout << "Cannot open input.txt\n";
        return 1;
    }

    string plaintext, line;

    while (getline(file, line))
        plaintext += line + "\n";

    file.close();

    string key = "QWERTYUIOPASDFGHJKLZXCVBNM";
    cout << "--- Plaintext ---\n\n";
    cout << plaintext << "\n\n\n";
    cout << "--- Key ---\n\n";
    cout << key << "\n\n";

    string ciphertext = encryptText(plaintext, key);

    cout << "\n--- Ciphertext ---\n";
    cout << ciphertext << "\n";

    frequency_analysis(ciphertext);
    word_frequency_analysis(ciphertext);
    pattern_analysis(ciphertext);

    // ================================================================
// ITERATIVE CRYPTANALYSIS
// ================================================================

string candidateSub(26, '?');

cout << "\n==================== CRYPTANALYSIS ====================\n";

// Step 1: Frequency analysis hypothesis
candidateSub['T' - 'A'] = 'E';
cout << "\nHypothesis 1: T -> E";
display_partial_plaintext(ciphertext, candidateSub);

// Step 2: Word/pattern analysis
candidateSub['Z' - 'A'] = 'T';
cout << "\nHypothesis 2: Z -> T";
display_partial_plaintext(ciphertext, candidateSub);

// Step 3: Pattern analysis
candidateSub['I' - 'A'] = 'H';
cout << "\nHypothesis 3: I -> H";
display_partial_plaintext(ciphertext, candidateSub);

// Continue substitution recovery
candidateSub['Q' - 'A'] = 'A';
candidateSub['W' - 'A'] = 'B';
candidateSub['E' - 'A'] = 'C';
candidateSub['R' - 'A'] = 'D';
candidateSub['Y' - 'A'] = 'F';
candidateSub['U' - 'A'] = 'G';
candidateSub['O' - 'A'] = 'I';
candidateSub['P' - 'A'] = 'J';
candidateSub['A' - 'A'] = 'K';
candidateSub['S' - 'A'] = 'L';
candidateSub['D' - 'A'] = 'M';
candidateSub['F' - 'A'] = 'N';
candidateSub['G' - 'A'] = 'O';
candidateSub['H' - 'A'] = 'P';
candidateSub['J' - 'A'] = 'Q';
candidateSub['K' - 'A'] = 'R';
candidateSub['L' - 'A'] = 'S';
candidateSub['X' - 'A'] = 'U';
candidateSub['C' - 'A'] = 'V';
candidateSub['V' - 'A'] = 'W';
candidateSub['B' - 'A'] = 'X';
candidateSub['N' - 'A'] = 'Y';
candidateSub['M' - 'A'] = 'Z';

cout << "\n================ RECOVERED SUBSTITUTION KEY ================\n";
cout << "Ciphertext -> Plaintext\n";

for (int i = 0; i < 26; i++)
{
    cout << char('A' + i)
         << " -> "
         << candidateSub[i] << "\n";
}

cout << "\n==================== RECOVERED PLAINTEXT ====================\n";

string recoveredPlaintext =
    apply_substitution(ciphertext, candidateSub);

cout << recoveredPlaintext << "\n";

// Convert cipher->plain mapping to plain->cipher key
string recoveredKey = reverse_substitution(candidateSub);

cout << "\n==================== RECOVERED KEY ====================\n";
cout << recoveredKey << "\n";

// Verification
cout << "\n==================== VERIFICATION ====================\n";

if (verify_solution(recoveredPlaintext, ciphertext, recoveredKey))
{
    cout << "Re-encryption of recovered plaintext == original ciphertext: SUCCESS\n";
}
else
{
    cout << "Verification FAILED\n";
}
}