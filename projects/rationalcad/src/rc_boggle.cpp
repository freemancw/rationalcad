/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 12/19/2013
 */

#include "rc_boggle.h"

namespace RCAD {

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
        Node* next_node = nullptr;

        for (size_t j = 0; j < cur_node->children().size(); ++j) {
            if (cur_node->children()[j]->c() == word[i]) {
                next_node = cur_node->children()[j];
                break;
            }
        }

        if (next_node == nullptr) {
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

//=============================================================================
// Implementation: BoggleBoard
//=============================================================================

BoggleBoard::BoggleBoard() :
    rows_(0),
    cols_(0),
    vertices_(nullptr) {}

BoggleBoard::BoggleBoard(const size_t rows, const size_t cols,
                         const std::string& s) :
    rows_(rows),
    cols_(cols),
    vertices_(new Vertex[rows*cols]) {

    if (!s.empty()) {
        assert(s.size() == rows*cols);
        for (size_t i = 0; i < s.size(); ++i) {
            vertices_[i].set_c(s[i]);
        }
    }
}

BoggleBoard::~BoggleBoard() {
    delete[] vertices_;
}

void BoggleBoard::PrintBoard() const {
    std::stringstream board;
    for (size_t row = 0; row < rows_; ++row) {
        for (size_t col = 0; col < cols_; ++col) {
            board << (*this)(row, col).c() << " ";
        }
        board << "\n";
    }
    qDebug() << board.str().c_str();
}

const BoggleBoard::Vertex& BoggleBoard::operator()(const size_t row,
                                                   const size_t col) const {
    assert(row < rows_ && col < cols_);
    return vertices_[row*cols_+col];
}

BoggleBoard::Vertex& BoggleBoard::operator()(const size_t row,
                                             const size_t col) {
    assert(row < rows_ && col < cols_);
    return vertices_[row*cols_+col];
}

const size_t BoggleBoard::rows() const {
    return rows_;
}
const size_t BoggleBoard::cols() const {
    return cols_;
}

//=============================================================================
// Implementation: BoggleBoard::Vertex
//=============================================================================

BoggleBoard::Vertex::Vertex() :
    c_('\0'),
    v_(false) {}

BoggleBoard::Vertex::Vertex(const char c, const bool v) :
    c_(c),
    v_(v) {}

const char BoggleBoard::Vertex::c() const {
    return c_;
}
const bool BoggleBoard::Vertex::v() const {
    return v_;
}
void BoggleBoard::Vertex::set_c(const char c) {
    c_ = c;
}
void BoggleBoard::Vertex::set_v(const bool v) {
    v_ = v;
}

//=============================================================================
// Solving functions
//=============================================================================

typedef std::pair<size_t, size_t> grididx;

std::vector<grididx> list_neighbors(const size_t row,
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

/*!
 * Recursive function for solving a boggle board. In the middle of solving a
 * board, we will have traversed from starting point through a path of
 * neighboring vertices. We will be located at some particular vertex on the
 * board. If we add the character for this vertex to the prefix passed into us,
 * then we may do several things:
 *
 * 1. Generate a word.
 * 2. Generate a valid prefix.
 * 3. Generate an invalid prefix.
 *
 * (1) may occur with either (2) or (3), but obviously (2) and (3) are mutually
 * exclusive. If (2) happens, then we have to investigate any unvisited
 * neighbors, otherwise we can return our current wordlist since it isn't
 * possible to generate any more words.
 *
 * If our dictionary used a hashtable with entire words as keys, we would not
 * be able to quickly answer whether or not a prefix is valid. In this scheme,
 * we would have to exhaustively check all possible paths through the board,
 * without using the valid prefix stopping criterion. This would require a
 * O(1) lookup for every path in the board.
 *
 * Another option for the dictionary is to have a fixed-size array for each
 * letter. If we consider
 *
 * Our dictionary type should be able to return to us a node data structure
 * that supports fast access to
 */
std::vector<std::string> solve_boggle_r(BoggleBoard& board,
                                        const size_t row,
                                        const size_t col,
                                        const Dictionary::Node* cur_node,
                                        const std::string& cur_prefix) {
    std::vector<std::string> wordlist;
    std::string next_prefix = cur_prefix+board(row, col).c();
    board(row, col).set_v(true);

    Dictionary::Node* next_node = nullptr;
    for (size_t i = 0; i < cur_node->children().size(); ++i) {
        if (cur_node->children()[i]->c() == board(row, col).c()) {
            next_node = cur_node->children()[i];
            break;
        }
    }

    if (next_node) {
        if (next_node->is_word() && next_prefix.size() > 3) {
            wordlist.push_back(next_prefix);
        }

        auto neighbors = list_neighbors(row, col, board.rows(), board.cols());
        for (size_t i = 0; i < neighbors.size(); ++i) {
            auto n = board(neighbors[i].first, neighbors[i].second);
            if (!n.v()) {
                auto subsln = solve_boggle_r(board, neighbors[i].first,
                                             neighbors[i].second, next_node,
                                             next_prefix);
                wordlist.insert(end(wordlist), begin(subsln), end(subsln));
            }
        }
    }

    board(row, col).set_v(false);
    return wordlist;
}

std::vector<std::string> solve_board(BoggleBoard& board,
                                     const Dictionary& dictionary) {
    std::vector<std::string> wordlist;

    for (size_t row = 0; row < board.rows(); ++row) {
        for (size_t col = 0; col < board.cols(); ++col) {
            auto subsln = solve_boggle_r(board, row, col,
                                         dictionary.root(), "");
            wordlist.insert(end(wordlist), begin(subsln), end(subsln));
        }
    }

    return wordlist;
}

} // namespace RCAD


