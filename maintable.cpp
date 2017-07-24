#include "maintable.h"
#include "ui_maintable.h"


MainTable::MainTable(int ways,int words,int block_size,bool totally,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainTable)
{
    //Setup ui and disable cell editing
    ui->setupUi(this);
    setWindowTitle(tr("Table"));
    this->ui->LiveTable->setEditTriggers(QAbstractItemView::EditTriggers(0));

    //Default values
    if(!ways){
        ways = 1;
    }
    if(!words){
        words = 1;
    }
    if(!block_size){
        block_size = 1;
    }

    this->ways = ways;
    this->words = words;
    this->block_size = block_size;
    this->is_associative = totally;
    this->words_per_way = words/ways;
    this->blocks_per_way = words_per_way/block_size;

    if(!totally){
        this->rep_info.reserve(ways);
        for(int i=0;i<blocks_per_way;i++){
            this->rep_info.push_back(Info(ways));
        }
    }
    else{
        this->associative_info.init(ways,this->blocks_per_way);
    }

    //Set columns and rows
    this->ui->LiveTable->setRowCount(this->words_per_way);
    this->ui->LiveTable->setColumnCount(ways);
}

MainTable::~MainTable()
{
    delete ui;
}

//Return selected method. Random will be the "default"
std::string MainTable::current_method(){
    if(this->ui->FIFOMethod->isChecked()){
        return "FIFO";
    }
    else if(this->ui->LIFOMethod->isChecked()){
        return "LIFO";
    }
    else if(this->ui->LRUMethod->isChecked()){
        return "LRU";
    }
    else if(this->ui->LFUMethod->isChecked()){
        return "LFU";
    }
    return "RANDOM";
}


//Insert value button
void MainTable::on_AddVButton_clicked()
{
   bool is_integer;
   int value = this->ui->VToInsert->text().toInt(&is_integer,10);
   //std::cout << "method is : " << this->current_method() << std::endl;
   if(is_integer && value > -1){
       bool found = false;
       this->total++;
       //Necessary values
       int temp_value = value/this->block_size;
       int block_address = temp_value%this->blocks_per_way;
       int position = value%this->block_size;
       if(!this->is_associative){
           //Search if there's any hit
           int index = 0;
           QTableWidgetItem *t;
           while(index < this->ways && !found){
               //Item at selected position
               t = this->ui->LiveTable->item(this->block_size*block_address+position,index);
               if(t && t->text().toInt(&is_integer,10) == value){
                   this->hits++;
                   found = true;
               }
               index++;
           }
           if(!found){
               index = this->all_occuped(block_address);
               if(index == -1){
                   index = ((*this).*rep_funct[this->current_method()])(block_address);
               }
               this->fill_block(block_address,position,value,index);
               this->update_info(block_address,index,true);
           }
           else{
               index--;
               this->update_info(block_address,index,false);
           }

       }
       else{
           int ind1=0;
           int ind2=0;
           QTableWidgetItem *t;
           while(ind1 < this->blocks_per_way && !found){
               ind2=0;
               while(ind2<this->ways && !found){
                   t = this->ui->LiveTable->item(ind1,ind2);
                   if(t && t->text().toInt(&is_integer,10) == value){
                       found = true;
                       this->hits++;
                       ind2--;
                       ind1--;
                   }
                   ind2++;
               }
               ind1++;
           }
           if(!found){
               std::tuple<int,int> tup = this->all_occuped_as();
               if(std::get<0>(tup) == -1){
                   tup = ((*this).*as_rep_funct[this->current_method()])();
               }
               this->fill_block(std::get<0>(tup),position,value,std::get<1>(tup));
               this->update_as_info(std::get<0>(tup),std::get<1>(tup),true);
           }
           else{
               this->update_as_info(ind1,ind2,false);
           }

       }
       this->ui->HitPercentValue->setText(QVariant(this->hit_percent()).toString());
       this->ui->FailPercentValue->setText(QVariant(this->fail_percent()).toString());
    }
}


/** Insert value and update info section **/

void MainTable::fill_block(int address, int position, int value,int way){
    int start = address*this->block_size;
    int start_value = value - position;
    for(int i=0;i<this->block_size;i++){
        QTableWidgetItem *e = new QTableWidgetItem(QVariant(start_value+i).toString());
        this->ui->LiveTable->setItem(start+i,way,e);
    }
}

void MainTable::update_info(int ad, int w, bool replacing){
    //Inserting order
    std::vector<int> &order = this->rep_info[ad].element_order;
    std::vector<int> &frecuency = this->rep_info[ad].frecuency;
    std::vector<int> &used = this->rep_info[ad].used_order;
    int max_v = *max_element(order.begin(),order.end());
    //All ways filled
    /*Assumming a cache of 5 ways with the following inserting order
    [1,4,3,5,2] and inserting element in the way 3, this'll be the procedure
    -rest all values higher than the selected way value by 1
    [1,3,2,4,2]
    -Replace the way value with the higher possible value
    [1,3,2,4,5]*/
    if(max_v == this->ways){
        for(int i=0;i<this->ways;i++){
            if(replacing && order[i] > order[w]){
                order[i]--;
            }
            if(used[i] > used[w]){
                used[i]--;
            }
        }
        if(replacing) order[w] = max_v;
        used[w] = max_v;
    }
    else{
        if(replacing) order[w] = max_v+1;
        used[w] = max_v+1;
    }
    //Used order

    //Frecuency order
    if(replacing){
        frecuency[w] = 1;
    }
    else{
        frecuency[w]++;
    }
}

int MainTable::all_occuped(int address){
    QTableWidgetItem *t;
    for(int i=0;i<this->ways;i++){
        t = this->ui->LiveTable->item(this->block_size*address,i);
        if(!t) return i;

    }
    return -1;
}
/** ------Replacement functions for no-totally-associative cache ----- **/

int MainTable::random(int index){
    time_t sec;
    time(&sec);
    srand((unsigned int) sec);
    return (rand()+index)%this->ways;
}

int MainTable::lfu(int index){
    int min_value = *min_element(this->rep_info[index].frecuency.begin(),this->rep_info[index].frecuency.end());
    for(int i=0;i<this->ways;i++){
        if(this->rep_info[index].frecuency[i] == min_value)
            return i;
    }
    return 0;
}
int MainTable::lru(int index){
    for(int i=0;i<this->ways;i++){
        if(this->rep_info[index].used_order[i] == 1)
            return i;
    }
    return 0;
}
int MainTable::fifo(int index){
    for(int i=0;i<this->ways;i++){
        if(this->rep_info[index].element_order[i] == 1)
            return i;
    }
    return 0;
}
int MainTable::lifo(int index){
    for(int i=0;i<this->ways;i++){
        if(this->rep_info[index].element_order[i] == this->ways)
            return i;
    }
    return 0;
}
/** -------------------------------------------------------------------- **/

/** Fail/Hit percent **/

double MainTable::hit_percent(){
    return (double)hits/(double)total;
}

double MainTable::fail_percent(){
    return 1-this->hit_percent();
}
/** -------Totally associative section ---------- **/

/** --- Max/Min element of matrix **/
int MainTable::max_as_element(std::vector<std::vector<int> > &v){
    int current = 0;
    for(unsigned int i=0;i<v.size();i++){
        for(unsigned int j=0;j<v[i].size();j++){
            if(v[i][j] > current)
                current = v[i][j];
        }
    }
    return current;
}

int MainTable::min_as_element(std::vector<std::vector<int> > &v){
    int current = INT_MAX;
    for(unsigned int i = 0;i<v.size();i++){
        for(unsigned int j=0;j<v[i].size();j++){
            if(v[i][j] < current){
                current = v[i][j];
            }
        }
    }
    return current;
}





void MainTable::update_as_info(int pos, int w, bool replacing){
    //Inserting order
    std::vector<std::vector<int> > &order = this->associative_info.element_order;
    std::vector<std::vector<int> > &frecuency = this->associative_info.frecuency;
    std::vector<std::vector<int> > &used = this->associative_info.used_order;
    int max_v = max_as_element(order);
    //All blocks filled
    if(max_v == this->blocks_per_way * this->ways){
        for(int i=0;i<this->blocks_per_way;i++){
            for(int j=0;j<this->ways;j++){
                if(replacing && order[i][j] > order[pos][w]){
                    order[i][j]--;
                }
                if(used[i][j] > used[pos][w]){
                    used[i][j]--;
                }
            }

        }
        if(replacing) order[pos][w] = max_v;
        used[pos][w] = max_v;
    }
    else{
        if(replacing) order[pos][w] = max_v+1;
        used[pos][w] = max_v+1;
    }
    //Used order

    //Frecuency order
    if(replacing){
        frecuency[pos][w] = 1;
    }
    else{
        frecuency[pos][w]++;
    }

}

std::tuple<int,int> MainTable::all_occuped_as(){
   QTableWidgetItem *t;
   for(int i=0;i<this->blocks_per_way;i++){
       for(int j=0;j<ways;j++){
           t = this->ui->LiveTable->item(this->block_size*i,j);
           if(!t) return std::make_tuple(i,j);
       }
   }
   return std::make_tuple(-1,-1);
}

//Associative replacement functions

std::tuple<int,int> MainTable::as_random(){
    time_t sec;
    time(&sec);
    srand((unsigned int) sec);
    return std::make_tuple(rand()%this->blocks_per_way,rand()%this->ways);

}
std::tuple<int,int> MainTable::as_fifo(){
    for(int i=0;i<this->blocks_per_way;i++){
        for(int j=0;j<this->ways;j++){
            if(this->associative_info.element_order[i][j] == 1)
                return std::make_tuple(i,j);
        }

    }
    return std::make_tuple(0,0);

}

std::tuple<int,int> MainTable::as_lifo(){
    for(int i=0;i<this->blocks_per_way;i++){
        for(int j=0;j<this->ways;j++){
            if(this->associative_info.element_order[i][j] == this->blocks_per_way*this->ways)
                return std::make_tuple(i,j);
        }
    }
    return std::make_tuple(0,0);

}

std::tuple<int,int> MainTable::as_lfu(){
    int min_value = min_as_element(this->associative_info.frecuency);
    for(int i=0;i<this->blocks_per_way;i++){
        for(int j=0;j<this->ways;j++){
            if(this->associative_info.frecuency[i][j] == min_value)
                return std::make_tuple(i,j);
        }

    }
    return std::make_tuple(0,0);

}
std::tuple<int,int> MainTable::as_lru(){
    for(int i=0;i<this->blocks_per_way;i++){
        for(int j=0;j<this->ways;j++){
            if(this->associative_info.used_order[i][j] == 1)
                return std::make_tuple(i,j);
        }

    }
    return std::make_tuple(0,0);

}

/** ----------------------------------------------- **/
