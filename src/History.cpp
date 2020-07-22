#include "History.h"
#include "ECE150_Project3.h"

using namespace ece150;

History::History() {
    p_head = nullptr;
}

History::~History() {
    Transaction *p_current = p_head;
    while (p_current != nullptr) {
        Transaction *p_next = p_current->get_next();
        delete p_current;
        p_current = p_next;
    }
}

void History::read_history() {
    open_file();
    while (next_trans_entry()) {
        Transaction *p_next{new Transaction(get_trans_symbol(), get_trans_day(), get_trans_month(), get_trans_year(),
                                            get_trans_type(), get_trans_shares(), get_trans_amount())};
        insert(p_next);
    }
    close_file();
}

void History::insert(Transaction *p_new_trans) {
    if (p_head == nullptr) {
        p_head = p_new_trans;
        p_head->set_next(nullptr);
    } else {
        Transaction *p_current = get_p_head();
        //Direct the p_current to the end of the linked list
        while (p_current->get_next() != nullptr) {
            p_current = p_current->get_next();
        }
        //Direct the p_current to the end of the linked list
        p_current->set_next(p_new_trans);
    }
}

void History::sort_by_date() {
    //Create a new Linked list
    Transaction *p_new_head = p_head;
    if (p_head != nullptr) {
        Transaction *p_current = p_head->get_next();
        //After getting the node that p_head is pointing at, it has to be initialized for the new linked list
        p_new_head->set_next(nullptr);
        Transaction *p_trace;
        while (p_current != nullptr) {
            //p_next is initialized at first in case p_current is changed
            Transaction *p_next = p_current->get_next();
            if (p_current->operator<(p_new_head)) {
                p_current->set_next(p_new_head);
                p_new_head = p_current;
            } else {
                p_trace = p_new_head;
                Transaction *p_trace_next;
                while (p_trace->get_next() != nullptr) {
                    p_trace_next = p_trace->get_next();
                    if (p_current->operator<(p_trace_next)) {
                        p_current->set_next(p_trace_next);
                        p_trace->set_next(p_current);
                        break;
                    }
                    p_trace = p_trace_next;
                }
                //Determine whether the tracing reaches the end or not
                if (p_trace->get_next() == nullptr) {
                    p_trace->set_next(p_current);
                    //If the current node is at the end of the new array, must set its next node to nullptr
                    p_current->set_next(nullptr);
                }
            }
            p_current = p_next;
        }
    }
    //Replace the original linked list with the sorted linked list
    p_head = p_new_head;


    ///////////////////////////////////////////////Previous attempts///////////////////////////////////////////////////////
    //	Transaction *p_current=get_p_head();
    //	Transaction *p_next=p_current->get_next();
    //	while(p_next!=nullptr){
    //		p_next=p_current->get_next();
    //		Transaction *k=p_next;
    //		while(k->get_next()!=nullptr){
    //			if(!p_current->operator<(k)){
    //				Transaction *temp=k->get_next();
    //				k->set_next(p_current);
    //				p_current->set_next(temp);
    //			}
    //			k=k->get_next();
    //		}
    //		p_current=p_next;
    //	}


    //	Transaction *trans_1=p_head, *trans_2=trans_1->get_next(), *trans_3=trans_2->get_next();
    //	while(trans_3->get_next()!=nullptr){
    //		if(!trans_2->operator<(trans_3)){
    //			Transaction *temp=trans_2;
    //			temp->set_next(trans_3);
    //			trans_1->set_next(temp);
    //			trans_2=trans_3;
    //			trans_3=temp;
    //		}
    //		trans_1=trans_2;
    //		trans_2=trans_3;
    //		trans_3=trans_3->get_next();
    //	}

    //	Transaction *p_previous=p_head;
    //	Transaction *p_current=p_previous->get_next();
    //	Transaction *p_next=p_current->get_next();
    //	while(p_current->get_next()!=nullptr){
    //		Transaction *p_temp1=p_current;
    //		Transaction *p_temp2=p_next;
    //		Transaction *p_temp3=p_next->get_next();
    //		while(p_temp2->get_next()!=nullptr){
    //			if(!(*p_current<p_temp2)){
    //				p_temp1->set_next(p_current);
    //				p_current->set_next(p_temp3);
    //				p_previous->set_next(p_temp2);
    //				p_temp2->set_next(p_next);
    //				Transaction *p_temp=p_temp2;
    //				p_temp2=p_current;
    //				p_current=p_temp;
    //			}
    //			p_temp1=p_temp2;
    //			p_temp2=p_temp3;
    //			p_temp3=p_temp3->get_next();
    //		}
    //		p_previous=p_current;
    //		p_current=p_next;
    //		p_next=p_next->get_next();
    //	}
    ///////////////////////////////////////////////Previous attempts///////////////////////////////////////////////////////
}

void History::update_acb_cgl() {
    Transaction *p_current = p_head;
    double acb = 0;
    double acb_per_share = 0;
    unsigned int share_balance = 0;
    double cgl = 0;
    while (p_current != nullptr) {
        Transaction *p_next = p_current->get_next();
        if (p_current->get_trans_type()) {
            acb += p_current->get_amount();
            share_balance += p_current->get_shares();
            acb_per_share = acb / share_balance;
            p_current->set_acb(acb);
            p_current->set_acb_per_share(acb_per_share);
            p_current->set_share_balance(share_balance);
        } else {
            acb -= p_current->get_shares() * acb_per_share;
            share_balance -= p_current->get_shares();
            cgl = p_current->get_amount() - p_current->get_shares() * acb_per_share;
            p_current->set_acb(acb);
            p_current->set_share_balance(share_balance);
            p_current->set_acb_per_share(acb_per_share);
            p_current->set_cgl(cgl);
        }
        p_current = p_next;
    }
}

double History::compute_cgl(unsigned int year) {
    Transaction *p_current = p_head;
    double cgl = 0;
    while (p_current != nullptr) {
        Transaction *p_next = p_current->get_next();
        if (p_current->get_year() == year) {
            cgl += p_current->get_cgl();
        }
        p_current = p_next;
    }
    return cgl;
}

void History::print() {
    std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
    Transaction *p_current = get_p_head();
    while (p_current != nullptr) {
        p_current->print();
        p_current = p_current->get_next();
    }
    std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}

Transaction *History::get_p_head() const { return p_head; }