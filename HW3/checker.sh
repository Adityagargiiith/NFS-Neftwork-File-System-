#!/bin/bash

# USAGE: ./checker.sh <question_number> <query>
# EXAMPLE: ./checker.sh 1 "SELECT * FROM Movies;"
# NOTE: Make sure that the database is named IMDB and you fill in the login credentials for mysql. 
# i.e. replace Root@1234 with your password.

export MYSQL_PWD="Akshit@2003" # mysql password for root

ans_file="answers/$1"
query=$2 

mysql -u root -e "USE IMDB; $query" > answers/temp.txt

# remove first line
gawk -i inplace 'NR>1' "answers/temp.txt"


if cmp -s "$ans_file" "answers/temp.txt"; then
    echo -e "\e[1;32m Correct Answer \e[0m"
else 
    echo -e "\e[1;31m Wrong Answer \e[0m"
fi
rm answers/temp.txt