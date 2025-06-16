#ifndef ACCOUNT_WRAPPER_H
#define ACCOUNT_WRAPPER_H

static inline int updateCheckingAccountBal(long int CPF, double value){
    (void)CPF; (void)value; return 0;
}

static inline int updateSavingsAccountBal(long int CPF, double value){
    (void)CPF; (void)value; return 0;
}

#endif /* ACCOUNT_WRAPPER_H */ 