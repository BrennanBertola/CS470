import argparse
import copy
import sys

class Puzzle:
    def __init__(self, board):
        self.board: list[list[str]]= copy.deepcopy(board)

    def __str__(self):
        output = ""
        for i in self.board:
            output += str(i) + '\n'
        return output

    # Checks if all values to the right of the X are empty/open
    def isSolved(self):
        row = self.board[2]
        seenX = False

        for curr in row:
            if not seenX:
                if curr == 'X':
                    seenX = True
            else:
                if curr.isalpha() and curr != 'X':
                    return False

        return True

    #returns first occurence(row then column) of a give value
    def findIndex(self, value):
        for i, x in enumerate(self.board):
            if value in x:
                return (i, x.index(value))

    def makeMove(self, car: str, move: int):

        #get the index of the car
        r, c = self.findIndex(self.board, car)
        inc = 1

        # if move is positve, we need other end of car
        if move > 0:
            inc = -1
            if car.islower():
                while self.board[r+1][c] == car:
                    r += 1
            else:
                while self.board[r][c+1] == car:
                    c += 1
        #make a vertical move
        if car.islower():
            offset = 0
            while True:
                if self.board[r+offset][c] != car:
                    break
                self.board[r+offset+move][c] = car
                self.board[r+offset][c] = '-'
                offset += inc
        #make a horizontal move
        else:
            offset = 0
            while True:
                if self.board[r][c+offset] != car:
                    break
                self.board[r][c+offset+move] = car
                self.board[r][c+offset] = '-'
                offset += inc

    def getValidMoves(self):
        moves: dict[str, set[int]] = {}
        for r in range(6):
            for c in range(6):
                #no car at current location
                if not self.board[r][c].isalpha():
                    continue

                #vertical facing car
                elif self.board[r][c].islower():
                    if self.board[r][c] not in moves:
                        moves[self.board[r][c]] = set()
                    i = -1
                    while r+i >= 0:
                        if self.board[r+i][c] == self.board[r][c] or self.board[r+i][c].isalpha():
                            break
                        moves[self.board[r][c]].add(i)
                        i -= 1
                    i = 1
                    while r+i < 6:
                        if self.board[r+i][c] == self.board[r][c] or self.board[r+i][c].isalpha():
                            break
                        moves[self.board[r][c]].add(i)
                        i += 1

                #horizontal facing car
                else:
                    if self.board[r][c] not in moves:
                        moves[self.board[r][c]] = set()
                    i = -1
                    while c + i >= 0:
                        if self.board[r][c+i] == self.board[r][c] or self.board[r][c+i].isalpha():
                            break
                        moves[self.board[r][c]].add(i)
                        i -= 1
                    i = 1
                    while c + i < 6:
                        if self.board[r][c+i] == self.board[r][c] or self.board[r][c+i].isalpha():
                            break
                        moves[self.board[r][c]].add(i)
                        i += 1
        return moves

    def solve(self):
        pass


#Used to help fill the test files with their puzzles
def convertString(string: str):
    string = "Puzzle string here"
    newString = ""
    for i in range(len(string)):
        if not string[i].isalpha():
            newString += string[i]
        elif i == 0:
            if string[i] != string[i+1]:
                newString += string[i].lower()
            else:
                newString += string[i]
        elif i == len(string) -1:
            if string[i] != string[i - 1]:
                newString += string[i].lower()
            else:
                newString += string[i]
        else:
            if string[i] != string[i-1] and string[i] != string[i+1]:
                newString += string[i].lower()
            else:
                newString += string[i]
    print(newString)

def stringToBoard(string: str):
    board = []
    for i in range(6):
        idx = i * 6
        curr = string[idx:idx + 6]
        tmp = [j for j in curr.rstrip()]
        board.append(tmp)
    return board

def getOptions(args=sys.argv[1:]):
    parser = argparse.ArgumentParser(description="Rush Hour Solver.")
    parser.add_argument('-f', metavar='FILENAME', type=str, help="File of puzzles to solve", default="")
    parser.add_argument('-n', '--runs', type=int, help="Number of puzzles to run", default=10)
    parser.add_argument('-p', "--puzzle", help="used to input a puzzle through the command line", default="")
    options = parser.parse_args(args)
    return options

if __name__ == '__main__':
    options = getOptions()

    if options.puzzle != "":
        if len(options.puzzle) != 36:
            print("Please enter a valid puzzle")
            sys.exit()
        board = stringToBoard(options.puzzle)
        puzzle = Puzzle(board)
        print("User Puzzle:\n" + str(puzzle))
        sys.exit()


    file = open(options.file, 'r')
    runLimit = options.runs
    runCounter = 0

    print("Input File:", options.file)
    for puzzleString in file.readlines():
        #get rid of comments in the file
        if puzzleString[0] == '#':
            continue
        runCounter += 1
        if runCounter > runLimit:
            break

        board = stringToBoard(puzzleString)
        puzzle = Puzzle(board)
        print("current puzzle:\n" + str(puzzle))

