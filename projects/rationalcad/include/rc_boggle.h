/*!
 * @author Clinton Freeman <freeman@cs.unc.edu>
 * @date 12/19/2013
 */

#ifndef RC_BOGGLE_H
#define RC_BOGGLE_H

#include "rc_common.h"

namespace RCAD {

//=============================================================================
// Interface: Dictionary
//=============================================================================

class Dictionary {
public:

    class Node {
    public:
        Node();
        Node(const char c, bool is_word = false);

        void AddChild(Node* child);

        const char c() const;
        bool is_word() const;
        const std::vector<Node*>& children() const;

    private:
        char c_;
        bool is_word_;
        std::vector<Node*> children_;
    };

    Dictionary();
    Dictionary(const std::string& filename);

    void AddWord(const std::string& word);
    void PrintAll() const;
    //bool Contains(const std::string& word) const;

    const Node* root() const;

private:
    Node* root_;

    void PrintAll_r(const std::string& prefix, const Node* cur_node) const;
};

//=============================================================================
// Interface: BoggleBoard
//=============================================================================

/*!
 * A boggle board can be viewed as an undirected graph, where each "slot" is a
 * vertex, and neighboring vertices are connected by an edge. The maximum
 * degree of any vertex in a board is 8, and the minimum degree is 3. A board
 * B with n rows and m columns has |B.V| = nm vertices. A complete graph with
 * k vertices has
 *
 */
class BoggleBoard {
public:

    class Vertex {
    public:
        Vertex();
        Vertex(const char c, const bool visited = false);

        const char c() const;
        const bool v() const;
        void set_c(const char c);
        void set_v(const bool v);

    private:
        char c_;    // character at this vertex
        bool v_;    // has this vertex been visited?
    };

    BoggleBoard();
    BoggleBoard(const size_t rows, const size_t cols,
                const std::string& s = "");
    ~BoggleBoard();

    void PrintBoard() const;

    const Vertex& operator()(const size_t row, const size_t col) const;
    Vertex& operator()(const size_t row, const size_t col);

    const size_t rows() const;
    const size_t cols() const;

private:
    size_t rows_;
    size_t cols_;
    Vertex *vertices_;
};

std::vector<std::string> solve_board(BoggleBoard& board,
                                     const Dictionary& dictionary);

} // namespace RCAD

#endif // RC_BOGGLE_H
