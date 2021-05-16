#include "huffman.h"
#include<iostream>
void Huffman::create_node_array()
{
    int i,count;
    int freq[MAX_SIZE]={0};
    char in_char;

    while(!in_file.eof())
    {
        in_file.get(in_char);
        if(in_file.eof())
            break;
        freq[(unsigned char)in_char]++;
    }

    count = 0;
    for(i=0;i<MAX_SIZE;++i)
    {
        if(freq[i]<=0)
            continue;
        Node_ptr node = new Huffman_node();
        node->id=i;
        node->freq=freq[i];
        node->code="";
        node->left=NULL;
        node->right=NULL;
        node->parent=NULL;

        node_array[count++]=node;
    }

    Node_ptr node=new Huffman_node();
    node->id=PSEUDO_EOF;
    node->freq=1;
    node->code="";
    node->left=NULL;
    node->right=NULL;
    node->parent=NULL;

    node_array[count++]=node;

    size = count;
}

void Huffman::create_map_table(const Node_ptr node,bool left)
{
    if(left)
        node->code=node->parent->code +"0";
    else
        node->code = node->parent->code +"1";
    if(node->left == NULL&&node->right == NULL)
        table[node->id]=node->code;
    else
    {
        if(node->left != NULL)
            create_map_table(node->left,true);
        if(node->right != NULL)
            create_map_table(node->right,false);
    }
}
void Huffman::create_pq()
{
    int i;
    create_node_array();
    for(i=0;i<size;++i)
        pq.push(node_array[i]);
}
void Huffman::create_huffman_tree()
{
    root=NULL;
    while(!pq.empty())
    {
        Node_ptr first=pq.top();
        pq.pop();
        if(pq.empty())
        {
            root=first;
            break;
        }
        Node_ptr second=pq.top();
        pq.pop();
        Node_ptr new_node=new Huffman_node();
        new_node->freq=first->freq+second->freq;
        if(first->freq<=second->freq)
        {
            new_node->left=first;
            new_node->right=second;
        }
        else
        {
            new_node->left =second;
            new_node->right=first;
        }
        first->parent = new_node;
        second->parent = new_node;
        pq.push(new_node);
    }
}

void Huffman::calculate_huffman_codes()
{
    if(root==NULL)
    {
        printf("Build the huffman tree failed or no characters arecounted\n");
        exit(1);
    }
    if(root->left !=NULL)
        create_map_table(root->left,true);
    if(root->right!=NULL)
        create_map_table(root->right,false);
}

void Huffman::do_compress()
{
    int length,i,j,byte_count;
    char in_char;
    unsigned char out_c,tmp_c;
    string code,out_string;
    map<int,string>::iterator table_it;
    out_file<<size<<endl;
    for(table_it=table.begin();table_it != table.end();++table_it)
    {
        out_file<<table_it->first<<" "<<table_it->second<<endl;
    }

    in_file.clear();
    in_file.seekg(ios::beg);
    code.clear();
    while(!in_file.eof())
    {
        in_file.get(in_char);
        if(in_file.eof())
            break;
        table_it = table.find((unsigned char)in_char);
        if(table_it!=table.end())
            code+=table_it->second;
        else
        {
            printf("Can't find the huffman code of character %X\n", in_char);
            exit(1);

        }
        length=code.length();

        if(length>WRITE_BUFF_SIZE)
        {
            out_string.clear();
            for(i=0;i+7<length;i+=8)
            {
                out_c=0;
                for(j=0;j<8;j++)
                {
                    if('0'==code[i+j])
                        tmp_c=0;
                    else
                        tmp_c=1;
                    out_c+=tmp_c<<(7-j);
                }
                out_string+=out_c;
            }
            out_file<<out_string;
            code=code.substr(i,length-i);
        }
    }

    table_it=table.find(PSEUDO_EOF);
    if(table_it!=table.end())
        code+=table_it->second;
    else
    {
        printf("Can't find the huffman code of pseudo-EOF\n");
        exit(1);
    }

    length=code.length();
    out_c=0;
    for(i=0;i<length;i++)
    {
        if('0'==code[i])
            tmp_c=0;
        else
            tmp_c=1;
        out_c+=tmp_c<<(7-(i%8));
        if(0==(i+1)%8||i==length-1)
        {
            out_file<<out_c;
            out_c=0;
        }
    }
}

void Huffman::rebuid_huffman_tree()
{
    int i,j,id,length;
    string code;
    Node_ptr node,tmp,new_node;

    root= new Huffman_node();
    root->left = NULL;
    root->right=NULL;
    root->parent=NULL;
    in_file>>size;
    if(size>MAX_SIZE)
    {
        printf("The number of nodes is not valid.maybe the compressed file has been broken.\n");
        exit(1);
    }
    for(i=0;i<size;++i)
    {
        in_file>>id;
        in_file>>code;
        length=code.length();
        node=root;
        for(j=0;j<length;++j)
        {
            if('0'==code[j])
                tmp=node->left;
            else if('1'==code[j])
                tmp=node->right;
            else
            {
                printf("Decode error,huffman code is not made up with 0 or 1\n");
                exit(1);


            }
            if(tmp==NULL)
            {
                new_node = new Huffman_node();
                new_node->left=NULL;
                new_node->right=NULL;
                new_node->parent=node;

                if(j==length-1)
                {
                    new_node->id=id;
                    new_node->code=code;
                }
                if('0'==code[j])
                    node->left=new_node;
                else
                    node->right=new_node;
                tmp=new_node;
            }
            else if(j==length-1)
            {
                printf("Huffman code is not valid,maybe the compress file has been broken.\n");
                exit(1);
            }
            else if (tmp->left==NULL&&tmp->right==NULL)
            {
                printf("Huffman code is not valid,maybe the compressed file has been broken.\n");
                exit(1);
            }
            node = tmp;
        }
    }
}

void Huffman::decode_huffman()
{
    bool pseudo_eof;
    int i,id;
    char in_char;
    string out_string;
    unsigned char u_char,flag;
    Node_ptr node;

    out_string.clear();
    node = root;
    pseudo_eof=false;
    in_file.get(in_char);
    while(!in_file.eof())
    {
        in_file.get(in_char);
        u_char=(unsigned char)in_char;
        flag=0x80;
        for(i=0;i<8;++i)
        {
            if(u_char&flag)
                node = node->right;
            else
                node=node->left;
            if(node->left==NULL&&node->right==NULL)
            {
                id=node->id;
                if(id==PSEUDO_EOF)
                {
                    pseudo_eof=true;
                    break;
                }
                else
                {
                    out_string+=(char)node->id;
                    node=root;
                }
            }
            flag=flag>>1;
        }
        if(pseudo_eof)
            break;

        if(WRITE_BUFF_SIZE<out_string.length())
        {
            out_file<<out_string;
            out_string.clear();
        }
    }

    if(!out_string.empty())
        out_file<<out_string;

}

Huffman::Huffman(string in_file_name,string out_file_name)
{
    in_file.open(in_file_name.c_str(),ios::in);
    if(!in_file)
    {
        printf("Open file error,path is :%\n",in_file_name.c_str());
        exit(1);
    }

    out_file.open(out_file_name.c_str(),ios::out);
    if(!out_file)
    {
        printf("Open file error,path is:%s\n",out_file_name.c_str());
        exit(1);
    }


}

Huffman::~Huffman()
{
    in_file.close();
    out_file.close();
}

void Huffman::compress()
{
    create_pq();
    create_huffman_tree();
    calculate_huffman_codes();
    do_compress();
}

void Huffman::decompress()
{
    rebuid_huffman_tree();
    decode_huffman();
}


