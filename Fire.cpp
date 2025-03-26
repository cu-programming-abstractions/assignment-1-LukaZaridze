#include "Fire.h"
using namespace std;

void updateFire(Grid<int>& fire) {
    Grid<int> newFire = fire; // Create a copy to store updates

    // Propagate from bottom to top
    for (int row = fire.numRows() - 2; row >= 0; row--) {
        for (int col = 0; col < fire.numCols(); col++) {
            // Get value from cell below
            int belowValue = fire[row + 1][col];

            // If there's heat below, propagate it upward
            if (belowValue > 0) {
                // First try to propagate directly upward
                if (newFire[row][col] < belowValue) {  // Only propagate if target has less heat
                    int newHeat = belowValue;
                    // Apply cooling with 2/3 probability
                    if (randomChance(2.0 / 3)) {
                        newHeat = max(newHeat - 1, 0);
                    }
                    newFire[row][col] = newHeat;
                }

                // Then try diagonal propagation with 50% probability
                if (randomChance(0.5)) {
                    int direction = randomInteger(0, 1); // 0=left, 1=right
                    int targetCol = (direction == 0 && col > 0) ? col - 1 :
                                        ((direction == 1 && col < fire.numCols() - 1) ? col + 1 : col);

                    if (targetCol != col && newFire[row][targetCol] < belowValue) {
                        int newHeat = belowValue;
                        if (randomChance(2.0 / 3)) {
                            newHeat = max(newHeat - 1, 0);
                        }
                        newFire[row][targetCol] = newHeat;
                    }
                }
            }
            // Propagate zeros upward if cell below is zero
            else if (belowValue == 0 && newFire[row][col] > 0) {
                newFire[row][col] = 0;
            }
        }
    }

    fire = newFire; // Update the original grid
}




#include "GUI/SimpleTest.h"

/*** Tests for startFire ***/

PROVIDED_TEST("updateFire does not change dimensions of world.") {
    const int numRows = 5;
    const int numCols = 7;

    /* Make sure the fire has the right size to begin with. */
    Grid<int> fire(numRows, numCols);
    EXPECT_EQUAL(fire.numRows(), numRows);
    EXPECT_EQUAL(fire.numCols(), numCols);

    /* Update the fire; make sure everything is still there. */
    updateFire(fire);
    EXPECT_EQUAL(fire.numRows(), numRows);
    EXPECT_EQUAL(fire.numCols(), numCols);
}

PROVIDED_TEST("updateFire does not change bottom row.") {
    Grid<int> fire = {
        { 3, 3, 3 },
        { 5, 5, 5 },
        { 7, 7, 7 }
    };

    /* Update the fire; the bottom row should be unchanged. */
    updateFire(fire);
    for (int col = 0; col < 3; col++) {
        EXPECT_EQUAL(fire[2][col], 7);
    }
}

namespace {
/* Helper function to test if the specified value matches one of the items
     * from the given list.
     */
bool isOneOf(int) {
    return false;
}
template <typename First, typename... Rest> bool isOneOf(int value, First option1, Rest... remaining) {
    return value == option1 || isOneOf(value, remaining...);
}
}

PROVIDED_TEST("updateFire copies heat values upward.") {
    Grid<int> fire = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 4, 4, 4, 4, 4, 4, 4, 4 },
        { 9, 9, 9, 9, 9, 9, 9, 9 }
    };

    updateFire(fire);

    /* In the top row, every value should be either 0 (nothing copied
     * onto this point), 4 (copied from the row below), or 3 (copied,
     * then things cooled down).
     */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[0][col], 0, 3, 4));
    }

    /* The top row should include at least one 3 or 4. If not, nothing was propagated
     * upward from the row below.
     */
    bool found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (isOneOf(fire[0][col], 3, 4)) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Top row did not contain any 3 or 4 values.");
    }

    /* The middle row should be 4s, 8s, or 9s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[1][col], 4, 8, 9));
    }

    /* The middle row should include at least one 8 or one 9. If it doesn't, nothing
     * was copied upward.
     */
    found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (isOneOf(fire[1][col], 8, 9)) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Middle row did not contain any 8 or 9 values.");
    }

    /* The bottom row should be all 9s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT_EQUAL(fire[2][col], 9);
    }
}

PROVIDED_TEST("updateFire copies zero values upward.") {
    Grid<int> fire = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 4, 4, 4, 4, 4, 4, 4, 4 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    updateFire(fire);

    /* In the top row, every value should be either 0 (nothing copied
     * onto this point), 4 (copied from the row below), or 3 (copied,
     * then things cooled down).
     */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[0][col], 0, 3, 4));
    }

    /* The top row should include at least one 3 or 4. If not, nothing was propagated
     * upward from the row below.
     */
    bool found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (isOneOf(fire[0][col], 3, 4)) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Top row did not contain any 3 or 4 values.");
    }

    /* The middle row should be 4s or 0s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT(isOneOf(fire[1][col], 4, 0));
    }

    /* The middle row should include at least one 0. If not, nothing was propagated
     * upward from the row below.
     */
    found = false;
    for (int col = 0; col < fire.numCols(); col++) {
        if (fire[1][col] == 0) {
            found = true;
            break;
        }
    }
    if (!found) {
        SHOW_ERROR("Middle row did not contain any 0 values.");
    }

    /* The bottom row should be all 0s. */
    for (int col = 0; col < fire.numCols(); col++) {
        EXPECT_EQUAL(fire[2][col], 0);
    }
}

// Additional tests omitted for brevity, but similar to the ones above.
