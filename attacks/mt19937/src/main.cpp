#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <iomanip>
#include <cstdint>

using namespace std;

// Convert a normal string into bytes.
vector<unsigned char> stringToBytes(const string& text) {
    return vector<unsigned char>(text.begin(), text.end());
}

// Convert bytes back into a normal string.
string bytesToString(const vector<unsigned char>& data) {
    return string(data.begin(), data.end());
}

// Print bytes in hexadecimal format.
void printHex(const vector<unsigned char>& data) {
    for (unsigned char value : data) {
        cout << hex << setw(2) << setfill('0') << (int)value << " ";
    }
    cout << dec << setfill(' ') << '\n';
}

// Same function encrypts and decrypts.
vector<unsigned char> encryptDecrypt(
    const vector<unsigned char>& input,
    unsigned int seed
) {
    mt19937 generator(seed);
    vector<unsigned char> result;

    for (unsigned char value : input) {
        unsigned char keyByte = generator() & 0xFF;
        result.push_back(value ^ keyByte);
    }

    return result;
}

// Make a 16-byte token using the given seed.
vector<unsigned char> makeToken(unsigned int seed) {
    mt19937 generator(seed);
    vector<unsigned char> token;

    for (int i = 0; i < 16; i++) {
        unsigned char randomByte = generator() & 0xFF;
        token.push_back(randomByte);
    }

    return token;
}

int main() {
    // ---------------- PART A ----------------
    cout << "========== PART A: STREAM CIPHER ==========\n";

    random_device randomDevice;
    mt19937 randomGenerator(randomDevice());

    uniform_int_distribution<int> seedPicker(0, 65535);
    uniform_int_distribution<int> lengthPicker(5, 20);
    uniform_int_distribution<int> characterPicker(32, 126);

    // Generate a random 16-bit seed.
    unsigned int originalSeed = seedPicker(randomGenerator);

    // Generate a random printable prefix of 5 to 20 characters.
    int prefixLength = lengthPicker(randomGenerator);
    string prefix;

    for (int i = 0; i < prefixLength; i++) {
        prefix += (char)characterPicker(randomGenerator);
    }

    // Add exactly 14 A characters after the prefix.
    string plaintext = prefix + string(14, 'A');

    vector<unsigned char> plainBytes = stringToBytes(plaintext);

    // Encrypt the plaintext.
    vector<unsigned char> cipherBytes =
        encryptDecrypt(plainBytes, originalSeed);

    // Decrypt using the same function.
    vector<unsigned char> decryptedBytes =
        encryptDecrypt(cipherBytes, originalSeed);

    cout << "Original seed: " << originalSeed << '\n';
    cout << "Plaintext: " << plaintext << '\n';

    cout << "Ciphertext (hex): ";
    printHex(cipherBytes);

    cout << "Decryption check: "
         << (decryptedBytes == plainBytes ? "SUCCESS" : "FAILED")
         << "\n\n";

    // Try every possible 16-bit seed.
    string knownText(14, 'A');
    int recoveredSeed = -1;
    string recoveredPlaintext;

    for (int testSeed = 0; testSeed <= 65535; testSeed++) {
        vector<unsigned char> testBytes =
            encryptDecrypt(cipherBytes, testSeed);

        string testPlaintext = bytesToString(testBytes);

        if (testPlaintext.find(knownText) != string::npos &&
            recoveredSeed == -1) {
            recoveredSeed = testSeed;
            recoveredPlaintext = testPlaintext;
        }
    }

    cout << "Seeds tested: 65536\n";

    if (recoveredSeed != -1) {
        vector<unsigned char> finalPlainBytes =
            encryptDecrypt(cipherBytes, recoveredSeed);

        cout << "Recovered seed: " << recoveredSeed << '\n';
        cout << "Recovered plaintext: "
             << bytesToString(finalPlainBytes) << '\n';

        cout << "Full plaintext check: "
             << (finalPlainBytes == plainBytes ? "SUCCESS" : "FAILED")
             << '\n';
    } else {
        cout << "No seed found.\n";
    }

    // ---------------- PART B ----------------
    cout << "\n========== PART B: RESET TOKEN ==========\n";

    // Use the current Unix timestamp as the seed.
    unsigned int currentTime = (unsigned int)time(nullptr);

    // Generate the token.
    vector<unsigned char> originalToken = makeToken(currentTime);

    cout << "Original timestamp: " << currentTime << '\n';
    cout << "Original token (hex): ";
    printHex(originalToken);

    // Search timestamps from 5 minutes before to 5 minutes after.
    unsigned int startTime = currentTime - 300;
    unsigned int endTime = currentTime + 300;

    int recoveredTime = -1;
    int testedTimes = 0;

    for (unsigned int testTime = startTime;
         testTime <= endTime;
         testTime++) {

        testedTimes++;

        vector<unsigned char> testToken = makeToken(testTime);

        if (testToken == originalToken && recoveredTime == -1) {
            recoveredTime = testTime;
        }
    }

    cout << "Search interval: " << startTime
         << " to " << endTime << '\n';
    cout << "Timestamps tested: " << testedTimes << '\n';

    if (recoveredTime != -1) {
        vector<unsigned char> checkedToken = makeToken(recoveredTime);

        cout << "Recovered timestamp: " << recoveredTime << '\n';
        cout << "Regenerated token (hex): ";
        printHex(checkedToken);

        cout << "Token check: "
             << (checkedToken == originalToken ? "SUCCESS" : "FAILED")
             << '\n';
    } else {
        cout << "Timestamp not found.\n";
    }

    return 0;
}
