/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 12/19/2013
 */

#include "rc_reverse.h"

// now we want to go to the store by the traintracks
// 3   2  4    2  2  2  3   5     2  3   11

// now we want
// traintracks

/*
 * This implementation of reverse_words takes O(n) time and O(n) space.
 *
 */
void reverse_words(char* sentence) {
    // initialize pointers to beginning and end of sentence, O(n) time O(1) space
    char *left, *right;
    left = right = sentence;
    while (*right) {
        ++right;
    }
    --right;

    // reverse entire string, O(n) time O(1) space
    while (left < right) {
        char tmp = *right;
        *right = *left;
        *left = tmp;
        ++left;
        --right;
    }

    char *start;
    start = left = sentence;
    while (*left) {
        char *end = left;
        if (*end == ' ' || *(end+1) == '\0') {
            if (*end == ' ') {
                --end;
            }
            while (start < end) {
                char tmp = *end;
                *end = *start;
                *start = tmp;
                ++start;
                --end;
            }
            start = left+1;
        }
        ++left;
    }
}








/*
void reverse_words(char* sentence) {
    std::stack<char> tmp;

    // initialize pointers to beginning and end of sentence, O(n)
    char *fL, *fR, *bL, *bR, *back;
    fL = fR = bL = bR = back = sentence;
    while (*back) {
        ++back;
    }
    bL = bR = --back;

    while (fR < bL) {
        int diff = fR-fL;
        int diff2 = back-bR;
        int diff3 = bR-bL;
        tmp.push(*bL);

        if (*fR == ' ') {
            while (fL < fR) {
                *(bL+diff-(fR-fL)+1) = *fL;
                ++fL;
            }
            *bL = *fL;
        }

        if (tmp.top() == ' ') {
            while (bL < bR) {
                *(sentence+diff2+diff3-(bR-bL)) = tmp.top();
                tmp.pop();
                --bR;
            }
        }

        ++fR;
        --bL;
    }
}
*/
