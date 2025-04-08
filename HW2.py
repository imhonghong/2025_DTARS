def question_given(text):
    cut_text = [(text[i:i+3]) for i in range(0, len(text), 3)]  # cut text into 3 bits
    cut_text = [int(i, 2) for i in cut_text]    # string to decimal
    p0 = (0, 3, 1) # Question Given
    p1 = (0, 6 ,1) # tuple(departure, trans, arrival)
    p2 = (0, 4, 2)
    p3 = (1, 3, 3)
    p4 = (2, 6, 1)
    p5 = (2, 1, 3)
    p6 = (2, 4, 3)
    p7 = (3, 3, 0)
    defind_path_set = set([p0, p1, p2, p3, p4, p5, p6, p7])
    FSM_set = set((p[0], (p[1]//2)) for p in defind_path_set) # FSM set
    return cut_text, defind_path_set, FSM_set

def gen_tuple(cut_text): #gen all possible tuple
    for i in range(5):
        for j in cut_text:
            for k in range(5):
                yield (i, j, k)

def gen_candidate(cut_text, tuple_set): # step1: pick out trans meets cut_text[i]
    all_path = []
    for i in range(len(cut_text)):
        candidate = [path for path in tuple_set if path[1] == cut_text[i]]
        all_path.append(candidate)
    return all_path

def make_chain(all_path, current_path=[], step=0): # step2: generate all linked path
    if step == len(all_path):   # stop recursion
        return [current_path]  # Return completed path
    chains = []
    for candidate in all_path[step]:
        if not current_path or current_path[-1][2] == candidate[0]:  # Ensure continuity
            chains.extend(make_chain(all_path, current_path + [candidate], step + 1)) # make chain path recursively
    return chains

def find_extra_path(chain_path, defined_path_set):
    extra_path_num_list = []
    for i in range(len(chain_path)):
        unique_path = set(chain_path[i])
        extra_path_num = len(unique_path-defined_path_set)
        extra_path_num_list.append(extra_path_num)  # find evry path needs how many extra path
    min_extra = min(extra_path_num_list)    # we want least extra path
    best_path_idx = [i for i in range(len(extra_path_num_list)) if extra_path_num_list[i] == 4] # best solution may have multiple path
    best_path = []
    extra_path = []
    for i in best_path_idx:
        best_path.append(chain_path[i]) # best path
        extra_path.append(set(chain_path[i])-defined_path_set) # extra path corresponding to best path
    return best_path, extra_path, min_extra

def check_FSM(best_path, extra_path): #if the path include (a,b,c) and (a,b,d), we should remove the path
    # for b=0,1,2,3,4,5,6,7, 01,23,45,67 is also not allowed
    best_path1 = best_path.copy()
    extra_path1 = extra_path.copy()
    print(len(best_path1), len(best_path1[0]))

    idx = 0
    for path in best_path1:
        unique_FSM= set() # every path
        for step in path:
            unique_FSM.add((step[0], (step[1]//2)))
        if len(unique_FSM) != len(path):
            best_path1.remove(path)
            extra_path1.pop(idx) # remove the path which has same (a,b) but different c
        idx += 1
        unique_FSM.clear() # clear the set for next path

    return best_path1, extra_path1
            
def main():
    text = "001010010101100001110110"
    #text = "111010000100110101110000"
    cut_text, defind_path_set, FSM_set = question_given(text)
    
    print("cut_text", len(cut_text))
    
    tuple_set = set(gen_tuple(cut_text))
    all_path = gen_candidate(cut_text, tuple_set)
    chain_path = make_chain(all_path)
    best_path, the_extra_path, min_extra = find_extra_path(chain_path, defind_path_set)
    best_path1, the_extra_path1 = check_FSM(best_path, the_extra_path)
    print("best_path:")
    for path in range(len(best_path1)):
        extra_path = set(the_extra_path1[path])-defind_path_set
        used_path = set(p for p in best_path1[path])&defind_path_set
        path_FSM = set((p[0], (p[1]//2)) for p in extra_path)
        set_extra = set((ep[0], ep[1]//2) for ep in extra_path)
        if len(set_extra)!= len(extra_path):
            continue
        if len(FSM_set&path_FSM)!=0:
            continue
        print("best_path", path, best_path1[path])
        print("extra_path is", the_extra_path1[path])
        print("used_path is", used_path)
        print("")

    print("min_extra", min_extra)

if __name__ == "__main__":
    main()
