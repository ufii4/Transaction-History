#ifndef TRANSACTION_HISTORY_HISTORY_H
#define TRANSACTION_HISTORY_HISTORY_H

#include "Transaction.h"

class History {
public:
    History();

    ~History();

    void read_history();

    void insert(Transaction *p_new_trans);

    void sort_by_date();

    void update_acb_cgl();

    double compute_cgl(unsigned int year);

    void print();

    Transaction *get_p_head()const;

private:
    Transaction *p_head;
};


#endif //TRANSACTION_HISTORY_HISTORY_H
