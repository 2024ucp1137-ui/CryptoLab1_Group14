#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <iomanip>
#include <cfloat>
#include <algorithm>
#include <numeric>

using namespace std;

/*
============================================================
 Assignment 6
 Cryptanalysis of Vigenere Cipher using
 Kasiski Examination and Frequency Analysis

 Group 14 -> Even Group -> Ciphertext 2
============================================================
*/


// ============================================================
// 1. clean_ciphertext()
// ============================================================

string clean_ciphertext(const string& text)
{
    string result;

    for (char ch : text)
    {
        if (isalpha(static_cast<unsigned char>(ch)))
        {
            result += toupper(static_cast<unsigned char>(ch));
        }
    }

    return result;
}


// ============================================================
// 2. find_repeated_patterns()
// ============================================================

map<string, vector<int>> find_repeated_patterns(
    const string& text)
{
    map<string, vector<int>> patterns;

    // Look for repeated trigrams, 4-grams and 5-grams
    for (int length = 3; length <= 5; length++)
    {
        for (int i = 0;
             i + length <= (int)text.length();
             i++)
        {
            string pattern = text.substr(i, length);

            patterns[pattern].push_back(i);
        }
    }

    return patterns;
}


// ============================================================
// 3. calculate_distances()
// ============================================================

vector<int> calculate_distances(
    const map<string, vector<int>>& patterns)
{
    vector<int> distances;

    for (const auto& item : patterns)
    {
        const vector<int>& positions = item.second;

        if (positions.size() < 2)
            continue;

        for (size_t i = 1; i < positions.size(); i++)
        {
            distances.push_back(
                positions[i] - positions[i - 1]
            );
        }
    }

    return distances;
}


// ============================================================
// 4. find_factors()
// ============================================================

vector<int> find_factors(int distance)
{
    vector<int> factors;

    for (int i = 2; i <= distance; i++)
    {
        if (distance % i == 0)
        {
            factors.push_back(i);
        }
    }

    return factors;
}


// ============================================================
// 5. kasiski_analysis()
// ============================================================

int kasiski_analysis(const string& text)
{
    map<string, vector<int>> patterns =
        find_repeated_patterns(text);

    vector<int> distances =
        calculate_distances(patterns);

    map<int, int> factorCount;

    cout << "\n================ KASISKI ANALYSIS ================\n";

    cout << "\nRepeated patterns:\n";

    for (const auto& item : patterns)
    {
        if (item.second.size() > 1)
        {
            cout << item.first << " : ";

            for (int position : item.second)
            {
                cout << position << " ";
            }

            cout << "\n";
        }
    }

    cout << "\nFactor frequency:\n";

    for (int distance : distances)
    {
        vector<int> factors =
            find_factors(distance);

        for (int factor : factors)
        {
            // Key lengths above 20 are not useful here
            if (factor <= 20)
            {
                factorCount[factor]++;
            }
        }
    }

    vector<pair<int, int>> ranked;

    for (const auto& item : factorCount)
    {
        ranked.push_back(item);
    }

    sort(
        ranked.begin(),
        ranked.end(),
        [](const pair<int, int>& a,
           const pair<int, int>& b)
        {
            return a.second > b.second;
        }
    );

    for (const auto& item : ranked)
    {
        cout << "Length "
             << item.first
             << " -> "
             << item.second
             << " occurrences\n";
    }

    if (ranked.empty())
        return 1;

    return ranked[0].first;
}


// ============================================================
// 6. calculate_ic()
// ============================================================

double calculate_ic(const string& text)
{
    if (text.length() < 2)
        return 0.0;

    int frequency[26] = {0};

    for (char ch : text)
    {
        frequency[ch - 'A']++;
    }

    double numerator = 0;

    for (int i = 0; i < 26; i++)
    {
        numerator +=
            frequency[i] * (frequency[i] - 1);
    }

    double denominator =
        text.length() * (text.length() - 1);

    return numerator / denominator;
}


// ============================================================
// 7. split_into_groups()
// ============================================================

vector<string> split_into_groups(
    const string& text,
    int keyLength)
{
    vector<string> groups(keyLength);

    for (int i = 0; i < (int)text.length(); i++)
    {
        groups[i % keyLength] += text[i];
    }

    return groups;
}


// ============================================================
// English frequency table
// ============================================================

const double englishFrequency[26] =
{
    0.082, 0.015, 0.028, 0.043, 0.127,
    0.022, 0.020, 0.061, 0.070, 0.0015,
    0.0077, 0.040, 0.024, 0.067, 0.075,
    0.019, 0.00095, 0.060, 0.063, 0.091,
    0.028, 0.0098, 0.024, 0.0015, 0.020,
    0.00074
};


// ============================================================
// Helper: average IC for a candidate key length
// ============================================================

double average_ic_for_length(
    const string& text,
    int keyLength)
{
    vector<string> groups =
        split_into_groups(text, keyLength);

    double total = 0.0;

    for (const string& group : groups)
    {
        total += calculate_ic(group);
    }

    return total / groups.size();
}


// ============================================================
// Choose key length using Kasiski candidates + IC
// ============================================================

int choose_key_length(
    const string& text,
    int kasiskiLength)
{
    /*
       Kasiski may return a divisor such as 3.

       Important candidates are multiples of that
       length. We compare their average IC values.
    */

    vector<int> candidates;

    for (int length = 2; length <= 20; length++)
    {
        if (length % kasiskiLength == 0)
        {
            candidates.push_back(length);
        }
    }

    // Also explicitly test common candidates
    candidates.push_back(6);
    candidates.push_back(12);

    sort(candidates.begin(), candidates.end());

    candidates.erase(
        unique(candidates.begin(), candidates.end()),
        candidates.end()
    );

    int bestLength = candidates[0];
    double bestIC = 0.0;

    cout << "\n================ IC CANDIDATES ================\n";

    for (int length : candidates)
    {
        double avgIC =
            average_ic_for_length(text, length);

        cout << "Key length "
             << setw(2) << length
             << " -> Average IC = "
             << fixed << setprecision(4)
             << avgIC
             << "\n";

        if (avgIC > bestIC)
        {
            bestIC = avgIC;
            bestLength = length;
        }
    }

    return bestLength;
}


// ============================================================
// 8. frequency_analysis()
// ============================================================

void frequency_analysis(
    const string& group,
    int groupNumber)
{
    int frequency[26] = {0};

    for (char ch : group)
    {
        frequency[ch - 'A']++;
    }

    cout << "\n---------------- Group "
         << groupNumber + 1
         << " ----------------\n";

    cout << "Letter\tCount\n";

    for (int i = 0; i < 26; i++)
    {
        cout << char('A' + i)
             << "\t"
             << frequency[i]
             << "\n";
    }

    cout << "IC = "
         << fixed << setprecision(4)
         << calculate_ic(group)
         << "\n";
}


// ============================================================
// 9. find_shift()
// ============================================================

int find_shift(const string& group)
{
    int bestShift = 0;
    double bestScore = DBL_MAX;

    int n = group.length();

    for (int shift = 0; shift < 26; shift++)
    {
        int observed[26] = {0};

        /*
           Decrypt this group using every possible
           Caesar shift and compare its frequency
           distribution with English.
        */

        for (char ch : group)
        {
            int plain =
                (ch - 'A' - shift + 26) % 26;

            observed[plain]++;
        }

        double chiSquare = 0.0;

        for (int i = 0; i < 26; i++)
        {
            double expected =
                englishFrequency[i] * n;

            if (expected > 0)
            {
                chiSquare +=
                    (observed[i] - expected) *
                    (observed[i] - expected) /
                    expected;
            }
        }

        if (chiSquare < bestScore)
        {
            bestScore = chiSquare;
            bestShift = shift;
        }
    }

    return bestShift;
}


// ============================================================
// 10. find_key()
// ============================================================

string find_key(const vector<string>& groups)
{
    string key;

    for (const string& group : groups)
    {
        int shift =
            find_shift(group);

        key += char('A' + shift);
    }

    return key;
}


// ============================================================
// 11. vigenere_decrypt()
// ============================================================

string vigenere_decrypt(
    const string& ciphertext,
    const string& key)
{
    string plaintext;

    for (int i = 0;
         i < (int)ciphertext.length();
         i++)
    {
        int c =
            ciphertext[i] - 'A';

        int k =
            key[i % key.length()] - 'A';

        int p =
            (c - k + 26) % 26;

        plaintext +=
            char('A' + p);
    }

    return plaintext;
}


// ============================================================
// 12. vigenere_encrypt()
// ============================================================

string vigenere_encrypt(
    const string& plaintext,
    const string& key)
{
    string ciphertext;

    for (int i = 0;
         i < (int)plaintext.length();
         i++)
    {
        int p =
            plaintext[i] - 'A';

        int k =
            key[i % key.length()] - 'A';

        int c =
            (p + k) % 26;

        ciphertext +=
            char('A' + c);
    }

    return ciphertext;
}


// ============================================================
// 13. verify()
// ============================================================

bool verify(
    const string& originalCiphertext,
    const string& plaintext,
    const string& key)
{
    string encrypted =
        vigenere_encrypt(plaintext, key);

    return encrypted == originalCiphertext;
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    /*
    ------------------------------------------------------------
    Ciphertext 2 from Assignment 6
    Group 14 = Even Group
    ------------------------------------------------------------
    */

    string ciphertext = R"(
QRBAI UWYOK ILBRZ XTUWL EGXSN VDXWR XMHXY FCGMW
WWSME LSXUZ
MKMFS BNZIF YEIEG RFZRX WKUFA XQEDX DTTHY NTBRJ
LHTAI KOCZX
QHBND ZIGZG PXARJ EDYSJ NUMKI FLBTN HWISW NVLFM
EGXAI AAWSL
FMHXR SGRIG HEQTU MLGLV BRSIL AEZSG XCMHT OWHFM
LWMRK HPRFB
ELWGF RUGPB HNBEM KBNVW HHUEA KILBN BMLHK XUGML
YQKHP RFBEL
EJYNV WSIJB GAXGO TPMXR TXFKI WUALB RGWIE GHWHG
AMEWW LTAEL
NUMRE UWTBL SDPRL YVRET LEEDF ROBEQ UXTHX ZYOZB
XLKAC KSOHN
VWXKS MAEPH IYQMM FSECH RFYPB BSQTX TPIWH GPXQD
FWTAI KNNBX
SIYKE TXTLV BTMQA LAGHG OTPMX RTXTH XSFYG WMVKH
LOIVU ALMLD
LTSYV WYNVW MQVXP XRVYA BLXDL XSMLW SUIOI IMELI
SOYEB HPHNR
WTVUI AKEYG WIETG WWBVM VDUMA EPAUA KXWHK MAUPA
MUKHQ PWKCX
EFXGW WSDDE OMLWL FWTAM FAFEA MFZBN WIHYA
LXRWK MAMIK
GNGHJ UAZHM HGUAL YSULA ELYHJ BZMSI LAILH WWYIK
EWAHN PMLBN
NBVPJ XLBEF WRWGX KWIRH XWWGQ HRRXW IOMFY CZHZL
VXNVI OYZCM
YDDEY IPWXT MMSHS VHHXZ YEWNV OAOEL SMLSW KXXFX
STRVI HZLEF
JXDAS FIE
)";


    // ========================================================
    // STEP 1: PREPROCESS
    // ========================================================

    ciphertext =
        clean_ciphertext(ciphertext);

    cout << "====================================================\n";
    cout << "       VIGENERE CIPHER CRYPTANALYSIS\n";
    cout << "                    GROUP 14\n";
    cout << "====================================================\n";

    cout << "\nCiphertext length = "
         << ciphertext.length()
         << "\n";


    // ========================================================
    // STEP 2: KASISKI
    // ========================================================

    int kasiskiLength =
        kasiski_analysis(ciphertext);

    cout << "\nKasiski suggested key length = "
         << kasiskiLength
         << "\n";


    // ========================================================
    // STEP 3: INDEX OF COINCIDENCE
    // ========================================================

    cout << "\n================ INDEX OF COINCIDENCE ================\n";

    cout << "Overall IC = "
         << fixed << setprecision(4)
         << calculate_ic(ciphertext)
         << "\n";

    int keyLength =
        choose_key_length(
            ciphertext,
            kasiskiLength
        );

    cout << "\nEstimated key length = "
         << keyLength
         << "\n";


    // ========================================================
    // STEP 4: SPLIT INTO GROUPS
    // ========================================================

    vector<string> groups =
        split_into_groups(
            ciphertext,
            keyLength
        );

    double averageIC = 0.0;

    for (const string& group : groups)
    {
        averageIC +=
            calculate_ic(group);
    }

    averageIC /= groups.size();

    cout << "Average IC of groups = "
         << fixed << setprecision(4)
         << averageIC
         << "\n";


    // ========================================================
    // STEP 5: FREQUENCY ANALYSIS
    // ========================================================

    cout << "\n================ FREQUENCY ANALYSIS ================\n";

    for (int i = 0;
         i < (int)groups.size();
         i++)
    {
        frequency_analysis(
            groups[i],
            i
        );
    }


    // ========================================================
    // STEP 6: FIND KEY
    // ========================================================

    string key =
        find_key(groups);

    cout << "\n================ RECOVERED KEY ================\n";

    cout << "Key length = "
         << key.length()
         << "\n";

    cout << "Recovered key = "
         << key
         << "\n";


    // ========================================================
    // STEP 7: DECRYPT
    // ========================================================

    string plaintext =
        vigenere_decrypt(
            ciphertext,
            key
        );

    cout << "\n================ RECOVERED PLAINTEXT ================\n\n";

    /*
       Print plaintext in readable 80-character lines.
    */

    for (int i = 0;
         i < (int)plaintext.length();
         i++)
    {
        cout << plaintext[i];

        if ((i + 1) % 80 == 0)
            cout << "\n";
    }

    cout << "\n";


    // ========================================================
    // STEP 8: VERIFY
    // ========================================================

    cout << "\n================ VERIFICATION ================\n";

    if (verify(
            ciphertext,
            plaintext,
            key))
    {
        cout << "Re-encryption matches original ciphertext: SUCCESS\n";
    }
    else
    {
        cout << "Verification FAILED\n";
    }


    return 0;
}