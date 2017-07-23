#ifndef MAINTABLE_H
#define MAINTABLE_H

#include <QDialog>


//Required headers
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <tuple>

namespace Ui {
class MainTable;
}

class MainTable : public QDialog
{
    Q_OBJECT

public:
    explicit MainTable(int ways,int words,int block_size,bool totally,QWidget *parent = 0);
    ~MainTable();

private slots:
    void on_AddVButton_clicked();

private:
    //Ui declaration
    Ui::MainTable *ui;

    //Data structures
    struct Info{
        std::vector<int> element_order;
        std::vector<int> used_order;
        std::vector<int> frecuency;
        Info(int ways){
          this->element_order.resize(ways);
          this->used_order.resize(ways);
          this->frecuency.resize(ways);
        }
    };

    struct AsInfo{
        std::vector<std::vector<int> > element_order;
        std::vector<std::vector<int> > used_order;
        std::vector<std::vector<int> > frecuency;
        void init(int ways, int blocks){
            this->element_order.resize(blocks);
            this->used_order.resize(blocks);
            this->frecuency.resize(blocks);
            for(int i=0;i<blocks;i++){
                this->element_order[i].resize(ways);
                this->used_order[i].resize(ways);
                this->frecuency[i].resize(ways);
            }
        }
    };

    typedef int (MainTable::*ReplaceFunction)(int);
    typedef std::tuple<int,int> (MainTable::*AsReplaceFunction)(void);

    /* ------------ Functions/Procedures---------- */
    std::string current_method();
    void fill_block(int address, int position, int value, int way);
    int all_occuped(int address);
    void change_method(std::string new_method);
    int random(int index);
    int lfu(int index);
    int lru(int index);
    int fifo(int index);
    int lifo(int index);
    std::tuple<int,int> as_lfu();
    std::tuple<int,int> as_lru();
    std::tuple<int,int> as_fifo();
    std::tuple<int,int> as_lifo();
    std::tuple<int,int> as_random();
    void update_info(int ad,int w,bool replacing);
    double hit_percent();
    double fail_percent();

    //Only when cache is totally associative
    int max_as_element(std::vector<std::vector<int> > &v);
    int min_as_element(std::vector<std::vector<int> > &v);
    void update_as_info(int pos, int w, bool replacing);
    std::tuple<int,int> all_occuped_as();
    /* ------------------------------------------- */

    //Variables
    int ways;
    int words;
    int block_size;
    int words_per_way;
    int blocks_per_way;
    int total = 0;
    int hits = 0;
    bool is_associative;
    std::vector<Info> rep_info;
    AsInfo associative_info;
    //std::vector<Element> history;

    std::map<std::string,ReplaceFunction> rep_funct ={
        {"RANDOM",&MainTable::random},
        {"LFU",&MainTable::lfu},
        {"LRU",&MainTable::lru},
        {"FIFO",&MainTable::fifo},
        {"LIFO",&MainTable::lifo}
    };

    std::map<std::string,AsReplaceFunction> as_rep_funct{
        {"RANDOM",&MainTable::as_random},
        {"LFU",&MainTable::as_lfu},
        {"LRU",&MainTable::as_lru},
        {"FIFO",&MainTable::as_fifo},
        {"LIFO",&MainTable::as_lifo}
    };


};

#endif // MAINTABLE_H
