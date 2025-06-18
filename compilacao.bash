gcc -std=c99 \
    main.c \
    account/account.c \
    AcessControl/AcessControl.c \
    transactions/transactions.c \
    user/user.c \
    -I account -I AcessControl -I transactions -I user \
    -o modular_app