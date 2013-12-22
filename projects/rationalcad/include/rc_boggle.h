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

std::vector<std::string> solve_board(const size_t rows, const size_t cols,
                                     const std::string& s,
                                     const Dictionary& dictionary);

} // namespace RCAD

#endif // RC_BOGGLE_H
