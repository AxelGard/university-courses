# -----------------
# tenta 2016-08-17
# -----------------
# @axega544


#Up1
mem = [' ', 'att', 'lycka', 'tenta', 'till', 'på', 'är']

def expand(word_seq, int_seq):
    """ Takes a sequnce word_seq (with str as values)
    and uses the secound sequnce int_seq for finding
    positions to return new str """
    res = ""
    for pos in int_seq:
        if isinstance(pos, int):
            res += word_seq[pos]
        else:
            res += pos
    return res


#Up2
def interleave_r(seq_1, seq_2):
    """ puts evry athuer secound form the given two
    list seq_1 and seq_2 and returns them in a new list """
    if not seq_1 and not seq_2:
        return []
    elif len(seq_1) == 0:
        return [seq_2[0]] + interleave_r(seq_1, seq_2[1:])
    elif len(seq_2) == 0:
        return [seq_1[0]] + interleave_r(seq_1[1:], seq_2)
    else:
        return [seq_1[0], seq_2[0]] + interleave_r(seq_1[1:], seq_2[1:])


def interleave_i(seq_1, seq_2):
    """ puts evry athuer secound form the given two
    list seq_1 and seq_2 and returns them in a new list """
    new_seq = []
    if len(seq_1) >= len(seq_2):
        longest_seq = seq_1
        short_seq = seq_2
    else:
        longest_seq = seq_2
        short_seq = seq_1

    for pos in range(len(longest_seq)):
        if pos < len(short_seq):
            new_seq += [seq_1[pos], seq_2[pos]]
        else:
            new_seq += [longest_seq[pos]]
    return new_seq


#Up3 a
def reduce(f, seq):
    """ takes a lambda function f and
    runs thrue it with the given values list seq
    """
    if not seq:
        return 0
    elif isinstance(seq[0], list):
        return reduce(f, seq[0])
    elif len(seq) == 1:
        return seq[0]
    else:
        return f(seq[0], reduce(f, seq[1:]))


#Up3 b
#upgift är konstig
def reduce_if(f, p):
    pass
#print(reduce_if(lambda x, y: x+y, lambda x: x%2==1)(range(5)))


#Up4
def sum_all(seq):
    """ takes a list seq of unknown depth
    and returns the sum of all values in list
    """
    if not seq:
        return 0
    elif isinstance(seq[0], list):
        return sum_all(seq[0]) + sum_all(seq[1:])
    else:
        return seq[0] + sum_all(seq[1:])



#Up5
def make_deque():
    """ returns a new emty list """
    return []


def length(deque):
    """ returns the length of the que """
    return len(deque)


def front(deque):
    """ returns the first elementet in the que """
    if ( length(deque) > 0 ):
        return deque[0]
    else:
        return None



def back(deque):
    """ returns the last elemement in the que """
    if length(deque) > 0:
        return deque[-1]
    else:
        return None



def push_front_d(deque, elt): #Lägg till elt först i kön deque destruktivt och returnera kön.
    """ takes a que and a elt and sets the
    given elt as the first in the que (is destruktiv)
    """
    deque.insert(0, elt)
    return deque


def pop_front_d(deque): #Ta bort första elementet i kön deque destruktivt och returnera kön.
    """ removes the first elt in the que (is destruktiv)"""
    deque = deque.remove(front(deque))
    return deque


def push_back_d(deque, elt): #Lägg till elt sist i kön deque destruktivt och returnera kön.
    """ adds a elt last in que (is destruktiv)"""
    deque = deque + [elt]
    return deque


def pop_back_d(deque): #Ta bort sista elementet i kön deque destruktivt och returnera kön.
    """ removes the last elt in the que (is destruktiv) """
    if length(deque) > 0:
        deque.pop()
    return deque


def push_front_f(deque, elt): #Lägg till elt först i kön deque funktionellt och returnera kön.
    """ returns a new que with add a elt first in que
    (is not destruktiv)
    """
    new_que.insert(0, elt)
    return new_que


def pop_front_f(deque): #Ta bort första elementet i kön deque funktionellt och returnera kön.
    """ Returns a new que with the first elt
    removed from the que (is not destruktiv)
    """
    new_que = deque.remove(front(deque))
    return new_que


def push_back_f(deque, elt): #Lägg till elt sist i kön deque funktionellt och returnera kön.
    """ returns a new que with elt last in the given que
    (is not destruktiv)
    """
    new_que = deque + [elt]
    return new_que


def pop_back_f(deque): #Ta bort sista elementet i kön deque funktionellt och returnera kön.
    """ returns a new que with last elt removed in the que
    (is not destruktiv)
    """
    new_que = deque.remove(back(deque))
    return new_que


#test

q = make_deque()
q1 = push_front_d(q, 1)
print(length(q) == 1) #1
print(length(q)) #1

print(length(q1)) #1
print(push_back_d(q, 2))#[1, 2]
print(front(q))#1
print(q)
print(back(q))#2
print(pop_front_d(q)) #[2]
print(front(q))#2
print(pop_back_d(q))#[]



#Up6 --(need file is missing)
def make_undirected_graph():
    pass

def bfs(graph, start, goal):
    pass


#-----------
# Tenta 170419
#------------
#@axega544

#[('a', -1), ('a', 1)]
def analyze_data(seq):
    data_output = {}
    for data in seq:
        counter = 0
        new_data = []
        for val in range(1, len(seq)):
            new_data.append(val)
        data_output[data[0]] = new_data
        return data_output

def analyze_data_old(seq):
    data_output = {}
    for data in seq:
        counter = 0
        new_data = []
        for val_pos in range(1,len(data)):
            counter += 1
            new_data += [data[val_pos]]
            data_output.update({data[0]:[counter, new_data]})
    return data_output

def analyze_data(data_points:list) -> dict:
    """
    Extracting the amount of data points for one type of data and extracts
    the minimum and the maximum for each type of data
    :param data_points: All the data points
    :return: The analysis
    """
    stats = {}
    for dp in data_points:
        if dp[0] not in stats:
            stats[dp[0]] = [1, dp[1], dp[1]]
        else:
            stats[dp[0]][0] += 1
            stats[dp[0]][1] = min(stats[dp[0]][1], dp[1])
            stats[dp[0]][2] = max(stats[dp[0]][2], dp[1])
    return stats

#print(analyze_data([('a', -1), ('a', 1)]))#== { 'a': [2, -1, 1] })
#print(analyze_data([('a', 2), ('b', 0), ('a', 6), ('c', 0), ('b', 1)]) == { 'a': [2, 2, 6], 'b': [2, 0, 1], 'c': [1, 0, 0] } )


def merge_r(seq_1, seq_2):
    pass

def sort_list(seq):
    if not seq:
        return []
    elif len(seq) < 2:
        return sort_list(seq[1:]) + [seq[0]]
    elif seq[0] < seq[1]:
        return seq[0]

def merge_i(seq_1, seq_2):
    both_seq = seq_1 + seq_2
    new_seq = []
    for val_1  in seq_1:
        for val_2 in seq_2:
            if val_1 > val_2:
                new_seq = new_seq + [val_1]

    return new_seq
print(merge_i([1, 3, 5], [2, 4, 6]))


#-----------------
#tenta 2017-01-10
#-----------------
#@axega544


#Up1
def diophantine(a, b, c, d, min_val, max_val):
    """ finds the number of solotion to the diophantine equetion
    in the range of max and min val  """
    resulet = 0
    for x in range(min_val, max_val+1):
        for y in range(min_val, max_val+1):
            for z in range(min_val, max_val+1):
                if (a*x) + (b*y)+ (c*z) == d:
                    resulet += 1
    return resulet


#Up2
def sum_nth_r(seq, n, count=1):
    """ takes a list seq and a number n that is
    bigger then 0 and takes the sum of the n number """
    if not seq:
        return 0
    elif count == n:
        return seq[0]+sum_nth_r(seq[1:], n, 1)
    else:
        return sum_nth_r(seq[1:], n, count + 1)


def sum_nth_i(seq, n):
    """ takes a list seq and a number n that is
    bigger then 0 and takes the sum of the n number """
    if n > 0:
        res = 0
        for i in range(n-1, len(seq), n):
            print(i)
            res += seq[i]
        return res
    else:
        print("n needs to be bigger then 0 \n n > 0")


#Up5
def  count_change(coins, amount):
    """ Takes a list coins and a int amount.
    The function calculate the number of ways to you can transfer """
    if amount == 0:
        return 1
    elif not coins:
        return 0
    elif coins[0] <= amount:
        return count_change(coins, amount-coins[0]) + count_change(coins[1:], amount)
    else:
        return count_change(coins[1:], amount)
        
# -----------------
# Tenta 2016-01-12
# -----------------
# @axega544
# -----------------
import math

#Up 1 #fel i facit
def pi(max_k):
    """ Approximate pi using the first max_k+1 terms of
    Ramanujans series. Works for max_k up to 1228.
    """
    ans = 0
    for k in range(max_k+1):
        ans += math.factorial(4*k)*(1103+26390*k)/(
        math.factorial(k)**4 * 396**(4*k))
    return 9801/(2*math.sqrt(2)*ans)

# Test
#print(pi(0) == 3.1415927300133055) # True
#print(pi(2) == 3.1415926535897887) # False but suhld be true
#print(pi(2)) #3.141592653589793


def reverseeach_r(seq):
    """ Takes a list seq with values as
    str and revers eeach value
    """
    print(seq)
    if not seq:
        return []
    elif isinstance(seq, str):
        if seq == "":
            return ""
        else:
            return reverseeach_r(seq[1:]) + seq[0]
    else:
        return reverseeach_r(seq[0]) + reverseeach_r(seq[1:])


def reverseeach_i(seq):
    new_seq = []
    for val in seq:
        new_word = ""
        for cha_pos in range(len(val)):
            if cha_pos > 0:
                new_word += val[-cha_pos]
        new_seq += [new_word + val[0]]

    return new_seq

print(reverseeach_r(["paris", "i", "sirap"]) == ["sirap", "i", "paris"])
print(reverseeach_r(["hanna", "leo"]) == ["annah", "oel"])
  
#------------
# @axega544
#------------
# tenta 2016-01-12
#------------


#Up1
def encode(str):
    """ encodes a str acording to
    the ROT 13 way or ROT N deending on length of str
    """
    chr_set = "abcdefghijklmnopqrstuvwxyz" # would would be faster with ASCII,
                                         # but dont have acces to ASCII table
    # ROT 13
    if len(str) % 2 == 1:
        rot13_str = ""
        for chr in str:
            for let_pos in range(len(chr_set)):
                if chr_set[let_pos] == chr:
                    if let_pos + 13 < len(chr_set):
                        rot13_str += chr_set[let_pos + 13]
                    else:
                        new_pos = len(chr_set) - (let_pos + 13)
                        rot13_str += chr_set[new_pos]
        return rot13_str

    # ROT N
    if len(str) % 2 == 0:
        rotn_str = ""
        n = (len(str) / 2)
        for chr in str:
            for let_pos in range(len(chr_set)):
                if chr_set[let_pos] == chr:
                    if let_pos + n < len(chr_set):
                        rotn_str += chr_set[let_pos + n]
                    else:
                        new_pos = len(chr_set) - (let_pos + n)
                        rotn_str += chr_set[new_pos]
        return rotn_str


def decode(str):
    pass



def encode_2(str):
    chr_set = "abcdefghijklmnopqrstuvwxyz"
    rot13_str = ""
    for chr in str:
        for let_pos in range(len(chr_set)):
            if chr_set[let_pos] == chr:
                if let_pos + 13 < len(chr_set):
                    rot13_str += chr_set[let_pos + 13]
                else:
                    new_pos = len(chr_set) - (let_pos + 13)
                    rot13_str += chr_set[new_pos]
    return rot13_str

print(encode_2("hello")) # == uryyz | uryyb | b pos 2 | z pos 26 |  
#print(encode("hello") == "uryyb")
#print(encode("hell") == "jgnn")
#print(decode("uryyb") == "hello")
#print(encode("adam") == "cfco")
