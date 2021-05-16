#ifndef _huffman_h
#define _huffman_h

#include <cstdlib>
#include <string>
#include <string.h>
#include <queue>
#include <map>
#include <fstream>

#include<qapplication.h>
#include <qpushbutton.h>
#include <qfont.h>
#include <QFileDialog>
#include <QMessageBox>
using namespace std;

#define MAX_SIZE 270
#define WRITE_BUFF_SIZE 10
#define PSEUDO_EOF 256

struct Huffman_node
{
    int id;
    unsigned int freq;
    string code;
    Huffman_node *left,*right,*parent;

};

typedef Huffman_node* Node_ptr;

class Huffman{
private:
    Node_ptr node_array[MAX_SIZE];
    Node_ptr root;
    int size;
    fstream in_file,out_file;
    map<int,string> table;
    class Compare
    {
    public:
        bool operator()(const Node_ptr& c1,const Node_ptr& c2) const
        {
            return (*c1).freq > (*c2).freq;

        }
    };
    priority_queue<Node_ptr,vector<Node_ptr>,Compare>pq;
    void create_node_array();
    void create_map_table(const Node_ptr node,bool left);
    void create_pq();
    void create_huffman_tree();
    void calculate_huffman_codes();
    void do_compress();
    void  rebuid_huffman_tree();
    void decode_huffman();
public:
    Huffman(string in_file_name,string out_file_name);
    ~Huffman();
    void compress();
    void decompress();
};
#endif // HUFFAMN_H
