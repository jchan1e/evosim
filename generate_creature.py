
import sys
import json
import random #.randrange(a,b), .uniform(a,b)
import struct

# genome = {b_neurons:[0.1,0.2,0.3], b_actions:[0.1,...,0.10], connections:[(0,1,0.0)(-3,12,1.0)]}

def save(G, filename):
    with open(filename, "wb") as outfile:
        cb = bytearray(struct.pack('i', int(len(G["connections"]))))
        outfile.write(cb)
        nb = bytearray(struct.pack('i', int(len(G["b_neurons"]))))
        outfile.write(nb)
        for B in G["b_neurons"]:
            Bb = bytearray(struct.pack('f', B))
            outfile.write(Bb)
        for b in G["b_actions"]:
            bb = bytearray(struct.pack('f', b))
            outfile.write(bb)
        for C in G["connections"]:
            C0b = bytearray(struct.pack('f', float(C[0])))
            C1b = bytearray(struct.pack('f', float(C[1])))
            C2b = bytearray(struct.pack('f', C[2]))
            outfile.write(C0b)
            outfile.write(C1b)
            outfile.write(C2b)

def load(filename):
    G = {}
    G["b_neurons"] = []
    G["b_actions"] = []
    G["connections"] = []
    with open(filename, "rb") as infile:
        str_fmt_1 = '2i'
        str_len_1 = struct.calcsize(str_fmt_1)
        str_unpack_1 = struct.Struct(str_fmt_1).unpack_from
        tmp1 = str_unpack_1(infile.read(str_len_1))

        n_conns = int(tmp1[0])
        n_neurons = int(tmp1[1])

        str_fmt_2 = str(10+n_neurons)+'f'
        str_len_2 = struct.calcsize(str_fmt_2)
        str_unpack_2 = struct.Struct(str_fmt_2).unpack_from
        tmp2 = str_unpack_2(infile.read(str_len_2))

        G["b_neurons"] = list(tmp2[0:0+n_neurons])
        G["b_actions"] = list(tmp2[0+n_neurons:10+n_neurons])
        tmp3 = struct.Struct(str(n_conns*3)+'f').unpack_from(infile.read(struct.calcsize(str(n_conns*3)+'f')))
        for i in range(n_conns):
            C0 = tmp3[3*i]
            C1 = tmp3[3*i+1]
            C2 = tmp3[3*i+2]
            G["connections"].append((int(C0),int(C1),C2))
    return G

def generate_new(n_conns, n_neurons):
    G = {}
    G["b_neurons"] = []
    for i in range(n_neurons):
        G["b_neurons"].append(random.uniform(-4.0,4.0))
    G["b_actions"] = []
    for i in range(10):
        G["b_actions"].append(random.uniform(-4.0,4.0))
    G["connections"] = []
    for i in range(int(n_conns)):
        C = (random.randrange(-n_neurons,18), random.randrange(-n_neurons,10), random.uniform(-4.0,4.0))
        G["connections"].append(C)
    return G

def mitosis(parent_name):
    mutation_rate = 0.05 #currently static
    G = load(parent_name)
    n_neurons = len(G["b_neurons"])
    # change neuron bias weights
    for i in range(n_neurons):
        if random.uniform(0.0,1.0) < mutation_rate:
            G["b_neurons"][i] += random.uniform(-0.5, 0.5)
            G["b_neurons"][i] = min(max(-4.0, G["b_neurons"][i]), 4.0)
    # change action bias weights
    for i in range(10):
        if random.uniform(0.0,1.0) < mutation_rate:
            G["b_actions"][i] += random.uniform(-0.5, 0.5)
            G["b_actions"][i] = min(max(-4.0, G["b_actions"][i]), 4.0)
    # add or remove neurons
    if random.uniform(0.0,1.0) < mutation_rate:
        if random.randrange(0,2) == 1:
            # add a neuron
            G["b_neurons"].append(random.uniform(-4.0, 4.0))
            n_neurons += 1
        else:
            # remove a neuron (and its connections) at random
            if n_neurons > 0:
                i_n = random.randrange(len(G["b_neurons"]))
                del G["b_neurons"][i_n]
                G["connections"] = [conn for conn in G["connections"] if conn[0] != i_n and conn[1] != i_n]
                n_neurons -= 1
    # modify connections
    for i in range(len(G["connections"])):
        if random.uniform(0.0,1.0) < mutation_rate:
            C = list(G["connections"][i])
            r = random.randrange(0,3)
            if r != 2:
                # change connection endpoints
                C[r] = random.randrange(-n_neurons, 10)
            else:
                # change connection weight
                C[r] += random.uniform(-0.5,0.5)
                C[r] = min(max(-4.0, C[r]), 4.0)
            G["connections"][i] = (C[0],C[1],C[2])
    # add or remove connections
    if random.uniform(0.0,1.0) < mutation_rate:
        if random.randrange(0,2) == 1:
            # add a connection
            C0 = random.randrange(-n_neurons, 18)
            C1 = random.randrange(-n_neurons, 10)
            C2 = random.uniform(-4.0,4.0)
            G["connections"].append((C0,C1,C2))
        else:
            # delete one at random
            if len(G["connections"]) > 0:
                del G["connections"][random.randrange(len(G["connections"]))]
    return G

def mate(parent_name1, parent_name2, n_conns):
    return None

def main():
    if len(sys.argv) < 3 or len(sys.argv) > 5:
        print("Usage: python generate_creature.py <num_connections> <num_neurons>\n       python generate_creature.py <num_connections> <parent.gene> <child.gene>")
        exit(0)

    #elif len(sys.argv) == 3:
    #    try:
    #        # generate new creature
    #        save(generate_new(int(sys.argv[1]), int(sys.argv[2])), sys.argv[-1])
    #    except:
    #        # copy existing creature with mutation
    #        save(mitosis(sys.argv[2], sys.argv[1]), sys.argv[-1])

    # generate new creature
    elif len(sys.argv) == 3:
        save(mitosis(sys.argv[1]), sys.argv[-1])

    # parthenogenetic reproduction with mutation
    elif len(sys.argv) == 4:
        save(generate_new(int(sys.argv[1]), int(sys.argv[2])), sys.argv[-1])

    # sexual reproduction with mutation
    elif len(sys.argv) == 5:
        save(mate(sys.argv[2], sys.argv[3], sys.argv[1]), sys.argv[-1])

    else:
        print("This code should never run\n")

    exit(0)

if __name__ == "__main__":
    main()
