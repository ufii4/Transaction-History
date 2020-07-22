#include <iomanip>
#include "Transaction.h"

Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
                         unsigned int month_date, unsigned year_date, bool buy_sell_trans,
                         unsigned int number_shares, double trans_amount) {
    symbol = ticker_symbol;
    day = day_date;
    month = month_date;
    year = year_date;
    if (buy_sell_trans) {
        trans_type = "Buy";
    } else {
        trans_type = "Sell";
    }
    shares = number_shares;
    amount = trans_amount;
    set_next(nullptr);
    acb = 0;
    acb_per_share = 0;
    share_balance = 0;
    cgl = 0;
    trans_id = assigned_trans_id;
    assigned_trans_id++;
}

Transaction::~Transaction() {}

bool Transaction::operator<(Transaction *other) const {
    if (year < (other->get_year())) {
        return true;
    } else if (year == other->get_year()) {
        if (month < other->get_month()) {
            return true;
        } else if (month == other->get_month()) {
            if (day < other->get_day()) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

std::string Transaction::get_symbol() const { return symbol; }

unsigned int Transaction::get_day() const { return day; }

unsigned int Transaction::get_month() const { return month; }

unsigned int Transaction::get_year() const { return year; }

unsigned int Transaction::get_shares() const { return shares; }

double Transaction::get_amount() const { return amount; }

double Transaction::get_acb() const { return acb; }

double Transaction::get_acb_per_share() const { return acb_per_share; }

unsigned int Transaction::get_share_balance() const { return share_balance; }

double Transaction::get_cgl() const { return cgl; }

bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true : false; }

unsigned int Transaction::get_trans_id() const { return trans_id; }

Transaction *Transaction::get_next() const { return p_next; }

void Transaction::set_acb(double acb_value) { acb = acb_value; }

void Transaction::set_acb_per_share(double acb_share_value) { acb_per_share = acb_share_value; }

void Transaction::set_share_balance(unsigned int bal) { share_balance = bal; }

void Transaction::set_cgl(double value) { cgl = value; }

void Transaction::set_next(Transaction *p_new_next) { p_next = p_new_next; }

void Transaction::print() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(4) << get_trans_id() << " "
              << std::setw(4) << get_symbol() << " "
              << std::setw(4) << get_day() << " "
              << std::setw(4) << get_month() << " "
              << std::setw(4) << get_year() << " ";


    if (get_trans_type()) {
        std::cout << "  Buy  ";
    } else { std::cout << "  Sell "; }

    std::cout << std::setw(4) << get_shares() << " "
              << std::setw(10) << get_amount() << " "
              << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
              << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
              << std::setw(10) << std::setprecision(3) << get_cgl()
              << std::endl;
}