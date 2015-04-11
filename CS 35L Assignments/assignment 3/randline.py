#!/usr/bin/python

"""
Output lines selected randomly from a file

Copyright 2005, 2007 Paul Eggert.
Copyright 2010 Darrell Benjamin Carbajal.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

Please see <http://www.gnu.org/licenses/> for a copy of the license.

$Id: randline.py,v 1.4 2010/04/05 20:04:43 eggert Exp $
"""

import random, sys
from optparse import OptionParser

class randline:
    def __init__(self, filename):
        f = open(filename, 'r')
        self.lines = f.readlines()
        f.close()

    def chooseline(self):
        return random.choice(self.lines)

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE

Output randomly selected lines from FILE."""

    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-n", "--numlines",
                      action="store", dest="numlines", default=1,
                      help="output NUMLINES lines (default 1)")
    parser.add_option("-u", "--unique", 
                      action="store_true", dest="unique", default=False,
                      help="ignore duplicate lines")
    parser.add_option("-w", "--without_replacement", 
                      action="store_true", dest="woreplacement", default=False,
                      help="output lines without replacement")
    options, args = parser.parse_args(sys.argv[1:])

    try:
        numlines = int(options.numlines)
        
    except:
        parser.error("invalid NUMLINES: {0}".
                     format(options.numlines))
    if numlines < 0:
        parser.error("negative count: {0}".
                     format(numlines))
    #file number has to be at least one
    if len(args) < 1:
        parser.error("wrong number of operands")

    unique=bool(options.unique)
    woreplacement=bool(options.woreplacement)

    #take arbitrary positive number of input file arguments, 
    #use an array string to store all the lines
    fileList=[]
    for i in range(len(args)):
        try:
            allFile= open(args[i], 'r')
            allLine = allFile.readlines()
        except IOError:
            parser.error("Files not exsit or incorrect filenames.")
        #add '\n' to input file ends in a non-newline character
        if allLine:
            #check the last character of the last line, and see if it is '\n'
            if allLine[-1][-1]!='\n':
                allLine[-1]=allLine[-1]+'\n'
        #append the new lines to whole file
        fileList=fileList+allLine
        allFile.close()

    #check if -u option is required
    if unique:
        fileList=list(set(fileList))
    #create a file so that we can get all the lines in all the files, 
    #this requires to write all the lines into an empty new file.
    f2=open("mergedFile",'w')
    f2.writelines(fileList)
    f2.close()
    input_file="mergedFile"
    if len(fileList)==0:
        parser.error ("Input file(s) is/are empty.")

    try:
        generator = randline(input_file)
        #replacement is not allowed
        if woreplacement:
            if len(fileList)<numlines:
                parser.error ("Input file line number is insufficient.")
            for index in range(numlines):
                chosenLine=generator.chooseline()
                sys.stdout.write(chosenLine)
                generator.lines.remove(chosenLine)
        else:
            for i in range(numlines):
                sys.stdout.write(generator.chooseline())
    except IOError as err:
        #make the program work with python3 as well
        errno,strerror=err.args
        parser.error("I/O error({0}): {1}".
                     format(errno, strerror))

if __name__ == "__main__":
    main()
