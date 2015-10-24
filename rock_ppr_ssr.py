import sys

def get_letter(M): 
    if (M[0] != 'S'):
        return M[0]
    if (M == "Spock"):
        return 'K'
    return 'S'

def get_winner(A, B): 
    if (A == B):
        return 0
    if (B in beats[A]):
        return 1
    return -1

def print_result(A,B,T):
    if (A == B):
        print("Alice and Bob tie, each winning " + str(A) + " game(s) and tying " + str(T) + " game(s)")
    elif (A > B):
        print("Alice wins, by winning "+ str(A) + " game(s) and tying " + str(T) + " game(s)")
    else:
        print("Bob wins, by winning "+ str(B) + " game(s) and tying " + str(T) + " game(s)")        
        
        
beats = {'R':['S','L'],
            'P':['K','R'],
            'S':['L','P'],
            'L':['P','K'],
            'K':['R','S']
            }

loses = {'R':['P','K'],
            'P':['S','L'],
            'S':['K','R'],
            'L':['R','S'],
            'K':['L','P']
            }

def get_Amove(A,B,r):
    if (r == 1):
        return A
    return loses[B][0]
    
def get_Bmove(B,r):
    if (B != 'K'):
        return 'K'
    if (r == 1):
        return 'P'
    if (r == -1):
        return 'R'
    if (r == 0):
        return 'L'
    
a = sys.stdin.readline()
for line in sys.stdin:
    line = line.replace('\n', '')
    words = line.split(' ')
    
    A = 0
    B = 0
    T = 0
        
    A_m = get_letter(words[0])
    B_m = get_letter(words[1])
    turns = int(words[2])
    turns -= 1
    r = get_winner(A_m, B_m)
    if (r == 1):
        A += 1
    elif (r == -1):
        B += 1
    else:
        T += 1
        
    while (turns > 0):
        turns -= 1
        A_m = get_Amove(A_m,B_m,r)
        B_m = get_Bmove(B_m,r)
        
        r = get_winner(A_m, B_m)
        if (r == 1):
            A += 1
        elif (r == -1):
            B += 1
        else:
            T += 1
    
    print_result(A,B,T)
    #print (str(A) + " " + str(B) + " " + str(T))
