NAME:
=====
fairshare - gives the share of expenses depending on income

SYNOPSIS:
========
`fairshare [options]`

DESCRIPTION:
============
Uses simple percentile arithmetics to calculate the share each 
person has to pay for monthly expenses in a way that everybody 
pays the same percentage of ones income.

OPTIONS:
========
* -h [ --help ]         produce help message
* -1 [ --income1 ] arg  sets first income
* -2 [ --income2 ] arg  sets second income
* -r [ --rent ] arg     sets rent
* -u [ --util ] arg     sets utilities
* -f [ --food ] arg     sets food expenses
* -t [ --tele ] arg     sets telecommunication expenses


EXAMPLES:
=========
`./fairshare`

 Uses parameters given by settings.ini


`./fairshare -r 1500 -2000`

Ignores the income 2 and the rent from settings.ini and uses
the ones provided by argument the command line arguments 
instead.

Requirements
============
* Linux
* g++
* Boost

Todos
=====
* use person struct and expenses vector
* create initial settings file with user input
* user should be able to provide arbitrary number of self named expenses
* extent to use with arbitrary number of individuals
