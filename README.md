# ApartmentsManager
*** Important note ***
The input is assumed to be standard

This project manages a database of apartments.
The possible commands for the program are:
•    add-apt "address" price numOfRooms entryDate (for example: "Hasavyonim 23 Ramat Ono" 2000000 4 01 01 20)
•    find-apt –MaxPrice xxx
•    find-apt –MinNumRooms x
•    find-apt –MaxNumRooms x
•    find-apt –Date ddmmyyyy (finds the apartments that evacuates till the given date)
•    buy-apt <number>
•    find-apt –Enter <x> (find apartments that were entered to the database in the last x days)
•    delete-apt –Enter <x> (delete apartments that were entered to the database in the last x days)

The commands can be combined, for example:
•    find-apt    –MinNumRooms x –MaxNumRooms y –MaxPrice zzz

You can add the option -s or -sr to each command, and get the apartments sorted from the lower price to the higher price or from the higher price to the lower price, respectively.

The commands are stored in the memory, in a queue that contains the last 7 commands, and in a linked list that contains the older commands.

You can restore commands from the database:
Pressing '!!' will activate the last command that was added to the database.
Pressing '!<num>' will activate the command that it's number is 'num'.
Pressing 'short_history' will print the most recent commands that were added to the database.
Pressing 'history' will print all the commands that were added to the database.
Pressing '!<num>^str1^str2' will activate the command that it's number is 'num' and contains the string 'str1' and replace it with 'str2'. (for example, if command number 4 was: find-apt –MinNumRooms 3 then the following command: !4^3^2 would be: find-apt –MinNumRooms 2

Pressing 'exit' will finish the program.

In the end of the program the commands are saved in a text file, and the apartments are saved in a binary file.
