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


EXAMPLES:
=========
`./fairshare`

 Uses parameters given by settings.ini


`./fairshare -1 1500 -2 2000`

Ignores the income from  settings.ini and uses
the ones provided by the command line arguments 
instead.

Requirements
============
* Linux
* g++
* Boost

Todos
=====
* check if settings.ini exists, if not than create one
* extent to use with arbitrary number of individuals
