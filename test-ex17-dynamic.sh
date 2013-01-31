#!/bin/bash

function print_result() {
echo $1
    if [[ $1 ]]
    then
        printf "$2 action works\n"
    else
        printf "Something wrong with $2 action\n"
    fi
}

# testing create action
./ex17-dynamic address.db c 1>/dev/null 2>&1 <<EOF
5
20
EOF
print_result "$? -eq 0" Create

# testing s action
./ex17-dynamic address.db s 1 dima dima@gmail.com
print_result "$? -eq 0" Set

# testing that we can retrieve previously set record
expected=`./ex17-dynamic address.db g 1`
print_result "'$expected' == '1 dima dima@gmail.com'" Get

# testing that we cannot set record with id greater than max allowed id
expected=`./ex17-dynamic address.db s 6 grisha grisha@gmail.com`
echo $expected
print_result "'$expected' == 'ERROR: There\'s not that many records.'" Set
