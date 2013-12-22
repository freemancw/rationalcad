/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 12/19/2013
 */

#include "rc_array.h"
#include "rc_boggle.h"

namespace RCAD {

// shorten names
typedef std::pair<size_t, size_t> grididx;

// convenience function for generating a list of grid neighbors
static std::vector<grididx> list_neighbors(const size_t row, const size_t col,
                                           const size_t num_rows,
                                           const size_t num_cols);

// recursive function for actually solving the boggle board
static std::vector<std::string> solve_boggle_r(Array<bool>& visited,
                                               const Array<char>& board,
                                               const size_t row,
                                               const size_t col,
                                               const Dictionary::Node* node,
                                               const std::string& prefix);

// default to standard boggle rules
static const size_t kMinWordLength = 3;

//=============================================================================
// Solving functions
//=============================================================================

/*!
 * Given a boggle board and a dictionary, this function returns a list
 * containing all valid words that one may generate according to the rules
 * of the game.
 */
std::vector<std::string> solve_board(const size_t rows, const size_t cols,
                                     const std::string& entries,
                                     const Dictionary& dictionary) {
    std::vector<std::string> wordlist;

    // validate input
    if (rows < 2 || cols < 2) {
        qDebug() << "solve_board: each dimensions must be >= 2.";
        return wordlist;
    } else if (entries.size() != rows*cols) {
        qDebug() << "solve_board: input length must match board dimensions.";
        return wordlist;
    }

    // fill a 2D array with the board string
    Array<char> board(rows, cols);
    for (size_t i = 0; i < entries.size(); ++i) {
        board(i) = entries[i];
    }

    // init another 2D array to track whether we've visited a board cell
    Array<bool> visited(rows, cols);
    for (size_t i = 0; i < entries.size(); ++i) {
        visited(i) = false;
    }

    // recursively solve the board using each cell as a starting point
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            auto subsln = solve_boggle_r(visited, board, i, j,
                                         dictionary.root(), "");
            wordlist.insert(end(wordlist), begin(subsln), end(subsln));
        }
    }

    return wordlist;
}

/*!
 *
 */
static std::vector<std::string> solve_boggle_r(Array<bool>& visited,
                                               const Array<char>& board,
                                               const size_t row,
                                               const size_t col,
                                               const Dictionary::Node* node,
                                               const std::string& prefix) {
    std::vector<std::string> wordlist;
    std::string next_prefix = prefix+board(row, col);

    // base case: we stop the recursion whenever a cell's character does not
    // form a valid prefix (i.e. we cannot find the character in the input
    // node's children)

    // attempt to find the char among the children
    Dictionary::Node* next_node = NULL;
    for (size_t i = 0; i < node->children().size(); ++i) {
        if (node->children()[i]->c() == board(row, col)) {
            next_node = node->children()[i];
            break;
        }
    }

    // if we found it, do recursive work
    if (next_node) {
        visited(row, col) = true;

        // add valid words to output list
        if (next_node->is_word() && next_prefix.size() > kMinWordLength) {
            wordlist.push_back(next_prefix);
        }

        // recurse on each neighbor (if we haven't already)
        auto nlist = list_neighbors(row, col, board.n_rows(), board.n_cols());
        for (size_t i = 0; i < nlist.size(); ++i) {
            if (!visited(nlist[i].first, nlist[i].second)) {
                auto subsln = solve_boggle_r(visited, board, nlist[i].first,
                                             nlist[i].second, next_node,
                                             next_prefix);
                wordlist.insert(end(wordlist), begin(subsln), end(subsln));
            }
        }

        visited(row, col) = false;
    }
    // else, base case, return empty list

    return wordlist;
}

/*!
 *
 */
static std::vector<grididx> list_neighbors(const size_t row,
                                           const size_t col,
                                           const size_t num_rows,
                                           const size_t num_cols) {
    std::vector<std::pair<size_t, size_t>> neighbors;

    if (row == 0) {
        // all north edge vertices have a neighbor directly below
        neighbors.push_back(grididx(row+1, col));

        if (col == 0) {
            // northwest corner
            neighbors.push_back(grididx(row, col+1));
            neighbors.push_back(grididx(row+1, col+1));
        } else if (col > 0 && col < num_cols-1) {
            // north edge
            neighbors.push_back(grididx(row, col+1));
            neighbors.push_back(grididx(row, col-1));
            neighbors.push_back(grididx(row+1, col+1));
            neighbors.push_back(grididx(row+1, col-1));
        } else {
            // northeast corner
            neighbors.push_back(grididx(row, col-1));
            neighbors.push_back(grididx(row+1, col-1));
        }
    } else if (row > 0 && row < num_rows-1) {
        // all N-S interior vertices have neighbors above and below
        neighbors.push_back(grididx(row+1, col));
        neighbors.push_back(grididx(row-1, col));

        if (col == 0) {
            // west edge
            neighbors.push_back(grididx(row, col+1));
            neighbors.push_back(grididx(row+1, col+1));
            neighbors.push_back(grididx(row-1, col+1));
        } else if (col > 0 && col < num_cols-1) {
            // interior
            neighbors.push_back(grididx(row, col+1));
            neighbors.push_back(grididx(row, col-1));
            neighbors.push_back(grididx(row+1, col+1));
            neighbors.push_back(grididx(row+1, col-1));
            neighbors.push_back(grididx(row-1, col+1));
            neighbors.push_back(grididx(row-1, col-1));
        } else {
            // east edge
            neighbors.push_back(grididx(row, col-1));
            neighbors.push_back(grididx(row+1, col-1));
            neighbors.push_back(grididx(row-1, col-1));
        }
    } else {
        // all south edge vertices have a neighbor directly above
        neighbors.push_back(grididx(row-1, col));

        if (col == 0) {
            // southwest corner
            neighbors.push_back(grididx(row, col+1));
            neighbors.push_back(grididx(row-1, col+1));
        } else if (col > 0 && col < num_cols-1) {
            // south edge
            neighbors.push_back(grididx(row, col+1));
            neighbors.push_back(grididx(row, col-1));
            neighbors.push_back(grididx(row-1, col+1));
            neighbors.push_back(grididx(row-1, col-1));
        } else {
            // southeast corner
            neighbors.push_back(grididx(row, col-1));
            neighbors.push_back(grididx(row-1, col-1));
        }
    }

    return neighbors;
}

//=============================================================================
// Implementation: Dictionary
//=============================================================================

Dictionary::Dictionary() :
    root_(new Node()) {}

Dictionary::Dictionary(const std::string& filename) :
    root_(new Node()) {
    std::ifstream dfile;

    dfile.open(filename.c_str(), std::ifstream::in);
    if (dfile.good()) {
        qDebug() << "open successful.";
    } else {
        qDebug() << "open failed.";
    }

    while (!dfile.eof()) {
        std::string line;
        std::getline(dfile, line);
        AddWord(line);
    }
}

void Dictionary::AddWord(const std::string& word) {
    Node* cur_node = root_;

    for (size_t i = 0; i < word.size(); ++i) {
        Node* next_node = NULL;

        for (size_t j = 0; j < cur_node->children().size(); ++j) {
            if (cur_node->children()[j]->c() == word[i]) {
                next_node = cur_node->children()[j];
                break;
            }
        }

        if (!next_node) {
            next_node = new Node(word[i], i == word.size()-1);
            cur_node->AddChild(next_node);
        }

        cur_node = next_node;
    }
}

void Dictionary::PrintAll_r(const std::string &prefix,
                            const Node *cur_node) const {
    std::string next = prefix;

    if (cur_node->c()) {
         next = prefix + cur_node->c();
    }

    if (cur_node->is_word()) {
        qDebug() << next.c_str();
    }

    for (size_t i = 0; i < cur_node->children().size(); ++i) {
        PrintAll_r(next, cur_node->children()[i]);
    }
}

void Dictionary::PrintAll() const {
    PrintAll_r("", root_);
}

const Dictionary::Node* Dictionary::root() const {
    return root_;
}

//=============================================================================
// Implementation: Dictionary::Node
//=============================================================================

Dictionary::Node::Node() :
    c_('\0'),
    is_word_(false) {}

Dictionary::Node::Node(const char c, bool is_word) :
    c_(c),
    is_word_(is_word) {}

void Dictionary::Node::AddChild(Dictionary::Node* child) {
    children_.push_back(child);
}

const char Dictionary::Node::c() const {
    return c_;
}
bool Dictionary::Node::is_word() const {
    return is_word_;
}
const std::vector<Dictionary::Node*>& Dictionary::Node::children() const {
    return children_;
}

} // namespace RCAD


