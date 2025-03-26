/* File: OnlyConnect.cpp
 *
 * This program implements the function `onlyConnectize`, which converts a given string by
 * following a specific set of rules for transforming characters.
 *
 * Puzzle: Can you figure out the rule that turns the input string into the expected output?
 * Hint: The result contains only specific characters from the input string in uppercase.
 */

#include "OnlyConnect.h"
using namespace std;

string onlyConnectize(string phrase) {
    string result = "";

    // Loop through each character in the phrase
    for (char c : phrase) {
        // Check if the character is an alphabet letter
        if (isalpha(c)) {
            // If the character is a lowercase letter, convert it to uppercase
            if (islower(c)) {
                result += toupper(c);
            }
            // If the character is an uppercase letter, add it as-is
            else {
                result += c;
            }
        }
    }

    // Remove all vowels (A, E, I, O, U) from the result
    string filteredResult = "";
    for (char c : result) {
        if (c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            filteredResult += c;
        }
    }

    return filteredResult;
}

/* * * * * * Provided Test Cases * * * * * */
#include "GUI/SimpleTest.h"

PROVIDED_TEST("Handles single-character inputs.") {
    EXPECT_EQUAL(onlyConnectize("A"), "");
    EXPECT_EQUAL(onlyConnectize("Q"), "Q");
}

PROVIDED_TEST("Converts lower-case to upper-case.") {
    EXPECT_EQUAL(onlyConnectize("lowercase"), "LWRCS");
    EXPECT_EQUAL(onlyConnectize("uppercase"), "PPRCS");
}

/* * * * * * Student Test Case * * * * * */
STUDENT_TEST("Handles input with mixed case and vowels.") {
    EXPECT_EQUAL(onlyConnectize("aBcDeFgH"), "BCDFGH"); // All vowels removed
    EXPECT_EQUAL(onlyConnectize("helloWorld"), "HLLWRLD"); // Vowels removed, lowercase to uppercase
}

/* * * * * * Puzzle + Hint * * * * * */
//
// Puzzle: What pattern can you deduce about the characters that remain in the output after processing the string?
// Hint: Only some letters make it to the final string, and the case of the letters changes.
//






